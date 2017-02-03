LOCAL_PATH := $(call my-dir)

include $(call all-subdir-makefiles)

include $(CLEAR_VARS)

LOCAL_MODULE := vorbis

OGG_PATH := ../ogg/include/

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include \
					$(LOCAL_PATH)/lib \
					$(LOCAL_PATH)/$(OGG_PATH)

LOCAL_EXPORT_C_INCLUDES += $(LOCAL_C_INCLUDES)

LOCAL_SRC_FILES := $(subst $(LOCAL_PATH)/,, \
	$(wildcard $(LOCAL_PATH)/lib/*.c) \
	$(wildcard $(LOCAL_PATH)/lib/*.h))

LOCAL_SRC_FILES := $(filter-out $(subst $(LOCAL_PATH)/,, $(LOCAL_PATH)/lib/psytune.c), $(LOCAL_SRC_FILES))
LOCAL_SRC_FILES := $(filter-out $(subst $(LOCAL_PATH)/,, $(LOCAL_PATH)/lib/tone.c), $(LOCAL_SRC_FILES))

LOCAL_SHARED_LIBRARIES := ogg


#LOCAL_CFLAGS +=

include $(BUILD_SHARED_LIBRARY)
