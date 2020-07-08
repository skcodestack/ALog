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

#ifndef INC_2020_ANDROID_JK_ZSW_LOG_BUFFER_H
#define INC_2020_ANDROID_JK_ZSW_LOG_BUFFER_H

#include <stdlib.h>
#include <mutex>
#include "log_file.h"
#include "log_data.h"

namespace alog {

    class LogBuffer {

    public:

        LogBuffer(char *ptr, size_t size, char *file_path, bool isMmap);

        ~LogBuffer();


        size_t sync_write(char *str, size_t len);

        void async_write(char *str, size_t len);

        size_t available_size();

        size_t  written_size();

        void reset_buffer();

        void async_flush();

    private:
        bool isMmap = true;
        char *file_path = NULL;
        char *base_ptr = NULL;
        char *write_ptr = NULL;
        size_t buffer_size = 0;

        LogFile *logFile = NULL;
        std::recursive_mutex log_mtx;

    };


}
#endif //INC_2020_ANDROID_JK_ZSW_LOG_BUFFER_H
