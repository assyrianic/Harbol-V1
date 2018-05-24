
#include <stdlib.h>
#include <stdio.h>
#include "dsc.h"

/* Graph Edge Code */
struct GraphEdge *GraphEdge_New(void)
{
	return calloc(1, sizeof(struct GraphEdge));
}

struct GraphEdge *GraphEdge_NewVP(const union Value val, struct GraphVertex *const __restrict vert)
{
	struct GraphEdge *edge = calloc(1, sizeof *edge);
	if( edge ) {
		edge->Weight = val;
		edge->VertexSocket = vert;
	}
	return edge;
}
void GraphEdge_Del(struct GraphEdge *const __restrict edge, bool (*edge_dtor)())
{
	if( !edge )
		return;
	
	if( edge_dtor )
		(*edge_dtor)(&edge->Weight.Ptr);
	if( edge->NextEdge )
		GraphEdge_Free(&edge->NextEdge, edge_dtor);
	*edge = (struct GraphEdge){0};
}

void GraphEdge_Free(struct GraphEdge **__restrict edgeref, bool (*edge_dtor)())
{
	if( !*edgeref )
		return;
	
	GraphEdge_Del(*edgeref, edge_dtor);
	free(*edgeref), *edgeref=NULL;
}

union Value GraphEdge_GetWeight(const struct GraphEdge *const __restrict edge)
{
	return edge ? edge->Weight : (union Value){0};
}

void GraphEdge_SetWeight(struct GraphEdge *const __restrict edge, const union Value val)
{
	if( !edge )
		return;
	edge->Weight = val;
}

struct GraphVertex *GraphEdge_GetVertex(const struct GraphEdge *const __restrict edge)
{
	return edge ? edge->VertexSocket : NULL;
}

void GraphEdge_SetVertex(struct GraphEdge *const __restrict edge, struct GraphVertex *const vert)
{
	if( !edge )
		return;
	
	edge->VertexSocket = vert;
}
/**************************************/


/* Graph Vertex Code */
void GraphVertex_Del(struct GraphVertex *const __restrict vert, bool (*edge_dtor)(), bool (*vert_dtor)())
{
	if( !vert )
		return;
	
	if( vert_dtor )
		(*vert_dtor)(&vert->Data.Ptr);
	
	GraphEdge_Free(&vert->EdgeHead, edge_dtor);
	*vert = (struct GraphVertex){0};
}

struct GraphEdge *GraphVertex_GetEdges(struct GraphVertex *const __restrict vert)
{
	return vert ? vert->EdgeHead : NULL;
}

union Value GraphVertex_GetData(const struct GraphVertex *const __restrict vert)
{
	return vert ? vert->Data : (union Value){0};
}

void GraphVertex_SetData(struct GraphVertex *const __restrict vert, const union Value val)
{
	if( !vert )
		return;
	
	vert->Data = val;
}
/**************************************/


/* Graph Code Implementation */
struct Graph *Graph_New(bool (*edgedtor)(), bool (*vertdtor)())
{
	struct Graph *graph = calloc(1, sizeof *graph);
	Graph_SetItemDestructors(graph, edgedtor, vertdtor);
	return graph;
}

void Graph_Init(struct Graph *const __restrict graph, bool (*edgedtor)(), bool (*vertdtor)())
{
	if( !graph )
		return;
	*graph = (struct Graph){0};
	Graph_SetItemDestructors(graph, edgedtor, vertdtor);
}

void Graph_Del(struct Graph *const __restrict graph)
{
	if( !graph )
		return;
	
	for( size_t i=0 ; i<graph->Vertices ; i++ )
		GraphVertex_Del(graph->VertVec+i, graph->EdgeDestructor, graph->VertexDestructor);
	free(graph->VertVec), graph->VertVec=NULL;
	graph->Vertices = graph->VecLen = 0;
}

void Graph_Free(struct Graph **__restrict graphref)
{
	if( !*graphref )
		return;
	
	Graph_Del(*graphref);
	free(*graphref), *graphref=NULL;
}

bool Graph_InsertVertexByValue(struct Graph *const __restrict graph, const union Value val)
{
	if( !graph )
		return false;
	else if( !graph->VertVec or graph->Vertices >= graph->VecLen )
		Graph_Resize(graph);
	
	graph->VertVec[graph->Vertices++].Data = val;
	return true;
}

bool Graph_RemoveVertexByValue(struct Graph *const __restrict graph, const union Value val)
{
	if( !graph )
		return false;
	
	struct GraphVertex *vert = NULL;
	size_t index=0;
	for( index=0 ; index<graph->Vertices ; index++ ) {
		if( !vert and graph->VertVec[index].Data.UInt64==val.UInt64 )
			vert = graph->VertVec + index;
		// value exists, cut its links with other vertices.
		for( struct GraphEdge *edge=graph->VertVec[index].EdgeHead ; edge ; edge=edge->NextEdge ) {
			if( edge->VertexSocket == vert )
				// sever the link!
				edge->VertexSocket = NULL;
		}
	}
	if( vert ) {
		GraphVertex_Del(vert, graph->EdgeDestructor, graph->VertexDestructor);
		const size_t
			i=index+1,
			j=index
		;
		memmove(graph->VertVec+j, graph->VertVec+i, graph->Vertices * sizeof *graph->VertVec);
		graph->Vertices--;
		return true;
	}
	return false;
}

bool Graph_RemoveVertexByIndex(struct Graph *const __restrict graph, const size_t index)
{
	if( !graph )
		return false;
	struct GraphVertex *vert = Graph_GetVertexByIndex(graph, index);
	if( !vert )
		return false;
	
	for( struct GraphEdge *edge=vert->EdgeHead ; edge ; edge=edge->NextEdge )
		edge->VertexSocket = NULL;
	
	for( size_t i=0 ; i<graph->Vertices ; i++ ) {
		if( graph->VertVec+i == vert )
			continue;
		
		// cut links with other vertices.
		for( struct GraphEdge *edge=graph->VertVec[i].EdgeHead ; edge ; edge=edge->NextEdge ) {
			if( edge->VertexSocket == vert )
				// sever the link!
				edge->VertexSocket = NULL;
		}
	}
	
	GraphVertex_Del(vert, graph->EdgeDestructor, graph->VertexDestructor);
	size_t
		i=index+1,
		j=index
	;
	while( i<graph->Vertices )
		graph->VertVec[j++] = graph->VertVec[i++];
	graph->Vertices--;
	return true;
}

bool Graph_InsertEdgeBtwnVerts(struct Graph *const __restrict graph, const size_t index, const size_t otherindex, const union Value weight)
{
	if( !graph )
		return false;
	
	struct GraphVertex *vert1 = Graph_GetVertexByIndex(graph, index);
	struct GraphVertex *vert2 = Graph_GetVertexByIndex(graph, otherindex);
	if( !vert1 or !vert2 )
		return false;
	
	struct GraphEdge *edge = GraphEdge_NewVP(weight, vert2);
	if( !edge )
		return false;
	
	if( vert1->Edges ) {
		edge->NextEdge = NULL;
		vert1->EdgeTail->NextEdge = edge;
		vert1->EdgeTail = edge;
	}
	else vert1->EdgeHead = vert1->EdgeTail = edge;
	vert1->Edges++;
	return true;
}

bool Graph_RemoveEdgeBtwnVerts(struct Graph *const __restrict graph, const size_t index, const size_t otherindex)
{
	if( !graph )
		return false;
	
	struct GraphVertex *vert1 = Graph_GetVertexByIndex(graph, index);
	struct GraphVertex *vert2 = Graph_GetVertexByIndex(graph, otherindex);
	if( !vert1 or !vert2 )
		return false;
	
	// make sure the vertex #1 actually has a connection to vertex #2
	struct GraphEdge *n = NULL;
	for( n=vert1->EdgeHead ; n ; n=n->NextEdge ) {
		if( n->VertexSocket == vert2 )
			break;
	}
	// we found the connection between the two! let's break it!
	if( !n )
		return false;
	
	if( n==vert1->EdgeHead )
		vert1->EdgeHead = n->NextEdge;
	else {
		struct GraphEdge *travnode = vert1->EdgeHead;
		for( size_t i=0 ; i<vert1->Edges ; i++ ) {
			if( travnode->NextEdge == n ) {
				if( vert1->EdgeTail == n ) {
					travnode->NextEdge = NULL;
					vert1->EdgeTail = travnode;
				}
				else travnode->NextEdge = n->NextEdge;
				break;
			}
			travnode = travnode->NextEdge;
		}
	}
	GraphEdge_Free(&n, graph->EdgeDestructor);
	vert1->Edges--;
	return true;
}

struct GraphVertex *Graph_GetVertexByIndex(struct Graph *const __restrict graph, const size_t index)
{
	if( !graph or !graph->VertVec or index >= graph->Vertices )
		return NULL;
	
	return graph->VertVec + index;
}

union Value Graph_GetVertexDataByIndex(struct Graph *const __restrict graph, const size_t index)
{
	if( !graph or !graph->VertVec or index>=graph->Vertices )
		return (union Value){0};
	return graph->VertVec[index].Data;
}

void Graph_SetVertexDataByIndex(struct Graph *const __restrict graph, const size_t index, const union Value val)
{
	if( !graph or !graph->VertVec or index>=graph->Vertices )
		return;
	graph->VertVec[index].Data = val;
}

struct GraphEdge *Graph_GetEdgeBtwnVertices(struct Graph *const __restrict graph, const size_t index, const size_t otherindex)
{
	if( !graph )
		return NULL;
	
	struct GraphVertex *vert1 = Graph_GetVertexByIndex(graph, index);
	struct GraphVertex *vert2 = Graph_GetVertexByIndex(graph, otherindex);
	if( !vert1 or !vert2 )
		return NULL;
	
	for( struct GraphEdge *edge=vert1->EdgeHead ; edge ; edge=edge->NextEdge )
		if( edge->VertexSocket==vert2 )
			return edge;
	
	return NULL;
}

bool Graph_IsVertexAdjacent(struct Graph *const __restrict graph, const size_t index, const size_t otherindex)
{
	if( !graph )
		return false;
	
	struct GraphVertex *vert1 = Graph_GetVertexByIndex(graph, index);
	struct GraphVertex *vert2 = Graph_GetVertexByIndex(graph, otherindex);
	if( !vert1 or !vert2 )
		return false;
	
	for( struct GraphEdge *edge=vert1->EdgeHead ; edge ; edge=edge->NextEdge )
		if( edge->VertexSocket==vert2 )
			return true;
	return false;
}

struct GraphEdge *Graph_GetVertexNeighbors(struct Graph *const __restrict graph, const size_t index)
{
	if( !graph or index >= graph->Vertices )
		return NULL;
	
	return graph->VertVec[index].EdgeHead;
}

struct GraphVertex *Graph_GetVertexArray(struct Graph *const __restrict graph)
{
	return graph ? graph->VertVec : NULL;
}

size_t Graph_GetVerticeCount(const struct Graph *const __restrict graph)
{
	return graph ? graph->Vertices : 0;
}

size_t Graph_GetEdgeCount(const struct Graph *const __restrict graph)
{
	if( !graph )
		return 0;
	
	size_t totaledges = 0;
	for( size_t i=0 ; i<graph->Vertices ; i++ )
		totaledges += graph->VertVec[i].Edges;
	return totaledges;
}

void Graph_SetItemDestructors(struct Graph *const __restrict graph, bool (*edgedtor)(), bool (*vertdtor)())
{
	if( !graph )
		return;
	
	graph->EdgeDestructor = edgedtor;
	graph->VertexDestructor = vertdtor;
}

void Graph_Resize(struct Graph *const __restrict graph)
{
	if( !graph )
		return;
	
	size_t oldsize = graph->VecLen;
	graph->VecLen <<= 1;
	if( !graph->VecLen )
		graph->VecLen = 2;
	
	struct GraphVertex *newdata = calloc(graph->VecLen, sizeof *newdata);
	if( !newdata ) {
		graph->VecLen >>= 1;
		if( graph->VecLen == 1 )
			graph->VecLen=0;
		return;
	}
	
	if( graph->VertVec ) {
		memcpy(newdata, graph->VertVec, sizeof *newdata * oldsize);
		free(graph->VertVec); graph->VertVec = NULL;
	}
	graph->VertVec = newdata;
}

void Graph_Truncate(struct Graph *const __restrict graph)
{
	if( !graph )
		return;
	
	if( graph->Vertices < graph->VecLen>>1 ) {
		graph->VecLen >>= 1;
		struct GraphVertex *newdata = calloc(graph->VecLen, sizeof *newdata);
		if( !newdata )
			return;
		
		if( graph->VertVec ) {
			memcpy(newdata, graph->VertVec, sizeof *newdata * graph->VecLen);
			free(graph->VertVec), graph->VertVec = NULL;
		}
		graph->VertVec = newdata;
	}
}

void Graph_FromVector(struct Graph *const __restrict graph, const struct Vector *const __restrict vec)
{
	if( !graph or !vec )
		return;
	for( size_t i=0 ; i<vec->Count ; i++ )
		Graph_InsertVertexByValue(graph, vec->Table[i]);
}

void Graph_FromMap(struct Graph *const __restrict graph, const struct Hashmap *const __restrict map)
{
	if( !graph or !map )
		return;
	
	for( size_t i=0 ; i<map->Len ; i++ )
		for( struct KeyNode *n = map->Table[i] ; n ; n=n->Next )
			Graph_InsertVertexByValue(graph, n->Data);
}
void Graph_FromUniLinkedList(struct Graph *const __restrict graph, const struct UniLinkedList *const __restrict list)
{
	if( !graph or !list )
		return;
	
	for( struct UniListNode *n=list->Head ; n ; n=n->Next )
		Graph_InsertVertexByValue(graph, n->Data);
}
void Graph_FromBiLinkedList(struct Graph *const __restrict graph, const struct BiLinkedList *const __restrict list)
{
	if( !graph or !list )
		return;
	
	for( struct BiListNode *n=list->Head ; n ; n=n->Next )
		Graph_InsertVertexByValue(graph, n->Data);
}
void Graph_FromTuple(struct Graph *const __restrict graph, const struct Tuple *const __restrict tup)
{
	if( !graph or !tup )
		return;
	for( size_t i=0 ; i<tup->Len ; i++ )
		Graph_InsertVertexByValue(graph, tup->Items[i]);
}

struct Graph *Graph_NewFromVector(const struct Vector *const __restrict vec)
{
	if( !vec )
		return NULL;
	struct Graph *graph = Graph_New(NULL, NULL);
	Graph_FromVector(graph, vec);
	return graph;
}

struct Graph *Graph_NewFromMap(const struct Hashmap *const __restrict map)
{
	if( !map )
		return NULL;
	struct Graph *graph = Graph_New(NULL, NULL);
	Graph_FromMap(graph, map);
	return graph;
}
struct Graph *Graph_NewFromUniLinkedList(const struct UniLinkedList *const __restrict list)
{
	if( !list )
		return NULL;
	struct Graph *graph = Graph_New(NULL, NULL);
	Graph_FromUniLinkedList(graph, list);
	return graph;
}
struct Graph *Graph_NewFromBiLinkedList(const struct BiLinkedList *const __restrict list)
{
	if( !list )
		return NULL;
	struct Graph *graph = Graph_New(NULL, NULL);
	Graph_FromBiLinkedList(graph, list);
	return graph;
}
struct Graph *Graph_NewFromTuple(const struct Tuple *const __restrict tup)
{
	if( !tup )
		return NULL;
	struct Graph *graph = Graph_New(NULL, NULL);
	Graph_FromTuple(graph, tup);
	return graph;
}
/**************************************/
