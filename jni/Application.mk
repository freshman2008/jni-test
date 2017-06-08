APP_ABI					:= armeabi-v7a x86
APP_PLATFORM			:= android-19
NDK_TOOLCHAIN_VERSION	:= clang
APP_STL               	:= gnustl_static
APP_CPPFLAGS         	:= -fexceptions -frtti --std=c++11 -Wno-extern-c-compat
LIBCXX_FORCE_REBUILD  	:= true
APP_OPTIM             	:= debug 
