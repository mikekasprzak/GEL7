#pragma once

#ifdef USES_SDL2
	#ifdef USES_GL2
		#ifdef USES_GLEW
			#include <GL/glew.h>
		#endif // USES_GLEW

		#define GL_GLEXT_PROTOTYPES 1

		#include <SDL_opengl.h>
		#include <SDL_opengl_glext.h>
	#else // USES_GLES2
		#include <SDL_opengles2.h>
		#define glGenVertexArrays glGenVertexArraysOES
		#define glBindVertexArray glBindVertexArrayOES
	#endif // USES_GL2
#else // !USES_SDL2
	#error "Sorry, I don't know how to handle non-SDL OpenGL right now"
#endif // USES_SDL2
