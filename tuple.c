#include <stdlib.h>
#include <stdio.h>
#include <stdalign.h>
#ifdef OS_WINDOWS
	#define HARBOL_LIB
#endif
#include "harbol.h"

typedef uint32_t tuple_size_t;
typedef union {
	uint64_t PackedInt64;
	struct { tuple_size_t Offset, Size; };
	union HarbolValue Val;
} TupleElement;

/*
static size_t CalcPadding(const size_t offset, const size_t align)
{
	return -offset & (align - 1);
}
*/

static size_t AlignSize(const size_t size, const size_t align)
{
	return (size + (align-1)) & -align;
}


HARBOL_EXPORT struct HarbolTuple *HarbolTuple_New(const size_t array_len, const size_t datasizes[static array_len], const bool packed)
{
	struct HarbolTuple *tup = calloc(1, sizeof *tup);
	HarbolTuple_Init(tup, array_len, datasizes, packed);
	return tup;
}

HARBOL_EXPORT bool HarbolTuple_Free(struct HarbolTuple **tupref)
{
	if( !tupref || !*tupref )
		return false;
	HarbolTuple_Del(*tupref);
	free(*tupref), *tupref=NULL;
	return true;
}

HARBOL_EXPORT void HarbolTuple_Init(struct HarbolTuple *const tup, const size_t array_len, const size_t datasizes[static array_len], const bool packed)
{
	if( !tup )
		return;
	
	memset(tup, 0, sizeof *tup);
	const size_t sizeptr = sizeof(intptr_t);
	size_t largestmemb=0;
	// first we find the largest member of the tuple:
	for( size_t i=0 ; i<array_len ; i++ )
		if( largestmemb<datasizes[i] )
			largestmemb=datasizes[i];
	
	// next, compute padding and alignment:
	// we do this by having a next and previous size.
	size_t
		totalsize=0,
		prevsize=0
	;
	for( size_t i=0 ; i<array_len ; i++ ) {
		totalsize += datasizes[i];
		if( packed )
			continue;
		const size_t offalign = (i+1<array_len) ? datasizes[i+1] : prevsize;
		totalsize = AlignSize(totalsize, offalign>=sizeptr ? sizeptr : offalign);
		prevsize = datasizes[i];
	}
	// now do a final size alignment with the largest member.
	const size_t aligned_total = AlignSize(totalsize, largestmemb>=sizeptr ? sizeptr : largestmemb);
	
	tup->Datum = calloc(packed ? totalsize : aligned_total, sizeof *tup->Datum);
	if( !tup->Datum )
		return;
	
	tup->Len = packed ? totalsize : aligned_total;
	tuple_size_t offset = 0;
	for( size_t i=0 ; i<array_len ; i++ ) {
		TupleElement field = {0};
		field.Size = datasizes[i];
		field.Offset = offset;
		
		HarbolVector_Insert(&tup->Fields, field.Val);
		offset += datasizes[i];
		if( packed )
			continue;
		const size_t offalign = (i+1<array_len) ? datasizes[i+1] : prevsize;
		offset = AlignSize(offset, offalign>=sizeptr ? sizeptr : offalign);
		prevsize = datasizes[i];
	}
}

HARBOL_EXPORT void HarbolTuple_Del(struct HarbolTuple *const tup)
{
	if( !tup )
		return;
	HarbolVector_Del(&tup->Fields, NULL);
	free(tup->Datum);
	memset(tup, 0, sizeof *tup);
}

HARBOL_EXPORT size_t HarbolTuple_Len(const struct HarbolTuple *const tup)
{
	return tup ? tup->Len : 0;
}

HARBOL_EXPORT void *HarbolTuple_GetField(const struct HarbolTuple *const tup, const size_t index)
{
	if( !tup || !tup->Datum || index>=tup->Fields.Count )
		return NULL;
	const TupleElement field = {HarbolVector_Get(&tup->Fields, index).UInt64};
	return ( field.Offset >= tup->Len ) ? NULL : tup->Datum + field.Offset;
}

HARBOL_EXPORT size_t HarbolTuple_GetSize(const struct HarbolTuple *const tup, const size_t index)
{
	if( !tup || !tup->Datum || index>=tup->Fields.Count )
		return 0;
	const TupleElement field = {HarbolVector_Get(&tup->Fields, index).UInt64};
	return field.Size;
}

/*
HARBOL_EXPORT void *HarbolTuple_GetRawData(const struct HarbolTuple *const tup)
{
	return tup ? tup->Datum : NULL;
}
*/

HARBOL_EXPORT bool HarbolTuple_ToStruct(const struct HarbolTuple *const restrict tup, void *restrict structptr)
{
	if( !tup || !structptr || !tup->Datum )
		return false;
	
	memcpy(structptr, tup->Datum, tup->Len);
	return true;
}
