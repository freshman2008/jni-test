LOCAL_PATH := $(call my-dir) 

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/ \
$(LOCAL_PATH)/curl/$(TARGET_ARCH_ABI)/include/ \
$(LOCAL_PATH)/openssl/$(TARGET_ARCH_ABI)/include/ 

LOCAL_MODULE    		:= test_jni
LOCAL_SRC_FILES			:= TestJNIInterface.cpp \
						   util/MyLog.cpp
						   
LOCAL_STATIC_LIBRARIES	:= breakpad_client curl ssl crypto
#LOCAL_CPPFLAGS 			+= -fvisibility=hidden -mllvm -sub -mllvm -bcf -mllvm -fla
LOCAL_LDLIBS    		:= -llog 	# call __android_log_print in c++ code need add this line
include $(BUILD_SHARED_LIBRARY) 

$(call import-add-path, $(LOCAL_PATH))
$(call import-module, google-breakpad/android/google_breakpad)
$(call import-module,curl)
$(call import-module,openssl)
