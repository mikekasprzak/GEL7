#pragma once
// - ------------------------------------------------------------------------------------------ - //
// DataCompression - Compression code for Data library
// - ------------------------------------------------------------------------------------------ - //
#include "Raw_Core.h"
// - ------------------------------------------------------------------------------------------ - //
#include <lzma/lzma.h>
// - ------------------------------------------------------------------------------------------ - //
// Decode packed LZMA data to a void* (Size is lost though, as it isn't returned)
// - ------------------------------------------------------------------------------------------ - //
inline void* raw_LZMAUnpack( const void* _Src, const st32 _SrcSize, st32* _DestSize = 0 ) {
	const char* Src = reinterpret_cast<const char*>(_Src);

	// Take a copy of the File Header (for some reason) //
	unsigned char header[LZMA_PROPS_SIZE + 8];
	raw_Copy( Src, &header[0], sizeof( header ) );

	// Get the Uncompressed Size from the properties (WARNING! 5 BYTES IN!) //
	UInt64 UncompressedSize = (*((UInt64*)&Src[4]))>>8;
//	UInt64 UncompressedSize = *((UInt64*)&Src[LZMA_PROPS_SIZE]);	

	// Allocate LZMA Workspace //
	CLzmaDec state;
	LzmaDec_Construct(&state);
	{
		int Result = LzmaDec_Allocate(&state, header, LZMA_PROPS_SIZE, &lzma_alloc_struct);
		if (Result != SZ_OK)
			return 0;
	}

	// Allocate a new RawBlock for our uncompressed data
	RawBlock* UBuffer = rawBlock_New(UncompressedSize);

	SizeT destLength = (SizeT)UncompressedSize;
	SizeT srcLength = _SrcSize;
	ELzmaStatus LZMAStatus;

	// Initialize the LZMA Decoder //
	LzmaDec_Init(&state);

	// Decompress File //
    /*int Result =*/ LzmaDec_DecodeToBuf(
		&state,
		(unsigned char*)UBuffer->data,
		&destLength, 
        (unsigned char*)&Src[LZMA_PROPS_SIZE + 8],
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
	const char* Src = reinterpret_cast<const char*>(_Src);
	
	// Copy first byte of LZMA Properties.  Somehow, this is vitally important. //
	*LZMAProperties = Src[0];
	
	// Get the Uncompressed Size from the properties //
	UInt64 UncompressedSize = *((Int64*)&Src[LZMA_PROPERTIES_SIZE]);
	
	// If a destination size was passed //
	if ( _DestSize )
		*_DestSize = UncompressedSize;
	
	// Decoder State structure //
	CLzmaDecoderState state;
	
	// Decode the properties //
	if( LzmaDecodeProperties( &state.Properties, LZMAProperties, LZMA_PROPERTIES_SIZE) != LZMA_RESULT_OK ) {
		//Log( LOG_MESH_INFO, "Incorrect stream properties (LZMA.h)" );
 	  	return 0;
    }

	CProb Probs[ LzmaGetNumProbs(&state.Properties) ];
	state.Probs = (UInt16*)&Probs;
	
	// Allocate a new DataBlock for our uncompressed Data //
	const char* UBuffer = reinterpret_cast<const char*>(new_Data( UncompressedSize ));

	// Temporary variables modified by decode function, so to learn about how it went //
	SizeT inProcessed = 0;
	SizeT outProcessed = 0;
	
	// Decode the file //
	int Result = LzmaDecode( 
		&state,
		( unsigned char* )&Src[ LZMA_PROPERTIES_SIZE + 8 ],
		_Size,
		&inProcessed,
		( unsigned char* )UBuffer,
		UncompressedSize,
		&outProcessed
		);
	
	// Return our new LZMA decompressed data //
	return reinterpret_cast<const void*>(UBuffer);
*/
}
// - ------------------------------------------------------------------------------------------ - //
// Decode packed LZMA data to a passed void* and st32 //
// - ------------------------------------------------------------------------------------------ - //
inline st raw_LZMAUnpack( const void* _Src, const st32 _SrcSize, const void** _Dest, const st32 _DestSize ) {
	const char* Src = reinterpret_cast<const char*>(_Src);

	// Take a copy of the File Header (for some reason) //
	unsigned char header[LZMA_PROPS_SIZE + 8];
	raw_Copy( Src, &header[0], sizeof( header ) );

	// Get the Uncompressed Size from the properties (WARNING! 5 BYTES IN!) //
	UInt64 UncompressedSize = *((UInt64*)&Src[LZMA_PROPS_SIZE]);	

	// If the passed DataBlock is too small for our uncompressed data, fail //
	if ( UncompressedSize > _DestSize ) {
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
	SizeT srcLength = _SrcSize;
	ELzmaStatus LZMAStatus;

	// Initialize the LZMA Decoder //
	LzmaDec_Init(&state);

	// Decompress File //
    /*int Result =*/ LzmaDec_DecodeToBuf(
		&state,
		(unsigned char*)_Dest,
		&destLength, 
        (unsigned char*)&Src[ LZMA_PROPS_SIZE + 8 ],
		&srcLength,
		LZMA_FINISH_END,
		&LZMAStatus
		);

	// NOTE: This call can do *some bytes* by changing to LZMA_FINISH_ANY, and calling multiple times //
	// TODO: Something about errors //

	// Shutdown the LZMA Decoder
	LzmaDec_Free(&state, &lzma_alloc_struct);

	return UncompressedSize;

/*	unsigned char LZMAProperties[LZMA_PROPERTIES_SIZE];
	const char* Src = reinterpret_cast<const char*>(_Src);
	
	// Copy first byte of LZMA Properties.  Somehow, this is vitally important. //
	*LZMAProperties = Src[0];
	
	// Get the Uncompressed Size from the properties //
	unsigned long long int UncompressedSize = *((unsigned long long int*)&Src[LZMA_PROPERTIES_SIZE]);

	// If the passed DataBlock is too small for our uncompressed data, fail //
	if ( UncompressedSize > _DestSize ) {
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

	CProb Probs[ LzmaGetNumProbs(&state.Properties) ];
	state.Probs = (UInt16*)&Probs;
	
	// Temporary variables modified by decode function, so to learn about how it went //
	SizeT inProcessed = 0;
	SizeT outProcessed = 0;
	
	// Decode the file //
	int Result = LzmaDecode( 
		&state,
		( unsigned char* )&Src[ LZMA_PROPERTIES_SIZE + 8 ],
		_Size,
		&inProcessed,
		( unsigned char* )_Dest,
		UncompressedSize,
		&outProcessed
		);

	// Return our new LZMA decompressed data //
	return UncompressedSize;
*/
}
// - ------------------------------------------------------------------------------------------ - //
