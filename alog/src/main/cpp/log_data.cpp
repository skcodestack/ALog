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

#include "include/log_data.h"

alog::LogData::LogData( char *data, int len) :data(data),len(len){

}

alog::LogData::~LogData() {

    if(data != NULL){
        free(data);
        data = NULL;
    }
    len = 0;

}

bool alog::LogData::isVaild() {

    return data!=NULL && len > 0;
}
