#pragma once

void platform_Init(int argc, char* argv[]);
void platform_Exit();

struct PlatformState;
extern struct PlatformState* platformState;

int platform_WidthGet();
int platform_HeightGet();

void platform_Swap();
