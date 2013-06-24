LOCAL_PATH := $(call my-dir)

# AllJoyn specifics
ALLJOYN_ROOT := ../../../../..
ABOUT_ROOT := $(ALLJOYN_ROOT)/services/about
ALLJOYN_DIST := $(ALLJOYN_ROOT)/services/audio/build/android/arm/$(VARIANT)/dist

include $(CLEAR_VARS)

LOCAL_MODULE := libAllJoynAudioSink

TARGET_PLATFORM := android-8

LOCAL_C_INCLUDES := \
	$(ALLJOYN_DIST)/cpp/inc \
	$(ALLJOYN_DIST)/audio/inc \
	$(ABOUT_ROOT)/cpp/inc \
	$(ALLJOYN_ROOT)/common/inc \
	$(ALLJOYN_DIST)/cpp/inc/alljoyn \
	$(NDK_PLATFORMS_ROOT)/$(TARGET_PLATFORM)/arch-arm/usr/include \
	$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/include \
	$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/libs/armeabi/include \
	$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/4.6/include \
	$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/4.6/libs/armeabi/include

LOCAL_CFLAGS := -Wno-psabi -Wno-write-strings -DANDROID_NDK -DTARGET_ANDROID -DLINUX -DQCC_OS_GROUP_POSIX -DQCC_OS_ANDROID -DQCC_CPU_ARM -DANDROID

LOCAL_CPP_EXTENSION := .cc 

LOCAL_SRC_FILES := \
	AndroidJNIBridge.cc \
	MyAllJoynListeners.cc \
	MyAllJoynCode.cc

LOCAL_LDLIBS := \
	-L$(NDK_PLATFORMS_ROOT)/$(TARGET_PLATFORM)/arch-arm/usr/lib \
	-L$(ALLJOYN_DIST)/cpp/lib \
	-L$(ALLJOYN_DIST)/audio/lib \
    -L$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/libs/armeabi \
    -L$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/4.6/libs/armeabi \
	-lajdaemon -lalljoyn -llog -lz -ldl -lssl -lcrypto -lm -lc -lstdc++  -lgcc -lgnustl_static -lOpenSLES -lalljoyn_audio

LOCAL_ARM_MODE := arm

include $(BUILD_SHARED_LIBRARY)
