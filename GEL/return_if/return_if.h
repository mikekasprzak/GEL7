#pragma once

// - ------------------------------------------------------------------------------------------ - //
// A set of macros that combine the concept of testing and returning
// - ------------------------------------------------------------------------------------------ - //
//typedef const int __return_if_error_type;	// Assuming the standard error type is a signed integer
// - ------------------------------------------------------------------------------------------ - //
// Conditional Return //
#define return_if(__TEST) { if (__TEST) { return __TEST; } }
#define return_void_if(__TEST) { if (__TEST) { return; } }
#define return_null_if(__TEST) { if (__TEST) { return 0; } }
#define return_nullptr_if(__TEST) { if (__TEST) { return nullptr; } }		// C++11 only
#define return_value_if(__TEST, __RETCODE) { if (__TEST) { return __RETCODE; } }
//#define return_if_error( __TEST ) { if ( __return_if_error_type __Error ## __COUNTER__ = (__return_if_error_type)(__TEST) ) { return __Error ## __COUNTER__; } }

//#define return_if( __TEST ) { if ( bool __Error ## __COUNTER__ = (__TEST) ) { return __Error ## __COUNTER__; } }

// Undconditionally Return (included for syntactical completeness)
//#define return( __TEST ) { return (__TEST); } // DUH, this already exists in the C language!
//#define return_void( __TEST ) { (__TEST); return; }
//#define return_value( __RETCODE, __TEST ) { (__TEST); return __RETCODE; }
// - ------------------------------------------------------------------------------------------ - //


// TODO disable with debug mode
#define error_return_if(__TEST) { if (__TEST) { Error("<%s> (%s:%i) error_return_if: %i", __func__, __FILE__, __LINE__, (int)__TEST); return __TEST; } }
#define error_return_void_if(__TEST) { if (__TEST) { Error("<%s> (%s:%i) error_return_void_if: %i", __func__, __FILE__, __LINE__, (int)__TEST); return; } }
#define error_return_null_if(__TEST) { if (__TEST) { Error("<%s> (%s:%i) error_return_null_if: %i", __func__, __FILE__, __LINE__, (int)__TEST); return 0; } }
#define error_return_nullptr_if(__TEST) { if (__TEST) { Error("<%s> (%s:%i) error_return_nullptr_if: %i", __func__, __FILE__, __LINE__, (int)__TEST); return nullptr; } }	// C++11 only
#define error_return_value_if(__TEST, __RETCODE) { if (__TEST) { Error("<%s> (%s:%i) error_return_value_if: %i", __func__, __FILE__, __LINE__, (int)__TEST); return __RETCODE; } }
