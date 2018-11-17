
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "harbol.h"

void TestHarbolStringObject(void);
void TestHarbolVector(void);
void TestHarbolHashmap(void);
void TestUniList(void);
void TestBiList(void);
void TestHarbolByteBuffer(void);
void TestHarbolTuple(void);
void TestHarbolMemoryPool(void);
void TestHarbolGraph(void);
void TestTree(void);
void TestHarbolLinkMap(void);
void TestHarbolConversions(void);
void TestCfg(void);
//void TestPlugins(void);

FILE *HARBOL_debug_output = NULL;


int main()
{
	HARBOL_debug_output = fopen("data_structure_debug_output.txt", "wa+");
	if( !HARBOL_debug_output )
		return -1;
	/*
	TestHarbolStringObject();
	TestHarbolVector();
	TestHarbolHashmap();
	TestUniList();
	TestBiList();
	TestHarbolByteBuffer();
	TestHarbolTuple();
	TestHarbolMemoryPool();
	TestHarbolGraph();
	TestTree();
	TestHarbolLinkMap();
	TestHarbolConversions();
	*/
	TestCfg();
	fclose(HARBOL_debug_output), HARBOL_debug_output=NULL;
}

void TestHarbolStringObject(void)
{
	if( !HARBOL_debug_output )
		return;
	
	// Test allocation and initializations
	fputs("string :: test allocation/initialization.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	struct HarbolString *p = HarbolString_NewStr("test ptr with cstr!");
	assert( p );
	fputs(HarbolString_GetStr(p), HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	
	struct HarbolString i = (struct HarbolString){0};
	HarbolString_CopyStr(&i, "test stk with cstr!");
	fputs(HarbolString_GetStr(&i), HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	
	// test appending individual chars.
	fputs("string :: test appending individual chars.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolString_AddChar(p, '6');
	fputs(HarbolString_GetStr(p), HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	
	HarbolString_AddChar(&i, '6');
	fputs(HarbolString_GetStr(&i), HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	
	// test appending strings.
	fputs("string :: test appending C strings.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolString_AddStr(p, "\'new string!\'");
	fputs(HarbolString_GetStr(p), HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	
	HarbolString_AddStr(&i, "\'new string!\'");
	fputs(HarbolString_GetStr(&i), HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	
	// test appending string objects.
	fputs("\nstring :: test appending string objects.\n", HARBOL_debug_output);
	HarbolString_CopyStr(p, "A");
	HarbolString_CopyStr(&i, "B");
	HarbolString_Add(p, &i);
	HarbolString_Add(&i, p);
	fputs(HarbolString_GetStr(p), HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	fputs(HarbolString_GetStr(&i), HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	
	// test copying string objects.
	fputs("\nstring :: test copying string objects.\n", HARBOL_debug_output);
	HarbolString_CopyStr(p, "copied from ptr!");
	HarbolString_Copy(&i, p);
	fputs(HarbolString_GetStr(p), HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	fputs(HarbolString_GetStr(&i), HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	
	// test string formatting.
	fputs("\nstring :: test string formatting.\n", HARBOL_debug_output);
	HarbolString_Del(&i);
	HarbolString_Reserve(&i, 100);
	HarbolString_Format(&i, "%i + %f%% + %i", 900, 4242.2, 10);
	fputs(HarbolString_GetStr(&i), HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	
	// free data
	fputs("string :: test destruction.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolString_Del(&i);
	fprintf(HARBOL_debug_output, "i's string is null? '%s'\n", i.CStr ? "no" : "yes");
	
	HarbolString_Del(p);
	fprintf(HARBOL_debug_output, "p's string is null? '%s'\n", p->CStr ? "no" : "yes");
	HarbolString_Free(&p);
	fprintf(HARBOL_debug_output, "p is null? '%s'\n", p ? "no" : "yes");
}

void TestHarbolVector(void)
{
	// Test allocation and initializations
	fputs("vector :: test allocation/initialization.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	struct HarbolVector *p = HarbolVector_New();
	assert( p );
	
	struct HarbolVector i = (struct HarbolVector){0};
	
	// test data inserting
	fputs("vector :: test insertion.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolVector_Insert(p, (union HarbolValue){.Int64=100});
	HarbolVector_Insert(&i, (union HarbolValue){.Int64=100});
	
	// test data retrieval
	fputs("vector :: test retrieval.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	fprintf(HARBOL_debug_output, "ptr[0] == %" PRIi64 "\n", HarbolVector_Get(p, 0).Int64);
	fprintf(HARBOL_debug_output, "stk[0] == %" PRIi64 "\n", HarbolVector_Get(&i, 0).Int64);
	
	// test data setting
	fputs("vector :: test setting data.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolVector_Set(p, 0, (union HarbolValue){.Int64=10});
	HarbolVector_Set(&i, 0, (union HarbolValue){.Int64=9});
	fprintf(HARBOL_debug_output, "ptr[0] == %" PRIi64 "\n", HarbolVector_Get(p, 0).Int64);
	fprintf(HARBOL_debug_output, "stk[0] == %" PRIi64 "\n", HarbolVector_Get(&i, 0).Int64);
	
	// append the vectors
	fputs("vector :: test vector appending.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolVector_Add(p, &i);
	fprintf(HARBOL_debug_output, "ptr[1] == %" PRIi64 "\n", HarbolVector_Get(p, 1).Int64);
	fprintf(HARBOL_debug_output, "stk[1] == %" PRIi64 "\n", HarbolVector_Get(&i, 1).Int64);
	
	// test vector copying.
	fputs("vector :: test vector copying.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolVector_Del(&i, NULL);
	HarbolVector_Insert(&i, (union HarbolValue){.Int64=100});
	HarbolVector_Insert(&i, (union HarbolValue){.Int64=101});
	HarbolVector_Insert(&i, (union HarbolValue){.Int64=102});
	HarbolVector_Copy(p, &i);
	fprintf(HARBOL_debug_output, "ptr[1] == %" PRIi64 "\n", HarbolVector_Get(p, 1).Int64);
	fprintf(HARBOL_debug_output, "ptr[2] == %" PRIi64 "\n", HarbolVector_Get(p, 2).Int64);
	fprintf(HARBOL_debug_output, "stk[1] == %" PRIi64 "\n", HarbolVector_Get(&i, 1).Int64);
	fprintf(HARBOL_debug_output, "stk[2] == %" PRIi64 "\n", HarbolVector_Get(&i, 2).Int64);
	
	// test vector deleting and truncating.
	fputs("vector :: test item deletion and vector truncation.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolVector_Del(&i, NULL);
	HarbolVector_Del(p, NULL);
	HarbolVector_Insert(p, (union HarbolValue){.Int64=100});
	HarbolVector_Insert(p, (union HarbolValue){.Int64=101});
	HarbolVector_Insert(p, (union HarbolValue){.Int64=102});
	HarbolVector_Insert(p, (union HarbolValue){.Int64=103});
	HarbolVector_Insert(p, (union HarbolValue){.Int64=104});
	HarbolVector_Insert(p, (union HarbolValue){.Int64=105});
	for( size_t i=0 ; i<p->Count ; i++ )
		fprintf(HARBOL_debug_output, "ptr[%zu] == %" PRIi64 "\n", i, HarbolVector_Get(p, i).Int64);
	fputs("\n", HARBOL_debug_output);
	HarbolVector_Delete(p, 0, NULL); // deletes 100
	HarbolVector_Delete(p, 1, NULL); // deletes 102 since 101 because new 0 index
	HarbolVector_Delete(p, 2, NULL); // deletes 104
	for( size_t i=0 ; i<p->Count ; i++ )
		fprintf(HARBOL_debug_output, "ptr[%zu] == %" PRIi64 "\n", i, HarbolVector_Get(p, i).Int64);
	fprintf(HARBOL_debug_output, "\nptr[] len == %zu\n", HarbolVector_Len(p));
	HarbolVector_Truncate(p);
	fprintf(HARBOL_debug_output, "ptr[] len == %zu\n\n", HarbolVector_Len(p));
	for( size_t i=0 ; i<p->Count ; i++ )
		fprintf(HARBOL_debug_output, "ptr[%zu] == %" PRIi64 "\n", i, HarbolVector_Get(p, i).Int64);
	
	fputs("vector :: test vector popping.\n", HARBOL_debug_output);
	for( size_t i=0 ; i<p->Count ; i++ )
		fprintf(HARBOL_debug_output, "prepop ptr[%zu] == %" PRIi64 "\n", i, HarbolVector_Get(p, i).Int64);
	const union HarbolValue vec_item_2 = HarbolVector_Pop(p);
	for( size_t i=0 ; i<p->Count ; i++ )
		fprintf(HARBOL_debug_output, "postpop ptr[%zu] == %" PRIi64 "\n", i, HarbolVector_Get(p, i).Int64);
	
	fprintf(HARBOL_debug_output, "popped val == %" PRIi64 "\n", vec_item_2.Int64);
	// free data
	fputs("vector :: test destruction.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolVector_Del(&i, NULL);
	fprintf(HARBOL_debug_output, "i's table is null? '%s'\n", i.Table ? "no" : "yes");
	
	HarbolVector_Del(p, NULL);
	fprintf(HARBOL_debug_output, "p's table is null? '%s'\n", p->Table ? "no" : "yes");
	HarbolVector_Free(&p, NULL);
	fprintf(HARBOL_debug_output, "p is null? '%s'\n", p ? "no" : "yes");
}

void TestHarbolHashmap(void)
{
	// Test allocation and initializations
	fputs("hashmap :: test allocation / initialization.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	struct HarbolHashmap *p = HarbolMap_New();
	assert( p );
	
	struct HarbolHashmap i = (struct HarbolHashmap){0};
	
	// test insertion
	fputs("hashmap :: test insertion.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolMap_Insert(p, "1", (union HarbolValue){.Int64=1});
	HarbolMap_Insert(p, "2", (union HarbolValue){.Int64=2});
	HarbolMap_Insert(&i, "1", (union HarbolValue){.Int64=1});
	HarbolMap_Insert(&i, "2", (union HarbolValue){.Int64=2});
	
	// test retrieval.
	fputs("hashmap :: test data retrieval.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	fprintf(HARBOL_debug_output, "ptr[\"1\"] == %" PRIi64 "\n", HarbolMap_Get(p, "1").Int64);
	fprintf(HARBOL_debug_output, "ptr[\"2\"] == %" PRIi64 "\n", HarbolMap_Get(p, "2").Int64);
	fprintf(HARBOL_debug_output, "stk[\"1\"] == %" PRIi64 "\n", HarbolMap_Get(&i, "1").Int64);
	fprintf(HARBOL_debug_output, "stk[\"2\"] == %" PRIi64 "\n", HarbolMap_Get(&i, "2").Int64);
	
	// test setting.
	fputs("hashmap :: test data setting.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolMap_Set(p, "2", (union HarbolValue){.Int64=20});
	HarbolMap_Set(&i, "2", (union HarbolValue){.Int64=200});
	fprintf(HARBOL_debug_output, "ptr[\"1\"] == %" PRIi64 "\n", HarbolMap_Get(p, "1").Int64);
	fprintf(HARBOL_debug_output, "ptr[\"2\"] == %" PRIi64 "\n", HarbolMap_Get(p, "2").Int64);
	fprintf(HARBOL_debug_output, "stk[\"1\"] == %" PRIi64 "\n", HarbolMap_Get(&i, "1").Int64);
	fprintf(HARBOL_debug_output, "stk[\"2\"] == %" PRIi64 "\n", HarbolMap_Get(&i, "2").Int64);
	
	// test deletion
	fputs("hashmap :: test item deletion.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolMap_Delete(p, "2", NULL);
	fprintf(HARBOL_debug_output, "ptr[\"2\"] == %" PRIi64 "\n", HarbolMap_Get(p, "2").Int64);
	
	// free data
	fputs("hashmap :: test destruction.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolMap_Del(&i, NULL);
	fprintf(HARBOL_debug_output, "i's buckets are null? '%s'\n", i.Table ? "no" : "yes");
	
	HarbolMap_Del(p, NULL);
	fprintf(HARBOL_debug_output, "p's buckets are null? '%s'\n", p->Table ? "no" : "yes");
	HarbolMap_Free(&p, NULL);
	fprintf(HARBOL_debug_output, "p is null? '%s'\n", p ? "no" : "yes");
}

void TestUniList(void)
{
	// Test allocation and initializations
	fputs("unilist :: test allocation / initialization.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	struct HarbolUniList *p = HarbolUniList_New();
	assert( p );
	
	struct HarbolUniList i = (struct HarbolUniList){0};
	
	// test insertion.
	// test insertion at tail.
	fputs("unilist :: test tail insertion.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolUniList_InsertValueAtTail(p, (union HarbolValue){.Int64=1});
	for( struct HarbolUniListNode *n=HarbolUniList_GetHead(p) ; n ; n = HarbolUniListNode_GetNextNode(n) )
		fprintf(HARBOL_debug_output, "unilist value : %" PRIi64 "\n", HarbolUniListNode_GetValue(n).Int64);
	
	HarbolUniList_InsertValueAtTail(p, (union HarbolValue){.Int64=2});
	HarbolUniList_InsertValueAtTail(p, (union HarbolValue){.Int64=3});
	for( struct HarbolUniListNode *n=HarbolUniList_GetHead(p) ; n ; n = HarbolUniListNode_GetNextNode(n) )
		fprintf(HARBOL_debug_output, "unilist value : %" PRIi64 "\n", HarbolUniListNode_GetValue(n).Int64);
	
	// test insertion at head.
	fputs("unilist :: test head insertion.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolUniList_InsertValueAtHead(p, (union HarbolValue){.Int64=0});
	HarbolUniList_InsertValueAtHead(p, (union HarbolValue){.Int64=-1});
	HarbolUniList_InsertValueAtHead(p, (union HarbolValue){.Int64=-2});
	for( struct HarbolUniListNode *n=HarbolUniList_GetHead(p) ; n ; n = HarbolUniListNode_GetNextNode(n) )
		fprintf(HARBOL_debug_output, "unilist value : %" PRIi64 "\n", HarbolUniListNode_GetValue(n).Int64);
	
	// test retrieval.
	fputs("unilist :: test node retrival by index.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	{
		struct HarbolUniListNode *n = HarbolUniList_GetNode(p, 1);
		fprintf(HARBOL_debug_output, "unilist value : %" PRIi64 "\n", HarbolUniListNode_GetValue(n).Int64);
		
		// test item destruction.
		fputs("unilist :: test node removal & destruction by reference.", HARBOL_debug_output);
		fputs("\n", HARBOL_debug_output);
		HarbolUniList_DelNodeByRef(p, &n, NULL);
		for( struct HarbolUniListNode *k=HarbolUniList_GetHead(p) ; k ; k = HarbolUniListNode_GetNextNode(k) )
			fprintf(HARBOL_debug_output, "unilist value : %" PRIi64 "\n", HarbolUniListNode_GetValue(k).Int64);
		
		fputs("unilist :: test node removal & destruction by index.", HARBOL_debug_output);
		fputs("\n", HARBOL_debug_output);
		HarbolUniList_DelNodeByIndex(p, 1, NULL);
		for( struct HarbolUniListNode *k=HarbolUniList_GetHead(p) ; k ; k = HarbolUniListNode_GetNextNode(k) )
			fprintf(HARBOL_debug_output, "unilist value : %" PRIi64 "\n", HarbolUniListNode_GetValue(k).Int64);
	}
	
	// test setting data by index
	fputs("unilist :: test setting data by index (index of 0 is head node).", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolUniList_SetValue(p, 0, (union HarbolValue){.Int64=222}); // 0 is head
	for( struct HarbolUniListNode *n=HarbolUniList_GetHead(p) ; n ; n = HarbolUniListNode_GetNextNode(n) )
		fprintf(HARBOL_debug_output, "unilist value : %" PRIi64 "\n", HarbolUniListNode_GetValue(n).Int64);
	
	// test deleting head node after freeing.
	fputs("unilist :: test deleting head node after freeing and new insertion.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolUniList_Del(p, NULL);
	HarbolUniList_InsertValueAtHead(p, (union HarbolValue){.Int64=100});
	for( struct HarbolUniListNode *n=HarbolUniList_GetHead(p) ; n ; n = HarbolUniListNode_GetNextNode(n) )
		fprintf(HARBOL_debug_output, "unilist value : %" PRIi64 "\n", HarbolUniListNode_GetValue(n).Int64);
	HarbolUniList_DelNodeByIndex(p, 0, NULL);
	fprintf(HARBOL_debug_output, "unilist head node ptr : %p, tail node ptr : %p\n", HarbolUniList_GetHead(p), HarbolUniList_GetTail(p));
	
	
	// test deleting items by index on a list size of 2.
	fputs("unilist :: test deleting items by index on a list size of 2.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolUniList_Del(p, NULL);
	HarbolUniList_InsertValueAtTail(p, (union HarbolValue){.Int64=100});
	HarbolUniList_InsertValueAtTail(p, (union HarbolValue){.Int64=101});
	for( struct HarbolUniListNode *n=HarbolUniList_GetHead(p) ; n ; n = HarbolUniListNode_GetNextNode(n) )
		fprintf(HARBOL_debug_output, "unilist value : %" PRIi64 "\n", HarbolUniListNode_GetValue(n).Int64);
	HarbolUniList_DelNodeByIndex(p, 0, NULL);
	for( struct HarbolUniListNode *n=HarbolUniList_GetHead(p) ; n ; n = HarbolUniListNode_GetNextNode(n) )
		fprintf(HARBOL_debug_output, "unilist value : %" PRIi64 "\n", HarbolUniListNode_GetValue(n).Int64);
	
	// test insertion in middle of list
	fputs("unilist :: test middle insertion.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolUniList_Del(p, NULL);
	HarbolUniList_InsertValueAtIndex(p, (union HarbolValue){.Int64=100}, 0); // have it insert at head by inserting at index 0.
	HarbolUniList_InsertValueAtIndex(p, (union HarbolValue){.Int64=101}, 10); // have it insert at tail by having the index exceed the size of list
	HarbolUniList_InsertValueAtIndex(p, (union HarbolValue){.Int64=102}, 1);
	HarbolUniList_InsertValueAtIndex(p, (union HarbolValue){.Int64=102}, 1);
	HarbolUniList_InsertValueAtIndex(p, (union HarbolValue){.Int64=102}, 1);
	HarbolUniList_InsertValueAtIndex(p, (union HarbolValue){.Int64=102}, 1);
	HarbolUniList_InsertValueAtIndex(p, (union HarbolValue){.Int64=103}, 1);
	for( struct HarbolUniListNode *n=HarbolUniList_GetHead(p) ; n ; n = HarbolUniListNode_GetNextNode(n) )
		fprintf(HARBOL_debug_output, "unilist value : %" PRIi64 "\n", HarbolUniListNode_GetValue(n).Int64);
	
	// test deleting the tail through an index.
	fputs("unilist :: test deleting tail by index.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolUniList_Del(p, NULL);
	HarbolUniList_InsertValueAtTail(p, (union HarbolValue){.Int64=100});
	HarbolUniList_InsertValueAtTail(p, (union HarbolValue){.Int64=101});
	HarbolUniList_InsertValueAtTail(p, (union HarbolValue){.Int64=102});
	for( struct HarbolUniListNode *n=HarbolUniList_GetHead(p) ; n ; n = HarbolUniListNode_GetNextNode(n) )
		fprintf(HARBOL_debug_output, "unilist value : %" PRIi64 "\n", HarbolUniListNode_GetValue(n).Int64);
	HarbolUniList_DelNodeByIndex(p, 2, NULL);
	for( struct HarbolUniListNode *n=HarbolUniList_GetHead(p) ; n ; n = HarbolUniListNode_GetNextNode(n) )
		fprintf(HARBOL_debug_output, "unilist value : %" PRIi64 "\n", HarbolUniListNode_GetValue(n).Int64);
	
	// test finding a node by value.
	
	// free data
	fputs("unilist :: test destruction.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolUniList_Del(&i, NULL);
	HarbolUniList_Del(&i, NULL);
	fprintf(HARBOL_debug_output, "i's Head is null? '%s'\n", i.Head ? "no" : "yes");
	
	HarbolUniList_Del(p, NULL);
	fprintf(HARBOL_debug_output, "p's Head is null? '%s', but is p null? '%s'\n", p->Head ? "no" : "yes", p ? "no" : "yes");
	HarbolUniList_Free(&p, NULL);
	fprintf(HARBOL_debug_output, "p is null? '%s'\n", p ? "no" : "yes");
}

void TestBiList(void)
{
	// Test allocation and initializations
	fputs("bilist :: test allocation / initialization.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	struct HarbolBiList *p = HarbolBiList_New();
	assert( p );
	
	struct HarbolBiList i = (struct HarbolBiList){0};
	
	// test insertion.
	// test insertion at tail.
	fputs("bilist :: test tail insertion.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolBiList_InsertValueAtTail(p, (union HarbolValue){.Int64=1});
	for( struct HarbolBiListNode *n=HarbolBiList_GetHead(p) ; n ; n = HarbolBiListNode_GetNextNode(n) )
		fprintf(HARBOL_debug_output, "bilist value : %" PRIi64 "\n", HarbolBiListNode_GetValue(n).Int64);
	
	HarbolBiList_InsertValueAtTail(p, (union HarbolValue){.Int64=2});
	HarbolBiList_InsertValueAtTail(p, (union HarbolValue){.Int64=3});
	for( struct HarbolBiListNode *n=HarbolBiList_GetHead(p) ; n ; n = HarbolBiListNode_GetNextNode(n) )
		fprintf(HARBOL_debug_output, "bilist value : %" PRIi64 "\n", HarbolBiListNode_GetValue(n).Int64);
	
	// test insertion at head.
	fputs("bilist :: test head insertion.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolBiList_InsertValueAtHead(p, (union HarbolValue){.Int64=0});
	HarbolBiList_InsertValueAtHead(p, (union HarbolValue){.Int64=-1});
	HarbolBiList_InsertValueAtHead(p, (union HarbolValue){.Int64=-2});
	for( struct HarbolBiListNode *n=HarbolBiList_GetHead(p) ; n ; n = HarbolBiListNode_GetNextNode(n) )
		fprintf(HARBOL_debug_output, "bilist value : %" PRIi64 "\n", HarbolBiListNode_GetValue(n).Int64);
	
	// test retrieval.
	fputs("bilist :: test node retrival by index.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	{
		struct HarbolBiListNode *n = HarbolBiList_GetNode(p, 1);
		fprintf(HARBOL_debug_output, "bilist value : %" PRIi64 "\n", HarbolBiListNode_GetValue(n).Int64);
		
		// test item destruction.
		fputs("bilist :: test node removal & destruction by reference.", HARBOL_debug_output);
		fputs("\n", HARBOL_debug_output);
		HarbolBiList_DelNodeByRef(p, &n, NULL);
		for( struct HarbolBiListNode *n=HarbolBiList_GetHead(p) ; n ; n = HarbolBiListNode_GetNextNode(n) )
			fprintf(HARBOL_debug_output, "bilist value : %" PRIi64 "\n", HarbolBiListNode_GetValue(n).Int64);
		
		fputs("bilist :: test node removal & destruction by index.", HARBOL_debug_output);
		fputs("\n", HARBOL_debug_output);
		HarbolBiList_DelNodeByIndex(p, 1, NULL);
		for( struct HarbolBiListNode *n=HarbolBiList_GetHead(p) ; n ; n = HarbolBiListNode_GetNextNode(n) )
			fprintf(HARBOL_debug_output, "bilist value : %" PRIi64 "\n", HarbolBiListNode_GetValue(n).Int64);
	}
	
	// test setting data by index
	fputs("bilist :: test setting data by index (index of 0 is head node).", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolBiList_SetValue(p, 0, (union HarbolValue){.Int64=222}); // 0 is head
	for( struct HarbolBiListNode *n=HarbolBiList_GetHead(p) ; n ; n = HarbolBiListNode_GetNextNode(n) )
		fprintf(HARBOL_debug_output, "bilist value : %" PRIi64 "\n", HarbolBiListNode_GetValue(n).Int64);
	
	// test deleting head node after freeing.
	fputs("bilist :: test deleting head node after freeing and new insertion.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolBiList_Del(p, NULL);
	HarbolBiList_InsertValueAtHead(p, (union HarbolValue){.Int64=100});
	for( struct HarbolBiListNode *n=HarbolBiList_GetHead(p) ; n ; n = HarbolBiListNode_GetNextNode(n) )
		fprintf(HARBOL_debug_output, "bilist value : %" PRIi64 "\n", HarbolBiListNode_GetValue(n).Int64);
	HarbolBiList_DelNodeByIndex(p, 0, NULL);
	fprintf(HARBOL_debug_output, "bilist head node ptr : %p, tail node ptr : %p\n", HarbolBiList_GetHead(p), HarbolBiList_GetTail(p));
	
	
	// test deleting items by index on a list size of 2.
	fputs("bilist :: test deleting items by index on a list size of 2.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolBiList_Del(p, NULL);
	HarbolBiList_InsertValueAtTail(p, (union HarbolValue){.Int64=100});
	HarbolBiList_InsertValueAtTail(p, (union HarbolValue){.Int64=101});
	for( struct HarbolBiListNode *n=HarbolBiList_GetHead(p) ; n ; n = HarbolBiListNode_GetNextNode(n) )
		fprintf(HARBOL_debug_output, "bilist value : %" PRIi64 "\n", HarbolBiListNode_GetValue(n).Int64);
	HarbolBiList_DelNodeByIndex(p, 0, NULL);
	for( struct HarbolBiListNode *n=HarbolBiList_GetHead(p) ; n ; n = HarbolBiListNode_GetNextNode(n) )
		fprintf(HARBOL_debug_output, "bilist value : %" PRIi64 "\n", HarbolBiListNode_GetValue(n).Int64);
	
	// test insertion in middle of list
	fputs("bilist :: test middle insertion.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolBiList_Del(p, NULL);
	HarbolBiList_InsertValueAtIndex(p, (union HarbolValue){.Int64=100}, 0); // have it insert at head by inserting at index 0.
	HarbolBiList_InsertValueAtIndex(p, (union HarbolValue){.Int64=101}, 10); // have it insert at tail by having the index exceed the size of list
	HarbolBiList_InsertValueAtIndex(p, (union HarbolValue){.Int64=102}, 1);
	HarbolBiList_InsertValueAtIndex(p, (union HarbolValue){.Int64=102}, 1);
	HarbolBiList_InsertValueAtIndex(p, (union HarbolValue){.Int64=102}, 1);
	HarbolBiList_InsertValueAtIndex(p, (union HarbolValue){.Int64=102}, 1);
	HarbolBiList_InsertValueAtIndex(p, (union HarbolValue){.Int64=103}, 1);
	for( struct HarbolBiListNode *n=HarbolBiList_GetHead(p) ; n ; n = HarbolBiListNode_GetNextNode(n) )
		fprintf(HARBOL_debug_output, "bilist value : %" PRIi64 "\n", HarbolBiListNode_GetValue(n).Int64);
	
	
	// free data
	fputs("bilist :: test destruction.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolBiList_Del(&i, NULL);
	HarbolBiList_Del(&i, NULL);
	fprintf(HARBOL_debug_output, "i's Head is null? '%s'\n", i.Head ? "no" : "yes");
	
	HarbolBiList_Del(p, NULL);
	fprintf(HARBOL_debug_output, "p's Head is null? '%s', but is p null? '%s'\n", p->Head ? "no" : "yes", p ? "no" : "yes");
	HarbolBiList_Free(&p, NULL);
	fprintf(HARBOL_debug_output, "p is null? '%s'\n", p ? "no" : "yes");
}

void TestHarbolByteBuffer(void)
{
	if( !HARBOL_debug_output )
		return;
	
	// Test allocation and initializations
	fputs("bytebuffer :: test allocation/initialization.\n", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	struct HarbolByteBuffer *p = HarbolByteBuffer_New();
	assert( p );
	
	struct HarbolByteBuffer i = (struct HarbolByteBuffer){0};
	
	// test adding a byte.
	fputs("bytebuffer :: test byte appending.\n", HARBOL_debug_output);
	HarbolByteBuffer_InsertByte(p, 5);
	HarbolByteBuffer_InsertByte(&i, 6);
	for( size_t n=0 ; n<HarbolByteBuffer_Count(p) ; n++ )
		fprintf(HARBOL_debug_output, "bytebuffer value : %zu\n", HarbolByteBuffer_GetBuffer(p)[n]);
	for( size_t n=0 ; n<HarbolByteBuffer_Count(p) ; n++ )
		fprintf(HARBOL_debug_output, "bytebuffer value : %zu\n", HarbolByteBuffer_GetBuffer(&i)[n]);
	
	// test integer appending
	HarbolByteBuffer_Del(&i);
	HarbolByteBuffer_Del(p);
	uint16_t ush = 50;
	HarbolByteBuffer_InsertInt(p, ush, sizeof ush);
	HarbolByteBuffer_InsertInt(&i, ush, sizeof ush);
	for( size_t n=0 ; n<HarbolByteBuffer_Count(p) ; n++ )
		fprintf(HARBOL_debug_output, "bytebuffer value : %zu\n", HarbolByteBuffer_GetBuffer(p)[n]);
	for( size_t n=0 ; n<HarbolByteBuffer_Count(p) ; n++ )
		fprintf(HARBOL_debug_output, "bytebuffer value : %zu\n", HarbolByteBuffer_GetBuffer(&i)[n]);
	
	HarbolByteBuffer_Del(&i);
	HarbolByteBuffer_Del(p);
	uint64_t ull = 0xabcdef;
	HarbolByteBuffer_InsertInt(p, ull, sizeof ull);
	HarbolByteBuffer_InsertInt(&i, ull, sizeof ull);
	for( size_t n=0 ; n<HarbolByteBuffer_Count(p) ; n++ )
		fprintf(HARBOL_debug_output, "bytebuffer value : %zu\n", HarbolByteBuffer_GetBuffer(p)[n]);
	for( size_t n=0 ; n<HarbolByteBuffer_Count(p) ; n++ )
		fprintf(HARBOL_debug_output, "bytebuffer value : %zu\n", HarbolByteBuffer_GetBuffer(&i)[n]);
	
	
	// free data
	fputs("bytebuffer :: test destruction.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolByteBuffer_Del(&i);
	fprintf(HARBOL_debug_output, "i's buffer is null? '%s'\n", i.Buffer ? "no" : "yes");
	
	HarbolByteBuffer_Del(p);
	fprintf(HARBOL_debug_output, "p's buffer is null? '%s'\n", p->Buffer ? "no" : "yes");
	HarbolByteBuffer_Free(&p);
	fprintf(HARBOL_debug_output, "p is null? '%s'\n", p ? "no" : "yes");
}

void TestHarbolTuple(void)
{
	if( !HARBOL_debug_output )
		return;
	
	// Test allocation and initializations
	fputs("tuple :: test allocation/initialization.\n", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	
	union HarbolValue stuff[5] = {
		{.Int64 = 1},
		{.Int64 = 2},
		{.Int64 = 3},
		{.Int64 = 4},
		{.Int64 = 5},
	};
	struct HarbolTuple *p = HarbolTuple_New(5, stuff);
	assert( p );
	
	struct HarbolTuple i;
	HarbolTuple_Init(&i, 5, stuff);
	
	// test getting a single item and then iterating.
	fputs("tuple :: test single item retrieval and iterating.\n", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	
	union HarbolValue single_item = HarbolTuple_GetItem(p, 0);
	fprintf(HARBOL_debug_output, "single item tuple value : %" PRIu64 "\n", single_item.Int64);
	
	for( size_t n=0 ; n<HarbolTuple_Len(p) ; n++ )
		fprintf(HARBOL_debug_output, "tuple value : %" PRIu64 "\n", HarbolTuple_GetItem(p, n).Int64);
	
	// free data
	fputs("tuple :: test destruction.", HARBOL_debug_output);
	fputs("\n", HARBOL_debug_output);
	HarbolTuple_Del(&i);
	fprintf(HARBOL_debug_output, "i's item is null? '%s'\n", i.Items ? "no" : "yes");
	
	HarbolTuple_Del(p);
	fprintf(HARBOL_debug_output, "p's item is null? '%s'\n", p->Items ? "no" : "yes");
	HarbolTuple_Free(&p);
	fprintf(HARBOL_debug_output, "p is null? '%s'\n", p ? "no" : "yes");
}

void TestHarbolMemoryPool(void)
{
	if( !HARBOL_debug_output )
		return;
	
	// Test allocation and initializations
	fputs("heap :: test allocation/initialization.\n", HARBOL_debug_output);
	
	struct HarbolMemoryPool i = (struct HarbolMemoryPool){0};
#ifdef HARBOL_HEAP_NO_MALLOC
	HarbolMemoryPool_Init(&i);
#else
	HarbolMemoryPool_Init(&i, 1000);
#endif
	fprintf(HARBOL_debug_output, "remaining heap mem: '%zu'\n", HarbolMemoryPool_Remaining(&i));
	
	// test giving memory
	fputs("heap :: test giving memory.\n", HARBOL_debug_output);
	int *p = HarbolMemoryPool_Alloc(&i, sizeof *p);
	fprintf(HARBOL_debug_output, "p is null? '%s'\n", p ? "no" : "yes");
	if( p ) {
		*p = 500;
		fprintf(HARBOL_debug_output, "p's value: %i\n", *p);
	}
	
	float *f = HarbolMemoryPool_Alloc(&i, sizeof *f);
	fprintf(HARBOL_debug_output, "f is null? '%s'\n", f ? "no" : "yes");
	if( f ) {
		*f = 500.5f;
		fprintf(HARBOL_debug_output, "f's value: %f\n", *f);
	}
	fprintf(HARBOL_debug_output, "remaining heap mem: '%zu'\n", HarbolMemoryPool_Remaining(&i));
	
	// test releasing memory
	fputs("heap :: test releasing memory.\n", HARBOL_debug_output);
	HarbolMemoryPool_Dealloc(&i, f), f=NULL;
	HarbolMemoryPool_Dealloc(&i, p), p=NULL;
	
	// test re-giving memory
	fputs("heap :: test regiving memory.\n", HARBOL_debug_output);
	p = HarbolMemoryPool_Alloc(&i, sizeof *p);
	fprintf(HARBOL_debug_output, "p is null? '%s'\n", p ? "no" : "yes");
	if( p ) {
		*p = 532;
		fprintf(HARBOL_debug_output, "p's value: %i\n", *p);
	}
	f = HarbolMemoryPool_Alloc(&i, sizeof *f);
	fprintf(HARBOL_debug_output, "f is null? '%s'\n", f ? "no" : "yes");
	if( f ) {
		*f = 466.5f;
		fprintf(HARBOL_debug_output, "f's value: %f\n", *f);
	}
	fprintf(HARBOL_debug_output, "remaining heap mem: '%zu'\n", HarbolMemoryPool_Remaining(&i));
	HarbolMemoryPool_Dealloc(&i, p), p=NULL; // release memory that's from different region.
	HarbolMemoryPool_Dealloc(&i, f), f=NULL;
	fprintf(HARBOL_debug_output, "remaining heap mem: '%zu'\n", HarbolMemoryPool_Remaining(&i));
	
	// test giving array memory
	fputs("heap :: test giving array memory.\n", HARBOL_debug_output);
	const size_t arrsize = 100;
	p = HarbolMemoryPool_Alloc(&i, sizeof *p * arrsize);
	fprintf(HARBOL_debug_output, "p is null? '%s'\n", p ? "no" : "yes");
	if( p ) {
		for( size_t i=0 ; i<arrsize ; i++ )
			p[i] = i+1;
		for( size_t i=0 ; i<arrsize ; i++ )
			fprintf(HARBOL_debug_output, "p[%zu] value: %i\n", i, p[i]);
	}
	fprintf(HARBOL_debug_output, "remaining heap mem: '%zu'\n", HarbolMemoryPool_Remaining(&i));
	f = HarbolMemoryPool_Alloc(&i, sizeof *f * arrsize);
	fprintf(HARBOL_debug_output, "f is null? '%s'\n", f ? "no" : "yes");
	if( f ) {
		for( size_t i=0 ; i<arrsize ; i++ )
			f[i] = i+1.15f;
		for( size_t i=0 ; i<arrsize ; i++ )
			fprintf(HARBOL_debug_output, "f[%zu] value: %f\n", i, f[i]);
	}
	HarbolMemoryPool_Dealloc(&i, p), p=NULL;
	HarbolMemoryPool_Dealloc(&i, f), f=NULL;
	fprintf(HARBOL_debug_output, "remaining heap mem: '%zu'\n", HarbolMemoryPool_Remaining(&i));
	
	
	// test using heap to make a unilinked list!
	fputs("heap :: test using heap for unilinked list.\n", HARBOL_debug_output);
	struct HarbolUniList *list = HarbolMemoryPool_Alloc(&i, sizeof *list);
	assert( list );
	
	struct HarbolUniListNode *node1 = HarbolMemoryPool_Alloc(&i, sizeof *node1);
	assert( node1 );
	node1->Data = (union HarbolValue){.Int64 = 1};
	HarbolUniList_InsertNodeAtTail(list, node1);
	
	struct HarbolUniListNode *node2 = HarbolMemoryPool_Alloc(&i, sizeof *node2);
	assert( node2 );
	node2->Data = (union HarbolValue){.Int64 = 2};
	HarbolUniList_InsertNodeAtTail(list, node2);
	
	struct HarbolUniListNode *node3 = HarbolMemoryPool_Alloc(&i, sizeof *node3);
	assert( node3 );
	node3->Data = (union HarbolValue){.Int64 = 3};
	HarbolUniList_InsertNodeAtTail(list, node3);
	
	struct HarbolUniListNode *node4 = HarbolMemoryPool_Alloc(&i, sizeof *node4);
	assert( node4 );
	node4->Data = (union HarbolValue){.Int64 = 4};
	HarbolUniList_InsertNodeAtTail(list, node4);
	
	struct HarbolUniListNode *node5 = HarbolMemoryPool_Alloc(&i, sizeof *node5);
	assert( node5 );
	node5->Data = (union HarbolValue){.Int64 = 5};
	HarbolUniList_InsertNodeAtTail(list, node5);
	
	for( struct HarbolUniListNode *n=list->Head ; n ; n = n->Next )
		fprintf(HARBOL_debug_output, "unilist value : %" PRIi64 "\n", n->Data.Int64);
	
	HarbolMemoryPool_Dealloc(&i, node1), node1=NULL;
	HarbolMemoryPool_Dealloc(&i, node2), node2=NULL;
	HarbolMemoryPool_Dealloc(&i, node3), node3=NULL;
	HarbolMemoryPool_Dealloc(&i, node4), node4=NULL;
	HarbolMemoryPool_Dealloc(&i, node5), node5=NULL;
	HarbolMemoryPool_Dealloc(&i, list), list=NULL;
	
	// test "double freeing"
	fputs("\nheap :: test double freeing.\n", HARBOL_debug_output);
	p = HarbolMemoryPool_Alloc(&i, sizeof *p);
	fprintf(HARBOL_debug_output, "p is null? '%s'\n", p ? "no" : "yes");
	if( p ) {
		*p = 500;
		fprintf(HARBOL_debug_output, "p's value: %i\n", *p);
	}
	
	HarbolMemoryPool_Dealloc(&i, p);
	HarbolMemoryPool_Dealloc(&i, p);
	
	fprintf(HARBOL_debug_output, "\nheap :: pool size == %zu.\n", HarbolMemoryPool_Remaining(&i));
	for( struct HarbolAllocNode *n = i.FreeList ; n ; n = n->NextFree )
		fprintf(HARBOL_debug_output, "defragged heap :: n (%zu) size == %zu.\n", (uintptr_t)n, n->Size);
	
	float *hk = HarbolMemoryPool_Alloc(&i, sizeof *hk * 99);
	double *fg = HarbolMemoryPool_Alloc(&i, sizeof *fg * 10);
	char *fff = HarbolMemoryPool_Alloc(&i, sizeof *fff * 50);
	float *f32 = HarbolMemoryPool_Alloc(&i, sizeof *f32 * 23);
	char *jj = HarbolMemoryPool_Alloc(&i, sizeof *jj * 100);
	struct HarbolAllocNode *ac = HarbolMemoryPool_Alloc(&i, sizeof *ac * 31);
	HarbolMemoryPool_Dealloc(&i, fff);
	HarbolMemoryPool_Dealloc(&i, fg);
	HarbolMemoryPool_Dealloc(&i, ac);
	HarbolMemoryPool_Dealloc(&i, f32);
	fprintf(HARBOL_debug_output, "\nheap :: pool size == %zu.\n", HarbolMemoryPool_Remaining(&i));
	for( struct HarbolAllocNode *n = i.FreeList ; n ; n = n->NextFree )
		fprintf(HARBOL_debug_output, "heap :: n (%zu) size == %zu.\n", (uintptr_t)n, n->Size);
	fprintf(HARBOL_debug_output, "heap :: heap bottom (%zu).\n", (uintptr_t)i.HeapBottom);
	
	HarbolMemoryPool_Dealloc(&i, hk);
	fprintf(HARBOL_debug_output, "\ncrazy heap :: pool size == %zu.\n", HarbolMemoryPool_Remaining(&i));
	for( struct HarbolAllocNode *n = i.FreeList ; n ; n = n->NextFree )
		fprintf(HARBOL_debug_output, "crazy heap :: n (%zu) size == %zu.\n", (uintptr_t)n, n->Size);
		
	HarbolMemoryPool_Dealloc(&i, jj);
	
	fprintf(HARBOL_debug_output, "\nlast heap :: pool size == %zu.\n", HarbolMemoryPool_Remaining(&i));
	for( struct HarbolAllocNode *n = i.FreeList ; n ; n = n->NextFree )
		fprintf(HARBOL_debug_output, "last heap :: n (%zu) size == %zu.\n", (uintptr_t)n, n->Size);
	//fprintf(HARBOL_debug_output, "heap :: heap bottom (%zu).\n", (uintptr_t)i.HeapBottom);
	
	fputs("\nheap :: test reallocating.\n", HARBOL_debug_output);
	jj = HarbolMemoryPool_Alloc(&i, sizeof *jj);
	*jj = 50;
	fprintf(HARBOL_debug_output, "heap :: jj == %i.\n", *jj);
	
	int *newer = HarbolMemoryPool_Realloc(&i, jj, sizeof *newer);
	fprintf(HARBOL_debug_output, "heap :: newer == %i.\n", *newer);
	jj = HarbolMemoryPool_Realloc(&i, newer, sizeof *jj);
	fprintf(HARBOL_debug_output, "heap :: jj == %i.\n", *jj);
	
	newer = HarbolMemoryPool_Realloc(&i, jj, sizeof *newer * 10);
	for( size_t i=0 ; i<10 ; i++ ) {
		newer[i] = i+1;
		fprintf(HARBOL_debug_output, "heap :: newer[%zu] == %i.\n", i, newer[i]);
	}
	newer = HarbolMemoryPool_Realloc(&i, newer, sizeof *newer * 5);
	for( size_t i=0 ; i<5 ; i++ )
		fprintf(HARBOL_debug_output, "heap :: reallocated newer[%zu] == %i.\n", i, newer[i]);
	
	// free data
	fputs("heap :: test destruction.\n", HARBOL_debug_output);
	HarbolMemoryPool_Del(&i);
	fprintf(HARBOL_debug_output, "i's heap bottom is null? '%s'\n", i.HeapBottom ? "no" : "yes");
	fprintf(HARBOL_debug_output, "i's FreeList is null? '%s'\n", i.FreeList ? "no" : "yes");
}

void TestHarbolGraph(void)
{
	if( !HARBOL_debug_output )
		return;
	
	// Test allocation and initializations
	fputs("graph :: test allocation/initialization.\n", HARBOL_debug_output);
	
	struct HarbolGraph g = (struct HarbolGraph){0};
	HarbolGraph_Init(&g);
	struct HarbolGraph *p = HarbolGraph_New();
	if( p )
		fputs("graph :: allocation/initialization of p is GOOD.\n", HARBOL_debug_output);
	
	// Test adding Vertices
	fputs("\ngraph :: test adding Vertices.\n", HARBOL_debug_output);
	for( size_t c=0 ; c<5 ; c++ ) {
		HarbolGraph_InsertVertexByValue(&g, (union HarbolValue){.Int64=c+1});
		HarbolGraph_InsertVertexByValue(p, (union HarbolValue){.Int64=c+1});
	}
	for( size_t i=0 ; i<g.Count ; i++ ) {
		struct HarbolGraphVertex *vert = HarbolGraph_GetVertexByIndex(&g, i);
		fprintf(HARBOL_debug_output, "Vertex Data: '%" PRIi64 "'\n", vert->Data.Int64);
	}
	// test linking two Vertices
	fputs("\ngraph :: test linking Vertices.\n", HARBOL_debug_output);
	fprintf(HARBOL_debug_output, "linking was success?: '%u'\n", HarbolGraph_InsertEdgeBtwnVerts(&g, 0, 1, (union HarbolValue){0}));
	fprintf(HARBOL_debug_output, "Edge count in graph: '%u'\n", HarbolGraph_GetEdgeCount(&g));
	
	for( size_t i=0 ; i<g.Count ; i++ ) {
		struct HarbolGraphVertex *vert = g.Table[i].Ptr;
		fprintf(HARBOL_debug_output, "Vertex Data: '%" PRIi64 "'\n", vert->Data.Int64);
		for( size_t n=0 ; n<vert->Count ; n++ ) {
			struct HarbolGraphEdge *k = vert->Table[i].Ptr;
			fprintf(HARBOL_debug_output, "Edge Data: '%" PRIi64 "'\n", k->Weight.Int64);
			if( k->VertexSocket )
				fprintf(HARBOL_debug_output, "Vertex Socket Data: '%" PRIi64 "'\n", k->VertexSocket->Data.Int64);
		}
	}
	
	// test unlinking the two previous Vertices
	fputs("\ngraph :: test unlinking the two previous Vertices.\n", HARBOL_debug_output);
	fprintf(HARBOL_debug_output, "unlinking was success?: '%u'\n", HarbolGraph_RemoveEdgeBtwnVerts(&g, 0, 1, NULL));
	fprintf(HARBOL_debug_output, "Edge count in graph: '%u'\n", HarbolGraph_GetEdgeCount(&g));
	
	// test linking ALL available Vertices!
	fputs("\ngraph :: test linking all Vertices in graph.\n", HARBOL_debug_output);
	// link 1 to 2
	HarbolGraph_InsertEdgeBtwnVerts(&g, 0, 1, (union HarbolValue){5});
	// link 2 to 3
	HarbolGraph_InsertEdgeBtwnVerts(&g, 1, 2, (union HarbolValue){2});
	// link 3 to 4
	HarbolGraph_InsertEdgeBtwnVerts(&g, 2, 3, (union HarbolValue){24});
	// link 4 to 5
	HarbolGraph_InsertEdgeBtwnVerts(&g, 3, 4, (union HarbolValue){62});
	// link 5 to 3
	HarbolGraph_InsertEdgeBtwnVerts(&g, 4, 2, (union HarbolValue){77});
	fprintf(HARBOL_debug_output, "Edge count in graph: '%u'\n", HarbolGraph_GetEdgeCount(&g));
	
	for( size_t i=0 ; i<g.Count ; i++ ) {
		struct HarbolGraphVertex *vert = g.Table[i].Ptr;
		fprintf(HARBOL_debug_output, "Vertex Data: '%" PRIi64 "'\n", vert->Data.Int64);
		for( size_t n=0 ; n<vert->Count ; n++ ) {
			struct HarbolGraphEdge *k = vert->Table[n].Ptr;
			fprintf(HARBOL_debug_output, "Edge Data: '%" PRIi64 "'\n", k->Weight.Int64);
			if( k->VertexSocket )
				fprintf(HARBOL_debug_output, "Edge Vertex Socket Data: '%" PRIi64 "'\n", k->VertexSocket->Data.Int64);
		}
	}
	fprintf(HARBOL_debug_output, "\nRemoving 5th value success?: '%u'\n", HarbolGraph_RemoveVertexByIndex(&g, 4, NULL, NULL));
	for( size_t i=0 ; i<g.Count ; i++ ) {
		struct HarbolGraphVertex *vert = g.Table[i].Ptr;
		fprintf(HARBOL_debug_output, "Vertex Data: '%" PRIi64 "'\n", vert->Data.Int64);
		for( size_t n=0 ; n<vert->Count ; n++ ) {
			struct HarbolGraphEdge *k = vert->Table[n].Ptr;
			fprintf(HARBOL_debug_output, "Edge Data: '%" PRIi64 "'\n", k->Weight.Int64);
			if( k->VertexSocket )
				fprintf(HARBOL_debug_output, "Edge Vertex Socket Data: '%" PRIi64 "'\n", k->VertexSocket->Data.Int64);
		}
	}
	
	// test changing vertex data through an index!
	fprintf(HARBOL_debug_output, "\nTest changing vertex data by index\n");
	HarbolGraph_SetVertexDataByIndex(&g, 0, (union HarbolValue){.Int64 = 100});
	for( size_t i=0 ; i<g.Count ; i++ ) {
		struct HarbolGraphVertex *vert = g.Table[i].Ptr;
		fprintf(HARBOL_debug_output, "Vertex Data: '%" PRIi64 "'\n", vert->Data.Int64);
		for( size_t n=0 ; n<vert->Count ; n++ ) {
			struct HarbolGraphEdge *k = vert->Table[n].Ptr;
			fprintf(HARBOL_debug_output, "Edge Data: '%" PRIi64 "'\n", k->Weight.Int64);
			if( k->VertexSocket )
				fprintf(HARBOL_debug_output, "Vertex Socket Data: '%" PRIi64 "'\n", k->VertexSocket->Data.Int64);
		}
	}
	
	// test getting edge pointer
	fputs("\ngraph :: test getting edge pointer.\n", HARBOL_debug_output);
	struct HarbolGraphEdge *edge = HarbolGraph_GetEdgeBtwnVertices(&g, 0, 1);
	if( edge ) {
		fputs("edge ptr is VALID.\n", HARBOL_debug_output);
		if( edge->VertexSocket )
			fprintf(HARBOL_debug_output, "edge Vertex Socket Data: '%" PRIi64 "'\n", edge->VertexSocket->Data.Int64);
	}
	
	// test adjacency function
	fprintf(HARBOL_debug_output, "\nindex 0 is adjacent to index 1?: '%u'\n", HarbolGraph_IsVertexAdjacent(&g, 0, 1));
	
	// free data
	fputs("\ngraph :: test destruction.\n", HARBOL_debug_output);
	HarbolGraph_Del(&g, NULL, NULL);
	fprintf(HARBOL_debug_output, "i's Vertices vector is null? '%s'\n", g.Table ? "no" : "yes");
	
	HarbolGraph_Del(p, NULL, NULL);
	fprintf(HARBOL_debug_output, "p's Vertices vector is null? '%s'\n", p->Table ? "no" : "yes");
	HarbolGraph_Free(&p, NULL, NULL);
	fprintf(HARBOL_debug_output, "p is null? '%s'\n", p ? "no" : "yes");
}

void TestTree(void)
{
	if( !HARBOL_debug_output )
		return;
	
	// Test allocation and initializations
	fputs("tree :: test allocation/initialization.\n", HARBOL_debug_output);
	struct HarbolTree i = (struct HarbolTree){0};
	HarbolTree_Init(&i);
	struct HarbolTree *p = HarbolTree_New((union HarbolValue){0});
	if( p )
		fputs("graph :: allocation/initialization of p is GOOD.\n", HARBOL_debug_output);
	
	
	// Test insertions
	fputs("\ntree :: test insertions.\n", HARBOL_debug_output);
	fprintf(HARBOL_debug_output, "insertion by node success?: '%u'\n", HarbolTree_InsertChildByNode(p, HarbolTree_New((union HarbolValue){.Int64=20})));
	fprintf(HARBOL_debug_output, "insertion by value success?: '%u'\n", HarbolTree_InsertChildByValue(p, (union HarbolValue){.Int64=10}));
	HarbolTree_Del(p, NULL);
	
	// Test deletions
	fputs("\ntree :: test deletions by first adding 5 children.\n", HARBOL_debug_output);
	HarbolTree_InsertChildByValue(p, (union HarbolValue){.Int64=1});
	HarbolTree_InsertChildByValue(p, (union HarbolValue){.Int64=2});
	HarbolTree_InsertChildByValue(p, (union HarbolValue){.Int64=3});
	HarbolTree_InsertChildByValue(p, (union HarbolValue){.Int64=4});
	HarbolTree_InsertChildByValue(p, (union HarbolValue){.Int64=5});
	for( size_t n=0 ; n<p->Children.Count ; n++ ) {
		struct HarbolTree *child = p->Children.Table[n].Ptr;
		fprintf(HARBOL_debug_output, "child #%zu value: '%" PRIi64 "'\n", n, child->Data.Int64);
	}
	fputs("\ndeleting index 1\n", HARBOL_debug_output);
	HarbolTree_RemoveChildByIndex(p, 1, NULL);
	for( size_t n=0 ; n<p->Children.Count ; n++ ) {
		struct HarbolTree *child = p->Children.Table[n].Ptr;
		fprintf(HARBOL_debug_output, "child #%zu value: '%" PRIi64 "'\n", n, child->Data.Int64);
	}
	// Test delete by node reference
	fputs("\ntree :: test deletion by node reference.\n", HARBOL_debug_output);
	// delete first child!
	fputs("\ndeleting index 0\n", HARBOL_debug_output);
	HarbolTree_RemoveChildByRef(p, (struct HarbolTree **)&p->Children.Table[0].Ptr, NULL);
	for( size_t n=0 ; n<p->Children.Count ; n++ ) {
		struct HarbolTree *child = p->Children.Table[n].Ptr;
		fprintf(HARBOL_debug_output, "child #%zu value: '%" PRIi64 "'\n", n, child->Data.Int64);
	}
	// Test creating something of an abstract syntax tree.
	fputs("\ntree :: test creating something of an abstract syntax tree.\n", HARBOL_debug_output);
	HarbolTree_Del(p, NULL);
	HarbolTree_SetData(p, (union HarbolValue){.StrObjPtr=HarbolString_NewStr("program")});
	HarbolTree_InsertChildByValue(p, (union HarbolValue){.StrObjPtr=HarbolString_NewStr("stmt")});
	HarbolTree_InsertChildByValue(p->Children.Table[0].Ptr, (union HarbolValue){.StrObjPtr=HarbolString_NewStr("if")});
	HarbolTree_InsertChildByValue(p->Children.Table[0].Ptr, (union HarbolValue){.StrObjPtr=HarbolString_NewStr("cond")});
	HarbolTree_InsertChildByValue(p->Children.Table[0].Ptr, (union HarbolValue){.StrObjPtr=HarbolString_NewStr("stmt")});
	HarbolTree_InsertChildByValue(p->Children.Table[0].Ptr, (union HarbolValue){.StrObjPtr=HarbolString_NewStr("else")});
	fprintf(HARBOL_debug_output, "p's data: '%s'\n", p->Data.StrObjPtr->CStr);
	struct HarbolTree *kid = p->Children.Table[0].Ptr;
	fprintf(HARBOL_debug_output, "p's child data: '%s'\n", kid->Data.StrObjPtr->CStr);
	for( size_t n=0 ; n<kid->Children.Count ; n++ ) {
		struct HarbolTree *child = kid->Children.Table[n].Ptr;
		fprintf(HARBOL_debug_output, "p's child's children data: '%s'\n", child->Data.StrObjPtr->CStr);
	}
	fputs("\nfreeing string data.\n", HARBOL_debug_output);
	HarbolTree_Del(p, (bool (*)(void *))HarbolString_Free);
	
	// free data
	fputs("\ntree :: test destruction.\n", HARBOL_debug_output);
	HarbolTree_Del(&i, NULL);
	fprintf(HARBOL_debug_output, "i's Children vector is null? '%s'\n", i.Children.Table ? "no" : "yes");
	
	HarbolTree_Del(p, NULL);
	fprintf(HARBOL_debug_output, "p's Children vector is null? '%s'\n", p->Children.Table ? "no" : "yes");
	HarbolTree_Free(&p, NULL);
	fprintf(HARBOL_debug_output, "p is null? '%s'\n", p ? "no" : "yes");
}

void TestHarbolLinkMap(void)
{
	// Test allocation and initializations
	fputs("linkmap :: test allocation / initialization.\n", HARBOL_debug_output);
	struct HarbolLinkMap *p = HarbolLinkMap_New();
	assert( p );
	
	struct HarbolLinkMap i = (struct HarbolLinkMap){0};
	
	// test insertion
	fputs("\nlinkmap :: test insertion.\n", HARBOL_debug_output);
	HarbolLinkMap_Insert(p, "1", (union HarbolValue){.Int64=1});
	HarbolLinkMap_Insert(p, "2", (union HarbolValue){.Int64=2});
	HarbolLinkMap_Insert(p, "3", (union HarbolValue){.Int64=3});
	HarbolLinkMap_Insert(p, "4", (union HarbolValue){.Int64=4});
	HarbolLinkMap_Insert(p, "5", (union HarbolValue){.Int64=5});
	HarbolLinkMap_Insert(p, "6", (union HarbolValue){.Int64=6});
	HarbolLinkMap_Insert(p, "7", (union HarbolValue){.Int64=7});
	HarbolLinkMap_Insert(p, "8", (union HarbolValue){.Int64=8});
	HarbolLinkMap_Insert(p, "9", (union HarbolValue){.Int64=9});
	HarbolLinkMap_Insert(p, "10", (union HarbolValue){.Int64=10});
	
	HarbolLinkMap_Insert(&i, "1", (union HarbolValue){.Int64=1});
	HarbolLinkMap_Insert(&i, "2", (union HarbolValue){.Int64=2});
	HarbolLinkMap_Insert(&i, "3", (union HarbolValue){.Int64=3});
	HarbolLinkMap_Insert(&i, "4", (union HarbolValue){.Int64=4});
	HarbolLinkMap_Insert(&i, "5", (union HarbolValue){.Int64=5});
	HarbolLinkMap_Insert(&i, "6", (union HarbolValue){.Int64=6});
	HarbolLinkMap_Insert(&i, "7", (union HarbolValue){.Int64=7});
	HarbolLinkMap_Insert(&i, "8", (union HarbolValue){.Int64=8});
	HarbolLinkMap_Insert(&i, "9", (union HarbolValue){.Int64=9});
	HarbolLinkMap_Insert(&i, "10", (union HarbolValue){.Int64=10});
	
	// test retrieval.
	fputs("\nlinkmap :: test data retrieval.\n", HARBOL_debug_output);
	fprintf(HARBOL_debug_output, "ptr[\"1\"] == %" PRIi64 "\n", HarbolLinkMap_Get(p, "1").Int64);
	fprintf(HARBOL_debug_output, "ptr[\"2\"] == %" PRIi64 "\n", HarbolLinkMap_Get(p, "2").Int64);
	fprintf(HARBOL_debug_output, "stk[\"1\"] == %" PRIi64 "\n", HarbolLinkMap_Get(&i, "1").Int64);
	fprintf(HARBOL_debug_output, "stk[\"2\"] == %" PRIi64 "\n", HarbolLinkMap_Get(&i, "2").Int64);
	
	fputs("\nlinkmap :: looping through all data.\n", HARBOL_debug_output);
	for( size_t n=0 ; n<i.Order.Count ; n++ ) {
		struct HarbolKeyValPair *l = i.Order.Table[n].Ptr;
		fprintf(HARBOL_debug_output, "l's value == %" PRIi64 "\n", l->Data.Int64);
	}
	// test setting.
	fputs("\nlinkmap :: test data setting.\n", HARBOL_debug_output);
	HarbolLinkMap_Set(p, "2", (union HarbolValue){.Int64=20});
	HarbolLinkMap_Set(&i, "2", (union HarbolValue){.Int64=200});
	fprintf(HARBOL_debug_output, "ptr[\"1\"] == %" PRIi64 "\n", HarbolLinkMap_Get(p, "1").Int64);
	fprintf(HARBOL_debug_output, "ptr[\"2\"] == %" PRIi64 "\n", HarbolLinkMap_Get(p, "2").Int64);
	fprintf(HARBOL_debug_output, "stk[\"1\"] == %" PRIi64 "\n", HarbolLinkMap_Get(&i, "1").Int64);
	fprintf(HARBOL_debug_output, "stk[\"2\"] == %" PRIi64 "\n", HarbolLinkMap_Get(&i, "2").Int64);
	fputs("\nlinkmap :: looping through all data.\n", HARBOL_debug_output);
	for( size_t n=0 ; n<i.Order.Count ; n++ ) {
		struct HarbolKeyValPair *l = i.Order.Table[n].Ptr;
		fprintf(HARBOL_debug_output, "l's value == %" PRIi64 "\n", l->Data.Int64);
	}
	fputs("\n", HARBOL_debug_output);
	for( size_t n=0 ; n<p->Order.Count ; n++ ) {
		struct HarbolKeyValPair *l = p->Order.Table[n].Ptr;
		fprintf(HARBOL_debug_output, "l's value == %" PRIi64 "\n", l->Data.Int64);
	}
	
	// test deletion
	fputs("\nlinkmap :: test item deletion.\n", HARBOL_debug_output);
	HarbolLinkMap_Delete(p, "2", NULL);
	fprintf(HARBOL_debug_output, "ptr[\"2\"] == %" PRIi64 "\n", HarbolLinkMap_Get(p, "2").Int64);
	fputs("\nlinkmap :: looping through all data.\n", HARBOL_debug_output);
	for( size_t n=0 ; n<p->Order.Count ; n++ ) {
		struct HarbolKeyValPair *l = p->Order.Table[n].Ptr;
		fprintf(HARBOL_debug_output, "l's value == %" PRIi64 "\n", l->Data.Int64);
	}
	fputs("\nlinkmap :: test item deletion by index.\n", HARBOL_debug_output);
	HarbolLinkMap_DeleteByIndex(p, 2, NULL);
	for( size_t n=0 ; n<p->Order.Count ; n++ ) {
		struct HarbolKeyValPair *l = p->Order.Table[n].Ptr;
		fprintf(HARBOL_debug_output, "l's value == %" PRIi64 "\n", l->Data.Int64);
	}
	// test setting by index
	fputs("\nlinkmap :: test item setting by index.\n", HARBOL_debug_output);
	HarbolLinkMap_SetByIndex(p, 2, (union HarbolValue){.Int64=500});
	for( size_t n=0 ; n<p->Order.Count ; n++ ) {
		struct HarbolKeyValPair *l = p->Order.Table[n].Ptr;
		fprintf(HARBOL_debug_output, "l's value == %" PRIi64 "\n", l->Data.Int64);
	}
	// free data
	fputs("\nlinkmap :: test destruction.\n", HARBOL_debug_output);
	HarbolLinkMap_Del(&i, NULL);
	fprintf(HARBOL_debug_output, "i's buckets are null? '%s'\n", i.Order.Table ? "no" : "yes");
	
	HarbolLinkMap_Del(p, NULL);
	fprintf(HARBOL_debug_output, "p's buckets are null? '%s'\n", p->Order.Table ? "no" : "yes");
	HarbolLinkMap_Free(&p, NULL);
	fprintf(HARBOL_debug_output, "p is null? '%s'\n", p ? "no" : "yes");
}

void TestHarbolConversions(void)
{
	if( !HARBOL_debug_output )
		return;
	
	// inserting five items to all 4 of these collector type data structures.
	struct HarbolVector vec = (struct HarbolVector){0};
	HarbolVector_Insert(&vec, (union HarbolValue){.Int64 = 1});
	HarbolVector_Insert(&vec, (union HarbolValue){.Int64 = 2});
	HarbolVector_Insert(&vec, (union HarbolValue){.Int64 = 3});
	HarbolVector_Insert(&vec, (union HarbolValue){.Int64 = 4});
	HarbolVector_Insert(&vec, (union HarbolValue){.Int64 = 5});
	
	struct HarbolHashmap map = (struct HarbolHashmap){0};
	HarbolMap_Insert(&map, "1", (union HarbolValue){.Int64 = 1});
	HarbolMap_Insert(&map, "2", (union HarbolValue){.Int64 = 2});
	HarbolMap_Insert(&map, "3", (union HarbolValue){.Int64 = 3});
	HarbolMap_Insert(&map, "4", (union HarbolValue){.Int64 = 4});
	HarbolMap_Insert(&map, "5", (union HarbolValue){.Int64 = 5});
	
	struct HarbolUniList unilist = (struct HarbolUniList){0};
	HarbolUniList_InsertValueAtTail(&unilist, (union HarbolValue){.Int64 = 1});
	HarbolUniList_InsertValueAtTail(&unilist, (union HarbolValue){.Int64 = 2});
	HarbolUniList_InsertValueAtTail(&unilist, (union HarbolValue){.Int64 = 3});
	HarbolUniList_InsertValueAtTail(&unilist, (union HarbolValue){.Int64 = 4});
	HarbolUniList_InsertValueAtTail(&unilist, (union HarbolValue){.Int64 = 5});
	
	struct HarbolBiList bilist = (struct HarbolBiList){0};
	HarbolBiList_InsertValueAtTail(&bilist, (union HarbolValue){.Int64 = 1});
	HarbolBiList_InsertValueAtTail(&bilist, (union HarbolValue){.Int64 = 2});
	HarbolBiList_InsertValueAtTail(&bilist, (union HarbolValue){.Int64 = 3});
	HarbolBiList_InsertValueAtTail(&bilist, (union HarbolValue){.Int64 = 4});
	HarbolBiList_InsertValueAtTail(&bilist, (union HarbolValue){.Int64 = 5});
	
	struct HarbolTuple tuple = (struct HarbolTuple){0};
	union HarbolValue stuff[5] = {
		{.Int64 = 1},
		{.Int64 = 2},
		{.Int64 = 3},
		{.Int64 = 4},
		{.Int64 = 5},
	};
	HarbolTuple_Init(&tuple, 5, stuff);
	
	struct HarbolGraph graph = (struct HarbolGraph){0};
	HarbolGraph_InsertVertexByValue(&graph, (union HarbolValue){.Int64 = 1});
	HarbolGraph_InsertVertexByValue(&graph, (union HarbolValue){.Int64 = 2});
	HarbolGraph_InsertVertexByValue(&graph, (union HarbolValue){.Int64 = 3});
	HarbolGraph_InsertVertexByValue(&graph, (union HarbolValue){.Int64 = 4});
	HarbolGraph_InsertVertexByValue(&graph, (union HarbolValue){.Int64 = 5});
	
	struct HarbolLinkMap linkmap = (struct HarbolLinkMap){0};
	HarbolLinkMap_Insert(&linkmap, "1", (union HarbolValue){.Int64 = 1});
	HarbolLinkMap_Insert(&linkmap, "2", (union HarbolValue){.Int64 = 2});
	HarbolLinkMap_Insert(&linkmap, "3", (union HarbolValue){.Int64 = 3});
	HarbolLinkMap_Insert(&linkmap, "4", (union HarbolValue){.Int64 = 4});
	HarbolLinkMap_Insert(&linkmap, "5", (union HarbolValue){.Int64 = 5});
	
	
	// test vector conversion.
	fputs("\ndata struct conversions :: test vector conversions.\n", HARBOL_debug_output);
	{
		struct HarbolVector *p = NULL;
		
		p = HarbolVector_NewFromHarbolUniList(&unilist);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t i=0 ; i<p->Count ; i++ )
				fprintf(HARBOL_debug_output, "unilist -> ptr[%zu] == %" PRIi64 "\n", i, p->Table[i].Int64);
			HarbolVector_Free(&p, NULL);
		}
		
		p = HarbolVector_NewFromHarbolBiList(&bilist);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t i=0 ; i<p->Count ; i++ )
				fprintf(HARBOL_debug_output, "bilist -> ptr[%zu] == %" PRIi64 "\n", i, p->Table[i].Int64);
			HarbolVector_Free(&p, NULL);
		}
		
		p = HarbolVector_NewFromHarbolHashmap(&map);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t i=0 ; i<p->Count ; i++ )
				fprintf(HARBOL_debug_output, "map -> ptr[%zu] == %" PRIi64 "\n", i, p->Table[i].Int64);
			HarbolVector_Free(&p, NULL);
		}
		
		p = HarbolVector_NewFromHarbolTuple(&tuple);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t i=0 ; i<p->Count ; i++ )
				fprintf(HARBOL_debug_output, "tuple -> ptr[%zu] == %" PRIi64 "\n", i, p->Table[i].Int64);
			HarbolVector_Free(&p, NULL);
		}
		
		p = HarbolVector_NewFromHarbolGraph(&graph);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t i=0 ; i<p->Count ; i++ )
				fprintf(HARBOL_debug_output, "graph -> ptr[%zu] == %" PRIi64 "\n", i, p->Table[i].Int64);
			HarbolVector_Free(&p, NULL);
		}
		
		p = HarbolVector_NewFromHarbolLinkMap(&linkmap);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t i=0 ; i<p->Count ; i++ )
				fprintf(HARBOL_debug_output, "linked map -> ptr[%zu] == %" PRIi64 "\n", i, p->Table[i].Int64);
			HarbolVector_Free(&p, NULL);
		}
	}
	
	// test map conversion.
	fputs("\ndata struct conversions :: test map conversions.\n", HARBOL_debug_output);
	{
		struct HarbolHashmap *p = NULL;
		
		p = HarbolMap_NewFromHarbolUniList(&unilist);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t i=0 ; i<p->Count ; i++ ) {
				struct HarbolVector *vec = p->Table + i;
				for( size_t n=0 ; n<vec->Count ; n++ ) {
					struct HarbolKeyValPair *node = vec->Table[n].Ptr;
					fprintf(HARBOL_debug_output, "unilist -> ptr[\"%s\"] == %" PRIi64 "\n", node->KeyName.CStr, node->Data.Int64);
				}
			}
			HarbolMap_Free(&p, NULL);
		}
		
		p = HarbolMap_NewFromHarbolBiList(&bilist);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t i=0 ; i<p->Count ; i++ ) {
				struct HarbolVector *vec = p->Table + i;
				for( size_t n=0 ; n<vec->Count ; n++ ) {
					struct HarbolKeyValPair *node = vec->Table[n].Ptr;
					fprintf(HARBOL_debug_output, "bilist -> ptr[\"%s\"] == %" PRIi64 "\n", node->KeyName.CStr, node->Data.Int64);
				}
			}
			HarbolMap_Free(&p, NULL);
		}
		
		p = HarbolMap_NewFromHarbolVector(&vec);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t i=0 ; i<p->Count ; i++ ) {
				struct HarbolVector *vec = p->Table + i;
				for( size_t n=0 ; n<vec->Count ; n++ ) {
					struct HarbolKeyValPair *node = vec->Table[n].Ptr;
					fprintf(HARBOL_debug_output, "vec -> ptr[\"%s\"] == %" PRIi64 "\n", node->KeyName.CStr, node->Data.Int64);
				}
			}
			HarbolMap_Free(&p, NULL);
		}
		
		p = HarbolMap_NewFromHarbolTuple(&tuple);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t i=0 ; i<p->Count ; i++ ) {
				struct HarbolVector *vec = p->Table + i;
				for( size_t n=0 ; n<vec->Count ; n++ ) {
					struct HarbolKeyValPair *node = vec->Table[n].Ptr;
					fprintf(HARBOL_debug_output, "tuple -> ptr[\"%s\"] == %" PRIi64 "\n", node->KeyName.CStr, node->Data.Int64);
				}
			}
			HarbolMap_Free(&p, NULL);
		}
		
		p = HarbolMap_NewFromHarbolGraph(&graph);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t i=0 ; i<p->Count ; i++ ) {
				struct HarbolVector *vec = p->Table + i;
				for( size_t n=0 ; n<vec->Count ; n++ ) {
					struct HarbolKeyValPair *node = vec->Table[n].Ptr;
					fprintf(HARBOL_debug_output, "graph -> ptr[\"%s\"] == %" PRIi64 "\n", node->KeyName.CStr, node->Data.Int64);
				}
			}
			HarbolMap_Free(&p, NULL);
		}
		
		p = HarbolMap_NewFromHarbolLinkMap(&linkmap);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t i=0 ; i<p->Count ; i++ ) {
				struct HarbolVector *vec = p->Table + i;
				for( size_t n=0 ; n<vec->Count ; n++ ) {
					struct HarbolKeyValPair *node = vec->Table[n].Ptr;
					fprintf(HARBOL_debug_output, "linked map -> ptr[\"%s\"] == %" PRIi64 "\n", node->KeyName.CStr, node->Data.Int64);
				}
			}
			HarbolMap_Free(&p, NULL);
		}
	}
	
	// test uni linked list conversion.
	fputs("\ndata struct conversions :: test singly linked list conversions.\n", HARBOL_debug_output);
	{
		struct HarbolUniList *p = NULL;
		
		p = HarbolUniList_NewFromHarbolBiList(&bilist);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( struct HarbolUniListNode *n=p->Head ; n ; n = n->Next )
				fprintf(HARBOL_debug_output, "bilist value : %" PRIi64 "\n", n->Data.Int64);
			HarbolUniList_Free(&p, NULL);
		}
		
		p = HarbolUniList_NewFromHarbolHashmap(&map);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( struct HarbolUniListNode *n=p->Head ; n ; n = n->Next )
				fprintf(HARBOL_debug_output, "map value : %" PRIi64 "\n", n->Data.Int64);
			HarbolUniList_Free(&p, NULL);
		}
		
		p = HarbolUniList_NewFromHarbolVector(&vec);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( struct HarbolUniListNode *n=p->Head ; n ; n = n->Next )
				fprintf(HARBOL_debug_output, "vec value : %" PRIi64 "\n", n->Data.Int64);
			HarbolUniList_Free(&p, NULL);
		}
		
		p = HarbolUniList_NewFromHarbolTuple(&tuple);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( struct HarbolUniListNode *n=p->Head ; n ; n = n->Next )
				fprintf(HARBOL_debug_output, "tuple value : %" PRIi64 "\n", n->Data.Int64);
			HarbolUniList_Free(&p, NULL);
		}
		
		p = HarbolUniList_NewFromHarbolGraph(&graph);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( struct HarbolUniListNode *n=p->Head ; n ; n = n->Next )
				fprintf(HARBOL_debug_output, "graph value : %" PRIi64 "\n", n->Data.Int64);
			HarbolUniList_Free(&p, NULL);
		}
		
		p = HarbolUniList_NewFromHarbolLinkMap(&linkmap);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( struct HarbolUniListNode *n=p->Head ; n ; n = n->Next )
				fprintf(HARBOL_debug_output, "linked map value : %" PRIi64 "\n", n->Data.Int64);
			HarbolUniList_Free(&p, NULL);
		}
	}
	
	// test bi linked list conversion.
	fputs("\ndata struct conversions :: test doubly linked list conversions.\n", HARBOL_debug_output);
	{
		struct HarbolBiList *p = NULL;
		
		p = HarbolBiList_NewFromHarbolUniList(&unilist);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( struct HarbolBiListNode *n=p->Head ; n ; n = n->Next )
				fprintf(HARBOL_debug_output, "unilist value : %" PRIi64 "\n", n->Data.Int64);
			HarbolBiList_Free(&p, NULL);
		}
		
		p = HarbolBiList_NewFromHarbolHashmap(&map);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( struct HarbolBiListNode *n=p->Head ; n ; n = n->Next )
				fprintf(HARBOL_debug_output, "map value : %" PRIi64 "\n", n->Data.Int64);
			HarbolBiList_Free(&p, NULL);
		}
		
		p = HarbolBiList_NewFromHarbolVector(&vec);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( struct HarbolBiListNode *n=p->Head ; n ; n = n->Next )
				fprintf(HARBOL_debug_output, "vec value : %" PRIi64 "\n", n->Data.Int64);
			HarbolBiList_Free(&p, NULL);
		}
		
		p = HarbolBiList_NewFromHarbolTuple(&tuple);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( struct HarbolBiListNode *n=p->Head ; n ; n = n->Next )
				fprintf(HARBOL_debug_output, "tuple value : %" PRIi64 "\n", n->Data.Int64);
			HarbolBiList_Free(&p, NULL);
		}
		
		p = HarbolBiList_NewFromHarbolGraph(&graph);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( struct HarbolBiListNode *n=p->Head ; n ; n = n->Next )
				fprintf(HARBOL_debug_output, "graph value : %" PRIi64 "\n", n->Data.Int64);
			HarbolBiList_Free(&p, NULL);
		}
		
		p = HarbolBiList_NewFromHarbolLinkMap(&linkmap);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( struct HarbolBiListNode *n=p->Head ; n ; n = n->Next )
				fprintf(HARBOL_debug_output, "linked map value : %" PRIi64 "\n", n->Data.Int64);
			HarbolBiList_Free(&p, NULL);
		}
	}
	
	// test tuple conversion.
	fputs("\ndata struct conversions :: test tuple conversions.\n", HARBOL_debug_output);
	{
		struct HarbolTuple *p = NULL;
		
		p = HarbolTuple_NewFromHarbolUniList(&unilist);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t i=0 ; i<p->Len ; i++ )
				fprintf(HARBOL_debug_output, "unilist value : %" PRIi64 "\n", p->Items[i].Int64);
			HarbolTuple_Free(&p);
		}
		
		p = HarbolTuple_NewFromHarbolHashmap(&map);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t i=0 ; i<p->Len ; i++ )
				fprintf(HARBOL_debug_output, "map value : %" PRIi64 "\n", p->Items[i].Int64);
			HarbolTuple_Free(&p);
		}
		
		p = HarbolTuple_NewFromHarbolVector(&vec);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t i=0 ; i<p->Len ; i++ )
				fprintf(HARBOL_debug_output, "vec value : %" PRIi64 "\n", p->Items[i].Int64);
			HarbolTuple_Free(&p);
		}
		
		p = HarbolTuple_NewFromHarbolBiList(&bilist);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t i=0 ; i<p->Len ; i++ )
				fprintf(HARBOL_debug_output, "bilist value : %" PRIi64 "\n", p->Items[i].Int64);
			HarbolTuple_Free(&p);
		}
		
		p = HarbolTuple_NewFromHarbolGraph(&graph);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t i=0 ; i<p->Len ; i++ )
				fprintf(HARBOL_debug_output, "graph value : %" PRIi64 "\n", p->Items[i].Int64);
			HarbolTuple_Free(&p);
		}
		
		p = HarbolTuple_NewFromHarbolLinkMap(&linkmap);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t i=0 ; i<p->Len ; i++ )
				fprintf(HARBOL_debug_output, "linked map value : %" PRIi64 "\n", p->Items[i].Int64);
			HarbolTuple_Free(&p);
		}
	}
	
	// test graph conversion.
	fputs("\ndata struct conversions :: test graph conversions.\n", HARBOL_debug_output);
	{
		struct HarbolGraph *p = NULL;
		
		p = HarbolGraph_NewFromHarbolUniList(&unilist);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t i=0 ; i<p->Vertices.Count ; i++ ) {
				struct HarbolGraphVertex *v = HarbolGraph_GetVertexByIndex(p, i);
				fprintf(HARBOL_debug_output, "unilist value : %" PRIi64 "\n", v->Data.Int64);
			}
			HarbolGraph_Free(&p, NULL, NULL);
		}
		
		p = HarbolGraph_NewFromHarbolHashmap(&map);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t i=0 ; i<p->Vertices.Count ; i++ ) {
				struct HarbolGraphVertex *v = HarbolGraph_GetVertexByIndex(p, i);
				fprintf(HARBOL_debug_output, "map value : %" PRIi64 "\n", v->Data.Int64);
			}
			HarbolGraph_Free(&p, NULL, NULL);
		}
		
		p = HarbolGraph_NewFromHarbolVector(&vec);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t i=0 ; i<p->Vertices.Count ; i++ ) {
				struct HarbolGraphVertex *v = HarbolGraph_GetVertexByIndex(p, i);
				fprintf(HARBOL_debug_output, "vec value : %" PRIi64 "\n", v->Data.Int64);
			}
			HarbolGraph_Free(&p, NULL, NULL);
		}
		
		p = HarbolGraph_NewFromHarbolBiList(&bilist);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t i=0 ; i<p->Vertices.Count ; i++ ) {
				struct HarbolGraphVertex *v = HarbolGraph_GetVertexByIndex(p, i);
				fprintf(HARBOL_debug_output, "bilist value : %" PRIi64 "\n", v->Data.Int64);
			}
			HarbolGraph_Free(&p, NULL, NULL);
		}
		
		p = HarbolGraph_NewFromHarbolTuple(&tuple);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t i=0 ; i<p->Vertices.Count ; i++ ) {
				struct HarbolGraphVertex *v = HarbolGraph_GetVertexByIndex(p, i);
				fprintf(HARBOL_debug_output, "tuple value : %" PRIi64 "\n", v->Data.Int64);
			}
			HarbolGraph_Free(&p, NULL, NULL);
		}
		
		p = HarbolGraph_NewFromHarbolLinkMap(&linkmap);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t i=0 ; i<p->Vertices.Count ; i++ ) {
				struct HarbolGraphVertex *v = HarbolGraph_GetVertexByIndex(p, i);
				fprintf(HARBOL_debug_output, "linked map value : %" PRIi64 "\n", v->Data.Int64);
			}
			HarbolGraph_Free(&p, NULL, NULL);
		}
	}
	
	// test linked map conversion.
	fputs("\ndata struct conversions :: test linked map conversions.\n", HARBOL_debug_output);
	{
		struct HarbolLinkMap *p = NULL;
		
		p = HarbolLinkMap_NewFromHarbolUniList(&unilist);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t n=0 ; n<p->Order.Count ; n++ ) {
				struct HarbolKeyValPair *l = p->Order.Table[n].Ptr;
				fprintf(HARBOL_debug_output, "unilist value : %" PRIi64 "\n", l->Data.Int64);
			}
			HarbolLinkMap_Free(&p, NULL);
		}
		
		p = HarbolLinkMap_NewFromHarbolHashmap(&map);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t n=0 ; n<p->Order.Count ; n++ ) {
				struct HarbolKeyValPair *l = p->Order.Table[n].Ptr;
				fprintf(HARBOL_debug_output, "map value : %" PRIi64 "\n", l->Data.Int64);
			}
			HarbolLinkMap_Free(&p, NULL);
		}
		
		p = HarbolLinkMap_NewFromHarbolVector(&vec);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t n=0 ; n<p->Order.Count ; n++ ) {
				struct HarbolKeyValPair *l = p->Order.Table[n].Ptr;
				fprintf(HARBOL_debug_output, "vec value : %" PRIi64 "\n", l->Data.Int64);
			}
			HarbolLinkMap_Free(&p, NULL);
		}
		
		p = HarbolLinkMap_NewFromHarbolBiList(&bilist);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t n=0 ; n<p->Order.Count ; n++ ) {
				struct HarbolKeyValPair *l = p->Order.Table[n].Ptr;
				fprintf(HARBOL_debug_output, "bilist value : %" PRIi64 "\n", l->Data.Int64);
			}
			HarbolLinkMap_Free(&p, NULL);
		}
		
		p = HarbolLinkMap_NewFromHarbolTuple(&tuple);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t n=0 ; n<p->Order.Count ; n++ ) {
				struct HarbolKeyValPair *l = p->Order.Table[n].Ptr;
				fprintf(HARBOL_debug_output, "tuple value : %" PRIi64 "\n", l->Data.Int64);
			}
			HarbolLinkMap_Free(&p, NULL);
		}
		
		p = HarbolLinkMap_NewFromHarbolGraph(&graph);
		if( p ) {
			fputs("ptr is valid\n", HARBOL_debug_output);
			for( size_t n=0 ; n<p->Order.Count ; n++ ) {
				struct HarbolKeyValPair *l = p->Order.Table[n].Ptr;
				fprintf(HARBOL_debug_output, "graph value : %" PRIi64 "\n", l->Data.Int64);
			}
			HarbolLinkMap_Free(&p, NULL);
		}
	}
	
	HarbolVector_Del(&vec, NULL);
	HarbolMap_Del(&map, NULL);
	HarbolUniList_Del(&unilist, NULL);
	HarbolBiList_Del(&bilist, NULL);
	HarbolTuple_Del(&tuple);
	HarbolGraph_Del(&graph, NULL, NULL);
	HarbolLinkMap_Del(&linkmap, NULL);
}

void TestCfg(void)
{
	if( !HARBOL_debug_output )
		return;
	
	// Test allocation and initializations
	fputs("cfg :: test allocation/initialization.\n", HARBOL_debug_output);
	struct HarbolLinkMap *cfg = HarbolCfg_Parse("'section': { 'lel': null }");
	fprintf(HARBOL_debug_output, "cfg ptr valid?: '%s'\n", cfg ? "yes" : "no");
	
	if( cfg ) {
		fputs("\ncfg :: iterating entire config.\n", HARBOL_debug_output);
		struct HarbolString stringcfg = {NULL, 0};
		HarbolCfg_ToString(cfg, &stringcfg);
		fprintf(HARBOL_debug_output, "\ncfg :: testing config to string conversion: \n%s\n", stringcfg.CStr);
		HarbolString_Del(&stringcfg);
	}
	
	// Test realistic config file string
	fputs("cfg :: test realistic config file string.\n", HARBOL_debug_output);
	const char *restrict test_cfg = "'root' { \
		'firstName': 'John', \
		'lastName': 'Smith', \
		'isAlive': true, \
		'age': 0x18 , \
		'money': 35.42e4 \
		'address': { \
			'streetAddress': '21 2nd Street', \
			'city': 'New York', \
			'state': 'NY', \
			'postalCode': '10021-3100' \
		}, \
		'phoneNumbers': { \
			'1' { \
				'type': 'home', \
				'number': '212 555-1234' \
			}, \
			'2' { \
				'type': 'office', \
				'number': '646 555-4567' \
			}, \
			'3' { \
				'type': 'mobile', \
				'number': '123 456-7890' \
			} \
		}, \
		'children': {}, \
		'spouse': null \
	}";
	
	struct HarbolLinkMap *larger_cfg = HarbolCfg_Parse(test_cfg);
	if( larger_cfg ) {
		fputs("\ncfg :: iterating realistic config.\n", HARBOL_debug_output);
		struct HarbolString stringcfg = {NULL, 0};
		HarbolCfg_ToString(larger_cfg, &stringcfg);
		fprintf(HARBOL_debug_output, "\ncfg :: test config to string conversion:\n%s\n", stringcfg.CStr);
		HarbolString_Del(&stringcfg);
		
		fputs("cfg :: test retrieving sub section of realistic config.\n", HARBOL_debug_output);
		struct HarbolLinkMap *phone_numbers1 = HarbolCfg_GetSectionByKey(larger_cfg, "root.phoneNumbers.1");
		HarbolCfg_ToString(phone_numbers1, &stringcfg);
		fprintf(HARBOL_debug_output, "\nphone_numbers to string conversion: \n%s\n", stringcfg.CStr);
		HarbolString_Del(&stringcfg);
		if( phone_numbers1 ) {
			fputs("\ncfg :: iterating phone_numbers1 subsection.\n", HARBOL_debug_output);
			struct HarbolString stringcfg = {NULL, 0};
			HarbolCfg_ToString(phone_numbers1, &stringcfg);
			fprintf(HARBOL_debug_output, "\nphone_numbers1 to string conversion: \n%s\n", stringcfg.CStr);
			HarbolString_Del(&stringcfg);
		}
		
		fputs("cfg :: test retrieving string value from subsection of realistic config.\n", HARBOL_debug_output);
		struct HarbolString *type = HarbolCfg_GetStrByKey(larger_cfg, "root.phoneNumbers.1.type");
		fprintf(HARBOL_debug_output, "root.phoneNumbers.1.type type valid?: '%s'\n", type ? "yes" : "no");
		if( type ) {
			fprintf(HARBOL_debug_output, "root.phoneNumbers.1.type: %s\n", type->CStr);
		}
		type = HarbolCfg_GetStrByKey(larger_cfg, "root.age");
		fprintf(HARBOL_debug_output, "root.age type valid?: '%s'\n", type ? "yes" : "no");
		if( type ) {
			fprintf(HARBOL_debug_output, "root.age: %s\n", type->CStr);
		}
		const int64_t age = HarbolCfg_GetIntByKey(larger_cfg, "root.age");
		fprintf(HARBOL_debug_output, "root.age int?: '%" PRIi64 "'\n", age);
		const double money = HarbolCfg_GetFloatByKey(larger_cfg, "root.money");
		fprintf(HARBOL_debug_output, "root.money float?: '%f'\n", money);
		HarbolCfg_Free(&larger_cfg);
	}
	
	fputs("cfg :: test destruction.\n", HARBOL_debug_output);
	HarbolCfg_Free(&cfg);
	fprintf(HARBOL_debug_output, "cfg ptr valid?: '%s'\n", cfg ? "yes" : "no");
}

/*
#if OS_WINDOWS
	#include <direct.h>
#else
	#include <unistd.h>
#endif

void TestPlugins(void)
{
	if( !HARBOL_debug_output )
		return;
	
	struct PluginManager pm = (struct PluginManager){0};
	char currdir[FILENAME_MAX]; // FILENAME_MAX is defined in <stdio.h>
#if OS_WINDOWS
	if( GetCurrentDirectory(sizeof currdir, currdir) )
#else
	if( getcwd(currdir, sizeof currdir) )
#endif
		PluginManager_Init(&pm, currdir);
	
	bool r = PluginManager_LoadModule(&pm, "testplugin", 0, NULL);
	fprintf(HARBOL_debug_output, "module loaded testplugin.so?: %u\n", r);
	r = PluginManager_LoadModule(&pm, "testplugin2", 0, NULL);
	fprintf(HARBOL_debug_output, "module loaded testplugin2.so?: %u\n", r);
	//PluginManager_UnloadAllModules(&pm, 0, NULL);
	PluginManager_Del(&pm);
}
*/
