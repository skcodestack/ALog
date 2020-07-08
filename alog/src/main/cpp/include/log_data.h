/**
 *
 * @ProjectName:    2020_android_jk_zsw$
 * @Package:        $PACKAGE_NAME$
 * @ClassName:      log_data$
 * @Author:         sk
 * @CreateDate:     2020/7/7$ 16:59$
 * @UpdateUser:     sk
 * @UpdateDate:     2020/7/7$ 16:59$
 * @UpdateRemark:   
 * @Version:        1.0
 * @Description:    
 *
 */

#ifndef INC_2020_ANDROID_JK_ZSW_LOG_DATA_H
#define INC_2020_ANDROID_JK_ZSW_LOG_DATA_H

#include <stdlib.h>
#include <stddef.h>
namespace alog{

    class LogData{

    public:
        LogData( char * data,int len);
        ~LogData();
        bool  isVaild();

    public:
        char * data =NULL;
        int len = -1;
    };
}


#endif //INC_2020_ANDROID_JK_ZSW_LOG_DATA_H
