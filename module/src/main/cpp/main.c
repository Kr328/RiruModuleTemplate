#include <stdio.h>
#include <jni.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <android/log.h>
#include <sys/stat.h>
#include <sys/system_properties.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "Template", __VA_ARGS__)

#define EXPORT __attribute__((visibility("default"))) __attribute__((used))

#define INJECT_CLASS_NAME "com/github/kr328/template/Injector"
#define INJECT_METHOD_NAME "inject"
#define INJECT_METHOD_SIGNATURE "(Ljava/lang/String;)V"

static int load_and_invoke_dex(JNIEnv *env, void *dex_data, long dex_data_length, const char *argument) {
    // get system class loader
    jclass cClassLoader = (*env)->FindClass(env, "java/lang/ClassLoader");
    jmethodID mSystemClassLoader = (*env)->GetStaticMethodID(env, cClassLoader,
                                                             "getSystemClassLoader",
                                                             "()Ljava/lang/ClassLoader;");
    jobject oSystemClassLoader = (*env)->CallStaticObjectMethod(env, cClassLoader,
                                                                mSystemClassLoader);

    // load dex
    jobject bufferDex = (*env)->NewDirectByteBuffer(env, dex_data, dex_data_length);
    jclass cDexClassLoader = (*env)->FindClass(env, "dalvik/system/InMemoryDexClassLoader");
    jmethodID mDexClassLoaderInit = (*env)->GetMethodID(env, cDexClassLoader, "<init>",
                                                        "(Ljava/nio/ByteBuffer;Ljava/lang/ClassLoader;)V");
    jobject oDexClassLoader = (*env)->NewObject(env, cDexClassLoader,
                                                mDexClassLoaderInit,
                                                bufferDex,
                                                oSystemClassLoader);

    // get loaded dex inject method
    jmethodID mFindClass = (*env)->GetMethodID(env, cDexClassLoader, "loadClass",
                                               "(Ljava/lang/String;)Ljava/lang/Class;");
    jstring sInjectClassName = (*env)->NewStringUTF(env, INJECT_CLASS_NAME);
    jclass cInject = (jclass) (*env)->CallObjectMethod(env, oDexClassLoader,
                                                       mFindClass, sInjectClassName);

    // find method
    jmethodID mLoaded = (*env)->GetStaticMethodID(env, cInject, INJECT_METHOD_NAME,
                                                  INJECT_METHOD_SIGNATURE);

    jstring stringArgument = (*env)->NewStringUTF(env, argument);

    (*env)->CallStaticVoidMethod(env, cInject, mLoaded, stringArgument);

    // check status
    if ((*env)->ExceptionCheck(env)) {
        (*env)->ExceptionDescribe(env);
        (*env)->ExceptionClear(env);
        LOGI("Inject dex failure");

        return -1;
    }

    return 0;
}

EXPORT void nativeForkAndSpecializePre(
        JNIEnv *env, jclass clazz, jint *_uid, jint *gid, jintArray *gids, jint *runtimeFlags,
        jobjectArray *rlimits, jint *mountExternal, jstring *seInfo, jstring *niceName,
        jintArray *fdsToClose, jintArray *fdsToIgnore, jboolean *is_child_zygote,
        jstring *instructionSet, jstring *appDataDir, jboolean *isTopApp,
        jobjectArray *pkgDataInfoList,
        jobjectArray *whitelistedDataInfoList, jboolean *bindMountAppDataDirs,
        jboolean *bindMountAppStorageDirs) {
}

EXPORT int nativeForkAndSpecializePost(JNIEnv *env, jclass clazz, jint res) {
    if (res == 0) {
        // in app process
    } else {
        // in zygote process, res is child pid
        // don't print log here, see https://github.com/RikkaApps/Riru/blob/77adfd6a4a6a81bfd20569c910bc4854f2f84f5e/riru-core/jni/main/jni_native_method.cpp#L55-L66
    }
    return 0;
}

EXPORT __attribute__((visibility("default"))) void specializeAppProcessPre(
        JNIEnv *env, jclass clazz, jint *_uid, jint *gid, jintArray *gids, jint *runtimeFlags,
        jobjectArray *rlimits, jint *mountExternal, jstring *seInfo, jstring *niceName,
        jboolean *startChildZygote, jstring *instructionSet, jstring *appDataDir,
        jboolean *isTopApp, jobjectArray *pkgDataInfoList, jobjectArray *whitelistedDataInfoList,
        jboolean *bindMountAppDataDirs, jboolean *bindMountAppStorageDirs) {
    // added from Android 10, but disabled at least in Google Pixel devices
}

EXPORT __attribute__((visibility("default"))) int specializeAppProcessPost(
        JNIEnv *env, jclass clazz) {
    // added from Android 10, but disabled at least in Google Pixel devices
    return 0;
}

EXPORT void nativeForkSystemServerPre(
        JNIEnv *env, jclass clazz, uid_t *uid, gid_t *gid, jintArray *gids, jint *runtimeFlags,
        jobjectArray *rlimits, jlong *permittedCapabilities, jlong *effectiveCapabilities) {

}

EXPORT int nativeForkSystemServerPost(JNIEnv *env, jclass clazz, jint res) {
    if (res == 0) {
        // in system server process
    } else {
        // in zygote process, res is child pid
        // don't print log here, see https://github.com/RikkaApps/Riru/blob/77adfd6a4a6a81bfd20569c910bc4854f2f84f5e/riru-core/jni/main/jni_native_method.cpp#L55-L66
    }
    return 0;
}

EXPORT int shouldSkipUid(int uid) {
    // by default, Riru only call module functions in "normal app processes" (10000 <= uid % 100000 <= 19999)
    // false = don't skip
    return false;
}

EXPORT void onModuleLoaded() {
    // called when the shared library of Riru core is loaded
}