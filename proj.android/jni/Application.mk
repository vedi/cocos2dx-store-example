APP_STL := gnustl_static
APP_CPPFLAGS := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -DCOCOS2D_DEBUG=1 -DSOOMLA_DEBUG=1
APP_CFLAGS += -Wno-error=format-security

APP_ABI := armeabi
APP_PLATFORM := android-8
