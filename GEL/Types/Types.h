#pragma once

// - ------------------------------------------------------------------------------------------ - //
#include <string.h>				// for size_t
// - ------------------------------------------------------------------------------------------ - //
typedef signed char				s8;
typedef unsigned char			u8;
typedef signed short			s16;
typedef unsigned short			u16;
typedef signed int				s32;
typedef unsigned int			u32;
typedef signed long long int	s64;
typedef unsigned long long int	u64;
// - ------------------------------------------------------------------------------------------ - //
#define U8_MIN  (0)
#define U16_MIN (0)
#define U32_MIN (0)
#define U64_MIN (0ull)
#define S8_MIN  (-0x80)
#define S16_MIN (-0x8000)
#define S32_MIN (-0x80000000)
#define S64_MIN (-0x8000000000000000ull)

#define U8_MAX  (0xFF)
#define U16_MAX (0xFFFF)
#define U32_MAX (0xFFFFFFFF)
#define U64_MAX (0xFFFFFFFFFFFFFFFFull)
#define S8_MAX  (0x7F)
#define S16_MAX (0x7FFF)
#define S32_MAX (0x7FFFFFFF)
#define S64_MAX (0x7FFFFFFFFFFFFFFFull)

#define U8_MID  (0x7F)
#define U16_MID (0x7FFF)
#define U32_MID (0x7FFFFFFF)
#define U64_MID (0x7FFFFFFFFFFFFFFFull)
#define S8_MID  (0)
#define S16_MID (0)
#define S32_MID (0)
#define S64_MID (0ull)
// - ------------------------------------------------------------------------------------------ - //
typedef unsigned char			st8;
typedef unsigned short			st16;
typedef unsigned int 			st32;
typedef unsigned long long int	st64;
typedef size_t					st;			// Platform specific Size Type
// - ------------------------------------------------------------------------------------------ - //
#define ST8_MIN  U8_MIN
#define ST16_MIN U16_MIN
#define ST32_MIN U32_MIN
#define ST64_MIN U64_MIN

#define ST8_MAX  U8_MAX
#define ST16_MAX U16_MAX
#define ST32_MAX U32_MAX
#define ST64_MAX U64_MAX

#define ST8_MID  U8_MID
#define ST16_MID U16_MID
#define ST32_MID U32_MID
#define ST64_MID U64_MID

#define SZT_MIN U32_MIN
#define SZT_MAX U32_MAX
#define SZT_MID U32_MID
// - ------------------------------------------------------------------------------------------ - //
// http://sourceforge.net/p/predef/wiki/Architectures/
#if defined(_M_X64) || defined(__x86_64__) || defined(__aarch64__)
	// 64bit size_t
	#define ST_IS_ST64 1

	#define ST_MIN U64_MIN
	#define ST_MAX U64_MAX
	#define ST_MID U64_MID
#else // defined(_M_X64) || defined(__x86_64__) || defined(__aarch64__) //
	// 32bit size_t
	#define ST_IS_ST32 1

	#define ST_MIN U32_MIN
	#define ST_MAX U32_MAX
	#define ST_MID U32_MID
#endif // defined(_M_X64) || defined(__x86_64__) || defined(__aarch64__) //
// - ------------------------------------------------------------------------------------------ - //
typedef float					f32;
typedef double					f64;
// - ------------------------------------------------------------------------------------------ - //
