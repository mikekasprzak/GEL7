#pragma once

#include <GEL/Types/Types.h>
#include <GEL/Time/Time.h>

typedef s64 MAINFrame;

#define MAIN_FRAME_RATE		    (60)
#define MAIN_FRAME_LENGTH       (GEL_ONE_SECOND / MAIN_FRAME_RATE)


struct MainState;
extern MainState* mainState;

void main_Focus();
void main_Blur();
void main_Resize();

MAINFrame main_FrameGet();
MAINFrame main_DrawFrameGet();

int main_MSUntilNextFrame();
