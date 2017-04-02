LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LIBS_PATH := ../../../android/libs

LOCAL_MODULE := acerba_pre
LOCAL_SRC_FILES := $(LIBS_PATH)/$(TARGET_ARCH_ABI)/libacerba.so

include $(PREBUILT_SHARED_LIBRARY)
