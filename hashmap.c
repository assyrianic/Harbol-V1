#include <stdlib.h>
#include <stdio.h>
#ifdef OS_WINDOWS
	#define HARBOL_LIB
#endif
#include "harbol.h"


HARBOL_EXPORT struct HarbolKeyValPair *HarbolKeyValPair_New(void)
{
	return calloc(1, sizeof(struct HarbolKeyValPair));
}

HARBOL_EXPORT struct HarbolKeyValPair *HarbolKeyValPair_NewSP(const char cstr[restrict], const union HarbolValue val)
{
	struct HarbolKeyValPair *restrict n = HarbolKeyValPair_New();
	if( n ) {
		HarbolString_InitStr(&n->KeyName, cstr);
		n->Data = val;
	}
	return n;
}

HARBOL_EXPORT void HarbolKeyValPair_Del(struct HarbolKeyValPair *const n, fnDestructor *const dtor)
{
	if( !n )
		return;
	
	HarbolString_Del(&n->KeyName);
	if( dtor )
		(*dtor)(&n->Data.Ptr);
}

HARBOL_EXPORT void HarbolKeyValPair_Free(struct HarbolKeyValPair **noderef, fnDestructor *const dtor)
{
	if( !noderef || !*noderef )
		return;
	
	HarbolKeyValPair_Del(*noderef, dtor);
	free(*noderef), *noderef=NULL;
}

// size_t general hash function.
HARBOL_EXPORT size_t GenHash(const char cstr[restrict])
{
	if( !cstr )
		return SIZE_MAX;
	
	size_t h = 0;
	while( *cstr )
		h = 37 * h + *cstr++;
	return h;
}

uint32_t Int32Hash(uint32_t a)
{
	a = (a+0x7ed55d16) + (a<<12);
	a = (a^0xc761c23c) ^ (a>>19);
	a = (a+0x165667b1) + (a<<5);
	a = (a+0xd3a2646c) ^ (a<<9);
	a = (a+0xfd7046c5) + (a<<3);
	a = (a^0xb55a4f09) ^ (a>>16);
	return a;
}

uint64_t Int64Hash(uint64_t a)
{
	a = (~a) + (a << 21);
	a = a ^ (a >> 24);
	a = (a + (a << 3)) + (a << 8);
	a = a ^ (a >> 14);
	a = (a + (a << 2)) + (a << 4);
	a = a ^ (a >> 28);
	a = a + (a << 31);
	return a;
}

size_t GenIntHash(size_t a)
{
	return sizeof(size_t)==4 ? Int32Hash(a) : sizeof(size_t)==8 ? Int64Hash(a) : 0;
}

size_t PtrHash(const void *const p)
{
	size_t y = (size_t)p;
	return (y >> 4u) | (y << (8u * sizeof(void *) - 4u));
}

size_t FloatHash(const float fval)
{
	union {
		float f;
		size_t s;
	} conv = {fval};
	return GenIntHash(conv.s);
}

uint64_t DoubleHash(const double dbl)
{
	union {
		double d;
		uint64_t l;
	} conv = {dbl};
	return Int64Hash(conv.l);
}


HARBOL_EXPORT struct HarbolHashmap *HarbolMap_New(void)
{
	struct HarbolHashmap *map = calloc(1, sizeof *map);
	return map;
}

HARBOL_EXPORT void HarbolMap_Init(struct HarbolHashmap *const map)
{
	if( !map )
		return;
	
	memset(map, 0, sizeof *map);
}

HARBOL_EXPORT void HarbolMap_Del(struct HarbolHashmap *const map, fnDestructor *const dtor)
{
	if( !map || !map->Table )
		return;
	
	for( size_t i=0 ; i<map->Len ; i++ ) {
		const union HarbolValue *const end = HarbolVector_GetIterEndLen(map->Table+i);
		for( union HarbolValue *iter=HarbolVector_GetIter(map->Table+i) ; iter && iter != end ; iter++ ) {
			struct HarbolKeyValPair *kv = iter->Ptr;
			HarbolKeyValPair_Free(&kv, dtor);
		}
		HarbolVector_Del(map->Table+i, NULL);
	}
	free(map->Table), map->Table=NULL;
	memset(map, 0, sizeof *map);
}

HARBOL_EXPORT void HarbolMap_Free(struct HarbolHashmap **mapref, fnDestructor *const dtor)
{
	if( !*mapref )
		return;
	
	HarbolMap_Del(*mapref, dtor);
	free(*mapref), *mapref=NULL;
}

HARBOL_EXPORT size_t HarbolMap_Count(const struct HarbolHashmap *const map)
{
	return map ? map->Count : 0;
}

HARBOL_EXPORT size_t HarbolMap_Len(const struct HarbolHashmap *const map)
{
	return map ? map->Len : 0;
}

HARBOL_EXPORT bool HarbolMap_Rehash(struct HarbolHashmap *const map)
{
	if( !map || !map->Table )
		return false;
	
	const size_t old_size = map->Len;
	map->Len <<= 1;
	map->Count = 0;
	
	struct HarbolVector
		*curr = NULL,
		*temp = calloc(map->Len, sizeof *temp)
	;
	if( !temp ) {
		puts("**** Memory Allocation Error **** HarbolMap_Rehash::temp is NULL\n");
		map->Len = 0;
		return false;
	}
	
	curr = map->Table;
	map->Table = temp;
	
	for( size_t i=0 ; i<old_size ; i++ ) {
		const union HarbolValue *const end = HarbolVector_GetIterEndCount(curr+i);
		for( union HarbolValue *iter=HarbolVector_GetIter(curr+i) ; iter && iter != end ; iter++ ) {
			struct HarbolKeyValPair *node = iter->Ptr;
			HarbolMap_InsertNode(map, node);
		}
		HarbolVector_Del(curr+i, NULL);
	}
	free(curr), curr=NULL;
	return true;
}

HARBOL_EXPORT bool HarbolMap_InsertNode(struct HarbolHashmap *const map, struct HarbolKeyValPair *node)
{
	if( !map || !node || !node->KeyName.CStr )
		return false;
	
	else if( !map->Len ) {
		map->Len = 8;
		map->Table = calloc(map->Len, sizeof *map->Table);
		if( !map->Table ) {
			puts("**** Memory Allocation Error **** HarbolMap_InsertNode::map->Table is NULL\n");
			map->Len = 0;
			return false;
		}
	}
	else if( map->Count >= map->Len )
		HarbolMap_Rehash(map);
	else if( HarbolMap_HasKey(map, node->KeyName.CStr) ) {
		puts("HarbolMap_InsertNode::map already has entry!\n");
		return false;
	}
	
	const size_t hash = GenHash(node->KeyName.CStr) % map->Len;
	HarbolVector_Insert(map->Table + hash, (union HarbolValue){.Ptr=node});
	++map->Count;
	return true;
}

HARBOL_EXPORT bool HarbolMap_Insert(struct HarbolHashmap *const restrict map, const char strkey[restrict], const union HarbolValue val)
{
	if( !map || !strkey )
		return false;
	
	struct HarbolKeyValPair *node = HarbolKeyValPair_NewSP(strkey, val);
	bool b = HarbolMap_InsertNode(map, node);
	if( !b )
		HarbolKeyValPair_Free(&node, NULL);
	return b;
}

HARBOL_EXPORT union HarbolValue HarbolMap_Get(const struct HarbolHashmap *const restrict map, const char strkey[restrict])
{
	if( !map || !map->Table || !HarbolMap_HasKey(map, strkey) )
		return (union HarbolValue){0};
	
	const size_t hash = GenHash(strkey) % map->Len;
	const union HarbolValue *const end = HarbolVector_GetIterEndCount(map->Table+hash);
	for( union HarbolValue *iter=HarbolVector_GetIter(map->Table+hash) ; iter && iter != end ; iter++ ) {
		const struct HarbolKeyValPair *const restrict kv = iter->Ptr;
		if( !HarbolString_CmpCStr(&kv->KeyName, strkey) )
			return kv->Data;
	}
	return (union HarbolValue){0};
}

HARBOL_EXPORT void HarbolMap_Set(struct HarbolHashmap *const restrict map, const char strkey[restrict], const union HarbolValue val)
{
	if( !map || !HarbolMap_HasKey(map, strkey) )
		return;
	
	const size_t hash = GenHash(strkey) % map->Len;
	const union HarbolValue *const end = HarbolVector_GetIterEndCount(map->Table+hash);
	for( union HarbolValue *iter=HarbolVector_GetIter(map->Table+hash) ; iter && iter != end ; iter++ ) {
		struct HarbolKeyValPair *const restrict kv = iter->Ptr;
		if( !HarbolString_CmpCStr(&kv->KeyName, strkey) )
			kv->Data = val;
	}
}

HARBOL_EXPORT void HarbolMap_Delete(struct HarbolHashmap *const restrict map, const char strkey[restrict], fnDestructor *const dtor)
{
	if( !map || !map->Table || !HarbolMap_HasKey(map, strkey) )
		return;
	
	const size_t hash = GenHash(strkey) % map->Len;
	struct HarbolVector *restrict vec = map->Table+hash;
	for( size_t i=0 ; i<vec->Count ; i++ ) {
		struct HarbolKeyValPair *kv = vec->Table[i].Ptr;
		if( !HarbolString_CmpCStr(&kv->KeyName, strkey) ) {
			HarbolKeyValPair_Del(kv, dtor);
			HarbolVector_Delete(vec, i, NULL);
			map->Count--;
			break;
		}
	}
}

HARBOL_EXPORT bool HarbolMap_HasKey(const struct HarbolHashmap *const restrict map, const char strkey[restrict])
{
	if( !map || !map->Table )
		return false;
	
	const size_t hash = GenHash(strkey) % map->Len;
	const union HarbolValue *const end = HarbolVector_GetIterEndCount(map->Table+hash);
	for( union HarbolValue *iter=HarbolVector_GetIter(map->Table+hash) ; iter && iter != end ; iter++ ) {
		const struct HarbolKeyValPair *const restrict kv = iter->Ptr;
		if( !HarbolString_CmpCStr(&kv->KeyName, strkey) )
			return true;
	}
	return false;
}

HARBOL_EXPORT struct HarbolKeyValPair *HarbolMap_GetNode(const struct HarbolHashmap *const restrict map, const char strkey[restrict])
{
	if( !map || !strkey || !map->Table )
		return NULL;
	
	const size_t hash = GenHash(strkey) % map->Len;
	const union HarbolValue *const end = HarbolVector_GetIterEndCount(map->Table+hash);
	for( union HarbolValue *iter=HarbolVector_GetIter(map->Table+hash) ; iter && iter != end ; iter++ ) {
		struct HarbolKeyValPair *restrict kv = iter->Ptr;
		if( !HarbolString_CmpCStr(&kv->KeyName, strkey) )
			return kv;
	}
	return NULL;
}

HARBOL_EXPORT struct HarbolVector *HarbolMap_GetBuckets(const struct HarbolHashmap *const map)
{
	return map ? map->Table : NULL;
}

HARBOL_EXPORT void HarbolMap_FromHarbolUniList(struct HarbolHashmap *const map, const struct HarbolUniList *const list)
{
	if( !map || !list )
		return;
	
	size_t i=0;
	for( struct HarbolUniListNode *n=list->Head ; n ; n = n->Next ) {
		char cstrkey[21] = {0};
		sprintf(cstrkey, "%zu", i);
		HarbolMap_Insert(map, cstrkey, n->Data);
		i++;
	}
}

HARBOL_EXPORT void HarbolMap_FromHarbolBiList(struct HarbolHashmap *const map, const struct HarbolBiList *const list)
{
	if( !map || !list )
		return;
	
	size_t i=0;
	for( struct HarbolBiListNode *n=list->Head ; n ; n = n->Next ) {
		char cstrkey[21] = {0};
		sprintf(cstrkey, "%zu", i);
		HarbolMap_Insert(map, cstrkey, n->Data);
		i++;
	}
}

HARBOL_EXPORT void HarbolMap_FromHarbolVector(struct HarbolHashmap *const map, const struct HarbolVector *const v)
{
	if( !map || !v || !v->Table )
		return;
	
	for( size_t i=0 ; i<v->Count ; i++ ) {
		char cstrkey[21] = {0};
		sprintf(cstrkey, "%zu", i);
		HarbolMap_Insert(map, cstrkey, v->Table[i]);
	}
}

HARBOL_EXPORT void HarbolMap_FromHarbolGraph(struct HarbolHashmap *const map, const struct HarbolGraph *const graph)
{
	if( !map || !graph )
		return;
	
	for( size_t i=0 ; i<graph->Vertices.Count ; i++ ) {
		char cstrkey[21] = {0};
		sprintf(cstrkey, "%zu", i);
		struct HarbolGraphVertex *vert = graph->Vertices.Table[i].Ptr;
		HarbolMap_Insert(map, cstrkey, vert->Data);
	}
}

HARBOL_EXPORT void HarbolMap_FromHarbolLinkMap(struct HarbolHashmap *const map, const struct HarbolLinkMap *const linkmap)
{
	if( !map || !linkmap )
		return;
	
	for( size_t i=0 ; i<linkmap->Order.Count ; i++ ) {
		struct HarbolKeyValPair *n = linkmap->Order.Table[i].Ptr;
		HarbolMap_InsertNode(map, HarbolKeyValPair_NewSP(n->KeyName.CStr, n->Data));
	}
}

HARBOL_EXPORT struct HarbolHashmap *HarbolMap_NewFromHarbolUniList(const struct HarbolUniList *const list)
{
	if( !list )
		return NULL;
	
	struct HarbolHashmap *map = HarbolMap_New();
	HarbolMap_FromHarbolUniList(map, list);
	return map;
}

HARBOL_EXPORT struct HarbolHashmap *HarbolMap_NewFromHarbolBiList(const struct HarbolBiList *const list)
{
	if( !list )
		return NULL;
	
	struct HarbolHashmap *map = HarbolMap_New();
	HarbolMap_FromHarbolBiList(map, list);
	return map;
}

HARBOL_EXPORT struct HarbolHashmap *HarbolMap_NewFromHarbolVector(const struct HarbolVector *const v)
{
	if( !v )
		return NULL;
	
	struct HarbolHashmap *map = HarbolMap_New();
	HarbolMap_FromHarbolVector(map, v);
	return map;
}

HARBOL_EXPORT struct HarbolHashmap *HarbolMap_NewFromHarbolGraph(const struct HarbolGraph *const graph)
{
	if( !graph )
		return NULL;
	
	struct HarbolHashmap *map = HarbolMap_New();
	HarbolMap_FromHarbolGraph(map, graph);
	return map;
}

HARBOL_EXPORT struct HarbolHashmap *HarbolMap_NewFromHarbolLinkMap(const struct HarbolLinkMap *const linkmap)
{
	if( !linkmap )
		return NULL;
	
	struct HarbolHashmap *map = HarbolMap_New();
	HarbolMap_FromHarbolLinkMap(map, linkmap);
	return map;
}
