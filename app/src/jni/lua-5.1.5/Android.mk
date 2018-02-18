LOCAL_PATH:=$(call my-dir)

LOCAL_MULTILIB := "both"
LOCAL_MODULE   := liblua

LOCAL_C_INCLUDES := $(LOCAL_PATH)/src $(LOCAL_PATH)

CORE_O=	lapi.c lcode.c ldebug.c ldo.c ldump.c lfunc.c lgc.c llex.c lmem.c \
	lobject.c lopcodes.c lparser.c lstate.c lstring.c ltable.c ltm.c  \
	lundump.c lvm.c lzio.c
LIB_O=	lauxlib.c lbaselib.c ldblib.c liolib.c lmathlib.c loslib.c ltablib.c \
	lstrlib.c loadlib.c linit.c


LOCAL_SRC_FILES := $(addprefix $(LOCAL_PATH)/src/,$(CORE_O)) \
		$(addprefix $(LOCAL_PATH)/src/,$(LIB_O))


include $(BUILD_STATIC_LIBRARY)

