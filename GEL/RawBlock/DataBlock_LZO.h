// - ------------------------------------------------------------------------------------------ - //
// DataBlockCompression - Compression code for DataBlock library //
// - ------------------------------------------------------------------------------------------ - //
#ifndef __Library_Data_DataBlock_LZO_H__
#define __Library_Data_DataBlock_LZO_H__
// - ------------------------------------------------------------------------------------------ - //
// TODO: Add endianness adaptation to this code.  If we're in big endian mode, be sure we //
//   convert sizes defaulty encoded as little endian to big endian. //
// - ------------------------------------------------------------------------------------------ - //
#include "DataBlock.h"
// - ------------------------------------------------------------------------------------------ - //
extern "C" {
#include <External/Lzo/minilzo.h>
};
// - ------------------------------------------------------------------------------------------ - //
//namespace Data {
// - ------------------------------------------------------------------------------------------ - //
// Decode packed LZO data to a new DataBlock //
inline DataBlock* unpack_LZO_DataBlock( DataBlock* _Src ) {
	// Assume the compressed DataBlock contains a DataBlock, who's size is the uncompressed size //
	DataBlock* Compressed = reinterpret_cast<DataBlock*>(_Src->Data);
	
	// Allocate a memory block equal to the uncompressed size //
	DataBlock* Uncompressed = new_DataBlock( Compressed->Size );
	
	// Decompress the Data //
	int Error = lzo1x_decompress(
		(const lzo_bytep)Compressed->Data, // Compressed Data (After Size, and Uncompressed Size (8 bytes)) //
		_Src->Size - 4, // Source size, since we have an extra 4 byte uncompressed size //
		(lzo_bytep)Uncompressed->Data,
		(lzo_uintp)&Uncompressed->Size,
		(lzo_voidp)0 // No work memory needed //
		);
	// TODO: Assert on uncompress error //
	//if ( Error != Z_OK )
	
	// Return the uncompressed buffer //
	return Uncompressed;
}
// - ------------------------------------------------------------------------------------------ - //
// Decode packed LZO data to a new DataBlock //
inline DataBlock* unpack_LZO_DataBlock( void* _Src, const st32 _SrcSize ) {
	// Assume the compressed DataBlock contains a DataBlock, who's size is the uncompressed size //
	DataBlock* Compressed = reinterpret_cast<DataBlock*>(_Src);
	
	// Allocate a memory block equal to the uncompressed size //
	DataBlock* Uncompressed = new_DataBlock( Compressed->Size );
	
	// Decompress the Data //
	int Error = lzo1x_decompress(
		(const lzo_bytep)Compressed->Data, // Compressed Data (After Size, and Uncompressed Size (8 bytes)) //
		_SrcSize - 4, // Source size, since we have an extra 4 byte uncompressed size //
		(lzo_bytep)Uncompressed->Data,
		(lzo_uintp)&Uncompressed->Size,
		(lzo_voidp)0 // No work memory needed //
		);
	// TODO: Assert on uncompress error //
	//if ( Error != Z_OK )
	
	// Return the uncompressed buffer //
	return Uncompressed;
}
// - ------------------------------------------------------------------------------------------ - //
// Encode packed LZO data to a new DataBlock //
inline DataBlock* pack_LZO_DataBlock( DataBlock* _Src ) {
	// Allocate worst case space to store compressed data //
	DataBlock* Compressed = new_DataBlock( _Src->Size + (_Src->Size / 1000) + 12 + 4 );
	
	st32* UncompressedSize = reinterpret_cast<st32*>(&Compressed->Data[0]);
	*UncompressedSize = 0;
	st32 CompressedSize = Compressed->Size-4;
	
	// Note: BAD!  Should not allocate here, should pre allocate these! //
	char WorkMemory[LZO1X_1_MEM_COMPRESS]; 
	
	// Compress the Data //
	int Error = lzo1x_1_compress(
		(const lzo_bytep)_Src->Data,
		_Src->Size,
		(lzo_bytep)(&(Compressed->Data[4])),
		(lzo_uintp)&CompressedSize,
		(lzo_voidp)WorkMemory
		);
	// TODO: Assert on compress error //
	//if ( Error != Z_OK )
	
	// Note: BAD!  Should not have to free here! //
	//delete [] WorkMemory;
	
	// Store Sizes //
	Compressed->Size = CompressedSize + 4;
	*UncompressedSize = _Src->Size;

	// If zlib worked, zlib will have changed the size in the DataBlock to less than the //
	//   memory allocated.  Thusly, we can call reallocate to force the data buffer to be //
	//   re-allocated to reduce memory usage (causing normal OS fragmentation). //
	reallocate_DataBlock( &Compressed );
	
	// Return the compressed buffer //
	return Compressed;
}
// - ------------------------------------------------------------------------------------------ - //
// Encode packed LZO data to a new DataBlock //
inline DataBlock* pack_LZO_DataBlock( void* _Src, const st32 _SrcSize ) {
	// Allocate worst case space to store compressed data //
	DataBlock* Compressed = new_DataBlock( _SrcSize + (_SrcSize / 1000) + 12 + 4 );
	
	st32* UncompressedSize = reinterpret_cast<st32*>(&Compressed->Data[0]);
	*UncompressedSize = 0;
	st32 CompressedSize = Compressed->Size-4;
	
	// Note: BAD!  Should not allocate here, should pre allocate these! //
	char WorkMemory[LZO1X_1_MEM_COMPRESS]; 
	
	// Compress the Data //
	int Error = lzo1x_1_compress(
		(const lzo_bytep)_Src,
		_SrcSize,
		(lzo_bytep)(&(Compressed->Data[4])),
		(lzo_uintp)&CompressedSize,
		(lzo_voidp)WorkMemory
		);
	// TODO: Assert on compress error //
	//if ( Error != Z_OK )
	
	// Store Sizes //
	Compressed->Size = CompressedSize + 4;
	*UncompressedSize = _SrcSize;

	// If zlib worked, zlib will have changed the size in the DataBlock to less than the //
	//   memory allocated.  Thusly, we can call reallocate to force the data buffer to be //
	//   re-allocated to reduce memory usage (causing normal OS fragmentation). //
	reallocate_DataBlock( &Compressed );
	
	// Return the compressed buffer //
	return Compressed;
}
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
cDataBlock cDataBlock::UnpackLZO( cDataBlock& _Src ) {
	return cDataBlock( unpack_LZO_DataBlock( _Src._Data ) );
}
// - ------------------------------------------------------------------------------------------ - //
cDataBlock cDataBlock::UnpackLZO( DataBlock* _Src ) {
	return cDataBlock( unpack_LZO_DataBlock( _Src ) );
}
// - ------------------------------------------------------------------------------------------ - //
cDataBlock cDataBlock::PackLZO( cDataBlock& _Src ) {
	return cDataBlock( pack_LZO_DataBlock( _Src._Data ) );
}
// - ------------------------------------------------------------------------------------------ - //
cDataBlock cDataBlock::PackLZO( DataBlock* _Src ) {
	return cDataBlock( pack_LZO_DataBlock( _Src ) );
}
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
//}; // namespace Data //
// - ------------------------------------------------------------------------------------------ - //
#endif // __Library_Data_DataBlock_LZO_H__ //
// - ------------------------------------------------------------------------------------------ - //
