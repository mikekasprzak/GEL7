#pragma once
// - ------------------------------------------------------------------------------------------ - //
#ifdef _MSC_VER
#pragma warning (disable : 4200) // Zero sized array member
#endif // _MSC_VER
// - ------------------------------------------------------------------------------------------ - //
#include <GEL/Types/Types.h>
#include <GEL/Raw/Raw_Core.h>
// - ------------------------------------------------------------------------------------------ - //
struct RawBlock {
	st32 size; // NOT size_t due to 64bit compatibility
	char data[0];
};
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
inline RawBlock* rawBlock_New( const st32 _size ) {
	RawBlock* p = reinterpret_cast<RawBlock*>(raw_New(_size + sizeof(RawBlock)));

	if (p) {
		p->size = _size;
		return p;
	}
	return 0;
}
// - ------------------------------------------------------------------------------------------ - //
inline void rawBlock_Delete( RawBlock* p ) {
	raw_Delete(reinterpret_cast<char*>(p));
}
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
// Use this alternative "new" function when you want to initialize the block to a value
// - ------------------------------------------------------------------------------------------ - //
inline RawBlock* rawBlock_New( const st32 _size, const int _initValue ) {
	// Allocate it
	RawBlock* newBlock = rawBlock_New(_size);
	
	// Initialize it
	raw_Set(_initValue, newBlock->data, newBlock->size);
	
	// Return it
	return newBlock;
}
// - ------------------------------------------------------------------------------------------ - //


// - ------------------------------------------------------------------------------------------ - //
// Copy one DataBlock to another, no larger than Destination Size Bytes
// - ------------------------------------------------------------------------------------------ - //
inline void rawBlock_Copy( const RawBlock* _src, RawBlock* _dest ) {
	// If source is smaller than the destination
	if (_dest->size > _src->size) {
		// Copy source number of bytes
		raw_Copy(_src->data, _dest->data, _src->size);
	}
	else {
		// Otherwise, copy destination number of bytes
		raw_Copy(_src->data, _dest->data, _dest->size);
	}
}
// - ------------------------------------------------------------------------------------------ - //
inline void rawBlock_Copy( const char* _src, const st32 _srcSize, RawBlock* _dest ) {
	// If source is smaller than the destination
	if (_dest->size > _srcSize) {
		// Copy source number of bytes
		raw_Copy(_src, _dest->data, _srcSize);
	}
	else {
		// Otherwise, copy destination number of bytes
		raw_Copy(_src, _dest->data, _dest->size);
	}
}
// - ------------------------------------------------------------------------------------------ - //
// Return a duplicate of a DataBlock
// - ------------------------------------------------------------------------------------------ - //
inline RawBlock* rawBlock_Copy( const RawBlock* _src ) {
	// Allocate our new block
	RawBlock* newBlock = rawBlock_New(_src->size);
	
	// Copy the data to our new block
	rawBlock_Copy(_src, newBlock);
	
	// Return the block
	return newBlock;
}
// - ------------------------------------------------------------------------------------------ - //
inline RawBlock* rawBlock_New( const RawBlock* _src ) {
	return rawBlock_Copy(_src);
}
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
inline RawBlock* rawBlock_Copy( const char* _src, const st32 _srcSize ) {
	// Allocate our new block
	RawBlock* newBlock = rawBlock_New(_srcSize);
	
	// Copy the data to our new block
	rawBlock_Copy(_src, _srcSize, newBlock);
	
	// Return the block
	return newBlock;
}
// - ------------------------------------------------------------------------------------------ - //
inline RawBlock* rawBlock_New( const char* _src, const st32 _srcSize ) {
	return rawBlock_Copy(_src, _srcSize);
}
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
// Set DataBlock
// - ------------------------------------------------------------------------------------------ - //
inline void rawBlock_Set( RawBlock* _data, const int _initValue ) {
	raw_Set(_initValue, _data->data, _data->size);
}
// - ------------------------------------------------------------------------------------------ - //


// - ------------------------------------------------------------------------------------------ - //
// A useful accessory, resizing.
// Reducing size can work safely, but expanding would require new allocation
//   this is because new/delete ignore my internal size variable
// - ------------------------------------------------------------------------------------------ - //
inline void rawBlock_Reallocate( RawBlock** p, const st32 _newSize ) {
	// Allocate our new block
	RawBlock* newBlock = rawBlock_New(_newSize);
	
	// Copy the data to our new block
	rawBlock_Copy(*p, newBlock);
	
	// Delete the old block ponted to
	rawBlock_Delete(*p);
	
	// Make the pointer point to the new block
	(*p) = newBlock;
}
// - ------------------------------------------------------------------------------------------ - //
inline void rawBlock_Resize( RawBlock** p, const st32 _newSize ) {
	// A cheat. We can resize the block without reallocating
	if ( _newSize <= (*p)->size ) {
		// Set the size to the new size, and we're done
		(*p)->size = _newSize;
	}
	else {
		// Well, we tried. We need to reallocate and copy the data.
		rawBlock_Reallocate(p, _newSize);
	}
}
// - ------------------------------------------------------------------------------------------ - //
// Variation of reallocate that looks at the internal size
// - ------------------------------------------------------------------------------------------ - //
inline void rawBlock_Reallocate( RawBlock** p ) {
	rawBlock_Reallocate(p, (*p)->size);
}
// - ------------------------------------------------------------------------------------------ - //


// - ------------------------------------------------------------------------------------------ - //
// Initializer capable versions of resize, reallocate, and copy
// - ------------------------------------------------------------------------------------------ - //
// Copy one DataBlock to another, no larger than destination->size bytes
inline void rawBlock_Copy( RawBlock* _src, RawBlock* _dest, const int _initValue ) {
	// If source is smaller than the destination
	if (_dest->size > _src->size) {
		// Copy source number of bytes
		raw_Copy(_src->data, _dest->data, _src->size);
		// Fill the rest of the data with initializer
		raw_Set(_initValue, &_dest->data[_src->size], _dest->size - _src->size);
	}
	else {
		// Otherwise, copy destination number of bytes
		raw_Copy(_src->data, _dest->data, _dest->size);
	}
}
// - ------------------------------------------------------------------------------------------ - //
inline void rawBlock_Reallocate( RawBlock** p, const st32 _newSize, const int _initValue ) {
	// Allocate our new block
	RawBlock* newBlock = rawBlock_New(_newSize);
	
	// Copy the data to our new block
	rawBlock_Copy(*p, newBlock, _initValue);
	
	// Delete the old block ponted to
	rawBlock_Delete(*p);
	
	// Make the pointer point to the new block
	(*p) = newBlock;
}
// - ------------------------------------------------------------------------------------------ - //
inline void rawBlock_Resize( RawBlock** p, const st32 _newSize, const int _initValue ) {
	// A cheat. We can resize the block without reallocating
	if (_newSize <= (*p)->size) {
		//st32 oldSize = (*p)->size;
		
		// Set the size to the new size, and we're done
		(*p)->size = _newSize;
		
		// Note this will never happen, since this is normally a max size test
//		// If this grows us
//		if (oldSize < _newSize) {
//			// Fill in new values with passed value
//			for (st idx = (_newSize - oldSize); idx--;) {
//				(*p)->data[oldSize + idx] = _initValue;
//			}
//		}
	}
	else {
		// Well, we tried.  We need to reallocate and copy the data.
		rawBlock_Reallocate(p, _newSize, _initValue);
	}
}
// - ------------------------------------------------------------------------------------------ - //

// - ------------------------------------------------------------------------------------------ - //
inline void rawBlock_PushBack( RawBlock** p, const int _initValue ) {
	rawBlock_Resize(p, (*p)->size + 1, _initValue);
} 
// - ------------------------------------------------------------------------------------------ - //
inline int rawBlock_PopBack( RawBlock** p ) {
	// TODO: Assert if Size == 0
	(*p)->size--;
	
	return (*p)->data[(*p)->size];
} 
// - ------------------------------------------------------------------------------------------ - //
