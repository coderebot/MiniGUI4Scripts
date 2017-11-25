
#include <jni.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <android/bitmap.h>
#include "common.h"
#include "minigui.h"
#include "gdi.h"
#include "window.h"
#include "mgncs.h"
#include "android-backend.h"
#include "miui-base.h"

JavaVM * g_jvm = NULL;

extern "C" GHANDLE __mgncs_get_mgetc (void);

#define ItemListClass "com/example/dongjunjie/myapplication/ItemListActivity"

extern "C" void* get_surface_buffer(int *pw, int *ph);
extern "C" int startMiniGUI(int, int);
static int ial_fd_write;

static void render_updateBitmap(JNIEnv* env, jclass, jobject bitmap) {
    AndroidBitmapInfo bmpInfo;
    memset(&bmpInfo, 0, sizeof(bmpInfo));

    AndroidBitmap_getInfo(env, bitmap, &bmpInfo);

    if (bmpInfo.format != ANDROID_BITMAP_FORMAT_RGB_565) {
        ALOGE("RENDER", "Only ARGB_8888 bitmap support");
        return ;
    }

    int height = bmpInfo.height;
    int width  = bmpInfo.width;

    void *ppixels = NULL;

    AndroidBitmap_lockPixels(env, bitmap, &ppixels);

    unsigned char* pbuff = (unsigned char*)ppixels;

    int sw, sh;
    void *pixel = get_surface_buffer(&sw, &sh);
    if (pixel != NULL) {
        int start = 0;
        for (int i = 0; i < sh; i++) {
            memcpy(pbuff + start, (char*)pixel + start , sw);
            start += sw;
        }
    }

    AndroidBitmap_unlockPixels(env, bitmap);
}

static void drawRect(HDC hdc, int x, int y, int w, int h, DWORD color) {
    SetBrushColor(hdc, DWORD2PIXEL(hdc, color));
    FillBox(hdc, x, y, w, h);

}


#include "test-button.c"
static jlong jni_startMiniGUI(JNIEnv* env, jclass, jint width, jint height, jobject strarry) {
    if(!startMiniGUI(width, height)) {
        ALOGE("MiniGUI", "start minigui failed");
    }

    g_ncsEtcHandle = __mgncs_get_mgetc();
    ncsInitialize();

    return (jlong)create_button_dialog();
}

static jboolean jni_processMessage(JNIEnv* env, jclass, jlong hwnd) {
    MSG msg;
    HWND hMainWnd = (HWND)hwnd;
	if(hMainWnd!=HWND_DESKTOP && !(hMainWnd!=(HWND)0 && IsMainWindow(hMainWnd)))
        return false;

    if (!GetMessage(&msg, hMainWnd))
        return false;

    TranslateMessage(&msg);
    DispatchMessage(&msg);
    return true;
}

static void jni_updateTouchEvent(JNIEnv* env, jclass, jint x, jint y, jint button) {
    EventData ed;
    ed.x = x;
    ed.y = y;
    ed.button = button;
    if (ial_fd_write > 0) {
        write(ial_fd_write, &ed, sizeof(ed));
    }
}


static JNINativeMethod _render_methods[] {
    {"startMiniGUI", "(II[Ljava/lang/String;)J",
        (void*)(jni_startMiniGUI)},
    {"updateGAL", "(Landroid/graphics/Bitmap;)V",
        (void*)(render_updateBitmap)},
    {"processMessage", "(J)Z",
        (void*)(jni_processMessage)},
    {"updateTouchEvent", "(III)V",
        (void*)(jni_updateTouchEvent)},
};

static bool InitRender(JNIEnv* env) {
    jclass clazz = env->FindClass(ItemListClass);
    if (env->RegisterNatives(clazz, _render_methods, sizeof(_render_methods)/sizeof(_render_methods[0]))) {
        return false;
    }

    ial_fd_write = create_ial_fd();

    return true;
}

jint JNI_OnLoad(JavaVM* vm, void*) {
    JNIEnv* env = NULL;
    jint result = -1;

    g_jvm = vm;

    if (vm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
        ALOGE("RENDER", "intialize failed");
        return result;
    }

    InitRender(env);


    return JNI_VERSION_1_4;
}

int android_fprintf(FILE* f, const char* fmt, ...) {
    int ret = -1;
    va_list args;
    va_start(args, fmt);
    if (f == stderr || f == stdout) {
        char szbuf[1024];
        ret = vsnprintf(szbuf, sizeof(szbuf), fmt, args);
        if (f == stderr) {
            ALOGE("MiniGUI", "%s", szbuf);
        } else {
            ALOGI("MiniGUI", "%s", szbuf);
        }
    } else {
        ret = vfprintf(f, fmt, args);
    }

    va_end(args);
    return ret;
}

