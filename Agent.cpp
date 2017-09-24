#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <string>

#include "Agent.h"
#include "util.h"

using std::cout;
using std::endl;
using std::string;

JvmtiUtil jvmtiUtil;

void  classFileLoadHook
(jvmtiEnv *jvmti_env,
    JNIEnv* jni_env,
    jclass class_being_redefined,
    jobject loader,
    const char* name,
    jobject protection_domain,
    jint class_data_len,
    const unsigned char* class_data,
    jint* new_class_data_len,
    unsigned char** new_class_data) {
    //*new_class_data = (unsigned char*)malloc(sizeof(unsigned char)*class_data_len);
    jvmti_env->Allocate(class_data_len, new_class_data);
    if (strcmp(name, "encrypt/TestClass") != 0) {
        *new_class_data_len = class_data_len;
        //std::cout << "unencrypted class " << name << std::endl;
        memcpy(*new_class_data, class_data, class_data_len);
    }
    else {
        *new_class_data_len = class_data_len;
        int i;
        unsigned char tmp = 0;
        for (i = 0; i < class_data_len; i++) {
            tmp = (class_data[i]);
            (*new_class_data)[i] = (unsigned char)(~tmp);
            //(*new_class_data)[i] = tmp;
        }
        //if((*new_class_data)[0]==0xca && (*(new_class_data)[1]==0xfe)
        std::cout << "unencrypt " << name << " successfully !!!" << std::endl;
    }
}

void  classFileLoadHook2
(jvmtiEnv *jvmti_env,
    JNIEnv* jni_env,
    jclass class_being_redefined,
    jobject loader,
    const char* name,
    jobject protection_domain,
    jint class_data_len,
    const unsigned char* class_data,
    jint* new_class_data_len,
    unsigned char** new_class_data) {
    //............
    std::cout << "init jvmti failed" << std::endl;
    std::cout << "className=" << name << std::endl;
    std::cout << "class_data_length=" << class_data_len << std::endl;
    std::ofstream f;
    std::string s("G:/WORK_SOURCE/jvmTools/x64/Debug");
    s += name;
    s = +".class";
    f.open(s, std::ios::out | std::ios::binary);
    f.write((const char*)class_data, class_data_len);
    f.flush();
    f.close();
    new_class_data = (unsigned char**)&class_data;
    *new_class_data_len = class_data_len;
}
void  classLoad
(jvmtiEnv *jvmti_env,
    JNIEnv* jni_env,
    jthread thread,
    jclass klass) {
    const char* clsName = jvmtiUtil.getClassName(jni_env, klass);
    std::cout << "Load " << clsName << std::endl;
}

void fieldAccess
(jvmtiEnv *jvmti_env,
    JNIEnv* jni_env,
    jthread thread,
    jmethodID method,
    jlocation location,
    jclass field_klass,
    jobject object,
    jfieldID field) {

    jclass cls;
    jvmti_env->GetMethodDeclaringClass(method, &cls);
    //
    std::string fName;
    std::string sig;
    std::string gen;
    jvmti_env->GetFieldName(field_klass, field, (char**)&fName, (char**)&sig, (char**)&gen);
    std::cout << "FIELD_ACCESS:" << fName << " " << sig << " " << gen << std::endl;
}

void fieldModification
(jvmtiEnv *jvmti_env,
    JNIEnv* jni_env,
    jthread thread,
    jmethodID method,
    jlocation location,
    jclass field_klass,
    jobject object,
    jfieldID field,
    char signature_type,
    jvalue new_value) {
    //......

    std::cout << "fieldModification start" << std::endl;
    std::cout << method << std::endl;
    std::cout << location << std::endl;
    std::cout << field_klass << std::endl;
    std::cout << object << std::endl;
    std::cout << field << std::endl;
    std::cout << signature_type << std::endl;
    std::cout << new_value.i << std::endl;
    std::cout << "fieldModification end" << std::endl;

}

void exceptionOccur
(jvmtiEnv *jvmti_env,
    JNIEnv* jni_env,
    jthread thread,
    jmethodID method,
    jlocation location,
    jobject exception,
    jmethodID catch_method,
    jlocation catch_location) {

    char* curMethodName = NULL;
    jclass exceptionCls = jni_env->GetObjectClass(exception);
    const char* xName = jvmtiUtil.getClassName(jni_env, exceptionCls);
    if (strcmp(xName, "jvmti.ExceptionForTest") != 0) {
        return;
    }

    //thread info
    jvmtiThreadInfo threadInfo = { 0 };
    jvmti_env->GetThreadInfo(thread, &threadInfo);
    cout << "Exception Occured!!!" << endl;
    cout << "thread name = " << threadInfo.name << endl;

    //trace
    jvmtiFrameInfo fInfos[5] = { 0 };
    jint count = 0;
    jvmti_env->GetStackTrace(thread, 0, 3, fInfos, &count);
    //jvmti_env->exc
    int i = 0;
    for (; i < count; i++) {
        jvmtiUtil.printMethodLocalVarAndValue(jvmti_env, jni_env, thread, fInfos + i, i);
    }
}

void exceptionOccur2
(jvmtiEnv *jvmti_env,
    JNIEnv* jni_env,
    jthread thread,
    jmethodID method,
    jlocation location,
    jobject exception,
    jmethodID catch_method,
    jlocation catch_location) {
    //
    char *methodName = nullptr;
    jvmti_env->GetMethodName(method, &methodName, nullptr, nullptr);
    //jvmti_env->getmetho
    //if(strcmp(methodName,""))
    const char *proto = jvmtiUtil.getMethodProto(jvmti_env, method);
    //jvmti_env->getmethod
    const char* xName = jvmtiUtil.getClassName(jni_env, jni_env->GetObjectClass(exception));
    jclass declClass = { 0 };
    jvmti_env->GetMethodDeclaringClass(method, &declClass);
    const char* clsName = jvmtiUtil.getClassName(jni_env, declClass);
    std::cout << "*********** Exception = " << xName << " ***********" << std::endl;
    std::cout << "*********** method = " << clsName << "." << proto << " ***********" << std::endl;

    //jvmti_env->GetLocalObject(thread,)
    //jvmti_env->GetLocalObject()

    jvmti_env->Deallocate((unsigned char*)proto);

    //thread info
    jvmtiThreadInfo threadInfo = { 0 };
    jvmtiThreadGroupInfo gThreadInfo = { 0 };
    jvmti_env->GetThreadInfo(thread, &threadInfo);
    jvmti_env->GetThreadGroupInfo(thread, &gThreadInfo);
    cout << "thread name = " << threadInfo.name << endl;
    if (gThreadInfo.name == nullptr) {
        gThreadInfo.name = "Unknown ThreadGroupInfo";
    }
    cout << "threadGroup name = " << gThreadInfo.name << endl;
    //trace
    jvmtiFrameInfo fInfos[5] = { 0 };
    jint count = 0;
    jvmti_env->GetStackTrace(thread, 0, 3, fInfos, &count);
    //jvmti_env->exc
    int i = 0;
    for (; i < count; i++) {
        string proto(jvmtiUtil.getMethodProto(jvmti_env, fInfos[i].method));
        cout << proto << endl;
        //jvmti_env->GetLineNumberTable(fInfos[i]);
    }
    jvmtiLocalVariableEntry *lvEntryPtr = NULL;
    jint entryCounter = 0;
    jvmtiError err = jvmti_env->GetLocalVariableTable(method, &entryCounter, &lvEntryPtr);
    if (err != JVMTI_ERROR_NONE) {
        cout << "GetLocalVariableTable" << endl;
    }
}



jint Agent::initJvmti(JavaVM *jvm)
{
    jint state = jvm->GetEnv((void**)&jvmti, JVMTI_VERSION_1_0);
    if (state != JNI_OK) {
        return state;
    }
#if 0
    memset(&cap, 1, sizeof(cap));
#else 
    memset(&cap, 0, sizeof(cap));
    cap.can_signal_thread = 1;
    cap.can_signal_thread = 1;
    cap.can_get_owned_monitor_info = 1;
    cap.can_generate_method_entry_events = 1;
    cap.can_generate_exception_events = 1;
    cap.can_generate_vm_object_alloc_events = 1;
    cap.can_tag_objects = 1;
    cap.can_access_local_variables = 1;
#endif

    jvmtiError err = jvmti->AddCapabilities(&cap);
    if (err != JVMTI_ERROR_NONE) {
        return JNI_ERR;
    }
    return JNI_OK;
}

jint Agent::initCallBacks()
{
    eventCallBakcs.ClassFileLoadHook = (jvmtiEventClassFileLoadHook)classFileLoadHook;
    eventCallBakcs.ClassLoad = (jvmtiEventClassLoad)classLoad;
    eventCallBakcs.FieldAccess = (jvmtiEventFieldAccess)fieldAccess;
    eventCallBakcs.FieldModification = (jvmtiEventFieldModification)fieldModification;
    eventCallBakcs.Exception = (jvmtiEventException)exceptionOccur;

    jvmti->SetEventCallbacks(&eventCallBakcs, sizeof(eventCallBakcs));
    return 0;
}

jint Agent::registerEvent()
{
    jvmtiError err;
    //err = jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, 0);

    //jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_CLASS_LOAD, 0);
    //jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_FIELD_ACCESS, 0);
    //jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_FIELD_MODIFICATION, 0);

    //jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_VM_INIT, (jthread)NULL);
    //jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_VM_DEATH, (jthread)NULL);
    //jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_VM_OBJECT_ALLOC, (jthread)NULL);
    err = jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_EXCEPTION, NULL);
    return err;
}
