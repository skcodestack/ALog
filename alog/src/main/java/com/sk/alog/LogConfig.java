package com.sk.alog;

/**
 * @ProjectName: 2020_android_jk_zsw$
 * @Package: com.sk.alog$
 * @ClassName: LogConfig$
 * @Author: sk
 * @CreateDate: 2020/7/8$ 11:34$
 * @UpdateUser: sk
 * @UpdateDate: 2020/7/8$ 11:34$
 * @UpdateRemark:
 * @Version: 1.0
 * @Description:
 */
public class LogConfig {


    private String buffer_path;
    private int buffer_size;
    private String log_path;
    private boolean isCompress;


    private LogConfig() {

    }


    public String getBuffer_path() {
        return buffer_path;
    }


    public int getBuffer_size() {
        return buffer_size;
    }


    public String getLog_path() {
        return log_path;
    }


    public boolean isCompress() {
        return isCompress;
    }


    public static class Builder {

        private String buffer_path;
        private int buffer_size = 0;
        private String log_path;
        private boolean isCompress = true;

        public Builder bufferPath(String buffer_path) {
            this.buffer_path = buffer_path;
            return this;
        }

        public Builder bufferSize(int buffer_size) {
            this.buffer_size = buffer_size;
            return this;
        }

        public Builder logPath(String log_path) {
            this.log_path = log_path;
            return this;
        }

        public Builder isCompress(boolean compress) {
            isCompress = compress;
            return this;
        }

        public LogConfig build() {
            if (isEmpty(buffer_path)) {
                throw new RuntimeException("buffer path not allow empty!");
            }
            if (isEmpty(log_path)) {
                throw new RuntimeException("buffer path not allow empty!");
            }

            LogConfig config = new LogConfig();
            if (buffer_size <= 1024) {
                buffer_size = 4096;
            }
            config.buffer_path = buffer_path;
            config.buffer_size = buffer_size;
            config.isCompress = isCompress;
            config.log_path = log_path;
            return config;
        }


        private boolean isEmpty(String str) {
            return str == null || "".equals(str.trim());
        }
    }

}
