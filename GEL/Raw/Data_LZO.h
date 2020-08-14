// - ------------------------------------------------------------------------------------------ - //
// DataCompression - Compression code for Data library //
// - ------------------------------------------------------------------------------------------ - //
#ifndef __Library_Data_Data_LZO_H__
#define __Library_Data_Data_LZO_H__
// - ------------------------------------------------------------------------------------------ - //
// TODO: Add endianness adaptation to this code.  If we're in big endian mode, be sure we //
//   convert sizes defaulty encoded as little endian to big endian. //
// - ------------------------------------------------------------------------------------------ - //
#include "Data_Core.h"
#include "DataBlock_Core.h"
#include "DataBlock_LZO.h"
// - ------------------------------------------------------------------------------------------ - //
//namespace Data {
// - ------------------------------------------------------------------------------------------ - //
// Decode packed LZO data to a new DataBlock //
inline DataBlock* unpack_LZO_Data( void* _Src, const st32 _SrcSize, void** _Dest, st32* _DestSize ) {
	// Assume the compressed DataBlock contains a DataBlock, who's size is the uncompressed size //
	DataBlock* Compressed = reinterpret_cast<DataBlock*>(_Src);
	
	st32 UncompressedSize = Compressed->Size;

	// Allocate a memory block equal to the uncompressed size //
	DataBlock* Uncompressed = new_DataBlock( UncompressedSize );
	
	// Decompress the Data //
	int Error = lzo1x_decompress(
		(const lzo_bytep)Compressed->Data, // Compressed Data (After Size, and Uncompressed Size (8 bytes)) //
		_SrcSize - 4, // Source size, since we have an extra 4 byte uncompressed size //
		(lzo_bytep)Uncompressed,
		(lzo_uintp)&UncompressedSize,
		(lzo_voidp)0 // No work memory needed //
		);
	// TODO: Assert on uncompress error //
	//if ( Error != Z_OK )
	
	// Return the uncompressed buffer //
	return Uncompressed;
}
// - ------------------------------------------------------------------------------------------ - //
//// Encode packed LZO data to a new DataBlock //
//inline DataBlock* pack_LZO_Data( void* _Src ) {
//	// Allocate worst case space to store compressed data //
//	DataBlock* Compressed = new_DataBlock( _Src->Size + (_Src->Size / 1000) + 12 + 4 );
//	
//	st32* UncompressedSize = reinterpret_cast<st32*>(&Compressed->Data[0]);
//	*UncompressedSize = 0;
//	st32 CompressedSize = Compressed->Size-4;
//	
//	// Note: BAD!  Should not allocate here, should pre allocate these! //
//	char* WorkMemory = new char[LZO1X_1_MEM_COMPRESS]; 
//	
//	// Compress the Data //
//	int Error = lzo1x_1_compress(
//		(const lzo_bytep)_Src->Data,
//		_Src->Size,
//		(lzo_bytep)(&(Compressed->Data[4])),
//		(lzo_uintp)&CompressedSize,
//		(lzo_voidp)WorkMemory
//		);
//	// TODO: Assert on compress error //
//	//if ( Error != Z_OK )
//	
//	// Note: BAD!  Should not have to free here! //
//	delete [] WorkMemory;
//	
//	// Store Sizes //
//	Compressed->Size = CompressedSize + 4;
//	*UncompressedSize = _Src->Size;
//
//	// If zlib worked, zlib will have changed the size in the DataBlock to less than the //
//	//   memory allocated.  Thusly, we can call reallocate to force the data buffer to be //
//	//   re-allocated to reduce memory usage (causing normal OS fragmentation). //
//	reallocate_DataBlock( &Compressed );
//	
//	// Return the compressed buffer //
//	return Compressed;
//}
//// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
//}; // namespace Data //
// - ------------------------------------------------------------------------------------------ - //
#endif // __Library_Data_Data_LZO_H__ //
// - ------------------------------------------------------------------------------------------ - //
