#pragma once 
// - ------------------------------------------------------------------------------------------ - //
// GelFile - C "FILE*" wrapping and utility library //
// - ------------------------------------------------------------------------------------------ - //
// Core functions
#ifdef USES_SDL2
#include "GelFile_SDL2.h"
#elif defined(USES_FILEIO_STUB)
#include "GelFile_Stub.h"
#else // USES_STDIO
#include "GelFile_STDIO.h"
#endif // USES_FILEIO
// - ------------------------------------------------------------------------------------------ - //
// Helper functions
#include "GenerateReadWriteFunctions.h"
MAKE_READWRITE_FUNCTIONS(gelFile, GelFile);
#undef MAKE_READWRITE_FUNCTIONS
// - ------------------------------------------------------------------------------------------ - //
