package com.sk.alog;

/**
 * @ProjectName: 2020_android_jk_zsw$
 * @Package: com.sk.alog$
 * @ClassName: LogBuffer$
 * @Author: sk
 * @CreateDate: 2020/7/8$ 10:18$
 * @UpdateUser: sk
 * @UpdateDate: 2020/7/8$ 10:18$
 * @UpdateRemark:
 * @Version: 1.0
 * @Description:
 */
class LogBuffer {
    static {
        System.loadLibrary("alog");
    }

    public native static long init(String buffer_path, int buffer_size, String log_path, boolean isCompress);

    public native static int write(long ptr, String str);

    public native static void flush(long ptr);

    public native static void release(long ptr);

}
