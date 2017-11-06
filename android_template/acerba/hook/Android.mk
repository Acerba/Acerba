LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#SDL_PATH := 3rdparty/SDL2 
#SDL_PATH := ../../../android/jni/SDL2
LIBS_PATH := ../../../android/libs

LOCAL_MODULE := acerba_hook

$(info $(BASE_PATH))

LOCAL_C_INCLUDES := $(BASE_PATH)/3rdparty/SDL2/include $(BASE_PATH)/3rdparty/matvek $(LOCAL_PATH)/include $(BASE_PATH)/3rdparty/OALWrapper/include $(BASE_PATH)/include $(LOCAL_EXPORT_C_INCLUDES)			

LOCAL_SRC_FILES := $(BASE_PATH)/3rdparty/SDL2/src/main/android/SDL_android_main.c\
		   hook.cpp
	
LOCAL_SHARED_LIBRARIES := acerba_pre main_pre SDL2_pre openal_pre OALWrapper_pre
LOCAL_CFLAGS += -fpermissive
LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog -latomic

include $(BUILD_SHARED_LIBRARY)