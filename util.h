#pragma once

#include <jvmti.h>

class JvmtiUtil {
public:
    const char* getClassName(JNIEnv* jniEnv, jclass klass);
    const char* getMethodProto(jvmtiEnv *jvmti_env, jmethodID method);
    void  printMethodLocalVarAndValue(jvmtiEnv * jvmti_env, JNIEnv *jni_env, jthread thread, jvmtiFrameInfo *frameInfo, jint depth);

    const char* toString(JNIEnv *jni_env, jobject jo);
};