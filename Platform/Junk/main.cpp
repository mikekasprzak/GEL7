#include <GEL.h>
#include <SDL.h>
#include <Game.h>

#include <GFX.h>
#include <Platform.h>

#include <string>

#define GL_GLEXT_PROTOTYPES 1

#ifdef USES_GL2
#ifdef USES_GLEW
#include <GL/glew.h>
#endif // USES_GLEW

#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>
#else // USES_GLES2
#include <SDL_opengles2.h>
#define glGenVertexArrays glGenVertexArraysOES
#define glBindVertexArray glBindVertexArrayOES
#endif // USES_GL


struct SystemState {
	SDL_Window* window;
	SDL_GLContext context;
};


// Shader sources
const GLchar* vertexSource =
	"attribute vec4 position;    \n"
	"void main()                  \n"
	"{                            \n"
	"	gl_Position = vec4(position.xyz, 1.0);  \n"
	"}                            \n";
const GLchar* fragmentSource =
	"#ifdef GL_ES\n"
	"precision mediump float;\n"
	"#endif\n"
	"void main()                                  \n"
	"{                                            \n"
	"	gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
	"}                                            \n";



int main(int argc, char* argv[]) {
	FLog("Start");
	
	platform_Init();
	
//	SDL_Init(SDL_INIT_VIDEO);
//	atexit(SDL_Quit);
	
//#ifdef __ANDROID__
//	Log("SDL_AndroidGetInternalStoragePath: %s", SDL_AndroidGetInternalStoragePath());
//#endif // __ANDROID__
	
//	Log("Run from: %s", SDL_GetBasePath());
//	Log("Pref path: %s", SDL_GetPrefPath("TheInteractiveSnackCompany", "Smiles"));
	
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
	
	
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
//	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
//	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

//	SystemState systemState;
//	
//	int windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
//#ifdef __ANDROID__
//	windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
//#endif // __ANDROID__
//	
//	systemState.window = SDL_CreateWindow("Snack Break Smiles [Prototype]", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, windowFlags);
//	systemState.context = SDL_GL_CreateContext(systemState.window);
//
//#ifdef USES_GLEW
//	// If glewExperimental is not set to GL_TRUE,
//	// the program crashes at glGenVertexArrays(1, &vao); with a segfault
//	// see http://stackoverflow.com/questions/8302625/segmentation-fault-at-glgenvertexarrays-1-vao
//	glewExperimental = GL_TRUE;
//
//	GLenum glewError = glewInit();
//	if (glewError != GLEW_OK) {
//		Error("Error starting GLEW: %s", glewGetErrorString(glewError));
//	}
//	else {
//		FLog("GLEW Initialized");
//	}
//#endif // USES_GLEW
//	
//	SDL_GL_SetSwapInterval(1);

	gfx_Init();
	game_Init();
	
	
	// Create and compile the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	
	GLint status = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	Log("Vertex Shader: %i", status);
	if (!status) {
		int length = 0;
		char charBuffer[2048];
		glGetShaderInfoLog(vertexShader, sizeof(charBuffer), &length, charBuffer);
		Log("%s", charBuffer);
	}
	

	// Create and compile the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	status = GL_FALSE;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	Log("Fragment Shader: %i", status);
	if (!status) {
		int length = 0;
		char charBuffer[2048];
		glGetShaderInfoLog(fragmentShader, sizeof(charBuffer), &length, charBuffer);
		Log("%s", charBuffer);
	}

	// Link the vertex and fragment shader into a shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	
	status = GL_FALSE;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
	Log("Program: %i", status);
	
	
	glUseProgram(shaderProgram);
	
   
   
	GLuint vbo;
	glGenBuffers(1, &vbo);

	GLfloat vertices[] = {0.0f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
 
	
	
	
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	
	
	int taste = 0;
	
	bool exitGameLoop = false;
	while (!exitGameLoop) {
		SDL_Event e;
		
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				exitGameLoop = true;
			}
		}

		game_Step();
		game_Draw();

		taste++;
		taste = taste & 127;

		glClearColor(taste / 127.0f, 0.125f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glDrawArrays(GL_TRIANGLES, 0, 3);

		platform_Swap();
	}
	
	game_Exit();
	gfx_Exit();
	platform_Exit();

	return 0;
}



















/*/

#include <exception>
#include <functional>

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
	#include <SDL.h>
	#include <SDL_image.h>
	#include <SDL_ttf.h>

	#define GL_GLEXT_PROTOTYPES 1
	#include <SDL_opengles2.h>
#else
	#include <SDL.h>

	#define GL_GLEXT_PROTOTYPES 1

	//#include <SDL2/SDL_opengl.h>
	//#include <SDL2/SDL_opengl_glext.h>
	
	#include <SDL_opengles2.h>
	#define glGenVertexArrays glGenVertexArraysOES
	#define glBindVertexArray glBindVertexArrayOES
#endif

// Shader sources
const GLchar* vertexSource =
	"attribute vec4 position;    \n"
	"void main()                  \n"
	"{                            \n"
	"   gl_Position = vec4(position.xyz, 1.0);  \n"
	"}                            \n";
const GLchar* fragmentSource =
	"precision mediump float;\n"
	"void main()                                  \n"
	"{                                            \n"
	"  gl_FragColor = vec4 (1.0, 1.0, 1.0, 1.0 );\n"
	"}                                            \n";


std::function<void()> loop;
void main_loop() { loop(); }

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	auto wnd(
		SDL_CreateWindow("Hello Worm", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN));
			
	Log("wnd: %i\n", wnd);

	auto glc = SDL_GL_CreateContext(wnd);
	
	Log("glc: %i\n", glc);

	SDL_GL_SetSwapInterval(1);

	//auto rdr = SDL_CreateRenderer(
	//    wnd, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
		
	Log("rdr: %i\n", rdr);
	
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_STENCIL_TEST);
	glDepthMask(false);

	// Create Vertex Array Object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create a Vertex Buffer Object and copy the vertex data to it
	GLuint vbo;
	glGenBuffers(1, &vbo);

	GLfloat vertices[] = {0.0f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create and compile the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	// Create and compile the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	// Link the vertex and fragment shader into a shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

	loop = [&]
	{
		SDL_Event e;
		while(SDL_PollEvent(&e))
		{
			if(e.type == SDL_QUIT) {
				exit(0);
			}
		}


		// Clear the screen to black
		glClearColor(0.6f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw a triangle from the 3 vertices
//        glDrawArrays(GL_TRIANGLES, 0, 3);

		SDL_GL_SwapWindow(wnd);
	};

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(main_loop, 0, true);
#else
	while(true) main_loop();
#endif

	return 0;
}
/**/

/*/
#include <stdlib.h>
#include <SDL2/SDL.h>

#define GL_GLEXT_PROTOTYPES 1

#include <SDL2/SDL_opengles2.h>
#define glGenVertexArrays glGenVertexArraysOES
#define glBindVertexArray glBindVertexArrayOES


const char *vertex_shader =
"in vec3 v;"
"void main() {"
"   gl_Position = vec4(v, 1.0);"
"}";

const char *fragment_shader =
"out vec4 frag_color;"
"void main() {"
"   frag_color = vec4(0.0, 0.0, 1.0, 1.0);"
"}";

const float points[] = {
	 0.0f,  0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f
};

int init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		Log("Failed to init SDL\n");
		exit(1);
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	return 0;
}

SDL_Window *create_window()
{
	SDL_Window *window = SDL_CreateWindow(
			"Hello Triangle",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			512,
			512,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
			);

	return window;
}

GLuint make_triangle_vbo()
{
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	return vbo;
}

GLuint make_triangle_vao(GLuint vbo)
{

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	return vao;
}

GLuint make_program(const char *vs_source, const char *fs_source)
{
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vs_source, NULL);
	glCompileShader(vs);

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fs_source, NULL);
	glCompileShader(fs);

	GLint is_compiled = 0;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &is_compiled);

	if(is_compiled == GL_FALSE) {
		GLint maxlength;
		glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &maxlength);
		char *infoLog = (char*)malloc(sizeof(char) * maxlength);
		glGetShaderInfoLog(vs, maxlength, &maxlength, &infoLog[0]);
		Log("Vertex shader failed to compile. \n MESSAGE: %s \n", infoLog);
		free(infoLog);

		exit(1);
	}

	glGetShaderiv(fs, GL_COMPILE_STATUS, &is_compiled);

	if(is_compiled == GL_FALSE) {
		GLint maxlength;
		glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &maxlength);
		char *infoLog = (char*)malloc(sizeof(char) * maxlength);
		glGetShaderInfoLog(fs, maxlength, &maxlength, &infoLog[0]);
		Log("Fragment shader failed to compile. %d \n MESSAGE: %s \n", maxlength, infoLog);
		free(infoLog);

		exit(1);
	}

	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, fs);
	glAttachShader(shader_program, vs);
	glLinkProgram(shader_program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	GLint isLinked = 0;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &isLinked);

	if(isLinked == GL_FALSE)
	{
		GLint maxlength;
		glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &maxlength);
		char *infoLog = (char*)malloc(sizeof(char) * maxlength);
		glGetProgramInfoLog(shader_program, maxlength, &maxlength, &infoLog[0]);
		Log("Shader failed to link. \n MESSAGE: %s \n", infoLog);
		free(infoLog);
	}

	return shader_program;
}

int main(int argc, char* argv[])
{
	init();

	SDL_Window *window = create_window();

	if(window == NULL) {
		Log("Could not create window: %s\n", SDL_GetError());
		return 1;
	} else {
		Log("Window created successfully.\n");
	}

	SDL_GLContext context = SDL_GL_CreateContext(window);

	if(context == NULL) {
		Log("OpenGL context could not be created: %s\n", SDL_GetError());
		return 1;
	} else {
		Log("OpenGL context created successfully.\n");
	}

	SDL_GL_SetSwapInterval(1);

	glClearColor ( 1.0, 1.0, 1.0, 1.0 );

	GLuint vbo = make_triangle_vbo();
	GLuint vao = make_triangle_vao(vbo);
	GLuint shader_program = make_program(vertex_shader, fragment_shader);

	SDL_Event events;

	int running = 1;
	while(running)
	{
		while(SDL_PollEvent(&events))
		{
			if(events.type == SDL_QUIT)
				running = 0;
		}

		glClear (GL_COLOR_BUFFER_BIT);
		glUseProgram(shader_program);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		SDL_GL_SwapWindow(window);
		SDL_Delay(33);
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
/**/

/*/
// A simple application using SDL2, OpenGL and GLSL to display a red background
// with a green triangle drawn on top. Based heavily (lots of code directly
// copied) on tutorials on http://www.arcsynthesis.org/gltut/.
//
// The tutorials on arcsynthesis are quite good, but they use a slightly
// modified version of the Unofficial OpenGL SDK.
// (http://glsdk.sourceforge.net/docs/html/index.html)
// I wanted a simple hello triangle application that does not use the entire
// SDK.
// Also, I am learning SDL2 these days, so I wanted to use it for managing the
// window instead of freeglut that the tutorials on arcsynthesis use. I looked
// around for a while and couldn't find a great expample of SDL2 along with
// the modern OpenGL Shading language. Hence I decided to write one.


#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>


const std::string strVertexShader(
	"#version 330\n"
	"layout(location = 0) in vec4 position;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = position;\n"
	"}\n"
);

const std::string strFragmentShader(
	"#version 330\n"
	"out vec4 outputColor;\n"
	"void main()\n"
	"{\n"
	"   outputColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
	"}\n"
);

const float vertexPositions[] = {
	0.75f, 0.75f, 0.0f, 1.0f,
	0.75f, -0.75f, 0.0f, 1.0f,
	-0.75f, -0.75f, 0.0f, 1.0f,
};

GLuint positionBufferObject;
GLuint vao;
GLuint program;

int PrintSDLError(const std::string &prepend) {
		std::cerr << prepend << "Error: " << SDL_GetError() << std::endl;
		return 0;
}

GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile) {
	GLuint shader = glCreateShader(eShaderType);
	const char *strFileData = strShaderFile.c_str();
	glShaderSource(shader, 1, &strFileData, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch(eShaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		Error("Compile failure in %s shader:\n%s\n",
				strShaderType, strInfoLog);
		delete[] strInfoLog;
	}

	return shader;
}

void InitializeVertexBuffer() {
	glGenBuffers(1, &positionBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int InitGL() {
	// Create shaders
	GLuint vertex_shader = CreateShader(GL_VERTEX_SHADER, strVertexShader);
	GLuint fragment_shader = CreateShader(GL_FRAGMENT_SHADER,
										  strFragmentShader);

	// Create program
	std :: cout << "Create program" << std :: endl;
	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);

	glDetachShader(program, vertex_shader);
	glDetachShader(program, fragment_shader);

	// Delete shader
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		std :: cerr << "Linker failure: " << strInfoLog << std :: endl;
		delete[] strInfoLog;
	}

	InitializeVertexBuffer();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	return 0;
}

int Render() {
	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);
	glUseProgram(0);

	return 0;
}

int main(int argc, char **argv) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("SDL2_GL", // window title
						   100, // x position of window
						   100, // y position of window
						   640, // width of the screen
						   480, // height of the screen
						   SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (window == NULL) {
		PrintSDLError("SDL_CreateWindow");
		return 1;
	}

	// If Major and Minor revisions are not set, shaders fail to compile and
	// give the following error:
	// Compile failure in vertex shader:
	// 0:1(10): error: GLSL 3.30 is not supported.
	// Supported versions are: 1.10, 1.20, 1.30, 1.00 ES, and 3.00 ES

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// If SDL_GL_DEPTH_SIZE is not set, results in only half of the window
	// being used sometimes.
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	//Request OpenGL 3.3 context.
	SDL_GLContext gl_context = SDL_GL_CreateContext(window);

	if(gl_context == NULL) {
			PrintSDLError("SDL_GL_CreateContext");
			return 1;
	}
	// Initialize glew
	// If glewExperimental is not set to GL_TRUE,
	// the program crashes at glGenVertexArrays(1, &vao); with a segfault
	// see http://stackoverflow.com/questions/8302625/segmentation-fault-at-glgenvertexarrays-1-vao
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		std :: cerr << "Error: " << glewGetErrorString(glewError)
					<< std :: endl;
	}

	// Initialize OpenGL
	if(InitGL() != 0) {
		// destroy window and quit if initializing openGL failed
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	};

	//Our event structure
	SDL_Event e;
	bool quit = false;
	while (!quit) {
		// handle all pending events
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;
			if (e.type == SDL_KEYDOWN) {
				switch(e.key.keysym.sym) {
					case SDLK_ESCAPE:
						quit = true;
						break;
				}
			}
		}
		Render();
		SDL_GL_SwapWindow(window);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

/**/
