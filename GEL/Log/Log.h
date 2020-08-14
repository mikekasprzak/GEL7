#pragma once

#ifdef __ANDROID__
#include "Log_Android.h"
#elif defined(__NX__)
#include "Log_NX.h"
#elif USE_NO_LOGGING
#include "Log_none.h"
#else // PLATFORM
#include "Log_printf.h"
#endif // PLATFORM

#define FLog(msg, ...)			Log("[%s] " msg, __func__, ## __VA_ARGS__)
