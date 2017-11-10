
# Uncomment this if you're using STL in your project
# See CPLUSPLUS-SUPPORT.html in the NDK documentation for more information
# APP_STL := stlport_static

APP_ABI := armeabi armeabi-v7a x86 arm64-v8a
#APP_STL := gnustl_static
APP_STL := c++_static
APP_CPPFLAGS += -std=c++11

# Fixes issues with clang compiler
NDK_TOOLCHAIN_VERSION = 4.9
