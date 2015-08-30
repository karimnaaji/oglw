#include <jni.h>
#include "core/log.h"
#include "oglwAPI.h"

extern "C" {

    JNIEXPORT void JNICALL Java_Main_helloOGLW(JNIEnv* jniEnv, jobject obj) {
        INFO("Hello from OGLW\n");
    }

    JNIEXPORT void JNICALL Java_Main_init(JNIEnv* jniEnv, jobject obj, jfloat width, jfloat height) {
        OGLW::init(width, height);
    }

}
