#pragma once 
// - ------------------------------------------------------------------------------------------ - //
#include "../Types/Types.h"
#include "../GelFile/GelFile.h"
//#include "../GelVFile/GelVFile.h"
//#include "GelContentHandle.h"
//#include "GelStorageHandle.h"

#include "RawBlock_Core.h"
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
// Construct and read a file from Disk
// - ------------------------------------------------------------------------------------------ - //
inline RawBlock* rawBlock_ReadNew( const char* _fileName ) {
	// Open File
	GelFile* fp = gelFile_Open_ReadOnly(_fileName);
	if (fp == 0) {
		return 0;
	}
	
	// Determine how large file is
	st size = gelFile_Size(fp);
	
	// Allocate space (Size is automatically set inside rawBlock_New)
	RawBlock* p = rawBlock_New(size);
	
	// Read data
	gelFile_Read(fp, p->data, size);
	
	// Close file
	gelFile_Close(fp);
	
	// Return data
	return p;
}
// - ------------------------------------------------------------------------------------------ - //
// Allocates an extra byte for a null terminator. Not included in RawBlock->size
inline RawBlock* rawBlock_ReadNew_NullTerminated( const char* _fileName ) {
	// Open File
	GelFile* fp = gelFile_Open_ReadOnly(_fileName);
	if (fp == 0) {
		return 0;
	}
	
	// Determine how large file is
	st size = gelFile_Size(fp);
	
	// Allocate space + 1 byte for a null (Size is automatically set inside rawBlock_New)
	RawBlock* p = rawBlock_New(size + 1);
	
	// Read data
	gelFile_Read(fp, p->data, size);
	
	// Write Zero to final byte
	p->data[size] = 0;
	
	// Set the internal size to the data size (hiding the null terminator)
	p->size = size;
	
	// Close file
	gelFile_Close(fp);
	
	// Return data
	return p;
}
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
inline RawBlock* rawBlock_ReadNew( GelFile* fp ) {
	// Read Size
	st size = gelFile_Read<st>(fp);
	
	// Allocate space (Size is automatically set inside new_RawBlock)
	RawBlock* p = rawBlock_New(size);
	
	// Read data
	gelFile_Read(fp, p->data, size);
	
	// Return data
	return p;
}
// - ------------------------------------------------------------------------------------------ - //
inline RawBlock* rawBlock_ReadNew_NullTerminated( GelFile* fp ) {
	// Read Size
	st size = gelFile_Read<st>(fp);
	
	// Allocate space (Size is automatically set inside new_RawBlock)
	RawBlock* p = rawBlock_New(size + 1);
	
	// Read data
	gelFile_Read(fp, p->data, size);

	// Write Zero to final byte
	p->data[size] = 0;

	// Set the internal size to the data size (hiding the null terminator)
	p->size = size;
	
	// Return data
	return p;
}
// - ------------------------------------------------------------------------------------------ - //
//inline RawBlock* new_read_RawBlock( GelVFile* fp ) {
//	// Read Size //
//	size_t Size = read_GelVFile<size_t>( fp );
//	
//	// Allocate space (Size is automatically set inside new_RawBlock) //
//	RawBlock* p = new_RawBlock( Size );
//	
//	// Read data //
//	read_GelVFile( fp, p->Data, Size );
//	
//	// Return data //
//	return p;
//}
// - ------------------------------------------------------------------------------------------ - //
//inline RawBlock* new_read_nullterminate_RawBlock( GelVFile* fp ) {
//	// Read Size //
//	size_t Size = read_GelVFile<size_t>( fp );
//	
//	// Allocate space (Size is automatically set inside new_RawBlock) //
//	RawBlock* p = new_RawBlock( Size + 1 );
//	
//	// Read data //
//	read_GelVFile( fp, p->Data, Size );
//
//	// Write Zero to final byte //
//	p->Data[Size] = 0;
//
//	// Set the internal size to the data size (hiding the null terminator) //
//	p->Size = Size;
//	
//	// Return data //
//	return p;
//}
// - ------------------------------------------------------------------------------------------ - //
//inline RawBlock* read_RawBlock( GelFile* fp ) {
//	return new_RawBlock( fp );
//}
// - ------------------------------------------------------------------------------------------ - //
//inline RawBlock* read_RawBlock( GelVFile* fp ) {
//	return new_RawBlock( fp );
//}
// - ------------------------------------------------------------------------------------------ - //
//DEPRECATED( RawBlock* new_RawBlock( GelFile* fp ) );
//inline RawBlock* new_RawBlock( GelFile* fp ) {
//	return new_read_RawBlock( fp );
//}
// - ------------------------------------------------------------------------------------------ - //
//DEPRECATED( RawBlock* new_RawBlock( GelVFile* fp ) );
//inline RawBlock* new_RawBlock( GelVFile* fp ) {
//	return new_read_RawBlock( fp );
//}
// - ------------------------------------------------------------------------------------------ - //


// - ------------------------------------------------------------------------------------------ - //
// These functions are for when you know how large a file is //
// TODO: Add file offsetting as optional 3rd argument (defalts to 0) //
// TODO: Or, add a "file" type that you can construct at the top of a function, and pass to these //
// NOTE: The above should be part of the streaming library? //
// - ------------------------------------------------------------------------------------------ - //
inline st rawBlock_Read( RawBlock* p, const char* _fileName ) {
	// Open File
	GelFile* fp = gelFile_Open_ReadOnly(_fileName);
	if (fp == 0) {
		return 0;
	}
	
	// Determine how large file is
	st size = gelFile_Size(fp);
	
	// Read data (only as much as the smallest size)
	st bytesRead = gelFile_Read(fp, p->data, (size > p->size) ? p->size : size);

	// Close file
	gelFile_Close(fp);
	
	// Return the number of bytes read
	return bytesRead;
}
// - ------------------------------------------------------------------------------------------ - //
inline st rawBlock_Write( const RawBlock* p, const char* _fileName ) {
	// Open File
	GelFile* fp = gelFile_Open_WriteOnly(_fileName);
	if (fp == 0) {
		return 0;
	}
	
	// Write the data
	st bytesWritten = gelFile_Write(fp, p->data, p->size);

	// TODO: Assert on fire write error
	
	// Close file
	gelFile_Close(fp);
		
	// Return the number of bytes write
	return bytesWritten;
}
// - ------------------------------------------------------------------------------------------ - //
// For syntactical compatability, read_RawBlock which functions the same as new_RawBlock. //
// Note: This is bad, as it's unclear allocation is done.  new_RawBlock() is preferred. //
// - ------------------------------------------------------------------------------------------ - //
//inline RawBlock* read_RawBlock( const char* _FileName ) {
//	return new_RawBlock( _FileName );
//}
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
inline st rawBlock_Read( RawBlock* p, GelFile* fp ) {
	// Read Size
	st size = gelFile_Read<st>(fp);
	
	// Read data (only as much as the smallest size)
	st bytesRead = gelFile_Read(fp, p->data, (size > p->size) ? p->size : size);
		
	// TODO: If I happen to only read some of the file, less than Size, that would be bad.
	
	// Return the number of bytes read
	return bytesRead + sizeof(size);
}
// - ------------------------------------------------------------------------------------------ - //
inline st rawBlock_Write( const RawBlock* p, GelFile* fp ) {
	// Write Size
	st bytesWritten = gelFile_Write(fp, p->size);
	
	// Write the data
	bytesWritten += gelFile_Write(fp, p->data, p->size);
	
	// TODO: Assert on fire write error
		
	// Return the number of bytes write
	return bytesWritten;
}
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
//inline const size_t read_RawBlock( RawBlock* p, GelVFile* fp ) {
//	// Read Size //
//	size_t Size = read_GelVFile<size_t>( fp );
//	
//	// Read data (only as much as the smallest size) //
//	size_t BytesRead = read_GelVFile( fp, p->Data, Size > p->Size ? p->Size : Size );
//		
//	// TODO: If I happen to only read some of the file, less than Size, that would be bad. //
//	
//	// Return the number of bytes read //
//	return BytesRead + sizeof( Size );
//}
// - ------------------------------------------------------------------------------------------ - //
//inline const size_t write_RawBlock( const RawBlock* p, GelVFile* fp ) {
//	// Write Size //
//	size_t BytesWritten = write_GelVFile( fp, p->Size );
//	
//	// Write the data //
//	BytesWritten += write_GelVFile( fp, p->Data, p->Size );
//	
//	// TODO: Assert on file write error //
//		
//	// Return the number of bytes write //
//	return BytesWritten;
//}
// - ------------------------------------------------------------------------------------------ - //
