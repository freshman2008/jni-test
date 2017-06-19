APP_ABI					:= armeabi-v7a
APP_PLATFORM			:= android-19
NDK_TOOLCHAIN_VERSION	:= clang3.4-obfuscator
APP_STL               	:= gnustl_static
APP_CPPFLAGS         	:= -fexceptions -frtti --std=c++11 -Wno-extern-c-compat
LIBCXX_FORCE_REBUILD  	:= true
APP_OPTIM             	:= debug 
