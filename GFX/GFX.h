#pragma once

void gfx_Init();
void gfx_Exit();

struct GFXProgramSource {
	const char* const vertexSource;
	const char* const fragmentSource;
	
	const char** const attribute;
	const char** const uniform;
};


struct GFXProgram;
struct GFXVertexArray;
struct GFXIndexArray;
struct GFXTexture2D;
struct GFXTexture3D;

typedef int GFXProgramAttributeIndex;
#define GFX_ATTRIBUTEINDEX_BAD					-1


//GFXProgramSource* gfx_ProgramSourceRead(char* fileName);
//GFXProgramSource* gfx_ProgramSourceRead(GELBuffer* file);
GFXProgram* gfx_ProgramCompile(GFXProgramSource* src);
void gfx_ProgramUse(GFXProgram* program);
void gfx_ProgramDelete(GFXProgram* program);

void gfx_ProgramEnableVertexArray(GFXProgram* program, const char* const attribute);


void gfx_ProgramBind(GFXProgram* p, const char* const attributeName, GFXVertexArray* va /*, int vertexComponents, int vertexType*/);

void gfx_ProgramBindMat4(GFXProgram* p, const char* const uniformName, float* mat);


GFXVertexArray* gfx_VertexArrayNew();
GFXVertexArray* gfx_VertexArrayNew(float* const vert, const st vertSizeInBytes);
void gfx_VertexArrayLoad(GFXVertexArray* va, float* const vert, const st vertSizeInBytes);
void gfx_VertexArrayDelete(GFXVertexArray* va);
