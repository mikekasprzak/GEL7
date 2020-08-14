#pragma once

#include <android/log.h>

#ifndef APPNAME
#define APPNAME "MyApp"
#endif // APPNAME

#define Log(...)			__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, ## __VA_ARGS__);
#define Error(...)			Log(__VA_ARGS__)
