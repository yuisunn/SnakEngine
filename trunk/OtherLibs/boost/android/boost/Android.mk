LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := boost_date_time
LOCAL_SRC_FILES := ../../lib_android_$(TARGET_ARCH_ABI)/lib/libboost_date_time-gcc-mt-s-1_47.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../

include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE := boost_filesystem
LOCAL_SRC_FILES := ../../lib_android_$(TARGET_ARCH_ABI)/lib/libboost_filesystem-gcc-mt-s-1_47.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../

include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE := program_options
LOCAL_SRC_FILES := ../../lib_android_$(TARGET_ARCH_ABI)/lib/libboost_program_options-gcc-mt-s-1_47.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../

include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE := boost_signals
LOCAL_SRC_FILES := ../../lib_android_$(TARGET_ARCH_ABI)/lib/libboost_signals-gcc-mt-s-1_47.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../

include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE := boost_system
LOCAL_SRC_FILES := ../../lib_android_$(TARGET_ARCH_ABI)/lib/libboost_system-gcc-mt-s-1_47.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../

include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE := boost_thread
LOCAL_SRC_FILES := ../../lib_android_$(TARGET_ARCH_ABI)/lib/libboost_thread_pthread-gcc-mt-s-1_47.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../

include $(PREBUILT_STATIC_LIBRARY)
