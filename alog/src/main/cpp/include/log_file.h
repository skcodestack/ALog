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

#ifndef INC_2020_ANDROID_JK_ZSW_LOG_FILE_H
#define INC_2020_ANDROID_JK_ZSW_LOG_FILE_H

#include <stdlib.h>
#include "log_util.h"
#include <unistd.h>
#include <asm/fcntl.h>
#include <fcntl.h>
#include <vector>
#include <__mutex_base>
#include <thread>
#include "log_data.h"

namespace alog {

    class LogFile {

    public:
        LogFile(char *log_file, bool isAsynFlush);

        ~LogFile();

        size_t sync_write(LogData *data);

        void async_write(LogData *data);



        void close_log();

        int file_size();

        void del_file();

        void stopAsync();

    private:
        void async_write_func();
        bool isAsynFlush = false;
        char *file_path = NULL;
        FILE *file = NULL;
        //异步相关
        std::vector<LogData *> vector;
        std::thread async_thread;
        std::condition_variable async_condition;
        std::mutex async_mtx;
        bool isStop = true;
    };

}


#endif //INC_2020_ANDROID_JK_ZSW_LOG_FILE_H
