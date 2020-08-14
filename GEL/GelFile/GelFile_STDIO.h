#pragma once
// - ------------------------------------------------------------------------------------------ - //
// GelFile - C style "FILE*" wrapping and utility library
// - ------------------------------------------------------------------------------------------ - //
// TODO: Add a function for reading a DataBlock from an open file
// TODO: Also add functions for reading data of an expected size, zero terminated strings, etc.
// TODO: Add functions for reading/adapting floating point numbers from IEEE to other needed types
// TODO: Add functions for converting/writing floats to fixed point numbers (a cheat)
// - ------------------------------------------------------------------------------------------ - //
#include <string.h>
#include <stdio.h>
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
//#define GelFile FILE
typedef FILE GelFile;
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
// Init FileIO
// - ------------------------------------------------------------------------------------------ - //
inline void gelFile_Init() {
	FLog("GelFile STDIO");
}
// - ------------------------------------------------------------------------------------------ - //
inline void gelFile_Exit() {
	FLog("GelFile STDIO");
}
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
// Get the size of a file in bytes
// - ------------------------------------------------------------------------------------------ - //
inline st gelFile_Size( const char* _fileName ) {
	// Open File
	FILE* fp = fopen(_fileName, "rb");
	if (!fp) {
		return 0;
	}
	
	// Determine how large file is
	fseek(fp, 0, SEEK_END);
	st size = ftell(fp);
	
	// Close file
	fclose(fp);
	
	// Return data
	return size;
}
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
// Get the size of an open file, in bytes
// - ------------------------------------------------------------------------------------------ - //
inline st gelFile_Size( GelFile* fp ) {
	st pos = ftell(fp);
	
//	fpos_t pos;
//	fgetpos(fp, &pos);
	
	fseek(fp, 0, SEEK_END);
	st size = ftell(fp);
	fseek(fp, pos, SEEK_SET);
	
//	fsetpos(fp, &pos);
	
	return size;
}
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
inline bool gelFile_Exists( const char* _fileName ) {
	// Open File //
	FILE* fp = fopen(_fileName, "rb");
	if (!fp) {
		return false;
	}
	
	// Close file
	fclose(fp);
	
	// Return data
	return true;
}
// - ------------------------------------------------------------------------------------------ - //
inline int gelFile_Delete( const char* _fileName ) {
	return remove(_fileName);
}
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
// Opening files
// - ------------------------------------------------------------------------------------------ - //
inline GelFile* gelFile_Open( const char* _fileName, const char* _openMask = "rb" ) {
	return fopen(_fileName, _openMask);
}
// - ------------------------------------------------------------------------------------------ - //
inline GelFile* gelFile_Open_ReadOnly( const char* _fileName ) {
	return gelFile_Open(_fileName, "rb");
}
// - ------------------------------------------------------------------------------------------ - //
inline GelFile* gelFile_Open_WriteOnly( const char* _fileName ) {
	return gelFile_Open(_fileName, "wb");
}
// - ------------------------------------------------------------------------------------------ - //
inline GelFile* gelFile_Open_AppendOnly( const char* _fileName ) {
	return gelFile_Open(_fileName, "ab");
}
// - ------------------------------------------------------------------------------------------ - //

// NOTE: C library has 2 interesting functions.  tmpfile() and tmpname( .. ).  tmpfile creates
//   a temorary file instance, that erases itself when closed.  tmpname returns a legit temporary
//   filename, that does not conflict (with what, I don't know).  If you choose to open the file,
//   don't forget to delete it with 'remove("file.blah");'


// - ------------------------------------------------------------------------------------------ - //
// Closing Files
// - ------------------------------------------------------------------------------------------ - //
inline void gelFile_Close( GelFile* fp ) {
	fclose(fp);
}
// - ------------------------------------------------------------------------------------------ - //


// NOTE: fread(targetPointer, dataSize, count, filePointer);

// - ------------------------------------------------------------------------------------------ - //
// Read Function
// - ------------------------------------------------------------------------------------------ - //
inline st gelFile_Read( GelFile* fp, char* data, const st size ) {
	return fread(data, size, 1, fp);
}
// - ------------------------------------------------------------------------------------------ - //
// Write Function
// - ------------------------------------------------------------------------------------------ - //
inline st gelFile_Write( GelFile* fp, const char* data, const st size ) {
	return fwrite(data, size, 1, fp);
}
// - ------------------------------------------------------------------------------------------ - //
