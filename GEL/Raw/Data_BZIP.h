// - ------------------------------------------------------------------------------------------ - //
// DataCompression - Compression code for Data library //
// - ------------------------------------------------------------------------------------------ - //
#ifndef __Library_Data_Data_BZIP_H__
#define __Library_Data_Data_BZIP_H__
// - ------------------------------------------------------------------------------------------ - //
// TODO: Add endianness adaptation to this code.  If we're in big endian mode, be sure we //
//   convert sizes defaulty encoded as little endian to big endian. //
// - ------------------------------------------------------------------------------------------ - //
#include "Data_Core.h"
#include "DataBlock_Core.h"
#include "DataBlock_BZIP.h"
// - ------------------------------------------------------------------------------------------ - //
//namespace Data {
// - ------------------------------------------------------------------------------------------ - //
// Decode packed BZIP data to a new DataBlock //
inline const st32 unpack_BZIP_Data( void* _Src, const st32 _SrcSize, void** _Dest, st32* _DestSize ) {
	// Assume the compressed DataBlock contains a DataBlock, who's size is the uncompressed size //
	DataBlock* Compressed = reinterpret_cast<DataBlock*>(_Src);
	
	st32 UncompressedSize = Compressed->Size;
	
	// Allocate a memory block equal to the uncompressed size //
	char* Uncompressed = (char*)new_Data( UncompressedSize );
	
	// Decompress the Data //
	int Error = BZ2_bzBuffToBuffDecompress(
		Uncompressed,
		&UncompressedSize,
		Compressed->Data, // Compressed Data (After Size, and Uncompressed Size (8 bytes)) //
		_SrcSize - 4, // Source size, since we have an extra 4 byte uncompressed size //
		0, // small (0 - more memory used, faster. 1 - less memory (2300k max), slower) //
		0 // verbosity (0, don't talk) //
		);
	// TODO: Assert on uncompress error //
	//if ( Error != BZ_OK )
	
	*_Dest = Uncompressed;
	*_DestSize = UncompressedSize;
	
	// Return the uncompressed buffer //
	return UncompressedSize;
}
// - ------------------------------------------------------------------------------------------ - //
// Encode packed BZIP data to a new DataBlock //
//inline const st32 pack_BZIP_Data( const void* _Src, const st32 _Size, void** _Dest, st32* _DestSize ) {
//	// Allocate worst case space to store compressed data //
//	DataBlock* Compressed = new_DataBlock( _Src->Size + (_Src->Size / 100) + 600 + 4 );
//	
//	st32* UncompressedSize = reinterpret_cast<st32*>(&Compressed->Data[0]);
//	*UncompressedSize = 0;
//	st32 CompressedSize = Compressed->Size-4;
//	
//	// Compress the Data //
//	int Error = BZ2_bzBuffToBuffCompress(
//		(char*)(&(Compressed->Data[4])),
//		&CompressedSize,
//		(char*)_Src->Data,
//		_Src->Size,
//		9, // blockSize100k (value * 100000 bytes allocated for work spcae (1-9)) //
//		0, // verbosity (0, don't talk) //
//		0 // workFactor (0 defaults to recommended case, 30) //
//		);
//	// TODO: Assert on compress error //
//	//if ( Error != BZ_OK )
//
//	
//	// Store Sizes //
//	Compressed->Size = CompressedSize + 4;
//	*UncompressedSize = _Src->Size;
//
//	// If bzip worked, bzip will have changed the size in the DataBlock to less than the //
//	//   memory allocated.  Thusly, we can call reallocate to force the data buffer to be //
//	//   re-allocated to reduce memory usage (causing normal OS fragmentation). //
//	reallocate_DataBlock( &Compressed );
//	
//	// Return the compressed buffer //
//	return Compressed;
//}
// - ------------------------------------------------------------------------------------------ - //
//	// Allocate worst case space to store compressed data //
//	st32 CompressedSize =  _Src->Size + (_Src->Size / 100) + 600 + 4;
//	char* Compressed = new_Data( CompressedSize );

// - ------------------------------------------------------------------------------------------ - //
//}; // namespace Data //
// - ------------------------------------------------------------------------------------------ - //
#endif // __Library_Data_Data_BZIP_H__ //
// - ------------------------------------------------------------------------------------------ - //
