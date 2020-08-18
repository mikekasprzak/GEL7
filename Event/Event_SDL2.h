#pragma once

#include <Platform/Platform_SDL2.h>

int event_CriticalHandler(void* userData, SDL_Event *e);
int event_Handler(void* userData, SDL_Event *e);
