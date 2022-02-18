#include <jni.h>
#include <string>
#include <android/log.h>
#include "CSocket.h"

extern "C" JNIEXPORT void JNICALL
Java_com_foobar_socketserver_MainActivity_doSomething(
        JNIEnv* env, jobject) {
    int socket = doSomething();

    __android_log_print(ANDROID_LOG_VERBOSE, "doSomething", "Socket value %d", socket);

    return;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_foobar_socketserver_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
