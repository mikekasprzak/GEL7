#pragma once
// - ------------------------------------------------------------------------------------------ - //
//#include "Endian.h"
// - ------------------------------------------------------------------------------------------ - //
#define MAKE_READWRITE_FUNCTIONS( _PREFIX, _TYPE ) \
\
template< class Type > \
inline Type _PREFIX##_Read( _TYPE* fp ) { \
	Type target; \
	_PREFIX##_Read(fp, (char*)&target, sizeof(target) ); \
	return target; \
} \
\
\
template< class Type > \
inline st _PREFIX##_Write( _TYPE* fp, const Type data ) { \
	return _PREFIX##_Write(fp, (const char*)&data, sizeof(data) ); \
} \
\
\
template< class Type > \
inline void _PREFIX##_Fill( _TYPE* fp, const Type data, int count ) { \
	for (; count--;) { \
		_PREFIX##_Write<Type>(fp, data); \
	} \
} \
\
// - ------------------------------------------------------------------------------------------ - //
// Endian code
//template< class Type > \
//inline Type readswap_##_TYPE( _TYPE* fp ) { \
//	Type Target = read_##_TYPE<Type>(fp); \
//	return byteswap(Target); \
//} \
//\
//template< class Type > \
//inline Type readbe_##_TYPE( _TYPE* fp ) { \
//	Type Target = read_##_TYPE<Type>(fp); \
//	return beswap(Target); \
//} \
//\
//template< class Type > \
//inline Type readle_##_TYPE( _TYPE* fp ) { \
//	Type Target = read_##_TYPE<Type>(fp); \
//	return leswap(Target); \
//} \
//\
//template< class Type > \
//inline st writeswap_##_TYPE( _TYPE* fp, const Type Data ) { \
//	Type Copy = byteswap(Data); \
//	return write_##_TYPE<Type>( fp, Copy ); \
//} \
//\
//template< class Type > \
//inline st writebe_##_TYPE( _TYPE* fp, const Type Data ) { \
//	Type Copy = beswap(Data); \
//	return write_##_TYPE<Type>( fp, Copy ); \
//} \
//\
//template< class Type > \
//inline st writele_##_TYPE( _TYPE* fp, const Type Data ) { \
//	Type Copy = leswap(Data); \
//	return write_##_TYPE<Type>( fp, Copy ); \
//} \
//
