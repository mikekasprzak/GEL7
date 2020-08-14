#pragma once
#include <GEL/Types/Types.h>

typedef s64 GELTime;        // signed so we can do math
#define GEL_ONE_SECOND      (1000000ll)

GELTime gel_TimeGet();
