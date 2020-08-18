#include <GEL/GEL.h>
#include <GFX/GFX.h>
#include <GFX/GFX_OpenGL.h>


struct GFXProgram {
	GLuint program;
	
	// Do we need these?
	GLuint vertex;
	GLuint fragment;
};

struct GFXVertexArray {
	GLuint buffer;
};



void gfx_Init() {
	FLog("+");
	FLog("-");
}

void gfx_Exit() {
	FLog("+");
	FLog("-");	
}



GFXProgram* gfx_ProgramCompile(GFXProgramSource* src) {
	error_return_null_if(!src);


	GFXProgram* p = new GFXProgram;


	// compile the vertex shader
	p->vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(p->vertex, 1, &src->vertexSource, NULL);
	glCompileShader(p->vertex);
	
	GLint status = GL_FALSE;
	glGetShaderiv(p->vertex, GL_COMPILE_STATUS, &status);
	Log("[%s] Vertex Shader Compiled: %s", __func__, status ? "Yes" : "No");
	if (!status) {
		int length = 0;
		char charBuffer[2048];
		glGetShaderInfoLog(p->vertex, sizeof(charBuffer), &length, charBuffer);
		Log("%s", charBuffer);
	}
	

	// compile the fragment shader
	p->fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(p->fragment, 1, &src->fragmentSource, NULL);
	glCompileShader(p->fragment);

	status = GL_FALSE;
	glGetShaderiv(p->fragment, GL_COMPILE_STATUS, &status);
	Log("[%s] Fragment Shader Compiled: %s", __func__, status ? "Yes" : "No");
	if (!status) {
		int length = 0;
		char charBuffer[2048];
		glGetShaderInfoLog(p->fragment, sizeof(charBuffer), &length, charBuffer);
		Log("%s", charBuffer);
	}
	

	// Link the vertex and fragment shader into a shader program
	p->program = glCreateProgram();
	glAttachShader(p->program, p->vertex);
	glAttachShader(p->program, p->fragment);
	// glBindFragDataLocation(p->program, 0, "outColor");

	if (src->attribute) {
		int attribIndex = 0;
		while (src->attribute[attribIndex] != 0) {
			Log("[%s] Attribute %i: %s", __func__, attribIndex, src->attribute[attribIndex]);
			
			// Not an empty string
			if (src->attribute[attribIndex][0] != 0) {
				glBindAttribLocation(p->program, attribIndex, src->attribute[attribIndex]);
				glEnableVertexAttribArray(attribIndex);
			}
			else {
				//glDisableVertexAttribArray(attribIndex);
			}
			attribIndex++;
			//attribIndex+=4;
		}
	}

	/*
	if (src->uniform) {
		int uniformIndex = 0;
		while (src->uniform[uniformIndex] != 0) {
			Log("[%s] Uniform %i: %s", __func__, uniformIndex, src->uniform[uniformIndex]);
			
			// Not an empty string
			if (src->uniform[attribIndex][0] != 0) {
				glUniformMatrix4fv(p->program, uniformIndex, src->uniform[uniformIndex]);
			}
			uniformIndex++;
		}
	}
	*/


	glLinkProgram(p->program);
	
	status = GL_FALSE;
	glGetProgramiv(p->program, GL_LINK_STATUS, &status);
	Log("[%s] Program Linked: %s", __func__, status ? "Yes" : "No");


	// At this point the shaders are attached to the program. The shaders will persist until
	// the the program is deleted. The docs say it's okay to delete a shader, as it will
	// not prematurely deallocate it until it is certain its no longer needed. That said
	// if you ever notice some wacky behavior related to shaders disappearing, check here. ;)
	glDeleteShader(p->vertex);
	glDeleteShader(p->fragment);
	
	return p;
}
	


void gfx_ProgramUse(GFXProgram* p) {
	error_return_void_if(!p);

	glUseProgram(p->program);
}



void gfx_ProgramDelete(GFXProgram* p) {
	error_return_void_if(!p);

	// We deleted the shaders earlier. If you notice that rendering stops working on an oddball
	// platform, you should check to see if prematurely deleting it after creation was to blame.
	//glDeleteShader(p->vertex);
	//glDeleteShader(p->fragment);

	glDeleteProgram(p->program);
	
	delete p;
}


// NOTE: We might not need this
void gfx_ProgramEnableVertexArray(GFXProgram* p, const char* const name) {
	error_return_void_if(!p);
	error_return_void_if(!name);

	//Log("Name: %s", name);
	GLint attrib = glGetAttribLocation(p->program, name); // -1 on error
	
	//Log("Attrib: %i", attrib);
	glEnableVertexAttribArray(attrib);
//	glVertexAttribPointer(attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
}


GFXProgramAttributeIndex gfx_ProgramGetAttributeIndex(GFXProgram* p, const char* const attributeName) {
	error_return_value_if(!p, GFX_ATTRIBUTEINDEX_BAD);
	error_return_value_if(!attributeName, GFX_ATTRIBUTEINDEX_BAD);

	return glGetAttribLocation(p->program, attributeName); // -1 on error
}


//void gfx_ProgramBindVertexArray
void gfx_ProgramBind(GFXProgram* p, const char* const attributeName, GFXVertexArray* va /*, int vertexComponents, int vertexType*/) {
	error_return_void_if(!p);
	error_return_void_if(!attributeName);
	error_return_void_if(!va);

	glBindBuffer(GL_ARRAY_BUFFER, va->buffer);
	
	GLint attrib = glGetAttribLocation(p->program, attributeName); // -1 on error
	return_void_if(attrib == -1);

	glVertexAttribPointer(attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
}


void gfx_ProgramBind(GFXProgram* p, GFXProgramAttributeIndex attributeIndex, GFXVertexArray* va /*, int vertexComponents, int vertexType*/) {
	error_return_void_if(!p);
	error_return_void_if(attributeIndex == GFX_ATTRIBUTEINDEX_BAD);
	error_return_void_if(!va);

	glBindBuffer(GL_ARRAY_BUFFER, va->buffer);
	glVertexAttribPointer(attributeIndex, 2, GL_FLOAT, GL_FALSE, 0, 0);
}


void gfx_ProgramBindMat4(GFXProgram* p, const char* const uniformName, float* mat) {
	GLint uniformIndex = glGetUniformLocation(p->program, uniformName);

	glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, mat);
}


GFXVertexArray* gfx_VertexArrayNew() {
	GFXVertexArray* va = new GFXVertexArray;
	glGenBuffers(1, &va->buffer);

	return va;
}


void gfx_VertexArrayLoad(GFXVertexArray* va, float* const vert, const st vertSizeInBytes) {
	error_return_void_if(!va);
	error_return_void_if(!vert);

	glBindBuffer(GL_ARRAY_BUFFER, va->buffer);
	glBufferData(GL_ARRAY_BUFFER, vertSizeInBytes, vert, GL_STATIC_DRAW);
}


GFXVertexArray* gfx_VertexArrayNew(float* const vert, const st vertSizeInBytes) {
	error_return_null_if(!vert);

	GFXVertexArray* va = gfx_VertexArrayNew();
	gfx_VertexArrayLoad(va, vert, vertSizeInBytes);

	return va;
}


void gfx_VertexArrayDelete(GFXVertexArray* va) {
	error_return_void_if(!va);

	glDeleteBuffers(1, &va->buffer);
	
	delete va;
}
