#include <GEL/GEL.h>
#include <GEL/GEL_Containers.h>

#include <spine/spine.h>
#include <spine/extension.h>



void _spAtlasPage_createTexture( spAtlasPage* self, const char* path ) {
	Log( "! SPINE ATLAS createTexture: Atlas: 0x%p  \"%s\"", self, path );
	
	// TODO: Read Atlas information to determine if we should smooth or not //

	// /*GelTexturePool::UID*/ st Asset = Gel::TexturePool.Load( path, true, false );
	// self->rendererObject = (void*)Asset;
	// self->width = Gel::TexturePool[Asset].GetWidth();
	// self->height = Gel::TexturePool[Asset].GetHeight();
}


void _spAtlasPage_disposeTexture( spAtlasPage* self ) {
	Log( "! SPINE ATLAS disposeTexture: Atlas: 0x%p", self );
}


// Allocations here are handled by the MALLOC and FREE macros defined in spine/extension.h
// NOTE: This function should be generalized, and not tied to a graphics API implementation, unlike the above.

char* _spUtil_readFile(const char* path, int* length) {
	Log("! SPINE ATLAS readFile: \"%s\"  LengthPtr: 0x%p", path, length);

	char* data = (char*)raw_ReadNew(path, (st*)length);

	Log("File [%i bytes]: %p", *length, data);

	return data;
}

