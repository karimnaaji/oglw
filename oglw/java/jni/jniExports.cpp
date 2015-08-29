#include <jni.h>
#include "core/log.h"

extern "C" {

    JNIEXPORT void JNICALL Java_OGLWApp_helloOGLW(JNIEnv* jniEnv, jobject obj) {
        INFO("Hello from OGLW\n");
    }

}
