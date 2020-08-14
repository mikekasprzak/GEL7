#pragma once
// - ------------------------------------------------------------------------------------------ - //
// GelFile_Stub
// - ------------------------------------------------------------------------------------------ - //
#include "../Types/Types.h"
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
struct GelFile {
	int Dummy;	// It needs a member
};
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
// Init FileIO
// - ------------------------------------------------------------------------------------------ - //
inline void gelFile_Init() {
	FLog("GelFile Stub");
}
// - ------------------------------------------------------------------------------------------ - //
inline void gelFile_Exit() {
	FLog("GelFile Stub");
}
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
// Get the size of a file in bytes
// - ------------------------------------------------------------------------------------------ - //
inline st gelFile_Size( const char* ) {
	return 0;
}
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
// Get the size of an open file, in bytes
// - ------------------------------------------------------------------------------------------ - //
inline st gelFile_Size( GelFile* ) {
	return 0;
}
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
inline bool gelFile_Exists( const char* ) {
	return false;
}
// - ------------------------------------------------------------------------------------------ - //
inline int gelFile_Delete( const char* ) {
	return 0;
}
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
// Opening files
// - ------------------------------------------------------------------------------------------ - //
inline GelFile* gelFile_Open( const char*, const char* _openMask = "rb" ) {
	return 0;
}
// - ------------------------------------------------------------------------------------------ - //
inline GelFile* gelFile_Open_ReadOnly( const char* ) {
	return 0;
}
// - ------------------------------------------------------------------------------------------ - //
inline GelFile* gelFile_Open_WriteOnly( const char* ) {
	return 0;
}
// - ------------------------------------------------------------------------------------------ - //
inline GelFile* gelFile_Open_AppendOnly( const char* ) {
	return 0;
}
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
// Closing Files
// - ------------------------------------------------------------------------------------------ - //
inline void gelFile_Close( GelFile* ) {
}
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
// Read Function
// - ------------------------------------------------------------------------------------------ - //
inline st gelFile_Read( GelFile*, char*, const st ) {
	return 0;
}
// - ------------------------------------------------------------------------------------------ - //
// Write Function
// - ------------------------------------------------------------------------------------------ - //
inline st gelFile_Write( GelFile*, const char*, const st ) {
	return 0;
}
// - ------------------------------------------------------------------------------------------ - //