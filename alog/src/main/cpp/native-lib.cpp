#include <jni.h>
#include <string>
#include <android/log.h>
#include <sys/mman.h>
#include "alogger.h"
#include "log_file.h"
#include "log_buffer.h"
#include "log_data.h"
#include "log_util.h"

static const char *const kTag = "ALog.JNI";
static JavaVM *kJvm;
extern "C" {

static JNIEnv *getJNIEnv() {
    //ensure kJvm init
    assert(kJvm != NULL);

    JNIEnv *env = NULL;
    int ret = kJvm->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (ret != JNI_OK) {
        __android_log_print(ANDROID_LOG_ERROR, kTag, "getJNIEnv !JNI_OK: %d", ret);
    }

    return env;
}

char *jstringToChars(JNIEnv *env, jstring jstr) {
    if (jstr == nullptr) {
        return nullptr;
    }

    jboolean isCopy = JNI_FALSE;
    const char *str = env->GetStringUTFChars(jstr, &isCopy);
    char *ret = strdup(str);
    env->ReleaseStringUTFChars(jstr, str);
    return ret;
}
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_sk_alog_LogBuffer_init(JNIEnv *env, jclass clazz, jstring buffer_path, jint buffer_size,
                                jstring log_path, jboolean is_compress) {

    char *buffer_path_str = jstringToChars(env, buffer_path);
    char *log_path_str = jstringToChars(env, log_path);
    size_t buffer_size_t = buffer_size;
    char *buffer_ptr = NULL;
    bool isMmap = true;
    int buffer_fd = open(buffer_path_str, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (buffer_fd == -1) {
        LOGE(kTag, "open buffer file[%s] failed;", buffer_path_str);
        return -1;
    }

    //mmap
    ftruncate(buffer_fd, static_cast<int>(buffer_size_t));
    lseek(buffer_fd, 0, SEEK_SET);
    buffer_ptr = static_cast<char *>(mmap(NULL, buffer_size_t, PROT_WRITE | PROT_READ,
                                          MAP_SHARED, buffer_fd, 0));

    if (buffer_ptr == MAP_FAILED) {
        LOGE(kTag, "mmap buffer file[%s] failed;", buffer_path_str);
        buffer_ptr = new char[buffer_size];
        isMmap = false;
    }
    if (buffer_ptr != NULL) {
        memset(buffer_ptr, '\0', buffer_size_t);
    }

    free(buffer_path_str);
    alog::LogBuffer *logBuffer = new alog::LogBuffer(buffer_ptr, buffer_size_t, log_path_str,
                                                     isMmap);
    return reinterpret_cast<jlong>(logBuffer);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_sk_alog_LogBuffer_write(JNIEnv *env, jclass clazz, jlong ptr, jstring str) {
    alog::LogBuffer *logBuffer = reinterpret_cast<alog::LogBuffer *>(ptr);
    if (logBuffer != NULL) {
        char *str_ = jstringToChars(env, str);
        logBuffer->async_write(str_, strlen(str_));
    }
    return 0;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_sk_alog_LogBuffer_flush(JNIEnv *env, jclass clazz, jlong ptr) {
    alog::LogBuffer *logBuffer = reinterpret_cast<alog::LogBuffer *>(ptr);
    if (logBuffer != NULL) {
        logBuffer->async_flush();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_sk_alog_LogBuffer_release(JNIEnv *env, jclass clazz, jlong ptr) {
    alog::LogBuffer *logBuffer = reinterpret_cast<alog::LogBuffer *>(ptr);
    if (logBuffer != NULL) {
        logBuffer->async_flush();
        delete logBuffer;
    }
}

JNIEXPORT jint
JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    __android_log_print(ANDROID_LOG_DEBUG, kTag, "JNI_OnLoad");

    kJvm = vm;
    //kJavaBridgeClass = reinterpret_cast<jclass>(env->NewGlobalRef(temp_cls));
    __android_log_print(ANDROID_LOG_DEBUG, kTag, "JNI_OnLoad done");
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL
JNI_OnUnload(JavaVM *vm, void *reserved) {
    __android_log_print(ANDROID_LOG_DEBUG, kTag, "JNI_OnUnload done");
    JNIEnv *env;
    kJvm->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (env != NULL) {

    }
}




