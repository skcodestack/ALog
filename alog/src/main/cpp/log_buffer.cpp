/**
 *
 * @ProjectName:    2020_android_jk_zsw$
 * @Package:        $PACKAGE_NAME$
 * @ClassName:      log_buffer$
 * @Author:         sk
 * @CreateDate:     2020/7/7$ 15:36$
 * @UpdateUser:     sk
 * @UpdateDate:     2020/7/7$ 15:36$
 * @UpdateRemark:   
 * @Version:        1.0
 * @Description:    
 *
 */

#include <sys/mman.h>
#include "include/log_buffer.h"


alog::LogBuffer::LogBuffer(char *ptr, size_t size,
                           char *file_path, bool isMmap) : base_ptr(ptr), write_ptr(ptr),
                                                           buffer_size(size), file_path(file_path),
                                                           isMmap(isMmap) {
    logFile = new LogFile(file_path, true);
}

alog::LogBuffer::~LogBuffer() {
    if (base_ptr != NULL) {
        if (isMmap) {
            munmap(base_ptr, buffer_size);
        } else {
            free(base_ptr);
        }
        write_ptr = NULL;
        base_ptr = NULL;
    }

    if (logFile != NULL) {
        delete (logFile);
    }
}


size_t alog::LogBuffer::sync_write(char *str, size_t len) {
    std::lock_guard<std::recursive_mutex> lock(log_mtx);
    LogData *data = new LogData(str, len);
    if (!data->isVaild()) {
        delete (data);
        return 0;
    }
    if (logFile != NULL) {
        logFile->sync_write(data);
        return len;
    } else {
        delete (data);
    }
    return 0;
}

size_t alog::LogBuffer::available_size() {
    return buffer_size - (write_ptr - base_ptr);
}

size_t alog::LogBuffer::written_size() {
    return write_ptr - base_ptr;
}

void alog::LogBuffer::async_write(char *str, size_t len) {
    std::lock_guard<std::recursive_mutex> lock(log_mtx);
    if (len > available_size()) {
        async_flush();
    }
    size_t freeSize = available_size();
    size_t writeSize = 0;
    writeSize = len <= freeSize ? len : freeSize;
    memcpy(write_ptr, str, writeSize);
    write_ptr += writeSize;
    free(str);
}

void alog::LogBuffer::reset_buffer() {
    std::lock_guard<std::recursive_mutex> lock(log_mtx);
    write_ptr = base_ptr;
    memset(write_ptr, '\0', available_size());
}

void alog::LogBuffer::async_flush() {

    std::lock_guard<std::recursive_mutex> lock(log_mtx);
    size_t written_size = alog::LogBuffer::written_size();
    char *tem = new char[buffer_size];
    memset(tem, '\0', buffer_size);
    char * bptr = base_ptr;
    memcpy(tem, bptr, written_size);
    LogData *data = new LogData(tem, written_size);
    if (!data->isVaild()) {
        delete (data);
        reset_buffer();
        return;
    }
    if (logFile != NULL) {
        logFile->async_write(data);
        reset_buffer();
        return;
    } else {
        delete (data);
    }

    reset_buffer();

}


