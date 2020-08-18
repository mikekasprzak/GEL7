#include <GEL/GEL.h>
#include <GEL/GelFile/GelFile.h>
#include <Platform/Platform.h>
#include <Input/Input.h>
#include <Event/Event.h>
#include <GFX/GFX.h>
#include <Game/Game.h>
#include <Main/Main.h>

#ifdef USES_SDL2
#include <Platform/Platform_SDL2.h>			// Required to correctly transform main function
#endif // USES_SDL2
	
//	{
//		std::string filename = "";
////		filename += SDL_AndroidGetInternalStoragePath();
////		filename += "/assets/";
//		filename += "moon/pie.txt";
//		
//		
//		SDL_RWops *in = SDL_RWFromFile(filename.c_str(), "rb");
//		Sint64 size = SDL_RWsize(in);
//		char* data = (char*)malloc(size + 1);
//
//        Sint64 nb_read_total = 0, nb_read = 1;
//        char* buf = data;
//        while (nb_read_total < size && nb_read != 0) {
//                nb_read = SDL_RWread(in, buf, 1, (size - nb_read_total));
//                nb_read_total += nb_read;
//                buf += nb_read;
//        }		
//		SDL_RWclose(in);
//		data[nb_read_total] = '\0';
//		
//		Log("Data [%i]: %s", nb_read_total, data);
//		free(data);
//	}

struct MainState {
	MAINFrame frame;
	MAINFrame drawFrame;		// NOTE: you should not time things to this value
	GELTime startTime;
	GELTime workTime;

	MAINFrame framesStepped;	// In DRAW this can be used to check how busy the last frame was
	MAINFrame framesPerSecond;
};
MainState* mainState;


void main_Focus() {
	return_void_if(!mainState);
 
	mainState->workTime = gel_TimeGet();
}


void main_Blur() {
}


void main_Resize() {
}



MAINFrame main_FrameGet() {
	error_return_null_if(!mainState);

	return mainState->frame;
}


MAINFrame main_DrawFrameGet() {
	error_return_null_if(!mainState);

	return mainState->drawFrame;
}

// I dunno if this even works
int main_MSUntilNextFrame() {
	GELTime t = (mainState->workTime + MAIN_FRAME_LENGTH) - gel_TimeGet();
	return (t < 0) ? (int)(t/1000ull) : 0; //0 : -(int)(t/1000ull);
}


int main(int argc, char* argv[]) {
	FLog("+");

	mainState = new MainState;
	MainState& ms = *mainState;

	gelFile_Init();
	platform_Init(argc, argv);
	event_Init();
	input_Init();
	gfx_Init();
	game_Init(platform_WidthGet(), platform_HeightGet());

	// Timing game loop for a non-networked game
	ms.startTime = gel_TimeGet();
	ms.workTime = ms.startTime;
	while (true) {
		if (!event_Step()) {
			break;
		}

		GELTime frameStartTime = gel_TimeGet();
		ms.framesStepped = 0;
		while ((ms.workTime + MAIN_FRAME_LENGTH) <= frameStartTime) {
			input_Step();
			game_Step();
			ms.frame++;
			ms.framesStepped++;
			ms.workTime += MAIN_FRAME_LENGTH;
		}

		// if a frame was stepped, we draw
		if (ms.framesStepped > 0) {
			ms.drawFrame++;
			game_Draw();
			platform_Swap();	// Show the screen we just drew, and yield if necessary
		}
	}

	game_Exit();
	gfx_Exit();
	input_Exit();
	event_Exit();
	platform_Exit();
	gelFile_Exit();

	delete mainState;

	FLog("-");
	return 0;
}
