#pragma once
// - ------------------------------------------------------------------------------------------ - //
// Compression code for RawBlock library
// - ------------------------------------------------------------------------------------------ - //
#include "RawBlock_Core.h"
// - ------------------------------------------------------------------------------------------ - //
extern "C" {
#include <zlib.h>
};
// - ------------------------------------------------------------------------------------------ - //
// Decode packed ZLIB data to a new RawBlock
inline RawBlock* rawBlock_ZLIBUnpack( RawBlock* _Src ) {
	// Assume the compressed RawBlock contains a RawBlock, who's size is the uncompressed size
	RawBlock* Compressed = reinterpret_cast<RawBlock*>(_Src->data);
	
	// Allocate a memory block equal to the uncompressed size
	RawBlock* Uncompressed = rawBlock_New( Compressed->size );
	
	// Decompress the Data
	/*int Error =*/ uncompress(
		(Bytef*)Uncompressed->data,
		(uLongf*)&Uncompressed->size,
		(const Bytef*)Compressed->data, // Compressed Data (After Size, and Uncompressed Size (8 bytes))
		_Src->size - 4 // Source size, since we have an extra 4 byte uncompressed size
		);
	// TODO: Assert on uncompress error
	//if ( Error != Z_OK )
	
	// Return the uncompressed buffer
	return Uncompressed;
}
// - ------------------------------------------------------------------------------------------ - //
// Decode packed ZLIB data to a new RawBlock
inline RawBlock* rawBlock_ZLIBUnpack( void* _Src, const st32 _SrcSize ) {
	// Assume the compressed RawBlock contains a RawBlock, who's size is the uncompressed size
	RawBlock* Compressed = reinterpret_cast<RawBlock*>(_Src);
	
	// Allocate a memory block equal to the uncompressed size
	RawBlock* Uncompressed = rawBlock_New( Compressed->size );
	
	// Decompress the Data
	/*int Error =*/ uncompress(
		(Bytef*)Uncompressed->data,
		(uLongf*)&Uncompressed->size,
		(const Bytef*)Compressed->data, // Compressed Data (After Size, and Uncompressed Size (8 bytes))
		_SrcSize - 4 // Source size, since we have an extra 4 byte uncompressed size
		);
	// TODO: Assert on uncompress error
	//if ( Error != Z_OK )
	
	// Return the uncompressed buffer
	return Uncompressed;
}
// - ------------------------------------------------------------------------------------------ - //
// Encode packed ZLIB data to a new RawBlock
inline RawBlock* rawBlock_ZLIBPack( RawBlock* _Src ) {
	// Allocate worst case space to store compressed data
	RawBlock* Compressed = rawBlock_New( _Src->size + (_Src->size / 1000) + 12 + 4 );
	
	st32* UncompressedSize = reinterpret_cast<st32*>(&Compressed->data[0]);
	*UncompressedSize = 0;
	st32 CompressedSize = Compressed->size-4;
	
	// Compress the Data
	/*int Error =*/ compress(
		(Bytef*)(&(Compressed->data[4])),
		(uLongf*)&CompressedSize,
		(const Bytef*)_Src->data,
		_Src->size
		//Z_DEFAULT_COMPRESSION // Compression level.  1 best - 9 worst. 0 none. default is 6
		);
	// TODO: Assert on compress error
	//if ( Error != Z_OK )
	
	// Store Sizes
	Compressed->size = CompressedSize + 4;
	*UncompressedSize = _Src->size;

	// If zlib worked, zlib will have changed the size in the RawBlock to less than the
	//   memory allocated.  Thusly, we can call reallocate to force the data buffer to be
	//   re-allocated to reduce memory usage (causing normal OS fragmentation).
	rawBlock_Reallocate( &Compressed );
	
	// Return the compressed buffer
	return Compressed;
}
// - ------------------------------------------------------------------------------------------ - //
// Encode packed ZLIB data to a new RawBlock
inline RawBlock* rawBlock_ZLIBPack( void* _Src, const st32 _SrcSize ) {
	// Allocate worst case space to store compressed data
	RawBlock* Compressed = rawBlock_New( _SrcSize + (_SrcSize / 1000) + 12 + 4 );
	
	st32* UncompressedSize = reinterpret_cast<st32*>(&Compressed->data[0]);
	*UncompressedSize = 0;
	st32 CompressedSize = Compressed->size-4;
	
	// Compress the Data
	/*int Error =*/ compress(
		(Bytef*)(&(Compressed->data[4])),
		(uLongf*)&CompressedSize,
		(const Bytef*)_Src,
		_SrcSize
		//Z_DEFAULT_COMPRESSION // Compression level.  1 best - 9 worst. 0 none. default is 6
		);
	// TODO: Assert on compress error
	//if ( Error != Z_OK )
	
	// Store Sizes
	Compressed->size = CompressedSize + 4;
	*UncompressedSize = _SrcSize;

	// If zlib worked, zlib will have changed the size in the RawBlock to less than the
	//   memory allocated.  Thusly, we can call reallocate to force the data buffer to be
	//   re-allocated to reduce memory usage (causing normal OS fragmentation).
	rawBlock_Reallocate( &Compressed );
	
	// Return the compressed buffer
	return Compressed;
}
// - ------------------------------------------------------------------------------------------ - //
