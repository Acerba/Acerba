ROOT_PATH := $(call my-dir)

########################################################################################################
include $(CLEAR_VARS)

LOCAL_MODULE     := openal
LOCAL_PATH       := $(ROOT_PATH)
LOCAL_C_INCLUDES := $(LOCAL_PATH) $(LOCAL_PATH)/include $(LOCAL_PATH)/OpenAL32/Include
LOCAL_SRC_FILES  := $(subst $(LOCAL_PATH)/,, \
					$(LOCAL_PATH)/OpenAL32/alAuxEffectSlot.c \
                    $(LOCAL_PATH)/OpenAL32/alBuffer.c        \
                    $(LOCAL_PATH)/OpenAL32/alEffect.c        \
                    $(LOCAL_PATH)/OpenAL32/alError.c         \
                    $(LOCAL_PATH)/OpenAL32/alExtension.c     \
                    $(LOCAL_PATH)/OpenAL32/alFilter.c        \
                    $(LOCAL_PATH)/OpenAL32/alListener.c      \
                    $(LOCAL_PATH)/OpenAL32/alSource.c        \
                    $(LOCAL_PATH)/OpenAL32/alState.c         \
                    $(LOCAL_PATH)/OpenAL32/alThunk.c         \
                    $(LOCAL_PATH)/Alc/ALc.c                  \
                    $(LOCAL_PATH)/Alc/alcConfig.c            \
                    $(LOCAL_PATH)/Alc/alcDedicated.c         \
                    $(LOCAL_PATH)/Alc/alcEcho.c              \
                    $(LOCAL_PATH)/Alc/alcModulator.c         \
                    $(LOCAL_PATH)/Alc/alcReverb.c            \
                    $(LOCAL_PATH)/Alc/alcRing.c              \
                    $(LOCAL_PATH)/Alc/alcThread.c            \
                    $(LOCAL_PATH)/Alc/ALu.c                  \
                    $(LOCAL_PATH)/Alc/bs2b.c                 \
                    $(LOCAL_PATH)/Alc/helpers.c              \
                    $(LOCAL_PATH)/Alc/hrtf.c                 \
                    $(LOCAL_PATH)/Alc/mixer.c                \
                    $(LOCAL_PATH)/Alc/panning.c              \
                    $(LOCAL_PATH)/Alc/backends/android.c     \
                    $(LOCAL_PATH)/Alc/backends/loopback.c    \
                    $(LOCAL_PATH)/Alc/backends/null.c        )
#                   $(LOCAL_PATH)/Alc/backends/opensl.c     \

LOCAL_CFLAGS     := -DAL_BUILD_LIBRARY -DAL_ALEXT_PROTOTYPES
LOCAL_LDLIBS     := -llog -Wl,-s

include $(BUILD_SHARED_LIBRARY)
