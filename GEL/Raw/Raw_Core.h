#pragma once
// - ------------------------------------------------------------------------------------------ - //
// Raw - Raw data utility/reading/writing wrapping library
// - ------------------------------------------------------------------------------------------ - //
#include <GEL/Types/Types.h>
//#include "Ptr.h"				// is_aligned_Ptr()
#include <stdio.h>				// FILE*, fopen, ftell, etc
// - ------------------------------------------------------------------------------------------ - //
// NOTE: This code could "technically" be made in to a general purpose reader/writer for all //
//   IO Operations.  F:/Art/DataFile.img for files, M1:/MyData.img for a memory card, 
//   FTP://Urb.com/neffle.dat for a remote file, etc. //
// This is open for debate though.  Memory Card code is almost worth abstracting to a custom //
//   interface, because it's a different task than reading game data, saving editor information. //
// The ability to poll local and remote data is fascinating though, even if it's LAN data. //
// Thusly, it's probably of interest to be able to either define a source (CD, HD, LAN), and all //
//   this code pulls from that source.  Do this on a per platform basis though. //
// DS or PSP could benefit greatly from this. //
// - ------------------------------------------------------------------------------------------ - //
// NOTE: Update.  Instead of this being a general purpose reader/writer, it's a framework. //
//   What this means is you simply write functions that take the new source a source, in custom //
//   "new_Data", "read_Data" and "write_Data" functions.  Name it "Data_MySource.h", and //
//   place is in your library heiarchy.  Then to support a new source, you include it from it's //
//   library's folder.  Easy clean extendability. //
// - ------------------------------------------------------------------------------------------ - //
// TODO: Come up with a standard interface for streamable data. //
// - ------------------------------------------------------------------------------------------ - //

// For fastest loops, use 64bit numbers and SIMD types //

//#define FASTLOOP(Count) for(unsigned ___idx_ ## __COUNTER__ = (Count)+1; --___idx_ ## __COUNTER__;)

//inline void u32set_Data( const unsigned _Value, void* _Data, const st _Size ) {
//	unsigned* Data = (unsigned*)_Data;
//	unsigned* DataTarget = (unsigned*)((char*)_Data+_Size);
//
//	// I need to test the pointer if it's at the target. No way to go faster. :( //
//	while ( Data != DataTarget ) {
//		*Data = _Value;
//		++Data;
//	}
//
//	// Fast yes, but index is unusuable //
////	FASTLOOP(_Size>>2) {
////		Data[
////	}
//	// The solution to the unusable index is to change the base pointer. //
//	// Then the index can simply be added to the target address before performing a store. //
//}

// - ------------------------------------------------------------------------------------------ - //
//inline void aligned_copy_Data( const void* const _Src, void* const _Dest, const st _Size ) {
//	unsigned* Src = (unsigned* const)_Src;
//	unsigned* Dest = (unsigned* const)_Dest;
//	--Src;
//	--Dest;
//
//	for ( st Index = _Size+1; --Index; ) {
//		Dest[Index] = Src[Index];
//	}
//}
// - ------------------------------------------------------------------------------------------ - //
// You probably want this, not MemSet //
template< typename tType >
inline void raw_Fill( const tType& Src, tType* Dest, const st Count ) {
	--Dest;

	for (st Index = Count+1; --Index;) {
		Dest[Index] = Src;
	}
}
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
// MemSET wrapper -- Only writes bytes (despite input being an int)
inline void raw_Set( const int _Value, void* _Data, const st _Size ) {
	memset(_Data, _Value, _Size);
}
// - ------------------------------------------------------------------------------------------ - //
// MemCPY wrapper
inline void raw_Copy( const void* _Src, void* _Dest, const st _Size ) {
	memcpy(_Dest, _Src, _Size);
}
// - ------------------------------------------------------------------------------------------ - //
// MemMove wrapper
inline void raw_Move( const void* _Src, void* _Dest, const st _Size ) {
	memmove(_Dest, _Src, _Size);
}
// - ------------------------------------------------------------------------------------------ - //
// MemCMP wrapper //
inline int raw_Compare( const void* const _Src, const void* _Dest, const st _Size ) {
	return memcmp(_Dest, _Src, _Size);
}
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
// copy data while applying padding //
// WARNING: SIZE SHOULD BE a multiple of Bytes*Padding!! //
inline void raw_CopyPadded( void* _Src, void* _Dest, const st _Size, const st Bytes, const st Padding, const int PadValue ) {
	char* Src = (char*)_Src;
	char* Dest = (char*)_Dest;
	char* TargetSrc = Src + _Size;
	
	while (Src < TargetSrc) {
		for (st Num = Bytes; Num--;) {
			*Dest = *Src;
			Dest++;
			Src++;
		}
		for (st Num = Padding; Num--;) {
			*Dest = PadValue;
			Dest++;
		}
	};
}
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
// Zero Memory
inline void raw_Zero( void* _data, const st _size ) {
	raw_Set(0, _data, _size);
}
// - ------------------------------------------------------------------------------------------ - //
// Write a bitmask of all 1's (-1)
inline void raw_One( void* _data, const st _size ) {
	raw_Set(-1, _data, _size);
}
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
// TODO: write 32bit aware version
#define GENERATE_RAW_OP(_OP, _NAME) \
inline void raw_##_NAME( void* _a, void* _b, void* _dest, st _size) { \
	u8* a = (u8*)_a; \
	u8* b = (u8*)_b; \
	u8* dest = (u8*)_dest; \
	\
	for (;_size--;) { \
		dest[_size] = a[_size] _OP b[_size]; \
	} \
} \
inline void raw_##_NAME( void* _a, void* _b, st _size) { \
	u8* a = (u8*)_a; \
	u8* b = (u8*)_b; \
	\
	for (;_size--;) { \
		b[_size] = a[_size] _OP b[_size]; \
	} \
}
// - ------------------------------------------------------------------------------------------ - //
GENERATE_RAW_OP(&, BitwiseAND);		// raw_BitwiseAND(...)
GENERATE_RAW_OP(|, BitwiseOR);		// raw_BitwiseOR(...)
GENERATE_RAW_OP(^, BitwiseXOR);		// raw_BitwiseXOR(...)
// - ------------------------------------------------------------------------------------------ - //


/*
// - ------------------------------------------------------------------------------------------ - //
// Special template variants of zeroing/oneing //
// - ------------------------------------------------------------------------------------------ - //
template< typename tType >
inline void Zero( tType& _Data ) {
	zero_Data( &_Data, sizeof(_Data) );
}
// - ------------------------------------------------------------------------------------------ - //
template< typename tType >
inline void Zero( tType* _Data, const st Count ) {
	zero_Data( _Data, sizeof(tType)*Count );
}
// - ------------------------------------------------------------------------------------------ - //
template< typename tType >
inline void One( tType& _Data ) {
	one_Data( &_Data, sizeof(_Data) );
}
// - ------------------------------------------------------------------------------------------ - //
template< typename tType >
inline void One( tType* _Data, const st Count ) {
	one_Data( _Data, sizeof(tType)*Count );
}
// - ------------------------------------------------------------------------------------------ - //
*/

// - ------------------------------------------------------------------------------------------ - //
inline st raw_Read( const char* _FileName, void* Data, const st _Size ) {
	// Open File //
	FILE* fp = fopen(_FileName, "rb");
	if (fp == 0) {
		// TODO: Log file open error //
		return 0;
	}
	
	// Determine how large file is //
	fseek(fp, 0, SEEK_END);
	st Size = ftell(fp);
#ifdef _MSC_VER
	fseek(fp, 0L, SEEK_SET);
#else // _MSC_VER //
	rewind(fp);
#endif // _MSC_VER //

	// Read data (only as much as the smallest size) //
	st BytesRead = fread(Data, 1, Size > _Size ? _Size : Size, fp);
	
	// Close file //
	fclose(fp);
	
	// Return the number of bytes read //
	return BytesRead;
}
// - ------------------------------------------------------------------------------------------ - //
inline st raw_Write( const char* _FileName, const void* Data, const st _Size ) {
	// Open File //
	FILE* fp = fopen(_FileName, "wb");
	if (fp == 0) {
		// TODO: Log file open error //
		return 0;
	}
	
	// Write the data //
	st BytesWritten = fwrite(Data, 1, _Size, fp);
	// TODO: Assert on fire write error //
	
	// Close file //
	fclose(fp);
	
	// Return the number of bytes written //
	return BytesWritten;
}
// - ------------------------------------------------------------------------------------------ - //


// - ------------------------------------------------------------------------------------------ - //
inline void* raw_New( const st _Size ) {
	return (void*)(new char[_Size]);
}
// - ------------------------------------------------------------------------------------------ - //
inline void raw_Delete( void* _Data ) {
	delete [] reinterpret_cast<char*>(_Data);
}
// - ------------------------------------------------------------------------------------------ - //
inline void* raw_ReadNew( const char* _fileName, st* _size ) {
	// Open File //
	FILE* fp = fopen(_fileName, "rb");
	if (fp == 0) {
		return 0;
	}
	
	// Determine how large file is //
	fseek(fp, 0, SEEK_END);
	st size = ftell(fp);
#ifdef _MSC_VER
	fseek(fp, 0L, SEEK_SET);
#else // _MSC_VER //
	rewind(fp);
#endif // _MSC_VER //
	
	// Allocate space //
	void* p = raw_New(size);
	// TODO: Assert failure allocating block //
	
	// Read data //
	fread(p, 1, size, fp);

	// Write size //
	if (_size) {
		*_size = size;
	}
	
	// Close file //
	fclose(fp);
	
	// Return data //
	return p;
}
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
// Copy one DataBlock to another, no larger than Destination Size Bytes //
// - ------------------------------------------------------------------------------------------ - //
inline void raw_Copy( const void* _SrcData, const st _SrcSize, void* _DestData, const st _DestSize ) {
	// If source is smaller than the destination //
	if (_DestSize > _SrcSize) {
		// Copy source number of bytes //
		raw_Copy(_SrcData, _DestData, _SrcSize);
	}
	else {
		// Otherwise, copy destination number of bytes //
		raw_Copy(_SrcData, _DestData, _DestSize);
	}
}
// - ------------------------------------------------------------------------------------------ - //
// Allocate and return a duplicate of a Data //
// - ------------------------------------------------------------------------------------------ - //
inline void* raw_Copy( const void* _Src, const st _Size ) {
	// Allocate our new block //
	void* NewData = raw_New(_Size);
	
	// Copy the data to our new block //
	raw_Copy(_Src, _Size, NewData, _Size);
	
	// Return the block //
	return NewData;
}
// - ------------------------------------------------------------------------------------------ - //


// - ------------------------------------------------------------------------------------------ - //
// A useful accessory, resizing. //
// Reducing size can work safely, but expanding would require new allocation //
//   this is because new/delete ignore my internal size variable //
// - ------------------------------------------------------------------------------------------ - //
inline void raw_Reallocate( void** p, const st _OldSize, const st _NewSize ) {
	// Allocate our new block //
	void* NewData = raw_New(_NewSize);
	
	// Copy the data to our new block //
	raw_Copy(*p, _OldSize, NewData, _NewSize);
	
	// Delete the old block ponted to //
	raw_Delete(*p);
	
	// Make the pointer point to the new block //
	(*p) = NewData;
}
// - ------------------------------------------------------------------------------------------ - //
inline void raw_Resize( void** p, const st _OldSize, const st _NewSize ) {
	// A cheat.  We can resize the block without reallocating
	if (_NewSize <= _OldSize) {
		// If the size wasn't something implicit, we'd set it right now. //
		// However, Size is an implied argument, so we just asume you're going to use the new //
		//   size as the correct size from now on //
	}
	else {
		// Well, we tried.  We need to reallocate and copy the data. //
		raw_Reallocate(p, _OldSize, _NewSize);
	}
}
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
// Initializer capable versions of resize, reallocate, and copy //
// - ------------------------------------------------------------------------------------------ - //
// Copy one DataBlock to another, no larger than Destination Size Bytes //
inline void raw_Copy( const void* _Src, const st _SrcSize, void* _Dest, const st _DestSize, const int _InitValue ) {
	// If source is smaller than the destination //
	if (_DestSize > _SrcSize) {
		// Copy source number of bytes //
		raw_Copy(_Src, _Dest, _SrcSize);
		// Fill the rest of the data with initializer //
		raw_Set(_InitValue, &(((char*)_Dest)[_SrcSize]), _DestSize - _SrcSize);
	}
	else {
		// Otherwise, copy destination number of bytes //
		raw_Copy(_Src, _Dest, _DestSize);
	}
}
// - ------------------------------------------------------------------------------------------ - //
inline void raw_Reallocate( void** p, const st _OldSize, const st _NewSize, const int _InitValue ) {
	// Allocate our new block //
	void* NewData = raw_New(_NewSize);
	
	// Copy the data to our new block //
	raw_Copy(*p, _OldSize, NewData, _NewSize, _InitValue);
	
	// Delete the old block ponted to //
	raw_Delete(*p);
	
	// Make the pointer point to the new block //
	(*p) = NewData;
}
// - ------------------------------------------------------------------------------------------ - //
inline void raw_Resize( void** p, const st _OldSize, const st _NewSize, const int _InitValue ) {
	// A cheat.  We can resize the block without reallocating
	if (_NewSize <= _OldSize) {
		// If the size wasn't something implicit, we'd set it right now. //
		// However, Size is an implied argument, so we just asume you're going to use the new //
		//   size as the correct size from now on //
	}
	else {
		// Well, we tried.  We need to reallocate and copy the data. //
		raw_Reallocate(p, _OldSize, _NewSize, _InitValue);
	}
}
// - ------------------------------------------------------------------------------------------ - //
