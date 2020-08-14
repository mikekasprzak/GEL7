#pragma once

//#ifdef    __APPLE__
//#define USES_SDL2_EVENT_FILTER 1
//#endif // __APPLE__

#define USES_SDL2_POLL_EVENT 1

// If POLL_EVENT is explicitly enabled, disable the event filter
// #ifdef    USES_SDL2_POLL_EVENT
// #undef USES_SDL2_EVENT_FILTER
// #endif // USES_SDL2_POLL_EVENT

void event_Init();
void event_Exit();

bool event_Step();
