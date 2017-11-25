
#V8_VERSION=3.29
#V8_VERSION=3.30
#V8_VERSION=4.4
#V8_VERSION=4.5
#V8_VERSION=4.6
#V8_VERSION=4.7
#V8_VERSION=4.8
#V8_VERSION=4.9
V8_VERSION=5.0
#V8_VERSION=5.1
#V8_VERSION=5.2
#V8_VERSION=5.5

V8_PATH=$(LOCAL_PATH)/v8-$(V8_VERSION)

V8_INCLUDES=$(V8_PATH) \
			$(V8_PATH)/include \
			$(V8_PATH)/src
V8_LIBPATH=$(V8_PATH)/obj/local/$(TARGET_ARCH_ABI)

generated_sources := $(TARGET_OBJS)

$(shell $(LOCAL_PATH)/tools/v8gensource.py $(V8_VERSION) $(LOCAL_PATH))

include $(LOCAL_PATH)/v8src-$(V8_VERSION).mk

#<4.4
ifneq ($(shell expr $(V8_VERSION) \< 4.4), 0)
JS2C_PY_FLAGS:=off
endif

ifneq ($(shell expr $(V8_VERSION) \>= 4.9), 0)
V8_INCLUDES += $(LOCAL_PATH)/third_party
endif


V8_SRC_FILES:= \
	$(v8_base_src) \
	$(v8_base_$(TARGET_ARCH)_src) \
	$(v8_libbase_src) \
	$(v8_libbase_$(TARGET_ARCH)_src) \
	$(v8_nosnapshot_src) \
	$(v8_libplatform_src) \
	$(v8_libsampler_src)

ifneq ($(shell expr $(V8_VERSION) \>= 5.5), 0)
V8_SRC_FILES += src/interpreter/bytecode-peephole-table.cc
endif

V8_SRC_FILES:=$(addprefix $(V8_PATH)/, $(V8_SRC_FILES))


LOCAL_JS_LIBRARY_FILES := $(addprefix $(V8_PATH)/, $(js2c_src) $(js2c_$(TARGET_ARCH)_src))

LOCAL_JS_EXPERIMENTAL_LIBRARY_FILES := $(addprefix $(V8_PATH)/, $(js2c_experimental_src) $(js2c_$(TARGET_ARCH)_experimental_src))

# Copy js2c.py to generated sources directory and invoke there to avoid
# generating jsmin.pyc in the source directory
JS2C_PY := $(generated_sources)/js2c.py $(generated_sources)/jsmin.py
$(JS2C_PY): $(generated_sources)/%.py : $(V8_PATH)/tools/%.py
	echo "Copying $@"
	mkdir -p $(dir $@)
	cp -fp $< $@

# Generate libraries.cc
GEN1 := $(generated_sources)/libraries.cc
$(GEN1): SCRIPT := $(generated_sources)/js2c.py
$(GEN1): $(LOCAL_JS_LIBRARY_FILES) $(JS2C_PY)
	@echo "Generating libraries.cc"
	@mkdir -p $(dir $@)
	python $(SCRIPT) $@ CORE $(JS2C_PY_FLAGS) $(LOCAL_JS_LIBRARY_FILES)
V8_GENERATED_LIBRARIES := $(generated_sources)/libraries.cc

# Generate experimental-libraries.cc
GEN2 := $(generated_sources)/experimental-libraries.cc
$(GEN2): SCRIPT := $(generated_sources)/js2c.py
$(GEN2): $(LOCAL_JS_EXPERIMENTAL_LIBRARY_FILES) $(JS2C_PY)
	@echo "Generating experimental-libraries.cc"
	@mkdir -p $(dir $@)
	python $(SCRIPT) $@ EXPERIMENTAL $(JS2C_PY_FLAGS) $(LOCAL_JS_EXPERIMENTAL_LIBRARY_FILES)
V8_GENERATED_LIBRARIES += $(generated_sources)/experimental-libraries.cc


ifneq ($(shell expr $(V8_VERSION) \>= 4.4), 0)
# Generate extra-libraries.cc
GEN3 := $(generated_sources)/extra-libraries.cc
$(GEN3): SCRIPT := $(generated_sources)/js2c.py
$(GEN3): $(JS2C_PY)
	@echo "Generating extra-libraries.cc"
	@mkdir -p $(dir $@)
	python $(SCRIPT) $@ EXTRAS
V8_GENERATED_LIBRARIES += $(generated_sources)/extra-libraries.cc
endif


ifneq ($(shell expr $(V8_VERSION) \>= 4.7), 0)
# Generate iexperimental-extra-libraries.cc
GEN4 := $(generated_sources)/experimental-extra-libraries.cc
$(GEN4): SCRIPT := $(generated_sources)/js2c.py
$(GEN4): $(JS2C_PY)
	@echo "Generating experimental-extra-libraries.cc"
	@mkdir -p $(dir $@)
	python $(SCRIPT) $@ EXPERIMENTAL_EXTRAS
V8_GENERATED_LIBRARIES += $(generated_sources)/experimental-extra-libraries.cc
endif

ifneq ($(shell expr $(V8_VERSION) \>= 4.6), 0)
ifneq ($(shell expr $(V8_VERSION) \<= 4.8), 0)
LOCAL_CODE_STUB_JS_LIBRARY_FILES:=$(addprefix $(V8_PATH)/, $(js2c_code_stub_src) $(js2c_$(TARGET_ARCH)_code_stub_src))
GEN5 := $(generated_sources)/codestub-libraries.cc
$(GEN5) : SCRIPT := $(generated_sources)/js2c.py
$(GEN5): $(JS2C_PY)
	@echo "Generating codestub-libraries.cc"
	@mkdir -p $(dir $@)
	python $(SCRIPT) $@ CODE_STUB $(LOCAL_CODE_STUB_JS_LIBRARY_FILES)
V8_GENERATED_LIBRARIES += $(generated_sources)/codestub-libraries.cc

endif
endif



V8_SRC_FILES += $(V8_GENERATED_LIBRARIES)


V8_CFLAGS =  \
	-Wno-endif-labels \
	-Wno-import \
	-Wno-format \
	-fno-exceptions \
	-fvisibility=hidden \
	-DENABLE_DEBUGGER_SUPPORT \
	-DENABLE_LOGGING_AND_PROFILING \
	-DENABLE_VMSTATE_TRACKING \
	-DV8_NATIVE_REGEXP \
	-DV8_SHARED \
	-DBUILDING_V8_SHARED \
	-DV8_OS_ANDROID \
	-DV8_I18N_SUPPORT \
	-D__STDINT_LIMITS \
	-std=gnu++0x \
	-Wno-unused-parameter \
	-O3



V8_CFLAGS_arm += -DV8_TARGET_ARCH_ARM
V8_CFLAGS_arm64 += -DV8_TARGET_ARCH_ARM64

# atomicops_internals_arm64_gcc.h:77:49: error:
# expected compatible register, symbol or integer in range [0, 4095]
LOCAL_CLANG_CFLAGS_arm64 += -no-integrated-as

V8_CFLAGS_mips += -DV8_TARGET_ARCH_MIPS \
	-Umips \
	-finline-limit=64 \
	-fno-strict-aliasing
V8_CFLAGS_mips64 += -DV8_TARGET_ARCH_MIPS64 \
	-Umips \
	-finline-limit=64 \
	-fno-strict-aliasing

V8_CFLAGS_x86 += -DV8_TARGET_ARCH_IA32
V8_CFLAGS_x86_64 += -DV8_TARGET_ARCH_X64

V8_CFLAGS += $(V8_CFLAGS_$(TARGET_ARCH))

#########################################################
include $(CLEAR_VARS)

LOCAL_CXX_STL := libc++
LOCAL_CPP_FEATURES := rtti

LOCAL_CPP_EXTENSION := .cc

LOCAL_MULTILIB := both

LOCAL_C_INCLUDES := $(V8_INCLUDES) \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/third_party \
	$(LOCAL_PATH)/third_party/icu4c/source/common \
	$(LOCAL_PATH)/third_party/icu4c/source/i18n

LOCAL_SRC_FILES := $(V8_SRC_FILES)

LOCAL_CFLAGS := $(V8_CFLAGS)

LOCAL_MODULE:=libv8

LOCAL_STATIC_LIBRARIES := libicuuc_static libicui18n_static

include $(BUILD_STATIC_LIBRARY)

