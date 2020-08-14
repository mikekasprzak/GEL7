#include "Time.h"
#include <GEL/GEL_SDL2.h>

GELTime gel_TimeGet() {
#ifdef USES_SDL2
    return (GELTime)SDL_GetTicks() * 1000ull;
#else // USES_UNKNOWN
    return 0;
#endif // USES_TIME
}
