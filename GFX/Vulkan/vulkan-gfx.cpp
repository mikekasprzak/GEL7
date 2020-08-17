#include <GEL/GEL.h>
#include <GEL/GEL_Metal.h>
#include <GFX/GFX.h>


struct GFXProgram {
};

struct GFXVertexArray {
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

	// TODO
	
	return p;
}
	


void gfx_ProgramUse(GFXProgram* p) {
	error_return_void_if(!p);

	// TODO
}



void gfx_ProgramDelete(GFXProgram* p) {
	error_return_void_if(!p);

	// TODO

	delete p;
}


// NOTE: We might not need this
void gfx_ProgramEnableVertexArray(GFXProgram* p, const char* const name) {
	error_return_void_if(!p);
	error_return_void_if(!name);

	// TODO
}


/*
GFXProgramAttributeIndex gfx_ProgramGetAttributeIndex(GFXProgram* p, const char* const attributeName) {
	error_return_value_if(!p, GFX_ATTRIBUTEINDEX_BAD);
	error_return_value_if(!attributeName, GFX_ATTRIBUTEINDEX_BAD);

	return glGetAttribLocation(p->program, attributeName); // -1 on error
}
*/

//void gfx_ProgramBindVertexArray
void gfx_ProgramBind(GFXProgram* p, const char* const attributeName, GFXVertexArray* va /*, int vertexComponents, int vertexType*/) {
	error_return_void_if(!p);
	error_return_void_if(!attributeName);
	error_return_void_if(!va);

	// TODO
}


void gfx_ProgramBind(GFXProgram* p, GFXProgramAttributeIndex attributeIndex, GFXVertexArray* va /*, int vertexComponents, int vertexType*/) {
	error_return_void_if(!p);
	error_return_void_if(attributeIndex == GFX_ATTRIBUTEINDEX_BAD);
	error_return_void_if(!va);

	// TODO
}


void gfx_ProgramBindMat4(GFXProgram* p, const char* const uniformName, float* mat) {
	GLint uniformIndex = glGetUniformLocation(p->program, uniformName);

	// TODO
}


GFXVertexArray* gfx_VertexArrayNew() {
	GFXVertexArray* va = new GFXVertexArray;
	
	// TODO

	return va;
}


void gfx_VertexArrayLoad(GFXVertexArray* va, float* const vert, const st vertSizeInBytes) {
	error_return_void_if(!va);
	error_return_void_if(!vert);

	// TODO
}


GFXVertexArray* gfx_VertexArrayNew(float* const vert, const st vertSizeInBytes) {
	error_return_null_if(!vert);

	// TODO

	return va;
}


void gfx_VertexArrayDelete(GFXVertexArray* va) {
	error_return_void_if(!va);

	// TODO
	
	delete va;
}
