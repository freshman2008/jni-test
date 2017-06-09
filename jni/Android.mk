LOCAL_PATH := $(call my-dir) 
include $(CLEAR_VARS)

LOCAL_MODULE    		:= test_jni
LOCAL_SRC_FILES			:= TestJNIInterface.cpp 
LOCAL_STATIC_LIBRARIES	:= breakpad_client
LOCAL_LDLIBS    		:= -llog 	# call __android_log_print in c++ code need add this line
include $(BUILD_SHARED_LIBRARY) 

$(call import-add-path, $(LOCAL_PATH))
$(call import-module, google-breakpad/android/google_breakpad)
