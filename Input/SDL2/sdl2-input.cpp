#include <GEL/GEL.h>
#include <GEL/GEL_Containers.h>
#include <Input/Input.h>

#define INPUT_NEW						(0)
#define INPUT_OLD						(1)

GameInputState* volatileInputState = nullptr;				// modified by Events
GameInputState* inputState[2] = {nullptr, nullptr};			// snapshots of the volatile state
GameInputState* weightedInputState[2] = {nullptr, nullptr};	// weighted towards an index (effectively pressed/released)


void input_Init() {
	FLog("+");

	volatileInputState = new GameInputState;
	raw_Zero(volatileInputState, sizeof(GameInputState));

	inputState[INPUT_NEW] = new GameInputState;
	raw_Zero(inputState[INPUT_NEW], sizeof(GameInputState));
	inputState[INPUT_OLD] = new GameInputState;
	raw_Zero(inputState[INPUT_OLD], sizeof(GameInputState));
	weightedInputState[INPUT_NEW] = new GameInputState;
	raw_Zero(weightedInputState[INPUT_NEW], sizeof(GameInputState));
	weightedInputState[INPUT_OLD] = new GameInputState;
	raw_Zero(weightedInputState[INPUT_OLD], sizeof(GameInputState));

	FLog("-");
}
void input_Exit() {
	FLog("+");

	delete weightedInputState[INPUT_OLD];
	delete weightedInputState[INPUT_NEW];
	delete inputState[INPUT_OLD];
	delete inputState[INPUT_NEW];
	delete volatileInputState;

	FLog("-");	
}


void inputInternal_SwapInputStates() {
	GameInputState* temp = inputState[INPUT_NEW];
	inputState[INPUT_NEW] = inputState[INPUT_OLD];
	inputState[INPUT_OLD] = temp;
}


void input_Step() {
	// New becomes old, and the old-old we're about to overwrite
	inputInternal_SwapInputStates();																								// a <swap> b

	// Snapshot the volatile state ovrwriting new
	raw_Copy(volatileInputState, sizeof(GameInputState), inputState[INPUT_NEW], sizeof(GameInputState));							// b = a

	// Generate raw bitmasks of the entire input data. Most of it is flags anyway
	raw_BitwiseXOR(inputState[INPUT_NEW], inputState[INPUT_OLD], weightedInputState[INPUT_NEW], sizeof(GameInputState)); 			// c = a ^ b
	raw_BitwiseAND(weightedInputState[INPUT_NEW], inputState[INPUT_OLD], weightedInputState[INPUT_OLD], sizeof(GameInputState));	// c = a & b
	raw_BitwiseAND(inputState[INPUT_NEW], weightedInputState[INPUT_NEW], sizeof(GameInputState));									// b = a & b
}


const GameInputState& input_Get() {
	return *inputState[INPUT_NEW];
}
const GameInputState& input_GetOld() {
	return *inputState[INPUT_OLD];
}
const GameInputState& input_GetPressed() {
	return *weightedInputState[INPUT_NEW];
}
const GameInputState& input_GetReleased() {
	return *weightedInputState[INPUT_OLD];
}
const GameInputState& input_GetVolatile() {
	return *volatileInputState;
}
