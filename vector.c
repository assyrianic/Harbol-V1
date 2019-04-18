#ifdef OS_WINDOWS
#	define HARBOL_LIB
#endif

#include "harbol.h"

/*
struct HarbolVector {
	union HarbolValue *Table;
	size_t	Len, Count;
};
*/

HARBOL_EXPORT struct HarbolVector *harbol_vector_new(void)
{
	struct HarbolVector *v = calloc(1, sizeof *v);
	return v;
}

HARBOL_EXPORT void harbol_vector_init(struct HarbolVector *const v)
{
	if( !v )
		return;
	
	memset(v, 0, sizeof *v);
}

HARBOL_EXPORT void harbol_vector_del(struct HarbolVector *const v, fnHarbolDestructor *const dtor)
{
	if( !v || !v->Table )
		return;
	
	if( dtor )
		for( size_t i=0; i<v->Len; i++ )
			(*dtor)(&v->Table[i].Ptr);
	
	free(v->Table); memset(v, 0, sizeof *v);
}

HARBOL_EXPORT void harbol_vector_free(struct HarbolVector **vecref, fnHarbolDestructor *const dtor)
{
	if( !*vecref )
		return;
	
	harbol_vector_del(*vecref, dtor);
	free(*vecref), *vecref=NULL;
}

HARBOL_EXPORT size_t harbol_vector_get_len(const struct HarbolVector *const v)
{
	return v ? v->Len : 0;
}

HARBOL_EXPORT size_t harbol_vector_get_count(const struct HarbolVector *const v)
{
	return v && v->Table ? v->Count : 0;
}

HARBOL_EXPORT union HarbolValue *harbol_vector_get_iter(const struct HarbolVector *const v)
{
	return v ? v->Table : NULL;
}

HARBOL_EXPORT union HarbolValue *harbol_vector_get_iter_end_len(const struct HarbolVector *const v)
{
	return v ? v->Table+v->Len : NULL;
}

HARBOL_EXPORT union HarbolValue *harbol_vector_get_iter_end_count(const struct HarbolVector *const v)
{
	return v ? v->Table+v->Count : NULL;
}

HARBOL_EXPORT void harbol_vector_resize(struct HarbolVector *const v)
{
	if( !v )
		return;
	else harbol_generic_vector_resizer(&v->Table, &v->Len, sizeof *v->Table);
}


HARBOL_EXPORT void harbol_vector_truncate(struct HarbolVector *const v)
{
	if( !v )
		return;
	else if( v->Count < (v->Len >> 1) )
		harbol_generic_vector_truncater(&v->Table, &v->Len, sizeof *v->Table);
}

HARBOL_EXPORT bool harbol_vector_insert(struct HarbolVector *const v, const union HarbolValue val)
{
	if( !v )
		return false;
	else if( !v->Table || v->Count >= v->Len )
		harbol_vector_resize(v);
	
	v->Table[v->Count++] = val;
	return true;
}

HARBOL_EXPORT union HarbolValue harbol_vector_pop(struct HarbolVector *const v)
{
	return ( !v || !v->Table || !v->Count ) ? (union HarbolValue){0} : v->Table[--v->Count];
}

HARBOL_EXPORT union HarbolValue harbol_vector_get(const struct HarbolVector *const v, const size_t index)
{
	return (!v || !v->Table || index >= v->Count) ? (union HarbolValue){0} : v->Table[index];
}

HARBOL_EXPORT void harbol_vector_set(struct HarbolVector *const v, const size_t index, const union HarbolValue val)
{
	if( !v || !v->Table || index >= v->Count )
		return;
	
	v->Table[index] = val;
}

HARBOL_EXPORT void harbol_vector_delete(struct HarbolVector *const v, const size_t index, fnHarbolDestructor *const dtor)
{
	if( !v || !v->Table || index >= v->Count )
		return;
	
	if( dtor )
		(*dtor)(&v->Table[index].Ptr);
	
	const size_t
		i=index+1,
		j=index
	;
	v->Count--;
	//while( i<v->Count )
	//	v->Table[j++] = v->Table[i++];
	memmove(v->Table+j, v->Table+i, (v->Count-j) * sizeof *v->Table);
	// can't keep auto-truncating, allocating memory every time can be expensive.
	// I'll let the programmers truncate whenever they need to.
	//harbol_vector_truncate(v);
}

HARBOL_EXPORT void harbol_vector_add(struct HarbolVector *const restrict vA, const struct HarbolVector *const restrict vB)
{
	if( !vA || !vB || !vB->Table )
		return;
	
	size_t i=0;
	while( i<vB->Count ) {
		if( !vA->Table || vA->Count >= vA->Len )
			harbol_vector_resize(vA);
		vA->Table[vA->Count++] = vB->Table[i++];
	}
}

HARBOL_EXPORT void harbol_vector_copy(struct HarbolVector *const restrict vA, const struct HarbolVector *const restrict vB)
{
	if( !vA || !vB || !vB->Table )
		return;
	
	harbol_vector_del(vA, NULL);
	size_t i=0;
	while( i<vB->Count ) {
		if( !vA->Table || vA->Count >= vA->Len )
			harbol_vector_resize(vA);
		vA->Table[vA->Count++] = vB->Table[i++];
	}
}

HARBOL_EXPORT void harbol_vector_from_unilist(struct HarbolVector *const v, const struct HarbolUniList *const list)
{
	if( !v || !list )
		return;
	else if( !v->Table || v->Count+list->Len >= v->Len )
		while( v->Count+list->Len >= v->Len )
			harbol_vector_resize(v);
	
	for( struct HarbolUniListNode *n=list->Head; n; n = n->Next )
		v->Table[v->Count++] = n->Data;
}

HARBOL_EXPORT void harbol_vector_from_bilist(struct HarbolVector *const v, const struct HarbolBiList *const list)
{
	if( !v || !list )
		return;
	else if( !v->Table || v->Count+list->Len >= v->Len )
		while( v->Count+list->Len >= v->Len )
			harbol_vector_resize(v);
	
	for( struct HarbolBiListNode *n=list->Head; n; n = n->Next )
		v->Table[v->Count++] = n->Data;
}

HARBOL_EXPORT void harbol_vector_from_hashmap(struct HarbolVector *const restrict v, const struct HarbolHashMap *const map)
{
	if( !v || !map )
		return;
	else if( !v->Table || v->Count+map->Count >= v->Len )
		while( v->Count+map->Count >= v->Len )
			harbol_vector_resize(v);
	
	for( size_t i=0; i<map->Len; i++ ) {
		struct HarbolVector *restrict vec = map->Table + i;
		for( size_t n=0; n<harbol_vector_get_count(vec); n++ ) {
			struct HarbolKeyValPair *node = vec->Table[n].Ptr;
			v->Table[v->Count++] = node->Data;
		}
	}
}

HARBOL_EXPORT void harbol_vector_from_graph(struct HarbolVector *const v, const struct HarbolGraph *const graph)
{
	if( !v || !graph )
		return;
	else if( !v->Table || v->Count+graph->Vertices.Count >= v->Len )
		while( v->Count+graph->Vertices.Count >= v->Len )
			harbol_vector_resize(v);
	
	for( size_t i=0; i<graph->Vertices.Count; i++ ) {
		struct HarbolGraphVertex *vert = graph->Vertices.Table[i].Ptr;
		v->Table[v->Count++] = vert->Data;
	}
}

HARBOL_EXPORT void harbol_vector_from_linkmap(struct HarbolVector *const v, const struct HarbolLinkMap *const map)
{
	if( !v || !map )
		return;
	else if( !v->Table || v->Count+map->Map.Count >= v->Len )
		while( v->Count+map->Map.Count >= v->Len )
			harbol_vector_resize(v);
	
	for( size_t i=0; i<map->Order.Count; i++ ) {
		struct HarbolKeyValPair *n = map->Order.Table[i].Ptr;
		v->Table[v->Count++] = n->Data;
	}
}

HARBOL_EXPORT struct HarbolVector *harbol_vector_new_from_unilist(const struct HarbolUniList *const list)
{
	if( !list )
		return NULL;
	struct HarbolVector *v = harbol_vector_new();
	harbol_vector_from_unilist(v, list);
	return v;
}

HARBOL_EXPORT struct HarbolVector *harbol_vector_new_from_bilist(const struct HarbolBiList *const list)
{
	if( !list )
		return NULL;
	struct HarbolVector *v = harbol_vector_new();
	harbol_vector_from_bilist(v, list);
	return v;
}

HARBOL_EXPORT struct HarbolVector *harbol_vector_new_from_hashmap(const struct HarbolHashMap *const map)
{
	if( !map )
		return NULL;
	struct HarbolVector *v = harbol_vector_new();
	harbol_vector_from_hashmap(v, map);
	return v;
}

HARBOL_EXPORT struct HarbolVector *harbol_vector_new_from_graph(const struct HarbolGraph *const graph)
{
	if( !graph )
		return NULL;
	struct HarbolVector *v = harbol_vector_new();
	harbol_vector_from_graph(v, graph);
	return v;
}

HARBOL_EXPORT struct HarbolVector *harbol_vector_new_from_linkmap(const struct HarbolLinkMap *const map)
{
	if( !map )
		return NULL;
	struct HarbolVector *v = harbol_vector_new();
	harbol_vector_from_linkmap(v, map);
	return v;
}


HARBOL_EXPORT void harbol_generic_vector_resizer(void *const table_ref, size_t *const restrict curr_len, const size_t element_size)
{
	void **const restrict table = table_ref;
	if( !table )
		return;
	
	// first we get our old size.
	// then resize the actual size.
	const size_t oldsize = *curr_len;
	*curr_len <<= 1;
	if( !*curr_len )
		*curr_len = 4;
	
	// allocate new table.
	void *newdata = calloc(*curr_len, element_size);
	if( !newdata ) {
		*curr_len >>= 1;
		if( *curr_len<=2 )
			*curr_len=0;
		return;
	}
	
	// copy the old table to new then free old table.
	if( *table ) {
		memcpy(newdata, *table, element_size * oldsize);
		free(*table), *table = NULL;
	}
	*table = newdata;
}

HARBOL_EXPORT void harbol_generic_vector_truncater(void *const table_ref, size_t *const restrict curr_len, const size_t element_size)
{
	void **const restrict table = table_ref;
	if( !table )
		return;
	
	*curr_len >>= 1;
	// allocate new table.
	void *newdata = calloc(*curr_len, element_size);
	if( !newdata )
		return;
	
	// copy the old table to new then free old table.
	if( *table ) {
		memcpy(newdata, *table, element_size * *curr_len);
		free(*table), *table = NULL;
	}
	*table = newdata;
}
