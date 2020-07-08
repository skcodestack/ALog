###  Android高性能日志库

### 1.原理

写入日志到文件中，涉及应用层缓存，系统调用，文件系统（page cache/buffer cache）,磁盘驱动等。
从linux系统来看各个层面给IO操作带来的影响：
###### 应用层缓存
c标准库中有写入数据的缓冲区，如果没有填充满缓冲区或者没有调用fflush等手动冲涮的情况下，如果应用崩溃或者机器断电，会导致数据没有及时保存到文件中而丢失。
反过来看，如果我们每次写入数据，都调用fflush或者直接使用不带缓冲区的api，虽然数据被及时保存到文件中或者page cache中，但是频繁的系统调用将会导致cpu浪费（用户态到内核态开销）。

###### 系统调用
频繁的系统调用将会导致cpu浪费。包括用户态到内核态的切换（trap调用，用户栈和内核栈寄存器状态保存与恢复）

###### page cache/buffer cache

读写文件时，有文件的页缓存（page cache）和 buffer 缓冲（buffer cache）.
我们这里主要看下写入的过程，如果我们需要向文件中写入数据时，Linux 2.4之前需要创建独立的buffer cache,Linux 2.4后buffer cache统一到page cache中，但是也避免不了buffer header的创建（组织buffer的结构体，其实就是链表）。
等到写入磁盘的时机或者手动调用fsync等冲涮函数，最后封装成bio结构，经过块设备层和调度层添加到磁盘驱动的队列中。


**入磁盘的时机**
-   手动调用 fsync()（主动）
-   定时回写，相关变量在/proc/sys/vm/dirty_writeback_centisecs和/proc/sys/vm/dirty_expire_centisecs中定义。
-   调用 write 的时候，发现 dirty page 占用内存超过系统内存一定比例，相关变量在/proc/sys/vm/dirty_background_ratio（ 后台运行不阻塞 write）和/proc/sys/vm/dirty_ratio（阻塞 write）中定义。
-   内存不足。

###### 磁盘

磁盘的写入速度就不必说了，一个字慢。


以上每个层面或多或少会导致文件写入操作的性能和完整性。那有什么可以的方案呢？


##### mmap
mmap 是使用逻辑内存对磁盘文件进行映射，中间只是进行映射没有任何拷贝操作，避免了写文件的数据拷贝。 操作内存就相当于在操作文件，避免了内核空间和用户空间的频繁切换。

**mmap 的回写时机**:
-   内存不足
-   进程退出
-   调用 msync 或者 munmap

mmap提高写入的性能问题和数据写入完整性。但是有什么缺点呢？
mmap映射文件中的区域在调用时就已经确定，对于追加的写入来说，不是很友好，所以我们采用mmap映射缓存文件，然后到达一定数据量时或者手动flush，再将数据写入到日志文件。
这样我们既可以通过mmap保证日志数据不丢失，也不用一直flush,占用过多的cpu时间导致主线程卡顿。

### 2.使用

**初始化**

```
    ## bufferPath 和logPath必填
    File file = new File(Environment.getExternalStorageDirectory(), "log_file.txt");
            File filecache = new File(Environment.getExternalStorageDirectory(), "log_file_cache.txt");
            ALogger.init(new LogConfig.Builder()
                    .bufferPath(filecache.getAbsolutePath())
                    .isCompress(true)
                    .logPath(file.getAbsolutePath())
                    .bufferSize(4096)
                    .build());
        }
```



**写入日志**

```
    ALogger.write(“。。。。。。”);
```

**手动flush**

```
    ALogger.flush(“。。。。。。”);
```


**释放资源**
```
    ALogger.release();
```

