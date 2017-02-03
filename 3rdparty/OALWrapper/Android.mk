LOCAL_PATH := $(call my-dir)

include $(call all-subdir-makefiles)
include $(CLEAR_VARS)

LOCAL_MODULE := OALWrapper

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include

LOCAL_EXPORT_C_INCLUDES += $(LOCAL_C_INCLUDES)

LOCAL_SRC_FILES := $(subst $(LOCAL_PATH)/,, \
	$(wildcard $(LOCAL_PATH)/sources/*.cpp))

LOCAL_SHARED_LIBRARIES := SDL2 vorbis ogg openal
LOCAL_CFLAGS += -fpermissive
LOCAL_LDLIBS := -landroid -latomic


include $(BUILD_SHARED_LIBRARY)
