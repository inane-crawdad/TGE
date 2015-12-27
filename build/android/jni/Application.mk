APP_ABI := all
APP_PLATFORM := android-9

APP_CPPFLAGS += --std=c++11 -fexceptions

APP_STL := gnustl_shared

MY_PATH := $(call my-dir)
NDK_APP_OUT := $(MY_PATH)/../../../bin/android/
NDK_APP_LIBS_OUT := $(MY_PATH)/../../../bin/android/