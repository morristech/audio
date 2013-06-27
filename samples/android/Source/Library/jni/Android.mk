LOCAL_PATH := $(call my-dir)

# AllJoyn specifics
ALLJOYN_DIST := ../../../../../build/android/arm/$(APP_OPTIM)/dist
AUDIO_SERVICE_PATH := ../../../../..
ABOUT_SERVICE_PATH := ../../../../../../about

include $(CLEAR_VARS)
 
LOCAL_MODULE := easy_alljoyn_audio_service

TARGET_PLATFORM := android-8

LOCAL_C_INCLUDES := \
	$(ALLJOYN_DIST)/cpp/inc \
	$(LOCAL_PATH)/$(AUDIO_SERVICE_PATH)/inc \
	$(LOCAL_PATH)/$(ABOUT_SERVICE_PATH)/cpp/inc \
	$(NDK_PLATFORMS_ROOT)/$(TARGET_PLATFORM)/arch-arm/usr/include \
	$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/include \
	$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/libs/armeabi/include \
	$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/4.6/include \
	$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/4.6/libs/armeabi/include

LOCAL_CFLAGS := -Wno-psabi -Wno-write-strings -DANDROID_NDK -DTARGET_ANDROID -DLINUX -DQCC_OS_GROUP_POSIX -DQCC_OS_ANDROID -DQCC_CPU_ARM -DANDROID

LOCAL_DEFAULT_CPP_EXTENSION := cc 

LOCAL_SRC_FILES := \
	AndroidJNIBridge.cc \
	MyAllJoynCode.cc \
	$(AUDIO_SERVICE_PATH)/src/AudioCodec.cc \
	$(AUDIO_SERVICE_PATH)/src/RawCodec.cc \
	$(AUDIO_SERVICE_PATH)/src/WavDataSource.cc \
	$(AUDIO_SERVICE_PATH)/src/SinkSearcher.cc \
	$(AUDIO_SERVICE_PATH)/src/SinkPlayer.cc \
	$(ABOUT_SERVICE_PATH)/cpp/src/AboutClient.cc

LOCAL_LDLIBS := \
	-L$(NDK_PLATFORMS_ROOT)/$(TARGET_PLATFORM)/arch-arm/usr/lib \
	-L$(ALLJOYN_DIST)/cpp/lib \
    -L$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/libs/armeabi \
    -L$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/4.6/libs/armeabi \
    $(ALLJOYN_DIST)/cpp/lib/BundledDaemon.o \
	-lajdaemon -lalljoyn -llog -lz -ldl -lssl -lcrypto -lm -lc -lstdc++ -lgcc -lgnustl_static

LOCAL_ARM_MODE := arm

include $(BUILD_SHARED_LIBRARY)
