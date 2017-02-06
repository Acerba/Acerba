LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

Acerba_PATH := ../acerba
SDL_PATH := ../acerba/3rdparty/SDL2

#LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include
LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(Acerba_PATH)/include $(LOCAL_EXPORT_C_INCLUDES)

# Add your application source files here...
#LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
	main.cpp

LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c\
					main.cpp
LOCAL_SHARED_LIBRARIES := Acerba SDL2 SDL2main OALWrapper
LOCAL_CFLAGS += -fpermissive
LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog -latomic

include $(BUILD_SHARED_LIBRARY)
