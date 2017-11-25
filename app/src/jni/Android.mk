BASE_PATH := $(call my-dir)

include $(BASE_PATH)/third_party/zlib/Android.mk
include $(BASE_PATH)/third_party/libpng-1.2.37/Android.mk
include $(BASE_PATH)/third_party/icu4c/Android.mk

LOCAL_PATH:=$(BASE_PATH)
include $(LOCAL_PATH)/Android.v8.mk

include $(CLEAR_VARS)
include $(LOCAL_PATH)/minigui/source.mk
include $(LOCAL_PATH)/mgutils/source.mk
include $(LOCAL_PATH)/mgncs/source.mk

LOCAL_MULTILIB := "both"
LOCAL_MODULE    := minigui

glue_sources= glue/glue_common.cpp \
			  glue/v8_glue.cpp

LOCAL_SRC_FILES := $(minigui_sources) \
	$(mgutils_sources) \
	$(mgncs_sources) \
	$(glue_sources) \
	mg-res/mgncs_etc.c \
	bitmap-video.c \
	minigui-jni.cpp \
	get_icudata_name.cpp

LOCAL_CPP_FEATURES := rtti
#$(error $(LOCAL_SRC_FILES))

debug_mode:=-D_DEBUG -DEBUG_VIDEO

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/mgincludes \
	$(minigui_includes) \
	$(mgutils_includes) \
	$(mgncs_includes) \
	$(LOCAL_PATH)/third_party/libpng-1.2.37 \
	$(V8_INCLUDES)

LOCAL_CFLAGS += -Wall -Wno-unused-function \
				-Wno-unused-variable -O3 \
				-funroll-loops -ftree-vectorize \
				-ffast-math -fpermissive \
				$(minigui_cflags) \
				$(mgncs_cflags) \
				$(debug_mode) \
				-Wall -Wno-unused-function -Wno-unused-variable \
				-O3 -funroll-loops -ftree-vectorize -ffast-math -fpermissive -fpic -D__STDINT_LIMITS

LOCAL_STATIC_LIBRARIES := libpng_static libz_static libicuuc_static libicui18n_static libv8
LOCAL_LDFLAGS = -llog -ljnigraphics -latomic

include $(BUILD_SHARED_LIBRARY)

