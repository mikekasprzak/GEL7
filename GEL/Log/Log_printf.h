#pragma once

#include <stdio.h>

#define Log(...)			printf(__VA_ARGS__); printf("\n"); fflush(0)
#define Error(...)			Log(__VA_ARGS__)
