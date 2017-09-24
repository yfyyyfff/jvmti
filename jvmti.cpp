// jvmti.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <iostream>

#include "Agent.h"


static Agent agent;


JNIEXPORT jint JNICALL Agent_OnLoad(JavaVM *vm, char *options, void *reserved) {
    std::cout << "Agent_OnLoad!!!" << std::endl;
    jint state = JNI_OK;
    state = agent.initJvmti(vm);
    if (state != JNI_OK) {
        std::cout << "initJvmti failure" << std::endl;
        return state;
    }
    state = agent.initCallBacks();
    if (state != JNI_OK) {
        std::cout << "initCallBacks failure" << std::endl;
        return state;
    }
    state = agent.registerEvent();
    if (state != JNI_OK) {
        std::cout << "registerEvent failure" << std::endl;
        return state;
    }
    return JNI_OK;
}

JNIEXPORT jint JNICALL Agent_OnAttach(JavaVM* vm, char* options, void* reserved) {
    std::cout << "Agent_OnAttach" << std::endl;
    return JNI_OK;
}

JNIEXPORT jint JNICALL Agent_OnUnload(JavaVM* vm, char* options, void* reserved) {
    std::cout << "Agent_OnUnload" << std::endl;
    return JNI_OK;
}