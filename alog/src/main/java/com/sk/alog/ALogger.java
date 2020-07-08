package com.sk.alog;

/**
 * @ProjectName: 2020_android_jk_zsw$
 * @Package: com.sk.alog$
 * @ClassName: alogger$
 * @Author: sk
 * @CreateDate: 2020/7/7$ 15:33$
 * @UpdateUser: sk
 * @UpdateDate: 2020/7/7$ 15:33$
 * @UpdateRemark:
 * @Version: 1.0
 * @Description:
 */
public class ALogger {

    private static long ptr = -1;
    private static boolean isInit = false;

    public static void init(LogConfig config) {
        if (isInit) {
            throw new RuntimeException("ALogger  not allow init twice!");
        }
        isInit = true;
        ptr = LogBuffer.init(config.getBuffer_path(), config.getBuffer_size(),
                config.getLog_path(), config.isCompress());
    }

    private static void checkInit() {
        if (!isInit) {
            throw new RuntimeException("ALogger  is not initilized !");
        }
    }

    public static void write(String str) {
        checkInit();
        if (str == null || "".equals(str.trim())) {
            return;
        }
        if (ptr != -1) {
            LogBuffer.write(ptr, str);
        }
    }

    public static void flush() {
        checkInit();
        if (ptr != -1) {
            LogBuffer.flush(ptr);
        }
    }

    public static void release() {
        checkInit();
        if (ptr != -1) {
            LogBuffer.release(ptr);
            ptr = -1;
        }
        isInit = false;
    }

}
