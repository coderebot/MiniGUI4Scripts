BASE_PATH := $(call my-dir)

include $(BASE_PATH)/third_party/zlib/Android.mk
include $(BASE_PATH)/third_party/libpng-1.2.37/Android.mk
include $(BASE_PATH)/third_party/icu4c/Android.mk
include $(BASE_PATH)/Python-2.7.14/Android.mk

LOCAL_PATH:=$(BASE_PATH)
include $(LOCAL_PATH)/Android.v8.mk

include $(CLEAR_VARS)
include $(LOCAL_PATH)/minigui/source.mk
include $(LOCAL_PATH)/mgutils/source.mk
include $(LOCAL_PATH)/mgncs/source.mk

LOCAL_MULTILIB := "both"
LOCAL_MODULE    := minigui

GLUE_PATH:=$(LOCAL_PATH)/glue
TOOL_PATH:=$(LOCAL_PATH)/tools
gen_source_dummy:=$(GLUE_PATH)/dummy.cpp

TOOLS:=$(TOOL_PATH)/genv8code.py $(TOOL_PATH)/parsewidgets.py

WIDGETS_DEFINES:=$(wildcard $(GLUE_PATH)/widgets/*.xml) $(GLUE_PATH)/widgets/compund-type.def

$(gen_source_dummy): $(TOOLS) $(GLUE_PATH)/widgets.json $(WIDGETS_DEFINES)
	echo $(GLUE_PATH)
	python $(TOOL_PATH)/genv8code.py $(GLUE_PATH)/widgets.json $(GLUE_PATH)/widget_methods_define.cpp
	python $(TOOL_PATH)/parsewidgets.py $(GLUE_PATH)/widgets $(GLUE_PATH)/widget_classes_list.cpp
	echo > $(gen_source_dummy)

glue_sources= $(gen_source_dummy) \
		   	  glue/glue_common.cpp \
			  glue/v8_glue.cpp \
			  glue/glue_utils.cpp

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

LOCAL_STATIC_LIBRARIES := libpng_static libz_static libicuuc_static libicui18n_static libv8 libpython_static
LOCAL_LDFLAGS = -llog -ljnigraphics -latomic

$(warning $(gen_source_dummy))
include $(BUILD_SHARED_LIBRARY)

