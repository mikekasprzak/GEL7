// - ------------------------------------------------------------------------------------------ - //
// Data MD5 Hash - MD5 Hashing code for Data library //
// - ------------------------------------------------------------------------------------------ - //
#ifndef __Library_Data_Data_MD5_H__
#define __Library_Data_Data_MD5_H__
// - ------------------------------------------------------------------------------------------ - //
#include "Data_Core.h"
#include "MD5Hash.h"
// - ------------------------------------------------------------------------------------------ - //
//namespace Data {
// - ------------------------------------------------------------------------------------------ - //
// Hash a block of data with an MD5 sum //
// - ------------------------------------------------------------------------------------------ - //
inline const MD5Hash hash_MD5_Data( const void* _Src, const size_t _Size ) {
	MD5Hash Key;
	
//	MD5_CTX HashInfo;
//	MD5_Init( &HashInfo );
//	md5_process_bytes( _Src, _Size, &HashInfo );
//	md5_finish_ctx( &HashInfo, (void*)&Key.Hash );

	MD5( (const unsigned char*)_Src, _Size, Key.Hash );
	
	sprintf( Key.Text, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
		Key.Hash[0],
		Key.Hash[1],
		Key.Hash[2],
		Key.Hash[3],
		Key.Hash[4],
		Key.Hash[5],
		Key.Hash[6],
		Key.Hash[7],
		Key.Hash[8],
		Key.Hash[9],
		Key.Hash[10],
		Key.Hash[11],
		Key.Hash[12],
		Key.Hash[13],
		Key.Hash[14],
		Key.Hash[15]
		);
	
	return Key;
}
// - ------------------------------------------------------------------------------------------ - //
//}; // namespace Data //
// - ------------------------------------------------------------------------------------------ - //
#endif // __Library_Data_Data_MD5_H__ //
// - ------------------------------------------------------------------------------------------ - //
