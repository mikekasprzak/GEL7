#pragma once
// - ------------------------------------------------------------------------------------------ - //
// Data Hash32 - Data Hashing //
// - ------------------------------------------------------------------------------------------ - //
#include <GEL/Types/Types.h>
// - ------------------------------------------------------------------------------------------ - //
// Taken from Gamedeveloper magazine's InnerProduct (Sean Barrett 2005-03-15)
// 
// circular shift hash -- produces good results if modding by a prime;
// longword at a time would be faster (need alpha-style "is any byte 0"),
// or just use the first longword
// - ------------------------------------------------------------------------------------------ - //
inline unsigned int raw_Hash32( const char *str, st size ) {
	if (!str) {
		return 0;
	}
 
	unsigned char *n = (unsigned char *)str;
	unsigned int acc = 0x55555555;
	
	for (; size; --size) {
		acc = (acc >> 27) + (acc << 5) + *n++;
	}
	
	return acc;
}
// - ------------------------------------------------------------------------------------------ - //
