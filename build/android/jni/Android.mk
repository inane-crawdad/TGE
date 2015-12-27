LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

MY_SRC_PATH = $(LOCAL_PATH)/../../../src/engine

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../include
LOCAL_MODULE    := tge
LOCAL_SRC_FILES := $(subst $(LOCAL_PATH)/,, $(wildcard $(MY_SRC_PATH)/*.cpp)) \
				   $(subst $(LOCAL_PATH)/,, $(wildcard $(MY_SRC_PATH)/platform/*.cpp)) \
				   $(subst $(LOCAL_PATH)/,, $(wildcard $(MY_SRC_PATH)/platform/android/*.cpp)) \
				   $(subst $(LOCAL_PATH)/,, $(wildcard $(MY_SRC_PATH)/MobileInput/*.cpp))

LOCAL_LDLIBS    := -landroid -lEGL -lGLESv2
LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)