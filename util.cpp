
#include "stdafx.h"
#include "util.h"
#include <string>
#include <iostream>

using namespace std;

jvmtiError JvmtiUtil::deallocate(jvmtiEnv * jvmti_env, void * block) {
    return jvmti_env->Deallocate((unsigned char*)block);
}

const char * JvmtiUtil::getClassName(JNIEnv * jniEnv, jclass klass) {
    jclass tClass = jniEnv->FindClass("java/lang/Class");
    jmethodID getNameMethod = jniEnv->GetMethodID(tClass, "getName", "()Ljava/lang/String;");
    jstring jName = (jstring)jniEnv->CallObjectMethod(klass, getNameMethod);
    const char *name = jniEnv->GetStringUTFChars(jName, 0);
    jniEnv->ReleaseStringUTFChars(jName, 0);
    return name;
}

const char * JvmtiUtil::getMethodProto(jvmtiEnv * jvmti_env, jmethodID method) {
    string result("");
    char *name = nullptr;
    char *sig = nullptr;
    char *gen = nullptr;
    jvmti_env->GetMethodName(method, &name, &sig, &gen);
    result += name;
    result += sig;
    //free
    jvmti_env->Deallocate((unsigned char*)name);
    jvmti_env->Deallocate((UCHAR*)sig);
    jvmti_env->Deallocate((UCHAR*)gen);
    //
    char* buf = (char*)malloc(sizeof(char)*result.length());
    memcpy(buf, result.c_str(), result.length());
    return buf;
}


void JvmtiUtil::printMethodLocalVarAndValue(jvmtiEnv * jvmti_env, JNIEnv *jni_env, jthread thread, jvmtiFrameInfo *frameInfo, jint depth) {

    char *name = nullptr;
    char *sig = nullptr;
    char *gen = nullptr;
    jmethodID method = frameInfo->method;
    jvmti_env->GetMethodName(method, &name, &sig, &gen);

    cout << name << " arguments values:" << endl;
    //free
    jvmti_env->Deallocate((UCHAR*)name);
    jvmti_env->Deallocate((UCHAR*)sig);
    jvmti_env->Deallocate((UCHAR*)gen);

    jint argSize = 0;
    jvmti_env->GetArgumentsSize(method, &argSize);
    jint entryCount = 0;
    jvmtiLocalVariableEntry *entries = NULL;
    jvmti_env->GetLocalVariableTable(method, &entryCount, &entries);
    jint i = 0;
    jvmtiLocalVariableEntry *tmpEntry;
    for (; i < entryCount; i++) {
        //先支持int
        tmpEntry = (entries + i);
        char sigChar = tmpEntry->signature[0];
        if (sigChar == 'I') {
            jint v = 0;
            jvmti_env->GetLocalInt(thread, depth, tmpEntry->slot, &v);
            cout << "\t" << tmpEntry->name << "=" << v << endl;
        } else if (sigChar == 'L' || sigChar == '[') {
            jobject jo = NULL;
            jvmti_env->GetLocalObject(thread, depth, tmpEntry->slot, &jo);
            if (jo == NULL) {
                cout << "\t" << tmpEntry->name << "=" << "null" << endl;
            } else {
                cout << "\t" << tmpEntry->name << "=" << toString(jni_env, jo) << endl;
            }
        } else {
            cout << "\t" << tmpEntry->name << "=暂不支持的类型" << endl;
        }
    }
}

/**调用一个对象的toString方法*/
const char* JvmtiUtil::toString(JNIEnv * jni_env, jobject jo) {
    jclass clz = jni_env->GetObjectClass(jo);
    jmethodID toStringMethod = jni_env->GetMethodID(clz, "toString", "()Ljava/lang/String;");
    jstring str = (jstring)jni_env->CallObjectMethod(jo, toStringMethod);
    const char* s = jni_env->GetStringUTFChars(str, 0);
    return s;
}

const char * JvmtiUtil::array2String(JNIEnv * jni_env, jarray array) {
    jsize size = jni_env->GetArrayLength(array);

}



