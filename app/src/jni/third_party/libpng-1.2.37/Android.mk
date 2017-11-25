
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE    := libpng_static

LOCAL_SRC_FILES:= png.c pngset.c pngget.c pngrutil.c pngtrans.c pngwutil.c \
	pngread.c pngrio.c pngwio.c pngwrite.c pngrtran.c \
	pngwtran.c pngmem.c pngerror.c pngpread.c

LOCAL_C_INCLUDES:=$(LOCAL_PATH)

LOCAL_CFLAGS+= -Os
LOCAL_LDFLAGS+=

include $(BUILD_STATIC_LIBRARY)

