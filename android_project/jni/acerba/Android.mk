PATH := $(call my-dir)

include $(call all-subdir-makefiles)

#LOCAL_PATH := $(call my-dir)
LOCAL_PATH := $(PATH)

include $(CLEAR_VARS)

LOCAL_MODULE := Acerba

$(warning $(PATH))

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include $(LOCAL_PATH)/3rdparty/stb \
					$(LOCAL_PATH)/3rdparty/Khronos \
					$(LOCAL_PATH)/3rdparty/SDL2/include\
					$(LOCAL_PATH)/3rdparty/OALWrapper/include\

LOCAL_EXPORT_C_INCLUDES += $(LOCAL_C_INCLUDES)

LOCAL_SRC_FILES := $(subst $(LOCAL_PATH)/,, \
	$(wildcard $(LOCAL_PATH)/src/Ace/*.cpp))

LOCAL_SHARED_LIBRARIES := SDL2 OALWrapper
LOCAL_CFLAGS += -DGL_GLEXT_PROTOTYPES -DANDOIRD -DAO_GLES -fpermissive
LOCAL_LDLIBS := -ldl -lGLESv1_CM -lGLESv2 -llog -landroid -latomic

include $(BUILD_SHARED_LIBRARY)
