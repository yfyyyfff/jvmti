#pragma once

#include <jvmti.h>
class Agent {

private:
    jvmtiEnv *jvmti;
    jvmtiEventCallbacks eventCallBakcs;
    jvmtiCapabilities cap;
public:
    jint initJvmti(JavaVM *jvm);
    jint initCallBacks();
    jint registerEvent();
};
