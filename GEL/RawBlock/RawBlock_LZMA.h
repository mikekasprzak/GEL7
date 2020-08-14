#pragma once
// - ------------------------------------------------------------------------------------------ - //
// DataBlockCompression - Compression code for DataBlock library //
// - ------------------------------------------------------------------------------------------ - //
#ifdef _MSC_VER
#pragma warning (disable : 4305) // Truncation
#pragma warning (disable : 4244) // Tructation, data loss potential 
#endif // _MSC_VER
// - ------------------------------------------------------------------------------------------ - //
#include "RawBlock_Core.h"
// - ------------------------------------------------------------------------------------------ - //
#include <lzma/lzma.h>
// - ------------------------------------------------------------------------------------------ - //
// Decode packed LZMA data to a new RawBlock
inline RawBlock* rawBlock_LZMAUnpackNew( const RawBlock* _Src ) {
	// Take a copy of the File Header (for some reason)
	unsigned char header[LZMA_PROPS_SIZE + 8];
	raw_Copy( &_Src->data[0], &header[0], sizeof( header ) );

	// Get the Uncompressed Size from the properties (WARNING! 5 BYTES IN!) //
	//UInt64 UncompressedSize = *((UInt64*)&_Src->data[LZMA_PROPS_SIZE]);	
	// Hack to avoid alignment issues //
	UInt64 UncompressedSize = (*((UInt64*)&_Src->data[4])) >> 8;	

	// Allocate LZMA Workspace //
	CLzmaDec state;
	LzmaDec_Construct(&state);
	{
		int Result = LzmaDec_Allocate(&state, header, LZMA_PROPS_SIZE, &lzma_alloc_struct);
		if (Result != SZ_OK)
			return 0;
	}

	// Allocate a new DataBlock for our uncompressed Data //
	RawBlock* UBuffer = rawBlock_New( UncompressedSize );

	SizeT destLength = (SizeT)UncompressedSize;
	SizeT srcLength = (SizeT)_Src->size;
	ELzmaStatus LZMAStatus;

	// Initialize the LZMA Decoder //
	LzmaDec_Init(&state);

	// Decompress File //
    /*int Result =*/ LzmaDec_DecodeToBuf(
		&state,
		(unsigned char*)UBuffer->data,
		&destLength, 
        (unsigned char*)&_Src->data[ LZMA_PROPS_SIZE + 8 ],
		&srcLength,
		LZMA_FINISH_END,
		&LZMAStatus
		);

	// NOTE: This call can do *some bytes* by changing to LZMA_FINISH_ANY, and calling multiple times //
	// TODO: Something about errors //

	// Shutdown the LZMA Decoder
	LzmaDec_Free(&state, &lzma_alloc_struct);

	return UBuffer;

/*
	unsigned char LZMAProperties[LZMA_PROPERTIES_SIZE];
	
	// Copy first byte of LZMA Properties.  Somehow, this is vitally important. //
	*LZMAProperties = _Src->Data[0];
	
	// Get the Uncompressed Size from the properties //
	unsigned long long int UncompressedSize = *((unsigned long long int*)&_Src->Data[LZMA_PROPERTIES_SIZE]);
	
	// Decoder State structure //
	CLzmaDecoderState state;
	
	// Decode the properties //
	if( LzmaDecodeProperties( &state.Properties, LZMAProperties, LZMA_PROPERTIES_SIZE) != LZMA_RESULT_OK ) {
		//Log( LOG_MESH_INFO, "Incorrect stream properties (LZMA.h)" );
 	  	return 0;
    }
	
#ifdef _MSC_VER
	CProb* Probs = new CProb[ LzmaGetNumProbs(&state.Properties) ];
#else // _MSC_VER //
	CProb Probs[ LzmaGetNumProbs(&state.Properties) ];
#endif // _MSC_VER //
	state.Probs = (UInt16*)&Probs;
	
	// Allocate a new DataBlock for our uncompressed Data //
	DataBlock* UBuffer = new_DataBlock( UncompressedSize );

	// Temporary variables modified by decode function, so to learn about how it went //
	SizeT inProcessed = 0;
	SizeT outProcessed = 0;
	
	// Decode the file //
	int Result = LzmaDecode( 
		&state,
		( unsigned char* )&_Src->Data[ LZMA_PROPERTIES_SIZE + 8 ],
		_Src->Size,
		&inProcessed,
		( unsigned char* )UBuffer->Data,
		UncompressedSize,
		&outProcessed
		);

#ifdef _MSC_VER
	delete [] Probs;
#endif // _MSC_VER //

	// Return our new LZMA decompressed data //
	return UBuffer;
*/
}
// - ------------------------------------------------------------------------------------------ - //
// Decode packed LZMA data to a passed DataBlock //
inline SizeT rawBlock_LZMAUnpack( const RawBlock* _Src, RawBlock* _Dest ) {
	// Take a copy of the File Header (for some reason) //
	unsigned char header[LZMA_PROPS_SIZE + 8];
	raw_Copy( &_Src->data[0], &header[0], sizeof( header ) );

	// Get the Uncompressed Size from the properties (WARNING! 5 BYTES IN!) //
	UInt64 UncompressedSize = *((UInt64*)&_Src->data[LZMA_PROPS_SIZE]);	

	// If the passed DataBlock is too small for our uncompressed data, fail //
	if ( UncompressedSize > _Dest->size ) {
		// Not enough memory available in passed block! //
		return 0;
	}

	// Allocate LZMA Workspace //
	CLzmaDec state;
	LzmaDec_Construct(&state);
	{
		int Result = LzmaDec_Allocate(&state, header, LZMA_PROPS_SIZE, &lzma_alloc_struct);
		if (Result != SZ_OK)
			return 0;
	}

	SizeT destLength = (SizeT)UncompressedSize;
	SizeT srcLength = (SizeT)_Src->size;
	ELzmaStatus LZMAStatus;

	// Initialize the LZMA Decoder //
	LzmaDec_Init(&state);

	// Decompress File //
    /*int Result =*/ LzmaDec_DecodeToBuf(
		&state,
		(unsigned char*)&_Dest->data,
		&destLength, 
        (unsigned char*)&_Src->data[ LZMA_PROPS_SIZE + 8 ],
		&srcLength,
		LZMA_FINISH_END,
		&LZMAStatus
		);

	// NOTE: This call can do *some bytes* by changing to LZMA_FINISH_ANY, and calling multiple times //
	// TODO: Something about errors //

	// Shutdown the LZMA Decoder
	LzmaDec_Free(&state, &lzma_alloc_struct);

	return UncompressedSize;

/*
	unsigned char LZMAProperties[LZMA_PROPERTIES_SIZE];
	
	// Copy first byte of LZMA Properties.  Somehow, this is vitally important. //
	*LZMAProperties = _Src->Data[0];
	
	// Get the Uncompressed Size from the properties //
	unsigned long long int UncompressedSize = *((unsigned long long int*)&_Src->Data[LZMA_PROPERTIES_SIZE]);
	
	// If the passed DataBlock is too small for our uncompressed data, fail //
	if ( UncompressedSize > _Dest->Size ) {
		// Not enough memory available in passed block! //
		return 0;
	}
	
	// Decoder State structure //
	CLzmaDecoderState state;
	
	// Decode the properties //
	if( LzmaDecodeProperties( &state.Properties, LZMAProperties, LZMA_PROPERTIES_SIZE) != LZMA_RESULT_OK ) {
		//Log( LOG_MESH_INFO, "Incorrect stream properties (LZMA.h)" );
 	  	return 0;
    }

#ifdef _MSC_VER
	CProb* Probs = new CProb[ LzmaGetNumProbs(&state.Properties) ];
#else // _MSC_VER //
	CProb Probs[ LzmaGetNumProbs(&state.Properties) ];
#endif // _MSC_VER //
	state.Probs = (UInt16*)&Probs;

	// Temporary variables modified by decode function, so to learn about how it went //
	SizeT inProcessed = 0;
	SizeT outProcessed = 0;
	
	// Decode the file //
	int Result = LzmaDecode( 
		&state,
		( unsigned char* )&_Src->Data[ LZMA_PROPERTIES_SIZE + 8 ],
		_Src->Size,
		&inProcessed,
		( unsigned char* )_Dest->Data,
		UncompressedSize,
		&outProcessed
		);

#ifdef _MSC_VER
	delete [] Probs;
#endif // _MSC_VER //

	// Return the size of the uncompressed data on success //
	return UncompressedSize;
*/
}
// - ------------------------------------------------------------------------------------------ - //
