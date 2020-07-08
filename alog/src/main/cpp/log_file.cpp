/**
 *
 * @ProjectName:    2020_android_jk_zsw$
 * @Package:        $PACKAGE_NAME$
 * @ClassName:      log_file$
 * @Author:         sk
 * @CreateDate:     2020/7/7$ 15:58$
 * @UpdateUser:     sk
 * @UpdateDate:     2020/7/7$ 15:58$
 * @UpdateRemark:   
 * @Version:        1.0
 * @Description:    
 *
 */

#include <sys/stat.h>
#include "include/log_file.h"

alog::LogFile::LogFile(char *log_file, bool isAsynFlush) : file_path(log_file),
                                                           isAsynFlush(isAsynFlush) {

    if (log_file == NULL || strlen(log_file) == 0) {
        LOGE(ATAG, "log file path is empty :%s", log_file);
        return;
    }
    if (access(log_file, F_OK) == 0) {
        file = fopen(log_file, "a");
    } else {
        file = fopen(log_file, "a");
    }

    if (file == NULL) {
        LOGE(ATAG, "open log file: %s   is Faild", log_file);
        return;
    }
    if (isAsynFlush) {
        isStop = false;
        async_thread = std::thread(&alog::LogFile::async_write_func, this);
        vector = std::vector<LogData *>();
    }

}

alog::LogFile::~LogFile() {
    if (file != NULL) {
        fclose(file);
        file = NULL;
    }

    if (file_path != NULL) {
        free(file_path);
        file_path = NULL;
    }

    stopAsync();
}

size_t alog::LogFile::sync_write(LogData *data) {
    if (data == NULL || !data->isVaild()) {
        delete data;
        return 0;
    }
    size_t write_bytes = 0;
    if (file != NULL) {
        write_bytes = fwrite(data->data, data->len, 1, file);
        fflush(file);
        delete data;
    }

    return write_bytes;
}

void alog::LogFile::close_log() {
    if (file != NULL) {
        fclose(file);
        file = NULL;
    }
}

void alog::LogFile::del_file() {

    if (file != NULL) {
        //ftruncate(file, 0);
        fclose(file);
        file = NULL;
        remove(file_path);

    }
}

int alog::LogFile::file_size() {
    struct stat statbuf;
    if (stat(file_path, &statbuf) == 0)
        return statbuf.st_size;
    return -1;

}

void alog::LogFile::async_write(alog::LogData *data) {
    std::unique_lock<std::mutex> lock(async_mtx);
    vector.push_back(data);
    async_condition.notify_all();
}

void alog::LogFile::async_write_func() {
    while (true) {
        std::unique_lock<std::mutex> lock(async_mtx);
        while (!vector.empty()) {
            LogData *data = vector.back();
            vector.pop_back();
            if (data == NULL) {
                continue;
            }
            fwrite(data->data, data->len, 1, file);
            fflush(file);
            delete data;
        }
        if (isStop) {
            break;
        }
        async_condition.wait(lock);
    }
}

void alog::LogFile::stopAsync() {
    isStop = true;
    async_condition.notify_all();
    async_thread.join();
}

