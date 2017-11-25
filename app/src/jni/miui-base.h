#ifndef MIUI_BASE_H
#define MIUI_BASE_H

#include <stdio.h>
#include <dlfcn.h>
#include <android/log.h>


#define PROP_VALUE_MAX 92
#define LOG_TAG "MINIGUI"
#define ALOG(TAG, LEVEL, ...) do {__android_log_print(ANDROID_LOG_##LEVEL, TAG, __VA_ARGS__); } while(0)
#define LOGD(...) ALOG(LOG_TAG, DEBUG, __VA_ARGS__)

#define ALOGI(TAG,...) ALOG(TAG, INFO, __VA_ARGS__)
#define ALOGD(TAG,...) ALOG(TAG, DEBUG, __VA_ARGS__)
#define ALOGE(TAG,...) ALOG(TAG, ERROR, __VA_ARGS__)
#define ALOGV(TAG,...) ALOG(TAG, VERBOSE, __VA_ARGS__)

#define printf(fmt, ...)  ALOGI("MiniGUI", fmt, #__VA_ARGS__)
#define fprintf   android_fprintf


#ifdef __cplusplus
extern "C"
#else
extern 
#endif
int android_fprintf(FILE* fd, const char* fmt, ...);

#endif

