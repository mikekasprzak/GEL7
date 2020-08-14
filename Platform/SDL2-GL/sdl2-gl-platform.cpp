#include <GEL/GEL.h>
#include <GEL/GEL_OpenGL.h>
#include <GEL/GEL_SDL2.h>
#include <GEL/GEL_SDL2_Internal.h>
#include <Main/Main.h>
#include <Platform/Platform.h>

#ifdef    USES_GLES2
#include <string.h>	// Need this to parse strings
#endif // USES_GLES2


struct PlatformState {
	SDL_Window* window;
	SDL_SysWMinfo info;
	SDL_GLContext context;
	char* basePath;
	char* prefsPath;
	
	u64 totalSwaps;
	
	PlatformState() :
		totalSwaps(0)
	{
	}
};
PlatformState* platformState = 0;


#ifdef USES_DESKTOP_GL
void GLAPIENTRY
platformInternal_GLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	Error("GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}
#endif // USES_DESKTOP_GL


void platform_Init(int argc, char* argv[]) {
	FLog("+");

	for (int idx = 0; idx < argc; ++idx) {
		Log("[%s] argv[%i]: %s", __func__, idx, argv[idx]);
	}

	// TODO: Extract executable directory from argv[0], and if different from working director, change
	// Desktop OS only

	FLog("SDL_Init");
	SDL_Init(SDL_INIT_VIDEO);

	platformState = new PlatformState;
	platformState->basePath = SDL_GetBasePath();
	platformState->prefsPath = SDL_GetPrefPath("TheInteractiveSnackCompany", "Smiles");
	
	Log("[%s] BasePath: %s", __func__, platformState->basePath);
	Log("[%s] PrefPath: %s", __func__, platformState->prefsPath);

	// Mainly for Angle, but it doesn't hurt to be specific
#ifdef USES_GLES2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#endif // USES_GLES2

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	int windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
#ifdef __ANDROID__
	windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
#endif // __ANDROID__

// NOTE: This is incorrect! Should be desktop OS, not GL.
//#ifdef USES_DESTKOP_GL
	windowFlags |= SDL_WINDOW_RESIZABLE;
//#endif // USES_DESKTOP_GL

#ifndef APPNAME
	#define APPNAME "MyApp"
#endif // APPNAME

	if ((platformState->window = SDL_CreateWindow(APPNAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, windowFlags))) {
		SDL_VERSION(&platformState->info.version);
		if (SDL_GetWindowWMInfo(platformState->window, &platformState->info)) {
			const SDL_SysWMinfo& info = (platformState->info);

			const char *subsystem = "Unknown System";
			switch (info.subsystem) {
				case SDL_SYSWM_UNKNOWN:   break;
				case SDL_SYSWM_WINDOWS:   subsystem = "Microsoft Windows";		break;
				case SDL_SYSWM_WINRT:     subsystem = "Microsoft UWP";          break;	// WinRT
				case SDL_SYSWM_COCOA:     subsystem = "Apple Cocoa";		    break;	// Legacy macOS
				case SDL_SYSWM_UIKIT:     subsystem = "Apple UIKit";            break;	// iOS, iPad, AppleTV, macOS
				case SDL_SYSWM_ANDROID:   subsystem = "Google Android";         break;
				case SDL_SYSWM_X11:       subsystem = "X Windows";				break;
				case SDL_SYSWM_WAYLAND:   subsystem = "Wayland";                break;
				case SDL_SYSWM_VIVANTE:   subsystem = "Steam Link";             break;
#if SDL_VERSION_ATLEAST(2, 0, 12)
				case SDL_SYSWM_HAIKU:     subsystem = "Haiku";                  break;	// Modern BeOS
#endif // SDL_VERSION_ATLEAST
				case SDL_SYSWM_DIRECTFB:  subsystem = "DirectFB";               break;	// webOS, jointSPACE, LinuxTV
				case SDL_SYSWM_MIR:       subsystem = "Mir";                    break;	// RIP
				case SDL_SYSWM_OS2:       subsystem = "IBM OS/2";				break;	// RIP
			}

			FLog("SDL v%d.%d.%d on %s", (int)info.version.major, (int)info.version.minor, (int)info.version.patch, subsystem);
		}
		else {
			Error("Couldn't get SDL window information: %s", SDL_GetError());
		}
	}


	if ((platformState->context = SDL_GL_CreateContext(platformState->window))) {
		FLog("Created OpenGL Context");
	}

#ifdef    USES_GLEW
	// If glewExperimental is not set to GL_TRUE,
	// the program crashes at glGenVertexArrays(1, &vao); with a segfault
	// see http://stackoverflow.com/questions/8302625/segmentation-fault-at-glgenvertexarrays-1-vao
	glewExperimental = GL_TRUE;

	FLog("glewInit");
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		Error("Error starting GLEW: %s", glewGetErrorString(glewError));
	}
#endif // USES_GLEW

	FLog("OpenGL vendor: %s", glGetString(GL_VENDOR));
	FLog("OpenGL renderer: %s", glGetString(GL_RENDERER));

	FLog("OpenGL version: %s", glGetString(GL_VERSION));
	FLog("OpenGL shading language version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

// OpenGL ES 2.0 only supports old style extension checking
#ifdef    USES_GLES2
	FLog("OpenGL extensions:");

	// Tokenize GL extensions
	{
		char *token;
		const char* extensions = (const char*)glGetString(GL_EXTENSIONS);
		
		char* extCopy = (char*)malloc(strlen(extensions) + 1);		
		if (extCopy == NULL) {Error("Unable to malloc");}
		strcpy(extCopy, extensions);

		token = strtok(extCopy, " ");		
		if (token) {
			GLint i = 0;
			do {
				FLog("[%d]: %s", i++, token); 
			} while ((token = strtok(0, " ")));
		}
		
		free(extCopy);
	}
#else // !USES_GLES2
	GLint numExtensions = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

	FLog("OpenGL extensions [%d]:", numExtensions);

	for (GLint i = 0; i < numExtensions; i++) { 
		FLog("[%d]: %s", i, glGetStringi(GL_EXTENSIONS, i)); 
	} 
#endif // USES_GLES2

#ifdef    USES_DESKTOP_GL
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(platformInternal_GLErrorCallback, 0);
#endif // USES_DESKTOP_GL

	// Unsynchronized swap interval
	// if (SDL_GL_SetSwapInterval(0)) {
	// 	FLog("SDL_GL_SetSwapInterval: Unable to set!");
	// }
	// else {
	// 	FLog("SDL_GL_SetSwapInterval: Unsynchronized interval set");
	// }

	// Adaptive, Synchronized, or Unsynchronized if unavailable
	if (SDL_GL_SetSwapInterval(-1)) {
		if (SDL_GL_SetSwapInterval(1)) {
			if (SDL_GL_SetSwapInterval(0)) {
				FLog("SDL_GL_SetSwapInterval: Unable to set!");
			}
			else {
				FLog("SDL_GL_SetSwapInterval: Unsynchronized interval set");
			}
		}
		else {
			FLog("SDL_GL_SetSwapInterval: Synchronized interval set (1)");	
		}
	}
	else {
		FLog("SDL_GL_SetSwapInterval: Adaptive synchronized interval set (-1)");
	}

	FLog("-");
}


int platform_WidthGet() {
	return_if(!platformState);
	PlatformState& ps = *platformState;

	int ret;
	SDL_GetWindowSize(ps.window, &ret, 0);

	return ret;
}


int platform_HeightGet() {
	return_if(!platformState);
	PlatformState& ps = *platformState;

	int ret;
	SDL_GetWindowSize(ps.window, 0, &ret);

	return ret;
}


void platform_Exit() {
	FLog("+");

	//glewExit(); // No need to call this, it doesn't exist

	SDL_GL_DeleteContext(platformState->context);
	SDL_DestroyWindow(platformState->window);

	SDL_free(platformState->prefsPath);
	SDL_free(platformState->basePath);
	delete platformState;

	FLog("SDL_Quit");
	SDL_Quit();
	FLog("-");	
}



void platform_Swap() {
	SDL_GL_SwapWindow(platformState->window);
	platformState->totalSwaps++;

/*
	// If swap interval is unbound (0), we should yield, otherwise we'll eat the CPU
	if (SDL_GL_GetSwapInterval() == 0) {
		SDL_Delay(main_MSUntilNextFrame());
	}
*/
}
