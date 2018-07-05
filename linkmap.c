
#include <stdlib.h>
#include <stdio.h>
#include "dsc.h"


struct LinkNode *LinkNode_New(void)
{
#ifdef DSC_NO_MALLOC
	return Heap_Alloc(&__heappool, sizeof(struct LinkNode));
#else
	return calloc(1, sizeof(struct LinkNode));
#endif
}

struct LinkNode *LinkNode_NewSP(const char *restrict cstr, const union Value val)
{
	struct LinkNode *n = LinkNode_New();
	if( n ) {
		String_InitStr(&n->KeyName, cstr);
		n->Data = val;
	}
	return n;
}

void LinkNode_Del(struct LinkNode *const restrict n, bool (*dtor)())
{
	if( !n )
		return;
	
	String_Del(&n->KeyName);
	if( dtor )
		(*dtor)(&n->Data.Ptr);
	if( n->After )
		LinkNode_Free(&n->After, dtor);
}

bool LinkNode_Free(struct LinkNode **restrict noderef, bool (*dtor)())
{
	if( !*noderef )
		return false;
	
	LinkNode_Del(*noderef, dtor);
#ifdef DSC_NO_MALLOC
	Heap_Release(&__heappool, *noderef);
#else
	free(*noderef);
#endif
	*noderef=NULL;
	return true;
}


// size_t general hash function.
static size_t GenHash(const char *cstr)
{
	size_t h = 0;
	if( !cstr )
		return h;
	
	for( const char *restrict us = cstr ; *us ; us++ )
		h = 37 * h + *us;
	return h;
}


struct LinkMap *LinkMap_New(bool (*dtor)())
{
#ifdef DSC_NO_MALLOC
	struct LinkMap *linkmap = Heap_Alloc(&__heappool, sizeof *linkmap);
#else
	struct LinkMap *linkmap = calloc(1, sizeof *linkmap);
#endif
	if( linkmap )
		linkmap->Destructor = dtor;
	return linkmap;
}

void LinkMap_Init(struct LinkMap *const restrict linkmap, bool (*dtor)())
{
	if( !linkmap )
		return;
	
	*linkmap = (struct LinkMap){0};
	linkmap->Destructor = dtor;
}
void LinkMap_Del(struct LinkMap *const restrict linkmap)
{
	if( !linkmap )
		return;
	
	LinkNode_Free(&linkmap->Head, linkmap->Destructor);
	
	if( linkmap->Table )
	#ifdef DSC_NO_MALLOC
		Heap_Release(&__heappool, linkmap->Table);
	#else
		free(linkmap->Table);
	#endif
	LinkMap_Init(linkmap, linkmap->Destructor);
}

void LinkMap_Free(struct LinkMap **restrict linkmapref)
{
	if( !*linkmapref )
		return;
	
	LinkMap_Del(*linkmapref);
#ifdef DSC_NO_MALLOC
	Heap_Release(&__heappool, *linkmapref);
#else
	free(*linkmapref);
#endif
	*linkmapref=NULL;
}

size_t LinkMap_Count(const struct LinkMap *const restrict linkmap)
{
	return linkmap ? linkmap->Count : 0;
}
size_t LinkMap_Len(const struct LinkMap *const restrict linkmap)
{
	return linkmap ? linkmap->Len : 0;
}
bool LinkMap_Rehash(struct LinkMap *const restrict linkmap)
{
	if( !linkmap )
		return false;
	
	linkmap->Len <<= 1;
	linkmap->Count = 0;
	
	struct LinkMap newlinkmap = (struct LinkMap){0};
	
#ifdef DSC_NO_MALLOC
	newlinkmap.Table = Heap_Alloc(&__heappool, linkmap->Len * sizeof *newlinkmap.Table);
#else
	newlinkmap.Table = calloc(linkmap->Len, sizeof *newlinkmap.Table);
#endif
	if( !newlinkmap.Table ) {
		linkmap->Len >>= 1;
		return false;
	}
	
	newlinkmap.Len = linkmap->Len;
	newlinkmap.Destructor = linkmap->Destructor;
	
	for( struct LinkNode *kv=linkmap->Head, *after=NULL ; kv ; kv=after ) {
		after = kv->After;
		LinkMap_InsertNode(&newlinkmap, kv);
	}
#ifdef DSC_NO_MALLOC
	Heap_Release(&__heappool, linkmap->Table);
#else
	free(linkmap->Table);
#endif
	linkmap->Table=NULL;
	*linkmap = newlinkmap;
	return true;
}

bool LinkMap_InsertNode(struct LinkMap *const restrict linkmap, struct LinkNode *node)
{
	if( !linkmap or !node )
		return false;
	else if( !linkmap->Table ) {
		linkmap->Len = 4;
	#ifdef DSC_NO_MALLOC
		linkmap->Table = Heap_Alloc(&__heappool, linkmap->Len * sizeof *linkmap->Table);
	#else
		linkmap->Table = calloc(linkmap->Len, sizeof *linkmap->Table);
	#endif
		if( !linkmap->Table ) {
			linkmap->Len = 0;
			return false;
		}
	}
	else if( linkmap->Count >= linkmap->Len )
		LinkMap_Rehash(linkmap);
	else if( LinkMap_HasKey(linkmap, node->KeyName.CStr) )
		return false;
	
	const size_t hash = GenHash(node->KeyName.CStr) % linkmap->Len;
	node->Next = linkmap->Table[hash];
	linkmap->Table[hash] = node;
	if( !linkmap->Head or !linkmap->Tail ) {
		linkmap->Head = linkmap->Tail = node;
		node->After = node->Before = NULL;
	}
	else {
		// (Head|Tail)x <-(node)x
		node->Before = linkmap->Tail;
		// (Head|Tail)x <-(node)-> NULL
		node->After = NULL;
		// (Head|Tail)-> <-(node)-> NULL
		linkmap->Tail->After = node;
		// (Head)-> <-(Tail)-> NULL
		linkmap->Tail = node;
	}
	++linkmap->Count;
	return true;
}
bool LinkMap_Insert(struct LinkMap *const restrict linkmap, const char *restrict strkey, const union Value val)
{
	if( !linkmap or !strkey )
		return false;
	
	struct LinkNode *node = LinkNode_NewSP(strkey, val);
	bool b = LinkMap_InsertNode(linkmap, node);
	if( !b )
		LinkNode_Free(&node, linkmap->Destructor);
	return b;
}

struct LinkNode *LinkMap_GetNodeByIndex(const struct LinkMap *const restrict linkmap, const size_t index)
{
	if( !linkmap or !linkmap->Table )
		return NULL;
	else if( index==0 and linkmap->Head )
		return linkmap->Head;
	else if( index>=linkmap->Count and linkmap->Tail )
		return linkmap->Tail;
	
	bool prev_dir = ( index >= linkmap->Count/2 );
	struct LinkNode *node = prev_dir ? linkmap->Tail : linkmap->Head;
	for( size_t i=prev_dir ? linkmap->Count-1 : 0 ; i<linkmap->Count ; prev_dir ? i-- : i++ ) {
		if( node and i==index )
			return node;
		if( (prev_dir and !node->Before) or (!prev_dir and !node->After) )
			break;
		node = prev_dir ? node->Before : node->After;
	}
	return NULL;
}

union Value LinkMap_Get(const struct LinkMap *const restrict linkmap, const char *restrict strkey)
{
	if( !linkmap or !linkmap->Table or !LinkMap_HasKey(linkmap, strkey) )
		return (union Value){0};
	
	const size_t hash = GenHash(strkey) % linkmap->Len;
	for( struct LinkNode *restrict kv=linkmap->Table[hash] ; kv ; kv=kv->Next )
		if( !String_CmpCStr(&kv->KeyName, strkey) )
			return kv->Data;
	
	return (union Value){0};
}
void LinkMap_Set(struct LinkMap *const restrict linkmap, const char *restrict strkey, const union Value val)
{
	if( !linkmap )
		return;
	
	else if( !LinkMap_HasKey(linkmap, strkey) ) {
		LinkMap_Insert(linkmap, strkey, val);
		return;
	}
	
	const size_t hash = GenHash(strkey) % linkmap->Len;
	for( struct LinkNode *restrict kv=linkmap->Table[hash] ; kv ; kv=kv->Next )
		if( !String_CmpCStr(&kv->KeyName, strkey) )
			kv->Data = val;
}

union Value LinkMap_GetByIndex(const struct LinkMap *const restrict linkmap, const size_t index)
{
	if( !linkmap or !linkmap->Table )
		return (union Value){0};
	
	struct LinkNode *node = LinkMap_GetNodeByIndex(linkmap, index);
	if( node )
		return node->Data;
	return (union Value){0};
}

void LinkMap_SetByIndex(struct LinkMap *const restrict linkmap, const size_t index, const union Value val)
{
	if( !linkmap or !linkmap->Table )
		return;
	
	struct LinkNode *node = LinkMap_GetNodeByIndex(linkmap, index);
	if( node )
		node->Data = val;
}

void LinkMap_SetItemDestructor(struct LinkMap *const restrict linkmap, bool (*dtor)())
{
	if( !linkmap )
		return;
	
	linkmap->Destructor = dtor;
}

void LinkMap_Delete(struct LinkMap *const restrict linkmap, const char *restrict strkey)
{
	if( !linkmap or !linkmap->Table or !LinkMap_HasKey(linkmap, strkey) )
		return;
	
	const size_t hash = GenHash(strkey) % linkmap->Len;
	for( struct LinkNode *kv=linkmap->Table[hash], *next=NULL ; kv ; kv=next ) {
		next = kv->Next;
		
		if( !String_CmpCStr(&kv->KeyName, strkey) ) {
			linkmap->Table[hash] = kv->Next;
			kv->Next = NULL;
			kv->Before ? (kv->Before->After = kv->After) : (linkmap->Head = kv->After);
			kv->After ? (kv->After->Before = kv->Before) : (linkmap->Tail = kv->Before);
			
			if( linkmap->Destructor )
				(*linkmap->Destructor)(&kv->Data.Ptr);
			String_Del(&kv->KeyName);
		#ifdef DSC_NO_MALLOC
			Heap_Release(&__heappool, kv);
		#else
			free(kv);
		#endif
			kv=NULL;
			linkmap->Count--;
		}
	}
}

void LinkMap_DeleteByIndex(struct LinkMap *const restrict linkmap, const size_t index)
{
	if( !linkmap or !linkmap->Table )
		return;
	
	struct LinkNode *kv = LinkMap_GetNodeByIndex(linkmap, index);
	if( !kv )
		return;
	
	const size_t hash = GenHash(kv->KeyName.CStr) % linkmap->Len;
	linkmap->Table[hash] = kv->Next;
	kv->Next = NULL;
	kv->Before ? (kv->Before->After = kv->After) : (linkmap->Head = kv->After);
	kv->After ? (kv->After->Before = kv->Before) : (linkmap->Tail = kv->Before);
	
	if( linkmap->Destructor )
		(*linkmap->Destructor)(&kv->Data.Ptr);
	String_Del(&kv->KeyName);
#ifdef DSC_NO_MALLOC
	Heap_Release(&__heappool, kv);
#else
	free(kv);
#endif
	kv=NULL;
	linkmap->Count--;
}

bool LinkMap_HasKey(const struct LinkMap *const restrict linkmap, const char *restrict strkey)
{
	if( !linkmap or !linkmap->Table )
		return false;
	
	const size_t hash = GenHash(strkey) % linkmap->Len;
	for( struct LinkNode *restrict n = linkmap->Table[hash] ; n ; n=n->Next )
		if( !String_CmpCStr(&n->KeyName, strkey) )
			return true;
	
	return false;
}
struct LinkNode *LinkMap_GetNodeByKey(const struct LinkMap *const restrict linkmap, const char *restrict strkey)
{
	if( !linkmap or !linkmap->Table )
		return NULL;
	
	const size_t hash = GenHash(strkey) % linkmap->Len;
	for( struct LinkNode *restrict n = linkmap->Table[hash] ; n ; n=n->Next )
		if( !String_CmpCStr(&n->KeyName, strkey) )
			return n;
	
	return NULL;
}
struct LinkNode **LinkMap_GetKeyTable(const struct LinkMap *const restrict linkmap)
{
	return linkmap ? linkmap->Table : NULL;
}

size_t LinkMap_GetIndexByName(const struct LinkMap *const restrict linkmap, const char *restrict strkey)
{
	if( !linkmap or !strkey )
		return SIZE_MAX;
	
	size_t index = 0;
	for( struct LinkNode *restrict n = linkmap->Head ; n ; n=n->After, index++ )
		if( !String_CmpCStr(&n->KeyName, strkey) )
			return index;
	return SIZE_MAX;
}

size_t LinkMap_GetIndexByNode(const struct LinkMap *const restrict linkmap, struct LinkNode *const restrict node)
{
	if( !linkmap or !node )
		return SIZE_MAX;
	
	size_t index = 0;
	for( struct LinkNode *restrict n = linkmap->Head ; n ; n=n->After, index++ )
		if( n==node )
			return index;
	return SIZE_MAX;
}

size_t LinkMap_GetIndexByValue(const struct LinkMap *const restrict linkmap, const union Value val)
{
	if( !linkmap )
		return SIZE_MAX;
	
	size_t index = 0;
	for( struct LinkNode *restrict n = linkmap->Head ; n ; n=n->After, index++ )
		if( n->Data.UInt64 == val.UInt64 )
			return index;
	return SIZE_MAX;
}


void LinkMap_FromMap(struct LinkMap *const restrict linkmap, const struct Hashmap *const restrict map)
{
	if( !linkmap or !map )
		return;
	
	for( size_t i=0 ; i<map->Len ; i++ )
		for( struct KeyNode *n = map->Table[i] ; n ; n=n->Next )
			LinkMap_InsertNode(linkmap, LinkNode_NewSP(n->KeyName.CStr, n->Data));
}
void LinkMap_FromUniLinkedList(struct LinkMap *const restrict linkmap, const struct UniLinkedList *const restrict list)
{
	if( !linkmap or !list )
		return;
	
	size_t i=0;
	for( struct UniListNode *n=list->Head ; n ; n = n->Next ) {
		char cstrkey[10] = {0};
		sprintf(cstrkey, "%zu", i);
		LinkMap_Insert(linkmap, cstrkey, n->Data);
		i++;
	}
}
void LinkMap_FromBiLinkedList(struct LinkMap *const restrict linkmap, const struct BiLinkedList *const restrict list)
{
	if( !linkmap or !list )
		return;
	
	size_t i=0;
	for( struct BiListNode *n=list->Head ; n ; n = n->Next ) {
		char cstrkey[10] = {0};
		sprintf(cstrkey, "%zu", i);
		LinkMap_Insert(linkmap, cstrkey, n->Data);
		i++;
	}
}
void LinkMap_FromVector(struct LinkMap *const restrict linkmap, const struct Vector *const restrict v)
{
	if( !linkmap or !v )
		return;
	
	for( size_t i=0 ; i<v->Count ; i++ ) {
		char cstrkey[10] = {0};
		sprintf(cstrkey, "%zu", i);
		LinkMap_Insert(linkmap, cstrkey, v->Table[i]);
	}
}
void LinkMap_FromTuple(struct LinkMap *const restrict linkmap, const struct Tuple *const restrict tup)
{
	if( !linkmap or !tup or !tup->Items or !tup->Len )
		return;
	
	for( size_t i=0 ; i<tup->Len ; i++ ) {
		char cstrkey[10] = {0};
		sprintf(cstrkey, "%zu", i);
		LinkMap_Insert(linkmap, cstrkey, tup->Items[i]);
	}
}
void LinkMap_FromGraph(struct LinkMap *const restrict linkmap, const struct Graph *const restrict graph)
{
	if( !linkmap or !graph )
		return;
	
	for( size_t i=0 ; i<graph->VertexCount ; i++ ) {
		char cstrkey[10] = {0};
		sprintf(cstrkey, "%zu", i);
		LinkMap_Insert(linkmap, cstrkey, graph->Vertices[i].Data);
	}
}

struct LinkMap *LinkMap_NewFromMap(const struct Hashmap *const restrict map)
{
	if( !map )
		return NULL;
	
	struct LinkMap *linkmap = LinkMap_New(map->Destructor);
	LinkMap_FromMap(linkmap, map);
	return linkmap;
}
struct LinkMap *LinkMap_NewFromUniLinkedList(const struct UniLinkedList *const restrict list)
{
	if( !list )
		return NULL;
	
	struct LinkMap *linkmap = LinkMap_New(list->Destructor);
	LinkMap_FromUniLinkedList(linkmap, list);
	return linkmap;
}
struct LinkMap *LinkMap_NewFromBiLinkedList(const struct BiLinkedList *const restrict list)
{
	if( !list )
		return NULL;
	
	struct LinkMap *linkmap = LinkMap_New(list->Destructor);
	LinkMap_FromBiLinkedList(linkmap, list);
	return linkmap;
}
struct LinkMap *LinkMap_NewFromVector(const struct Vector *const restrict vec)
{
	if( !vec )
		return NULL;
	
	struct LinkMap *linkmap = LinkMap_New(vec->Destructor);
	LinkMap_FromVector(linkmap, vec);
	return linkmap;
}
struct LinkMap *LinkMap_NewFromTuple(const struct Tuple *const restrict tup)
{
	if( !tup )
		return NULL;
	
	struct LinkMap *linkmap = LinkMap_New(NULL);
	LinkMap_FromTuple(linkmap, tup);
	return linkmap;
}
struct LinkMap *LinkMap_NewFromGraph(const struct Graph *const restrict graph)
{
	if( !graph )
		return NULL;
	
	struct LinkMap *linkmap = LinkMap_New(graph->VertexDestructor);
	LinkMap_FromGraph(linkmap, graph);
	return linkmap;
}
