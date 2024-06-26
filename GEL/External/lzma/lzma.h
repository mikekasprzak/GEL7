#pragma once
// - ------------------------------------------------------------------------------------------ - //
#include "LzmaTypes.h"
#include "LzmaDec.h"
// - ------------------------------------------------------------------------------------------ - //
//void *alloc_LZMA(void *p, SizeT size);
//void free_LZMA(void *p, void *address);
// - ------------------------------------------------------------------------------------------ - //
// VTable for allocating the memory LZMA needs
extern ISzAlloc lzma_alloc_struct;
// - ------------------------------------------------------------------------------------------ - //
