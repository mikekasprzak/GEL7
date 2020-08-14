#include <GEL/GEL.h>
#include <Event/Event.h>
#include <Event/Event_SDL2.h>
#include <Main/Main.h>
#include <Platform/Platform.h>
#include <Game/Game.h>
#include <Input/Input.h>
#include <GEL/GEL_SDL2.h>


struct EventState {
	bool alive;
};
EventState* eventState = 0;


void event_Init() {
	FLog("+");
	eventState = new EventState;
	eventState->alive = true;

#ifndef   USES_SDL2_POLL_EVENT
	SDL_SetEventFilter(event_CriticalHandler, 0);
	FLog("Events: using SDL_SetEventFilter");
#else
	FLog("Events: using SDL_PollEvent");
#endif // USES_SDL2_POLL_EVENT

	SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);
	FLog("Number of Joysticks: %i", SDL_NumJoysticks());

	for (int idx = 0; idx < SDL_NumJoysticks(); ++idx) {
		FLog("[%i] %s %s", idx, SDL_JoystickNameForIndex(idx), SDL_IsGameController(idx) ? "[GameController]" : "");
	}

	FLog("-");
}
void event_Exit() {
	FLog("+");

	SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);

	delete eventState;
	FLog("-");	
}


int event_CriticalHandler(void*, SDL_Event *event) {
	error_return_value_if(!event, 1);
	SDL_Event& e = *event;

	error_return_value_if(!eventState, 1);
	EventState& es = *eventState;

	switch (e.type) {
		// User Quit Event
		case SDL_QUIT: {
			FLog("SDL_QUIT");
			es.alive = false;
			return 0;
		}
		// Platform Quit Event (NOTE: Might never get called)
		case SDL_APP_TERMINATING: {
			FLog("SDL_APP_TERMINATING");
			es.alive = false;
			/* Terminate the app.
			Shut everything down before returning from this function.
			*/
			return 0;
		}
		case SDL_APP_LOWMEMORY: {
			FLog("SDL_APP_LOWMEMORY");
			/* You will get this when your app is paused and iOS wants more memory.
			Release as much memory as possible.
			*/
			return 0;
		}
		case SDL_APP_WILLENTERBACKGROUND: {
			FLog("SDL_APP_WILLENTERBACKGROUND");
			/* Prepare your app to go into the background.  Stop loops, etc.
			This gets called when the user hits the home button, or gets a call.
			*/
			return 0;
		}
		case SDL_APP_DIDENTERBACKGROUND: {
			FLog("SDL_APP_DIDENTERBACKGROUND");
			/* This will get called if the user accepted whatever sent your app to the background.
			If the user got a phone call and canceled it, you'll instead get an SDL_APP_DIDENTERFOREGROUND event and restart your loops.
			When you get this, you have 5 seconds to save all your state or the app will be terminated.
			Your app is NOT active at this point.
			*/

			// IMPORTANT!!! YOU MIGHT NEVER GET THE APP BACK! SAVE STUFF NOW!
			return 0;
		}
		case SDL_APP_WILLENTERFOREGROUND: {
			FLog("SDL_APP_WILLENTERFOREGROUND");
			/* This call happens when your app is coming back to the foreground.
			Restore all your state here.
			*/
			return 0;
		}
		case SDL_APP_DIDENTERFOREGROUND: {
			FLog("SDL_APP_DIDENTERFOREGROUND");
			/* Restart your loops here.
			Your app is interactive and getting CPU again.
			*/
			return 0;
		}
	};
	return 1;
}


int event_Handler(void*, SDL_Event *event) {
	error_return_value_if(!volatileInputState, 1);
	GameInputState& vis = *volatileInputState;

	error_return_value_if(!event, 1);
	SDL_Event& e = *event;

	//error_return_value_if(!eventState, 1);
	//EventState& es = *eventState;

	switch (e.type) {
		case SDL_MOUSEMOTION: {
			// Ignore touch events
			if (e.motion.which != SDL_TOUCH_MOUSEID) {
				//Log("[%s] Mouse Motion [%i]: %i, %i", __func__, e.motion.which, e.motion.x, e.motion.y);
				vis.pos.x = (float)e.motion.x / (float)platform_WidthGet();
				vis.pos.y = (float)e.motion.y / (float)platform_HeightGet();
				vis.info.isMouse = true;
			}
			return 0;
		}
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEBUTTONDOWN: {
			bool buttonState = (e.type == SDL_MOUSEBUTTONDOWN);

			// Any button, but ignore touches
			if (e.button.which != SDL_TOUCH_MOUSEID) {
				//Log("[%s] Mouse Button Up [%i]: %i, %i", __func__, e.button.which, e.button.x, e.button.y);
				vis.pos.x = (float)e.button.x / (float)platform_WidthGet();
				vis.pos.y = (float)e.button.y / (float)platform_HeightGet();
				vis.button.action = buttonState;
				vis.info.isMouse = true;
			}
			return 0;
		}
		case SDL_FINGERMOTION: {
			//Log("[%s] Finger Motion [%i][%i]: %f, %f [%f]", __func__, e.tfinger.touchId, e.tfinger.fingerId, e.tfinger.x, e.tfinger.y, e.tfinger.pressure);
			if ((e.tfinger.touchId == 1) && (e.tfinger.fingerId == 0)) {
				vis.pos.x = e.tfinger.x;
				vis.pos.y = e.tfinger.y;
				vis.info.isMouse = false;
			}
			return 0;
		}
		case SDL_FINGERUP:
		case SDL_FINGERDOWN: {
			bool fingerState = (e.type == SDL_FINGERDOWN);

			//Log("[%s] Finger Up [%i][%i]: %f, %f [%f]", __func__, e.tfinger.touchId, e.tfinger.fingerId, e.tfinger.x, e.tfinger.y, e.tfinger.pressure);
			if ((e.tfinger.touchId == 1) && (e.tfinger.fingerId == 0)) {
				vis.pos.x = e.tfinger.x;
				vis.pos.y = e.tfinger.y;
				vis.button.action = fingerState;
				vis.info.isMouse = false;
			}
			return 0;
		}
		case SDL_KEYUP:
		case SDL_KEYDOWN: {
			bool keyState = (e.type == SDL_KEYDOWN);

			// scancode stays the same if you change keyboard layouts
			// sym changes depending on keyboard layout
			//Log("[%s] Key %s [%i][%i]: %i %i", __func__, (e.type == SDL_KEYUP) ? "Up" : "Down", e.key.windowID, e.key.repeat, e.key.keysym.scancode, e.key.keysym.sym);
			if (e.key.repeat == 0) {
				switch (e.key.keysym.sym) {
					// Android back button: 270 (10E) 1073742094 (4000 010E)
					// SDL_SCANCODE_AC_BACK = 270
					// SDLK_AC_BACK = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_BACK)
					case SDLK_ESCAPE:
					case SDLK_AC_BACK:
					{
						vis.key.back = keyState;
						break;
					}
				};
			}
			return 0;
		}
		case SDL_WINDOWEVENT: {
			if (e.window.event == SDL_WINDOWEVENT_SHOWN) {
				// Seems to be triggered initially, desktop only
				//FLog("SDL_WINDOWEVENT_SHOWN");
			}
			else if (e.window.event == SDL_WINDOWEVENT_HIDDEN) {
				FLog("SDL_WINDOWEVENT_HIDDEN * IS THIS EVER CALLED?");
			}
			else if (e.window.event == SDL_WINDOWEVENT_EXPOSED) {
				// Seems to be triggered in addition to "SDL_WINDOWEVENT_FOCUS_GAINED"
				//FLog("SDL_WINDOWEVENT_EXPOSED");
			}
			else if (e.window.event == SDL_WINDOWEVENT_MOVED) {
				Log("[%s] SDL_WINDOWEVENT_MOVED: %i, %i", __func__, e.window.data1, e.window.data2);

				// Focus might need to trigger here too, but on Windows SDL_GetTime freezes while
				// moving the window.
				//main_Focus();
				//game_Focus();
			}
			else if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
				//Log("[%s] SDL_WINDOWEVENT_RESIZED: %i, %i", __func__, e.window.data1, e.window.data2);
				// NOTE: I'm not sure when this is called, but not the other (SIZE_CHANGED)
			}
			else if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {	// Resize, Maximize, etc
				Log("[%s] SDL_WINDOWEVENT_SIZE_CHANGED: %i, %i", __func__, e.window.data1, e.window.data2);
				game_Resize(e.window.data1, e.window.data2);
			}
			else if (e.window.event == SDL_WINDOWEVENT_MINIMIZED) {
				FLog("SDL_WINDOWEVENT_MINIMIZED [-]");
				game_Blur();
				main_Blur();
			}
			else if (e.window.event == SDL_WINDOWEVENT_MAXIMIZED) {
				Log("[%s] SDL_WINDOWEVENT_MAXIMIZED: %i, %i", __func__, e.window.data1, e.window.data2);
			}
			else if (e.window.event == SDL_WINDOWEVENT_RESTORED) {		// un-maximized
				// NOTE: Also triggered after un-maximizing
				FLog("SDL_WINDOWEVENT_RESTORED [+]");
				main_Focus();
				game_Focus();
			}
			else if (e.window.event == SDL_WINDOWEVENT_ENTER) {			// mouse focus
				//FLog("SDL_WINDOWEVENT_ENTER [MOUSE]");
				vis.info.hasHoverFocus = true;
				game_MouseFocus();
			}
			else if (e.window.event == SDL_WINDOWEVENT_LEAVE) {			// mouse focus lost
				//FLog("SDL_WINDOWEVENT_LEAVE [MOUSE]");
				vis.info.hasHoverFocus = false;
				game_MouseBlur();
			}
			else if (e.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {	// window/app focus
				FLog("SDL_WINDOWEVENT_FOCUS_GAINED");
				vis.info.hasWindowFocus = true;
			}
			else if (e.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {	// window/app focus lost
				FLog("SDL_WINDOWEVENT_FOCUS_LOST");
				vis.info.hasWindowFocus = false;
			}
			else if (e.window.event == SDL_WINDOWEVENT_CLOSE) {			// close button (desktop only)
				FLog("SDL_WINDOWEVENT_CLOSE");
			}
			else if (e.window.event == SDL_WINDOWEVENT_TAKE_FOCUS) {
				FLog("SDL_WINDOWEVENT_TAKE_FOCUS");
			}
			else if (e.window.event == SDL_WINDOWEVENT_HIT_TEST) {
				FLog("SDL_WINDOWEVENT_HIT_TEST");
			}
			return 0;
		}
		case SDL_CONTROLLERDEVICEADDED: {
			FLog("SDL_CONTROLLERDEVICEADDED: %i [%s]", e.cdevice.which, SDL_GameControllerNameForIndex(e.cdevice.which));
			return 0;
		}
		case SDL_CONTROLLERDEVICEREMOVED: {
			FLog("SDL_CONTROLLERDEVICEREMOVED: %i [%s]", e.cdevice.which, SDL_GameControllerNameForIndex(e.cdevice.which));
			return 0;
		}
		case SDL_CONTROLLERDEVICEREMAPPED: { // Steam notifying you of a controlle remap?
			FLog("SDL_CONTROLLERDEVICEREMAPPED: %i [%s]", e.cdevice.which, SDL_GameControllerNameForIndex(e.cdevice.which));
			return 0;
		}
	};
	return 1;
}


bool event_Step() {
	EventState& es = *eventState;
	
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
#ifdef USES_SDL2_POLL_EVENT
		if (event_CriticalHandler(0, &e) == 0) {
			continue;
		}
#endif // USES_SDL2_POLL_EVENT

		event_Handler(0, &e);
	}

	return es.alive;
}
