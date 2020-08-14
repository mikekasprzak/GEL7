#pragma once

#include <Game/Game.h>

extern GameInputState* volatileInputState;

void input_Init();
void input_Exit();

void input_Step();


const GameInputState& input_Get();
const GameInputState& input_GetOld();
const GameInputState& input_GetPressed();
const GameInputState& input_GetReleased();
const GameInputState& input_GetVolatile();      // you should only use this for debugging
