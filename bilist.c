
#include <stdlib.h>
#include <stdio.h>
#include "dsc.h"

/* Doubly Linked List Node code */
/////////////////////////////////////////
struct BiListNode *BiListNode_New(void)
{
	return calloc(1, sizeof(struct BiListNode));
}

struct BiListNode *BiListNode_NewVal(const union Value val)
{
	struct BiListNode *node = calloc(1, sizeof *node);
	if( node )
		node->Data = val;
	return node;
}

void BiListNode_Del(struct BiListNode *const __restrict node, bool (*dtor)())
{
	if( !node )
		return;
	
	// yes I know this syntax is unnecessary but it's for readability.
	if( dtor )
		(*dtor)(&node->Data.Ptr);
	
	node->Prev = NULL;
	BiListNode_Free(&node->Next, dtor);
}

void BiListNode_Free(struct BiListNode **__restrict noderef, bool (*dtor)())
{
	if( !*noderef )
		return;
	
	BiListNode_Del(*noderef, dtor);
	free(*noderef), *noderef=NULL;
}

struct BiListNode *BiListNode_GetNextNode(const struct BiListNode *const __restrict node)
{
	return node ? node->Next : NULL;
}

struct BiListNode *BiListNode_GetPrevNode(const struct BiListNode *const __restrict node)
{
	return node ? node->Prev : NULL;
}

union Value BiListNode_GetValue(const struct BiListNode *const __restrict node)
{
	return node ? node->Data : (union Value){0};
}
/////////////////////////////////////////



/* Doubly Linked List code */
/////////////////////////////////////////
struct BiLinkedList *BiLinkedList_New(bool (*dtor)())
{
	struct BiLinkedList *list = calloc(1, sizeof *list);
	BiLinkedList_SetDestructor(list, dtor);
	return list;
}

void BiLinkedList_Del(struct BiLinkedList *const __restrict list)
{
	if( !list )
		return;
	
	BiListNode_Free(&list->Head, list->Destructor);
	*list = (struct BiLinkedList){0};
}

void BiLinkedList_Free(struct BiLinkedList **__restrict listref)
{
	if( !*listref )
		return;
	
	BiLinkedList_Del(*listref);
	free(*listref), *listref=NULL;
}
void BiLinkedList_Init(struct BiLinkedList *const __restrict list, bool (*dtor)())
{
	if( !list )
		return;
	
	*list = (struct BiLinkedList){0};
	BiLinkedList_SetDestructor(list, dtor);
}

size_t BiLinkedList_Len(const struct BiLinkedList *const __restrict list)
{
	return list ? list->Len : 0;
}

bool BiLinkedList_InsertNodeAtHead(struct BiLinkedList *const __restrict list, struct BiListNode *const __restrict node)
{
	if( !list or !node )
		return false;
	
	if( !list->Head ) {
		list->Head = list->Tail = node;
		node->Next = node->Prev = NULL;
	}
	else {
		node->Next = list->Head;
		node->Prev = NULL;
		list->Head->Prev = node;
		list->Head = node;
	}
	list->Len++;
	return true;
}

bool BiLinkedList_InsertNodeAtTail(struct BiLinkedList *const __restrict list, struct BiListNode *const __restrict node)
{
	if( !list or !node )
		return false;
	
	if( list->Len ) {
		node->Prev = list->Tail;
		node->Next = NULL;
		list->Tail->Next = node;
		list->Tail = node;
	}
	else {
		list->Head = list->Tail = node;
		node->Next = node->Prev = NULL;
	}
	list->Len++;
	return true;
}

bool BiLinkedList_InsertNodeAtIndex(struct BiLinkedList *const __restrict list, struct BiListNode *const __restrict node, const size_t index)
{
	if( !list or !node )
		return false;
	else if( !list->Head or index==0 )
		return BiLinkedList_InsertNodeAtHead(list, node);
	// if index is out of bounds, append at tail end.
	else if( index >= list->Len )
		return BiLinkedList_InsertNodeAtTail(list, node);
	
	bool prev_dir = ( index >= list->Len/2 );
	struct BiListNode *curr = prev_dir ? list->Tail : list->Head;
	size_t i=prev_dir ? list->Len-1 : 0;
	while( (prev_dir ? curr->Prev != NULL : curr->Next != NULL) and i != index ) {
		curr = prev_dir ? curr->Prev : curr->Next;
		prev_dir ? i-- : i++;
	}
	if( i ) {
		// P-> <-(curr)
		// P-> x(node)x
		curr->Prev->Next = node;
		// P-> <-(node)x
		node->Prev = curr->Prev;
		// P-> <-(node)-> x(curr)
		node->Next = curr;
		// P-> <-(node)-> <-(curr)
		curr->Prev = node;
		list->Len++;
		return true;
	}
	return false;
}

bool BiLinkedList_InsertValueAtHead(struct BiLinkedList *const __restrict list, const union Value val)
{
	if( !list )
		return false;
	
	return BiLinkedList_InsertNodeAtHead(list, BiListNode_NewVal(val));
}

bool BiLinkedList_InsertValueAtTail(struct BiLinkedList *const __restrict list, const union Value val)
{
	if( !list )
		return false;
	
	return BiLinkedList_InsertNodeAtTail(list, BiListNode_NewVal(val));
}

bool BiLinkedList_InsertValueAtIndex(struct BiLinkedList *const __restrict list, const union Value val, const size_t index)
{
	if( !list )
		return false;
	
	return BiLinkedList_InsertNodeAtIndex(list, BiListNode_NewVal(val), index);
}


struct BiListNode *BiLinkedList_GetNode(const struct BiLinkedList *const __restrict list, const size_t index)
{
	if( !list )
		return NULL;
	else if( index >= list->Len )
		return list->Tail;
	
	bool prev_dir = ( index >= list->Len/2 );
	struct BiListNode *node = prev_dir ? list->Tail : list->Head;
	for( size_t i=prev_dir ? list->Len-1 : 0 ; i<list->Len ; prev_dir ? i-- : i++ ) {
		if( node and i==index )
			return node;
		node = prev_dir ? node->Prev : node->Next;
	}
	return NULL;
}

struct BiListNode *BiLinkedList_GetNodeByValue(const struct BiLinkedList *const __restrict list, const union Value val)
{
	if( !list )
		return NULL;
	for( struct BiListNode *i=list->Head ; i ; i=i->Next )
		if( !memcmp(&i->Data, &val, sizeof val) )
			return i;
	return NULL;
}

union Value BiLinkedList_GetValue(const struct BiLinkedList *const __restrict list, const size_t index)
{
	if( !list )
		return (union Value){0};
	else if( index >= list->Len )
		return list->Tail->Data;
	
	bool prev_dir = ( index >= list->Len/2 );
	struct BiListNode *node = prev_dir ? list->Tail : list->Head;
	for( size_t i=prev_dir ? list->Len-1 : 0 ; i<list->Len ; prev_dir ? i-- : i++ ) {
		if( node and i==index )
			return node->Data;
		node = prev_dir ? node->Prev : node->Next;
	}
	return (union Value){0};
}

void BiLinkedList_SetValue(struct BiLinkedList *const __restrict list, const size_t index, const union Value val)
{
	if( !list )
		return;
	else if( index >= list->Len ) {
		list->Tail->Data = val;
		return;
	}
	
	bool prev_dir = ( index >= list->Len/2 );
	struct BiListNode *node = prev_dir ? list->Tail : list->Head;
	for( size_t i=prev_dir ? list->Len-1 : 0 ; i<list->Len ; prev_dir ? i-- : i++ ) {
		if( node and i==index ) {
			node->Data = val;
			break;
		}
		node = prev_dir ? node->Prev : node->Next;
	}
}
bool BiLinkedList_DelNodeByIndex(struct BiLinkedList *const __restrict list, const size_t index)
{
	if( !list or !list->Len )
		return false;
	
	struct BiListNode *node = BiLinkedList_GetNode(list, index);
	node->Prev ? (node->Prev->Next = node->Next) : (list->Head = node->Next);
	node->Next ? (node->Next->Prev = node->Prev) : (list->Tail = node->Prev);
	
	if( list->Destructor )
		(*list->Destructor)(&node->Data.Ptr);
	
	free(node), node=NULL;
	list->Len--;
	return true;
}

bool BiLinkedList_DelNodeByRef(struct BiLinkedList *const __restrict list, struct BiListNode **__restrict noderef)
{
	if( !list or !*noderef )
		return false;
	
	struct BiListNode *node = *noderef;
	node->Prev ? (node->Prev->Next = node->Next) : (list->Head = node->Next);
	node->Next ? (node->Next->Prev = node->Prev) : (list->Tail = node->Prev);
	
	if( list->Destructor )
		(*list->Destructor)(&node->Data.Ptr);
	
	free(*noderef), *noderef=NULL;
	node = NULL;
	list->Len--;
	return true;
}

void BiLinkedList_SetDestructor(struct BiLinkedList *const __restrict list, bool (*dtor)())
{
	if( !list )
		return;
	
	list->Destructor = dtor;
}

struct BiListNode *BiLinkedList_GetHead(const struct BiLinkedList *const __restrict list)
{
	return list ? list->Head : NULL;
}
struct BiListNode *BiLinkedList_GetTail(const struct BiLinkedList *const __restrict list)
{
	return list ? list->Tail : NULL;
}

void BiLinkedList_FromUniLinkedList(struct BiLinkedList *const __restrict bilist, const struct UniLinkedList *const __restrict unilist)
{
	if( !bilist or !unilist )
		return;
	
	for( struct UniListNode *n=unilist->Head ; n ; n = n->Next )
		BiLinkedList_InsertValueAtTail(bilist, n->Data);
}

void BiLinkedList_FromMap(struct BiLinkedList *const __restrict bilist, const struct Hashmap *const __restrict map)
{
	if( !bilist or !map )
		return;
	
	for( size_t i=0 ; i<map->Len ; i++ )
		for( struct KeyNode *n = map->Table[i] ; n ; n=n->Next )
			BiLinkedList_InsertValueAtTail(bilist, n->Data);
}

void BiLinkedList_FromVector(struct BiLinkedList *const __restrict bilist, const struct Vector *const __restrict v)
{
	if( !bilist or !v )
		return;
	
	for( size_t i=0 ; i<v->Count ; i++ )
		BiLinkedList_InsertValueAtTail(bilist, v->Table[i]);
}

void BiLinkedList_FromTuple(struct BiLinkedList *const __restrict bilist, const struct Tuple *const __restrict tup)
{
	if( !bilist or !tup or !tup->Items or !tup->Len )
		return;
	
	for( size_t i=0 ; i<tup->Len ; i++ )
		BiLinkedList_InsertValueAtTail(bilist, tup->Items[i]);
}

void BiLinkedList_FromGraph(struct BiLinkedList *const __restrict bilist, const struct Graph *const __restrict graph)
{
	if( !bilist or !graph )
		return;
	for( size_t i=0 ; i<graph->Vertices ; i++ )
		BiLinkedList_InsertValueAtTail(bilist, graph->VertVec[i].Data);
}


struct BiLinkedList *BiLinkedList_NewFromUniLinkedList(const struct UniLinkedList *const __restrict unilist)
{
	if( !unilist )
		return NULL;
	struct BiLinkedList *bilist = BiLinkedList_New(unilist->Destructor);
	BiLinkedList_FromUniLinkedList(bilist, unilist);
	return bilist;
}

struct BiLinkedList *BiLinkedList_NewFromMap(const struct Hashmap *const __restrict map)
{
	if( !map )
		return NULL;
	struct BiLinkedList *bilist = BiLinkedList_New(map->Destructor);
	BiLinkedList_FromMap(bilist, map);
	return bilist;
}

struct BiLinkedList *BiLinkedList_NewFromVector(const struct Vector *const __restrict v)
{
	if( !v )
		return NULL;
	struct BiLinkedList *bilist = BiLinkedList_New(v->Destructor);
	BiLinkedList_FromVector(bilist, v);
	return bilist;
}

struct BiLinkedList *BiLinkedList_NewFromTuple(const struct Tuple *const __restrict tup)
{
	if( !tup or !tup->Items or !tup->Len )
		return NULL;
	struct BiLinkedList *bilist = BiLinkedList_New(NULL);
	BiLinkedList_FromTuple(bilist, tup);
	return bilist;
}

struct BiLinkedList *BiLinkedList_NewFromGraph(const struct Graph *const __restrict graph)
{
	if( !graph )
		return NULL;
	struct BiLinkedList *bilist = BiLinkedList_New(NULL);
	BiLinkedList_FromGraph(bilist, graph);
	return bilist;
}
