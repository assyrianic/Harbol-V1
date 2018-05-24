
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dsc.h"

void TestStringObject(void);
void TestVector(void);
void TestHashmap(void);
void TestUniList(void);
void TestBiList(void);
void TestByteBuffer(void);
void TestTuple(void);
void TestHeapPool(void);
void TestGraph(void);
void TestTree(void);
void TestDSConversions(void);

FILE *DSC_debug_output = NULL;

int main()
{
	DSC_debug_output = fopen("data_structure_debug_output.txt", "w+");
	if( !DSC_debug_output )
		return -1;
	/*
	TestStringObject();
	TestVector();
	TestHashmap();
	TestUniList();
	TestBiList();
	TestByteBuffer();
	TestTuple();
	TestHeapPool();
	TestGraph();
	*/
	TestTree();
	//TestDSConversions();
	fclose(DSC_debug_output), DSC_debug_output=NULL;
}

void TestStringObject(void)
{
	if( !DSC_debug_output )
		return;
	
	// Test allocation and initializations
	fputs("string :: test allocation/initialization.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	struct String *p = String_NewStr("test ptr with cstr!");
	assert( p );
	fputs(String_GetStr(p), DSC_debug_output);
	fputs("\n", DSC_debug_output);
	
	struct String i = (struct String){0};
	String_CopyStr(&i, "test stk with cstr!");
	fputs(String_GetStr(&i), DSC_debug_output);
	fputs("\n", DSC_debug_output);
	
	// test appending individual chars.
	fputs("string :: test appending individual chars.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	String_AddChar(p, '6');
	fputs(String_GetStr(p), DSC_debug_output);
	fputs("\n", DSC_debug_output);
	
	String_AddChar(&i, '6');
	fputs(String_GetStr(&i), DSC_debug_output);
	fputs("\n", DSC_debug_output);
	
	// test appending strings.
	fputs("string :: test appending C strings.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	String_AddStr(p, "\'new string!\'");
	fputs(String_GetStr(p), DSC_debug_output);
	fputs("\n", DSC_debug_output);
	
	String_AddStr(&i, "\'new string!\'");
	fputs(String_GetStr(&i), DSC_debug_output);
	fputs("\n", DSC_debug_output);
	
	// test appending string objects.
	fputs("string :: test appending string objects.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	String_CopyStr(p, "A");
	String_CopyStr(&i, "B");
	String_Add(p, &i);
	String_Add(&i, p);
	fputs(String_GetStr(p), DSC_debug_output);
	fputs("\n", DSC_debug_output);
	fputs(String_GetStr(&i), DSC_debug_output);
	fputs("\n", DSC_debug_output);
	
	// test copying string objects.
	fputs("string :: test copying string objects.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	String_CopyStr(p, "copied from ptr!");
	String_Copy(&i, p);
	fputs(String_GetStr(p), DSC_debug_output);
	fputs("\n", DSC_debug_output);
	fputs(String_GetStr(&i), DSC_debug_output);
	fputs("\n", DSC_debug_output);
	
	// free data
	fputs("string :: test destruction.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	String_Del(&i);
	fprintf(DSC_debug_output, "i's string is null? '%s'\n", i.CStr ? "no" : "yes");
	
	String_Del(p);
	fprintf(DSC_debug_output, "p's string is null? '%s'\n", p->CStr ? "no" : "yes");
	String_Free(&p);
	fprintf(DSC_debug_output, "p is null? '%s'\n", p ? "no" : "yes");
}

void TestVector(void)
{
	// Test allocation and initializations
	fputs("vector :: test allocation/initialization.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	struct Vector *p = Vector_New(NULL);
	assert( p );
	
	struct Vector i = (struct Vector){0};
	
	// test data inserting
	fputs("vector :: test insertion.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	Vector_Insert(p, (union Value){.Int64=100});
	Vector_Insert(&i, (union Value){.Int64=100});
	
	// test data retrieval
	fputs("vector :: test retrieval.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	fprintf(DSC_debug_output, "ptr[0] == %lli\n", Vector_Get(p, 0).Int64);
	fprintf(DSC_debug_output, "stk[0] == %lli\n", Vector_Get(&i, 0).Int64);
	
	// test data setting
	fputs("vector :: test setting data.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	Vector_Set(p, 0, (union Value){.Int64=10});
	Vector_Set(&i, 0, (union Value){.Int64=9});
	fprintf(DSC_debug_output, "ptr[0] == %lli\n", Vector_Get(p, 0).Int64);
	fprintf(DSC_debug_output, "stk[0] == %lli\n", Vector_Get(&i, 0).Int64);
	
	// append the vectors
	fputs("vector :: test vector appending.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	Vector_Add(p, &i);
	fprintf(DSC_debug_output, "ptr[1] == %lli\n", Vector_Get(p, 1).Int64);
	fprintf(DSC_debug_output, "stk[1] == %lli\n", Vector_Get(&i, 1).Int64);
	
	// test vector copying.
	fputs("vector :: test vector copying.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	Vector_Del(&i);
	Vector_Insert(&i, (union Value){.Int64=100});
	Vector_Insert(&i, (union Value){.Int64=101});
	Vector_Insert(&i, (union Value){.Int64=102});
	Vector_Copy(p, &i);
	fprintf(DSC_debug_output, "ptr[1] == %lli\n", Vector_Get(p, 1).Int64);
	fprintf(DSC_debug_output, "ptr[2] == %lli\n", Vector_Get(p, 2).Int64);
	fprintf(DSC_debug_output, "stk[1] == %lli\n", Vector_Get(&i, 1).Int64);
	fprintf(DSC_debug_output, "stk[2] == %lli\n", Vector_Get(&i, 2).Int64);
	
	// test vector deleting and truncating.
	fputs("vector :: test item deletion and vector truncation.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	Vector_Del(&i);
	Vector_Del(p);
	Vector_Insert(p, (union Value){.Int64=100});
	Vector_Insert(p, (union Value){.Int64=101});
	Vector_Insert(p, (union Value){.Int64=102});
	Vector_Insert(p, (union Value){.Int64=103});
	Vector_Insert(p, (union Value){.Int64=104});
	Vector_Insert(p, (union Value){.Int64=105});
	for( size_t i=0 ; i<p->Count ; i++ )
		fprintf(DSC_debug_output, "ptr[%zu] == %lli\n", i, Vector_Get(p, i).Int64);
	fputs("\n", DSC_debug_output);
	Vector_Delete(p, 0); // deletes 100
	Vector_Delete(p, 1); // deletes 102 since 101 because new 0 index
	Vector_Delete(p, 2); // deletes 104
	for( size_t i=0 ; i<p->Count ; i++ )
		fprintf(DSC_debug_output, "ptr[%zu] == %lli\n", i, Vector_Get(p, i).Int64);
	fprintf(DSC_debug_output, "\nptr[] len == %zu\n", Vector_Len(p));
	Vector_Truncate(p);
	fprintf(DSC_debug_output, "ptr[] len == %zu\n\n", Vector_Len(p));
	for( size_t i=0 ; i<p->Count ; i++ )
		fprintf(DSC_debug_output, "ptr[%zu] == %lli\n", i, Vector_Get(p, i).Int64);
	
	// free data
	fputs("vector :: test destruction.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	Vector_Del(&i);
	fprintf(DSC_debug_output, "i's table is null? '%s'\n", i.Table ? "no" : "yes");
	
	Vector_Del(p);
	fprintf(DSC_debug_output, "p's table is null? '%s'\n", p->Table ? "no" : "yes");
	Vector_Free(&p);
	fprintf(DSC_debug_output, "p is null? '%s'\n", p ? "no" : "yes");
}

void TestHashmap(void)
{
	// Test allocation and initializations
	fputs("hashmap :: test allocation / initialization.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	struct Hashmap *p = Map_New(NULL);
	assert( p );
	
	struct Hashmap i = (struct Hashmap){0};
	
	// test insertion
	fputs("hashmap :: test insertion.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	Map_Insert(p, "1", (union Value){.Int64=1});
	Map_Insert(p, "2", (union Value){.Int64=2});
	Map_Insert(&i, "1", (union Value){.Int64=1});
	Map_Insert(&i, "2", (union Value){.Int64=2});
	
	// test retrieval.
	fputs("hashmap :: test data retrieval.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	fprintf(DSC_debug_output, "ptr[\"1\"] == %lli\n", Map_Get(p, "1").Int64);
	fprintf(DSC_debug_output, "ptr[\"2\"] == %lli\n", Map_Get(p, "2").Int64);
	fprintf(DSC_debug_output, "stk[\"1\"] == %lli\n", Map_Get(&i, "1").Int64);
	fprintf(DSC_debug_output, "stk[\"2\"] == %lli\n", Map_Get(&i, "2").Int64);
	
	// test setting.
	fputs("hashmap :: test data setting.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	Map_Set(p, "2", (union Value){.Int64=20});
	Map_Set(&i, "2", (union Value){.Int64=200});
	fprintf(DSC_debug_output, "ptr[\"1\"] == %lli\n", Map_Get(p, "1").Int64);
	fprintf(DSC_debug_output, "ptr[\"2\"] == %lli\n", Map_Get(p, "2").Int64);
	fprintf(DSC_debug_output, "stk[\"1\"] == %lli\n", Map_Get(&i, "1").Int64);
	fprintf(DSC_debug_output, "stk[\"2\"] == %lli\n", Map_Get(&i, "2").Int64);
	
	// test deletion
	fputs("hashmap :: test item deletion.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	Map_Delete(p, "2");
	fprintf(DSC_debug_output, "ptr[\"2\"] == %lli\n", Map_Get(p, "2").Int64);
	
	// free data
	fputs("hashmap :: test destruction.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	Map_Del(&i);
	fprintf(DSC_debug_output, "i's buckets are null? '%s'\n", i.Table ? "no" : "yes");
	
	Map_Del(p);
	fprintf(DSC_debug_output, "p's buckets are null? '%s'\n", p->Table ? "no" : "yes");
	Map_Free(&p);
	fprintf(DSC_debug_output, "p is null? '%s'\n", p ? "no" : "yes");
}

void TestUniList(void)
{
	// Test allocation and initializations
	fputs("unilist :: test allocation / initialization.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	struct UniLinkedList *p = UniLinkedList_New(NULL);
	assert( p );
	
	struct UniLinkedList i = (struct UniLinkedList){0};
	
	// test insertion.
	// test insertion at tail.
	fputs("unilist :: test tail insertion.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	UniLinkedList_InsertValueAtTail(p, (union Value){.Int64=1});
	for( struct UniListNode *n=UniLinkedList_GetHead(p) ; n ; n = UniListNode_GetNextNode(n) )
		fprintf(DSC_debug_output, "unilist value : %lli\n", UniListNode_GetValue(n).Int64);
	
	UniLinkedList_InsertValueAtTail(p, (union Value){.Int64=2});
	UniLinkedList_InsertValueAtTail(p, (union Value){.Int64=3});
	for( struct UniListNode *n=UniLinkedList_GetHead(p) ; n ; n = UniListNode_GetNextNode(n) )
		fprintf(DSC_debug_output, "unilist value : %lli\n", UniListNode_GetValue(n).Int64);
	
	// test insertion at head.
	fputs("unilist :: test head insertion.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	UniLinkedList_InsertValueAtHead(p, (union Value){.Int64=0});
	UniLinkedList_InsertValueAtHead(p, (union Value){.Int64=-1});
	UniLinkedList_InsertValueAtHead(p, (union Value){.Int64=-2});
	for( struct UniListNode *n=UniLinkedList_GetHead(p) ; n ; n = UniListNode_GetNextNode(n) )
		fprintf(DSC_debug_output, "unilist value : %lli\n", UniListNode_GetValue(n).Int64);
	
	// test retrieval.
	fputs("unilist :: test node retrival by index.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	{
		struct UniListNode *n = UniLinkedList_GetNode(p, 1);
		fprintf(DSC_debug_output, "unilist value : %lli\n", UniListNode_GetValue(n).Int64);
		
		// test item destruction.
		fputs("unilist :: test node removal & destruction by reference.", DSC_debug_output);
		fputs("\n", DSC_debug_output);
		UniLinkedList_DelNodeByRef(p, &n);
		for( struct UniListNode *n=UniLinkedList_GetHead(p) ; n ; n = UniListNode_GetNextNode(n) )
			fprintf(DSC_debug_output, "unilist value : %lli\n", UniListNode_GetValue(n).Int64);
		
		fputs("unilist :: test node removal & destruction by index.", DSC_debug_output);
		fputs("\n", DSC_debug_output);
		UniLinkedList_DelNodeByIndex(p, 1);
		for( struct UniListNode *n=UniLinkedList_GetHead(p) ; n ; n = UniListNode_GetNextNode(n) )
			fprintf(DSC_debug_output, "unilist value : %lli\n", UniListNode_GetValue(n).Int64);
	}
	
	// test setting data by index
	fputs("unilist :: test setting data by index (index of 0 is head node).", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	UniLinkedList_SetValue(p, 0, (union Value){.Int64=222}); // 0 is head
	for( struct UniListNode *n=UniLinkedList_GetHead(p) ; n ; n = UniListNode_GetNextNode(n) )
		fprintf(DSC_debug_output, "unilist value : %lli\n", UniListNode_GetValue(n).Int64);
	
	// test deleting head node after freeing.
	fputs("unilist :: test deleting head node after freeing and new insertion.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	UniLinkedList_Del(p);
	UniLinkedList_InsertValueAtHead(p, (union Value){.Int64=100});
	for( struct UniListNode *n=UniLinkedList_GetHead(p) ; n ; n = UniListNode_GetNextNode(n) )
		fprintf(DSC_debug_output, "unilist value : %lli\n", UniListNode_GetValue(n).Int64);
	UniLinkedList_DelNodeByIndex(p, 0);
	fprintf(DSC_debug_output, "unilist head node ptr : %p, tail node ptr : %p\n", UniLinkedList_GetHead(p), UniLinkedList_GetTail(p));
	
	
	// test deleting items by index on a list size of 2.
	fputs("unilist :: test deleting items by index on a list size of 2.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	UniLinkedList_Del(p);
	UniLinkedList_InsertValueAtTail(p, (union Value){.Int64=100});
	UniLinkedList_InsertValueAtTail(p, (union Value){.Int64=101});
	for( struct UniListNode *n=UniLinkedList_GetHead(p) ; n ; n = UniListNode_GetNextNode(n) )
		fprintf(DSC_debug_output, "unilist value : %lli\n", UniListNode_GetValue(n).Int64);
	UniLinkedList_DelNodeByIndex(p, 0);
	for( struct UniListNode *n=UniLinkedList_GetHead(p) ; n ; n = UniListNode_GetNextNode(n) )
		fprintf(DSC_debug_output, "unilist value : %lli\n", UniListNode_GetValue(n).Int64);
	
	// test insertion in middle of list
	fputs("unilist :: test middle insertion.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	UniLinkedList_Del(p);
	UniLinkedList_InsertValueAtIndex(p, (union Value){.Int64=100}, 0); // have it insert at head by inserting at index 0.
	UniLinkedList_InsertValueAtIndex(p, (union Value){.Int64=101}, 10); // have it insert at tail by having the index exceed the size of list
	UniLinkedList_InsertValueAtIndex(p, (union Value){.Int64=102}, 1);
	UniLinkedList_InsertValueAtIndex(p, (union Value){.Int64=102}, 1);
	UniLinkedList_InsertValueAtIndex(p, (union Value){.Int64=102}, 1);
	UniLinkedList_InsertValueAtIndex(p, (union Value){.Int64=102}, 1);
	UniLinkedList_InsertValueAtIndex(p, (union Value){.Int64=103}, 1);
	for( struct UniListNode *n=UniLinkedList_GetHead(p) ; n ; n = UniListNode_GetNextNode(n) )
		fprintf(DSC_debug_output, "unilist value : %lli\n", UniListNode_GetValue(n).Int64);
	
	// test deleting the tail through an index.
	fputs("unilist :: test deleting tail by index.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	UniLinkedList_Del(p);
	UniLinkedList_InsertValueAtTail(p, (union Value){.Int64=100});
	UniLinkedList_InsertValueAtTail(p, (union Value){.Int64=101});
	UniLinkedList_InsertValueAtTail(p, (union Value){.Int64=102});
	for( struct UniListNode *n=UniLinkedList_GetHead(p) ; n ; n = UniListNode_GetNextNode(n) )
		fprintf(DSC_debug_output, "unilist value : %lli\n", UniListNode_GetValue(n).Int64);
	UniLinkedList_DelNodeByIndex(p, 2);
	for( struct UniListNode *n=UniLinkedList_GetHead(p) ; n ; n = UniListNode_GetNextNode(n) )
		fprintf(DSC_debug_output, "unilist value : %lli\n", UniListNode_GetValue(n).Int64);
	
	// test finding a node by value.
	
	// free data
	fputs("unilist :: test destruction.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	UniLinkedList_Del(&i);
	UniLinkedList_Del(&i);
	fprintf(DSC_debug_output, "i's Head is null? '%s'\n", i.Head ? "no" : "yes");
	
	UniLinkedList_Del(p);
	fprintf(DSC_debug_output, "p's Head is null? '%s', but is p null? '%s'\n", p->Head ? "no" : "yes", p ? "no" : "yes");
	UniLinkedList_Free(&p);
	fprintf(DSC_debug_output, "p is null? '%s'\n", p ? "no" : "yes");
}

void TestBiList(void)
{
	// Test allocation and initializations
	fputs("bilist :: test allocation / initialization.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	struct BiLinkedList *p = BiLinkedList_New(NULL);
	assert( p );
	
	struct BiLinkedList i = (struct BiLinkedList){0};
	
	// test insertion.
	// test insertion at tail.
	fputs("bilist :: test tail insertion.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	BiLinkedList_InsertValueAtTail(p, (union Value){.Int64=1});
	for( struct BiListNode *n=BiLinkedList_GetHead(p) ; n ; n = BiListNode_GetNextNode(n) )
		fprintf(DSC_debug_output, "bilist value : %lli\n", BiListNode_GetValue(n).Int64);
	
	BiLinkedList_InsertValueAtTail(p, (union Value){.Int64=2});
	BiLinkedList_InsertValueAtTail(p, (union Value){.Int64=3});
	for( struct BiListNode *n=BiLinkedList_GetHead(p) ; n ; n = BiListNode_GetNextNode(n) )
		fprintf(DSC_debug_output, "bilist value : %lli\n", BiListNode_GetValue(n).Int64);
	
	// test insertion at head.
	fputs("bilist :: test head insertion.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	BiLinkedList_InsertValueAtHead(p, (union Value){.Int64=0});
	BiLinkedList_InsertValueAtHead(p, (union Value){.Int64=-1});
	BiLinkedList_InsertValueAtHead(p, (union Value){.Int64=-2});
	for( struct BiListNode *n=BiLinkedList_GetHead(p) ; n ; n = BiListNode_GetNextNode(n) )
		fprintf(DSC_debug_output, "bilist value : %lli\n", BiListNode_GetValue(n).Int64);
	
	// test retrieval.
	fputs("bilist :: test node retrival by index.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	{
		struct BiListNode *n = BiLinkedList_GetNode(p, 1);
		fprintf(DSC_debug_output, "bilist value : %lli\n", BiListNode_GetValue(n).Int64);
		
		// test item destruction.
		fputs("bilist :: test node removal & destruction by reference.", DSC_debug_output);
		fputs("\n", DSC_debug_output);
		BiLinkedList_DelNodeByRef(p, &n);
		for( struct BiListNode *n=BiLinkedList_GetHead(p) ; n ; n = BiListNode_GetNextNode(n) )
			fprintf(DSC_debug_output, "bilist value : %lli\n", BiListNode_GetValue(n).Int64);
		
		fputs("bilist :: test node removal & destruction by index.", DSC_debug_output);
		fputs("\n", DSC_debug_output);
		BiLinkedList_DelNodeByIndex(p, 1);
		for( struct BiListNode *n=BiLinkedList_GetHead(p) ; n ; n = BiListNode_GetNextNode(n) )
			fprintf(DSC_debug_output, "bilist value : %lli\n", BiListNode_GetValue(n).Int64);
	}
	
	// test setting data by index
	fputs("bilist :: test setting data by index (index of 0 is head node).", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	BiLinkedList_SetValue(p, 0, (union Value){.Int64=222}); // 0 is head
	for( struct BiListNode *n=BiLinkedList_GetHead(p) ; n ; n = BiListNode_GetNextNode(n) )
		fprintf(DSC_debug_output, "bilist value : %lli\n", BiListNode_GetValue(n).Int64);
	
	// test deleting head node after freeing.
	fputs("bilist :: test deleting head node after freeing and new insertion.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	BiLinkedList_Del(p);
	BiLinkedList_InsertValueAtHead(p, (union Value){.Int64=100});
	for( struct BiListNode *n=BiLinkedList_GetHead(p) ; n ; n = BiListNode_GetNextNode(n) )
		fprintf(DSC_debug_output, "bilist value : %lli\n", BiListNode_GetValue(n).Int64);
	BiLinkedList_DelNodeByIndex(p, 0);
	fprintf(DSC_debug_output, "bilist head node ptr : %p, tail node ptr : %p\n", BiLinkedList_GetHead(p), BiLinkedList_GetTail(p));
	
	
	// test deleting items by index on a list size of 2.
	fputs("bilist :: test deleting items by index on a list size of 2.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	BiLinkedList_Del(p);
	BiLinkedList_InsertValueAtTail(p, (union Value){.Int64=100});
	BiLinkedList_InsertValueAtTail(p, (union Value){.Int64=101});
	for( struct BiListNode *n=BiLinkedList_GetHead(p) ; n ; n = BiListNode_GetNextNode(n) )
		fprintf(DSC_debug_output, "bilist value : %lli\n", BiListNode_GetValue(n).Int64);
	BiLinkedList_DelNodeByIndex(p, 0);
	for( struct BiListNode *n=BiLinkedList_GetHead(p) ; n ; n = BiListNode_GetNextNode(n) )
		fprintf(DSC_debug_output, "bilist value : %lli\n", BiListNode_GetValue(n).Int64);
	
	// test insertion in middle of list
	fputs("bilist :: test middle insertion.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	BiLinkedList_Del(p);
	BiLinkedList_InsertValueAtIndex(p, (union Value){.Int64=100}, 0); // have it insert at head by inserting at index 0.
	BiLinkedList_InsertValueAtIndex(p, (union Value){.Int64=101}, 10); // have it insert at tail by having the index exceed the size of list
	BiLinkedList_InsertValueAtIndex(p, (union Value){.Int64=102}, 1);
	BiLinkedList_InsertValueAtIndex(p, (union Value){.Int64=102}, 1);
	BiLinkedList_InsertValueAtIndex(p, (union Value){.Int64=102}, 1);
	BiLinkedList_InsertValueAtIndex(p, (union Value){.Int64=102}, 1);
	BiLinkedList_InsertValueAtIndex(p, (union Value){.Int64=103}, 1);
	for( struct BiListNode *n=BiLinkedList_GetHead(p) ; n ; n = BiListNode_GetNextNode(n) )
		fprintf(DSC_debug_output, "bilist value : %lli\n", BiListNode_GetValue(n).Int64);
	
	
	// free data
	fputs("bilist :: test destruction.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	BiLinkedList_Del(&i);
	BiLinkedList_Del(&i);
	fprintf(DSC_debug_output, "i's Head is null? '%s'\n", i.Head ? "no" : "yes");
	
	BiLinkedList_Del(p);
	fprintf(DSC_debug_output, "p's Head is null? '%s', but is p null? '%s'\n", p->Head ? "no" : "yes", p ? "no" : "yes");
	BiLinkedList_Free(&p);
	fprintf(DSC_debug_output, "p is null? '%s'\n", p ? "no" : "yes");
}

void TestByteBuffer(void)
{
	if( !DSC_debug_output )
		return;
	
	// Test allocation and initializations
	fputs("bytebuffer :: test allocation/initialization.\n", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	struct ByteBuffer *p = ByteBuffer_New();
	assert( p );
	
	struct ByteBuffer i = (struct ByteBuffer){0};
	
	// test adding a byte.
	fputs("bytebuffer :: test byte appending.\n", DSC_debug_output);
	ByteBuffer_InsertByte(p, 5);
	ByteBuffer_InsertByte(&i, 6);
	for( size_t n=0 ; n<ByteBuffer_Count(p) ; n++ )
		fprintf(DSC_debug_output, "bytebuffer value : %zu\n", ByteBuffer_GetBuffer(p)[n]);
	for( size_t n=0 ; n<ByteBuffer_Count(p) ; n++ )
		fprintf(DSC_debug_output, "bytebuffer value : %zu\n", ByteBuffer_GetBuffer(&i)[n]);
	
	// test integer appending
	ByteBuffer_Del(&i);
	ByteBuffer_Del(p);
	uint16_t ush = 50;
	ByteBuffer_InsertInt(p, ush, sizeof ush);
	ByteBuffer_InsertInt(&i, ush, sizeof ush);
	for( size_t n=0 ; n<ByteBuffer_Count(p) ; n++ )
		fprintf(DSC_debug_output, "bytebuffer value : %zu\n", ByteBuffer_GetBuffer(p)[n]);
	for( size_t n=0 ; n<ByteBuffer_Count(p) ; n++ )
		fprintf(DSC_debug_output, "bytebuffer value : %zu\n", ByteBuffer_GetBuffer(&i)[n]);
	
	ByteBuffer_Del(&i);
	ByteBuffer_Del(p);
	uint64_t ull = 0xabcdef;
	ByteBuffer_InsertInt(p, ull, sizeof ull);
	ByteBuffer_InsertInt(&i, ull, sizeof ull);
	for( size_t n=0 ; n<ByteBuffer_Count(p) ; n++ )
		fprintf(DSC_debug_output, "bytebuffer value : %zu\n", ByteBuffer_GetBuffer(p)[n]);
	for( size_t n=0 ; n<ByteBuffer_Count(p) ; n++ )
		fprintf(DSC_debug_output, "bytebuffer value : %zu\n", ByteBuffer_GetBuffer(&i)[n]);
	
	
	// free data
	fputs("bytebuffer :: test destruction.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	ByteBuffer_Del(&i);
	fprintf(DSC_debug_output, "i's buffer is null? '%s'\n", i.Buffer ? "no" : "yes");
	
	ByteBuffer_Del(p);
	fprintf(DSC_debug_output, "p's buffer is null? '%s'\n", p->Buffer ? "no" : "yes");
	ByteBuffer_Free(&p);
	fprintf(DSC_debug_output, "p is null? '%s'\n", p ? "no" : "yes");
}

void TestTuple(void)
{
	if( !DSC_debug_output )
		return;
	
	// Test allocation and initializations
	fputs("tuple :: test allocation/initialization.\n", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	
	union Value stuff[5] = {
		{.Int64 = 1},
		{.Int64 = 2},
		{.Int64 = 3},
		{.Int64 = 4},
		{.Int64 = 5},
	};
	struct Tuple *p = Tuple_New(5, stuff);
	assert( p );
	
	struct Tuple i;
	Tuple_Init(&i, 5, stuff);
	
	// test getting a single item and then iterating.
	fputs("tuple :: test single item retrieval and iterating.\n", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	
	union Value single_item = Tuple_GetItem(p, 0);
	fprintf(DSC_debug_output, "single item tuple value : %llu\n", single_item.Int64);
	
	for( size_t n=0 ; n<Tuple_Len(p) ; n++ )
		fprintf(DSC_debug_output, "tuple value : %llu\n", Tuple_GetItem(p, n).Int64);
	
	// free data
	fputs("tuple :: test destruction.", DSC_debug_output);
	fputs("\n", DSC_debug_output);
	Tuple_Del(&i);
	fprintf(DSC_debug_output, "i's item is null? '%s'\n", i.Items ? "no" : "yes");
	
	Tuple_Del(p);
	fprintf(DSC_debug_output, "p's item is null? '%s'\n", p->Items ? "no" : "yes");
	Tuple_Free(&p);
	fprintf(DSC_debug_output, "p is null? '%s'\n", p ? "no" : "yes");
}

void TestHeapPool(void)
{
	if( !DSC_debug_output )
		return;
	
	// Test allocation and initializations
	fputs("heap :: test allocation/initialization.\n", DSC_debug_output);
	
	struct Heap i = (struct Heap){0};
#ifdef DSC_NO_MALLOC
	Heap_Init(&i);
#else
	Heap_Init(&i, 1000);
#endif
	fprintf(DSC_debug_output, "remaining heap mem: '%zu'\n", Heap_Remaining(&i));
	
	// test giving memory
	fputs("heap :: test giving memory.\n", DSC_debug_output);
	int *p = Heap_Alloc(&i, sizeof *p);
	fprintf(DSC_debug_output, "p is null? '%s'\n", p ? "no" : "yes");
	if( p ) {
		*p = 500;
		fprintf(DSC_debug_output, "p's value: %i\n", *p);
	}
	
	float *f = Heap_Alloc(&i, sizeof *f);
	fprintf(DSC_debug_output, "f is null? '%s'\n", f ? "no" : "yes");
	if( f ) {
		*f = 500.5f;
		fprintf(DSC_debug_output, "f's value: %f\n", *f);
	}
	fprintf(DSC_debug_output, "remaining heap mem: '%zu'\n", Heap_Remaining(&i));
	
	// test releasing memory
	fputs("heap :: test releasing memory.\n", DSC_debug_output);
	Heap_Release(&i, f), f=NULL;
	Heap_Release(&i, p), p=NULL;
	
	// test re-giving memory
	fputs("heap :: test regiving memory.\n", DSC_debug_output);
	p = Heap_Alloc(&i, sizeof *p);
	fprintf(DSC_debug_output, "p is null? '%s'\n", p ? "no" : "yes");
	if( p ) {
		*p = 532;
		fprintf(DSC_debug_output, "p's value: %i\n", *p);
	}
	f = Heap_Alloc(&i, sizeof *f);
	fprintf(DSC_debug_output, "f is null? '%s'\n", f ? "no" : "yes");
	if( f ) {
		*f = 466.5f;
		fprintf(DSC_debug_output, "f's value: %f\n", *f);
	}
	fprintf(DSC_debug_output, "remaining heap mem: '%zu'\n", Heap_Remaining(&i));
	Heap_Release(&i, p), p=NULL; // release memory that's from different region.
	Heap_Release(&i, f), f=NULL;
	fprintf(DSC_debug_output, "remaining heap mem: '%zu'\n", Heap_Remaining(&i));
	
	// test giving array memory
	fputs("heap :: test giving array memory.\n", DSC_debug_output);
	const size_t arrsize = 100;
	p = Heap_Alloc(&i, sizeof *p * arrsize);
	fprintf(DSC_debug_output, "p is null? '%s'\n", p ? "no" : "yes");
	if( p ) {
		for( size_t i=0 ; i<arrsize ; i++ )
			p[i] = i+1;
		for( size_t i=0 ; i<arrsize ; i++ )
			fprintf(DSC_debug_output, "p[%zu] value: %i\n", i, p[i]);
	}
	fprintf(DSC_debug_output, "remaining heap mem: '%zu'\n", Heap_Remaining(&i));
	f = Heap_Alloc(&i, sizeof *f * arrsize);
	fprintf(DSC_debug_output, "f is null? '%s'\n", f ? "no" : "yes");
	if( f ) {
		for( size_t i=0 ; i<arrsize ; i++ )
			f[i] = i+1.15f;
		for( size_t i=0 ; i<arrsize ; i++ )
			fprintf(DSC_debug_output, "f[%zu] value: %f\n", i, f[i]);
	}
	Heap_Release(&i, p), p=NULL;
	Heap_Release(&i, f), f=NULL;
	fprintf(DSC_debug_output, "remaining heap mem: '%zu'\n", Heap_Remaining(&i));
	
	
	// test using heap to make a unilinked list!
	fputs("heap :: test using heap for unilinked list.\n", DSC_debug_output);
	struct UniLinkedList *list = Heap_Alloc(&i, sizeof *list);
	assert( list );
	
	struct UniListNode *node1 = Heap_Alloc(&i, sizeof *node1);
	assert( node1 );
	node1->Data = (union Value){.Int64 = 1};
	UniLinkedList_InsertNodeAtTail(list, node1);
	
	struct UniListNode *node2 = Heap_Alloc(&i, sizeof *node2);
	assert( node2 );
	node2->Data = (union Value){.Int64 = 2};
	UniLinkedList_InsertNodeAtTail(list, node2);
	
	struct UniListNode *node3 = Heap_Alloc(&i, sizeof *node3);
	assert( node3 );
	node3->Data = (union Value){.Int64 = 3};
	UniLinkedList_InsertNodeAtTail(list, node3);
	
	struct UniListNode *node4 = Heap_Alloc(&i, sizeof *node4);
	assert( node4 );
	node4->Data = (union Value){.Int64 = 4};
	UniLinkedList_InsertNodeAtTail(list, node4);
	
	struct UniListNode *node5 = Heap_Alloc(&i, sizeof *node5);
	assert( node5 );
	node5->Data = (union Value){.Int64 = 5};
	UniLinkedList_InsertNodeAtTail(list, node5);
	
	for( struct UniListNode *n=list->Head ; n ; n = n->Next )
		fprintf(DSC_debug_output, "unilist value : %lli\n", n->Data.Int64);
	
	Heap_Release(&i, node1), node1=NULL;
	Heap_Release(&i, node2), node2=NULL;
	Heap_Release(&i, node3), node3=NULL;
	Heap_Release(&i, node4), node4=NULL;
	Heap_Release(&i, node5), node5=NULL;
	Heap_Release(&i, list), list=NULL;
	
	// free data
	fputs("heap :: test destruction.\n", DSC_debug_output);
	Heap_Del(&i);
	fprintf(DSC_debug_output, "i's heap bottom is null? '%s'\n", i.HeapBottom ? "no" : "yes");
	fprintf(DSC_debug_output, "i's FreeList is null? '%s'\n", i.FreeList ? "no" : "yes");
}

void TestGraph(void)
{
	if( !DSC_debug_output )
		return;
	
	// Test allocation and initializations
	fputs("graph :: test allocation/initialization.\n", DSC_debug_output);
	
	struct Graph g = (struct Graph){0};
	Graph_Init(&g, NULL, NULL);
	struct Graph *p = Graph_New(NULL, NULL);
	if( p )
		fputs("graph :: allocation/initialization of p is GOOD.\n", DSC_debug_output);
	
	// Test adding VertexCount
	fputs("\ngraph :: test adding VertexCount.\n", DSC_debug_output);
	for( size_t c=0 ; c<5 ; c++ ) {
		Graph_InsertVertexByValue(&g, (union Value){.Int64=c+1});
		Graph_InsertVertexByValue(p, (union Value){.Int64=c+1});
	}
	for( size_t i=0 ; i<g.VertexCount ; i++ )
		fprintf(DSC_debug_output, "Vertex Data: '%lli'\n",g.Vertices[i].Data.Int64);
	
	// test linking two VertexCount
	fputs("\ngraph :: test linking VertexCount.\n", DSC_debug_output);
	fprintf(DSC_debug_output, "linking was success?: '%u'\n", Graph_InsertEdgeBtwnVerts(&g, 0, 1, (union Value){0}));
	fprintf(DSC_debug_output, "Edge count in graph: '%u'\n", Graph_GetEdgeCount(&g));
	
	for( size_t i=0 ; i<g.VertexCount ; i++ ) {
		fprintf(DSC_debug_output, "Vertex Data: '%lli'\n",g.Vertices[i].Data.Int64);
		for( struct GraphEdge *k=g.Vertices[i].EdgeHead ; k ; k=k->NextEdge ) {
			fprintf(DSC_debug_output, "Edge Data: '%lli'\n", k->Weight.Int64);
			if( k->VertexSocket )
				fprintf(DSC_debug_output, "Vertex Socket Data: '%lli'\n", k->VertexSocket->Data.Int64);
		}
	}
	
	// test unlinking the two previous VertexCount
	fputs("\ngraph :: test unlinking the two previous VertexCount.\n", DSC_debug_output);
	fprintf(DSC_debug_output, "unlinking was success?: '%u'\n", Graph_RemoveEdgeBtwnVerts(&g, 0, 1));
	fprintf(DSC_debug_output, "Edge count in graph: '%u'\n", Graph_GetEdgeCount(&g));
	
	// test linking ALL available VertexCount!
	fputs("\ngraph :: test linking all VertexCount in graph.\n", DSC_debug_output);
	// link 1 to 2
	Graph_InsertEdgeBtwnVerts(&g, 0, 1, (union Value){0});
	// link 2 to 3
	Graph_InsertEdgeBtwnVerts(&g, 1, 2, (union Value){0});
	// link 3 to 4
	Graph_InsertEdgeBtwnVerts(&g, 2, 3, (union Value){0});
	// link 4 to 5
	Graph_InsertEdgeBtwnVerts(&g, 3, 4, (union Value){0});
	// link 5 to 3
	Graph_InsertEdgeBtwnVerts(&g, 4, 2, (union Value){0});
	fprintf(DSC_debug_output, "Edge count in graph: '%u'\n", Graph_GetEdgeCount(&g));
	
	for( size_t i=0 ; i<g.VertexCount ; i++ ) {
		fprintf(DSC_debug_output, "Vertex Data: '%lli'\n", g.Vertices[i].Data.Int64);
		for( struct GraphEdge *k=g.Vertices[i].EdgeHead ; k ; k=k->NextEdge ) {
			fprintf(DSC_debug_output, "Edge Data: '%lli'\n", k->Weight.Int64);
			if( k->VertexSocket )
				fprintf(DSC_debug_output, "Vertex Socket Data: '%lli'\n", k->VertexSocket->Data.Int64);
		}
	}
	fprintf(DSC_debug_output, "\nRemoving 5th value success?: '%u'\n", Graph_RemoveVertexByIndex(&g, 4));
	for( size_t i=0 ; i<g.VertexCount ; i++ ) {
		fprintf(DSC_debug_output, "Vertex Data: '%lli'\n", g.Vertices[i].Data.Int64);
		for( struct GraphEdge *k=g.Vertices[i].EdgeHead ; k ; k=k->NextEdge ) {
			fprintf(DSC_debug_output, "Edge Data: '%lli'\n", k->Weight.Int64);
			if( k->VertexSocket )
				fprintf(DSC_debug_output, "Vertex Socket Data: '%lli'\n", k->VertexSocket->Data.Int64);
		}
	}
	
	// test changing vertex data through an index!
	fprintf(DSC_debug_output, "\nTest changing vertex data by index\n");
	Graph_SetVertexDataByIndex(&g, 0, (union Value){.Int64 = 100});
	for( size_t i=0 ; i<g.VertexCount ; i++ ) {
		fprintf(DSC_debug_output, "Vertex Data: '%lli'\n", g.Vertices[i].Data.Int64);
		for( struct GraphEdge *k=g.Vertices[i].EdgeHead ; k ; k=k->NextEdge ) {
			fprintf(DSC_debug_output, "Edge Data: '%lli'\n", k->Weight.Int64);
			if( k->VertexSocket )
				fprintf(DSC_debug_output, "Vertex Socket Data: '%lli'\n", k->VertexSocket->Data.Int64);
		}
	}
	
	// test getting edge pointer
	fputs("\ngraph :: test getting edge pointer.\n", DSC_debug_output);
	struct GraphEdge *edge = Graph_GetEdgeBtwnVertices(&g, 0, 1);
	if( edge ) {
		fputs("edge ptr is VALID.\n", DSC_debug_output);
		if( edge->VertexSocket )
			fprintf(DSC_debug_output, "edge Vertex Socket Data: '%lli'\n", edge->VertexSocket->Data.Int64);
	}
	
	// test adjacency function
	fprintf(DSC_debug_output, "\nindex 0 is adjacent to index 1?: '%u'\n", Graph_IsVertexAdjacent(&g, 0, 1));
	
	// free data
	fputs("\ngraph :: test destruction.\n", DSC_debug_output);
	Graph_Del(&g);
	fprintf(DSC_debug_output, "i's VertexCount vector is null? '%s'\n", g.Vertices ? "no" : "yes");
	
	Graph_Del(p);
	fprintf(DSC_debug_output, "p's VertexCount vector is null? '%s'\n", p->Vertices ? "no" : "yes");
	Graph_Free(&p);
	fprintf(DSC_debug_output, "p is null? '%s'\n", p ? "no" : "yes");
}

void TestTree(void)
{
	if( !DSC_debug_output )
		return;
	
	// Test allocation and initializations
	fputs("tree :: test allocation/initialization.\n", DSC_debug_output);
	struct TreeNode i = (struct TreeNode){0};
	TreeNode_Init(&i);
	struct TreeNode *p = TreeNode_New((union Value){0});
	if( p )
		fputs("graph :: allocation/initialization of p is GOOD.\n", DSC_debug_output);
	
	
	// Test insertions
	fputs("\ntree :: test insertions.\n", DSC_debug_output);
	fprintf(DSC_debug_output, "insertion by node success?: '%u'\n", TreeNode_InsertChildByNode(p, TreeNode_New((union Value){.Int64=20})));
	fprintf(DSC_debug_output, "insertion by value success?: '%u'\n", TreeNode_InsertChildByValue(p, (union Value){.Int64=10}));
	TreeNode_Del(p, NULL);
	
	// Test deletions
	fputs("\ntree :: test deletions by first adding 5 children.\n", DSC_debug_output);
	TreeNode_InsertChildByValue(p, (union Value){.Int64=1});
	TreeNode_InsertChildByValue(p, (union Value){.Int64=2});
	TreeNode_InsertChildByValue(p, (union Value){.Int64=3});
	TreeNode_InsertChildByValue(p, (union Value){.Int64=4});
	TreeNode_InsertChildByValue(p, (union Value){.Int64=5});
	for( size_t n=0 ; n<p->ChildCount ; n++ )
		fprintf(DSC_debug_output, "child #%zu value: '%lli'\n", n, p->Children[n]->Data.Int64);
	
	fputs("\ndeleting index 1\n", DSC_debug_output);
	TreeNode_RemoveChildByIndex(p, 1, NULL);
	for( size_t n=0 ; n<p->ChildCount ; n++ )
		fprintf(DSC_debug_output, "child #%zu value: '%lli'\n", n, p->Children[n]->Data.Int64);
	
	// Test delete by node reference
	fputs("\ntree :: test deletion by node reference.\n", DSC_debug_output);
	// delete first child!
	fputs("\ndeleting index 0\n", DSC_debug_output);
	TreeNode_RemoveChildByRef(p, p->Children, NULL);
	for( size_t n=0 ; n<p->ChildCount ; n++ )
		fprintf(DSC_debug_output, "child #%zu value: '%lli'\n", n, p->Children[n]->Data.Int64);
	
	// Test creating something of an abstract syntax tree.
	fputs("\ntree :: test creating something of an abstract syntax tree.\n", DSC_debug_output);
	TreeNode_Del(p, NULL);
	TreeNode_SetData(p, (union Value){.StrObjPtr=String_NewStr("program")});
	TreeNode_InsertChildByValue(p, (union Value){.StrObjPtr=String_NewStr("stmt")});
	TreeNode_InsertChildByValue(p->Children[0], (union Value){.StrObjPtr=String_NewStr("if")});
	TreeNode_InsertChildByValue(p->Children[0], (union Value){.StrObjPtr=String_NewStr("cond")});
	TreeNode_InsertChildByValue(p->Children[0], (union Value){.StrObjPtr=String_NewStr("stmt")});
	TreeNode_InsertChildByValue(p->Children[0], (union Value){.StrObjPtr=String_NewStr("else")});
	fprintf(DSC_debug_output, "p's data: '%s'\n", p->Data.StrObjPtr->CStr);
	fprintf(DSC_debug_output, "p's child data: '%s'\n", p->Children[0]->Data.StrObjPtr->CStr);
	for( size_t n=0 ; n<p->Children[0]->ChildCount ; n++ )
		fprintf(DSC_debug_output, "p's child's children data: '%s'\n", p->Children[0]->Children[n]->Data.StrObjPtr->CStr);
	
	fputs("\nfreeing string data.\n", DSC_debug_output);
	TreeNode_Del(p, String_Free);
	
	// free data
	fputs("\ntree :: test destruction.\n", DSC_debug_output);
	TreeNode_Del(&i, NULL);
	fprintf(DSC_debug_output, "i's Children vector is null? '%s'\n", i.Children ? "no" : "yes");
	
	TreeNode_Del(p, NULL);
	fprintf(DSC_debug_output, "p's Children vector is null? '%s'\n", p->Children ? "no" : "yes");
	TreeNode_Free(&p, NULL);
	fprintf(DSC_debug_output, "p is null? '%s'\n", p ? "no" : "yes");
}

void TestDSConversions(void)
{
	// inserting five items to all 4 of these collector type data structures.
	struct Vector vec = (struct Vector){0};
	Vector_Insert(&vec, (union Value){.Int64 = 1});
	Vector_Insert(&vec, (union Value){.Int64 = 2});
	Vector_Insert(&vec, (union Value){.Int64 = 3});
	Vector_Insert(&vec, (union Value){.Int64 = 4});
	Vector_Insert(&vec, (union Value){.Int64 = 5});
	
	struct Hashmap map = (struct Hashmap){0};
	Map_Insert(&map, "1", (union Value){.Int64 = 1});
	Map_Insert(&map, "2", (union Value){.Int64 = 2});
	Map_Insert(&map, "3", (union Value){.Int64 = 3});
	Map_Insert(&map, "4", (union Value){.Int64 = 4});
	Map_Insert(&map, "5", (union Value){.Int64 = 5});
	
	struct UniLinkedList unilist = (struct UniLinkedList){0};
	UniLinkedList_InsertValueAtTail(&unilist, (union Value){.Int64 = 1});
	UniLinkedList_InsertValueAtTail(&unilist, (union Value){.Int64 = 2});
	UniLinkedList_InsertValueAtTail(&unilist, (union Value){.Int64 = 3});
	UniLinkedList_InsertValueAtTail(&unilist, (union Value){.Int64 = 4});
	UniLinkedList_InsertValueAtTail(&unilist, (union Value){.Int64 = 5});
	
	struct BiLinkedList bilist = (struct BiLinkedList){0};
	BiLinkedList_InsertValueAtTail(&bilist, (union Value){.Int64 = 1});
	BiLinkedList_InsertValueAtTail(&bilist, (union Value){.Int64 = 2});
	BiLinkedList_InsertValueAtTail(&bilist, (union Value){.Int64 = 3});
	BiLinkedList_InsertValueAtTail(&bilist, (union Value){.Int64 = 4});
	BiLinkedList_InsertValueAtTail(&bilist, (union Value){.Int64 = 5});
	
	struct Tuple tuple = (struct Tuple){0};
	union Value stuff[5] = {
		{.Int64 = 1},
		{.Int64 = 2},
		{.Int64 = 3},
		{.Int64 = 4},
		{.Int64 = 5},
	};
	Tuple_Init(&tuple, 5, stuff);
	
	struct Graph graph = (struct Graph){0};
	Graph_InsertVertexByValue(&graph, (union Value){.Int64 = 1});
	Graph_InsertVertexByValue(&graph, (union Value){.Int64 = 2});
	Graph_InsertVertexByValue(&graph, (union Value){.Int64 = 3});
	Graph_InsertVertexByValue(&graph, (union Value){.Int64 = 4});
	Graph_InsertVertexByValue(&graph, (union Value){.Int64 = 5});
	
	
	// test vector conversion.
	fputs("\ndata struct conversions :: test vector conversions.\n", DSC_debug_output);
	{
		struct Vector *p = NULL;
		
		p = Vector_NewFromUniLinkedList(&unilist);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( size_t i=0 ; i<p->Count ; i++ )
				fprintf(DSC_debug_output, "unilist -> ptr[%zu] == %lli\n", i, p->Table[i].Int64);
			Vector_Free(&p);
		}
		
		p = Vector_NewFromBiLinkedList(&bilist);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( size_t i=0 ; i<p->Count ; i++ )
				fprintf(DSC_debug_output, "bilist -> ptr[%zu] == %lli\n", i, p->Table[i].Int64);
			Vector_Free(&p);
		}
		
		p = Vector_NewFromMap(&map);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( size_t i=0 ; i<p->Count ; i++ )
				fprintf(DSC_debug_output, "map -> ptr[%zu] == %lli\n", i, p->Table[i].Int64);
			Vector_Free(&p);
		}
		
		p = Vector_NewFromTuple(&tuple);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( size_t i=0 ; i<p->Count ; i++ )
				fprintf(DSC_debug_output, "tuple -> ptr[%zu] == %lli\n", i, p->Table[i].Int64);
			Vector_Free(&p);
		}
		
		p = Vector_NewFromGraph(&graph);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( size_t i=0 ; i<p->Count ; i++ )
				fprintf(DSC_debug_output, "graph -> ptr[%zu] == %lli\n", i, p->Table[i].Int64);
			Vector_Free(&p);
		}
	}
	
	// test map conversion.
	fputs("\ndata struct conversions :: test map conversions.\n", DSC_debug_output);
	{
		struct Hashmap *p = NULL;
		
		p = Map_NewFromUniLinkedList(&unilist);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( size_t i=0 ; i<p->Len ; i++ )
				for( struct KeyNode *n = p->Table[i] ; n ; n=n->Next )
					fprintf(DSC_debug_output, "unilist -> ptr[\"%s\"] == %lli\n", n->KeyName.CStr, n->Data.Int64);
			Map_Free(&p);
		}
		
		p = Map_NewFromBiLinkedList(&bilist);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( size_t i=0 ; i<p->Len ; i++ )
				for( struct KeyNode *n = p->Table[i] ; n ; n=n->Next )
					fprintf(DSC_debug_output, "bilist -> ptr[\"%s\"] == %lli\n", n->KeyName.CStr, n->Data.Int64);
			Map_Free(&p);
		}
		
		p = Map_NewFromVector(&vec);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( size_t i=0 ; i<p->Len ; i++ )
				for( struct KeyNode *n = p->Table[i] ; n ; n=n->Next )
					fprintf(DSC_debug_output, "vec -> ptr[\"%s\"] == %lli\n", n->KeyName.CStr, n->Data.Int64);
			Map_Free(&p);
		}
		
		p = Map_NewFromTuple(&tuple);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( size_t i=0 ; i<p->Len ; i++ )
				for( struct KeyNode *n = p->Table[i] ; n ; n=n->Next )
					fprintf(DSC_debug_output, "tuple -> ptr[\"%s\"] == %lli\n", n->KeyName.CStr, n->Data.Int64);
			Map_Free(&p);
		}
		
		p = Map_NewFromGraph(&graph);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( size_t i=0 ; i<p->Len ; i++ )
				for( struct KeyNode *n = p->Table[i] ; n ; n=n->Next )
					fprintf(DSC_debug_output, "graph -> ptr[\"%s\"] == %lli\n", n->KeyName.CStr, n->Data.Int64);
			Map_Free(&p);
		}
	}
	
	// test uni linked list conversion.
	fputs("\ndata struct conversions :: test singly linked list conversions.\n", DSC_debug_output);
	{
		struct UniLinkedList *p = NULL;
		
		p = UniLinkedList_NewFromBiLinkedList(&bilist);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( struct UniListNode *n=p->Head ; n ; n = n->Next )
				fprintf(DSC_debug_output, "bilist value : %lli\n", n->Data.Int64);
			UniLinkedList_Free(&p);
		}
		
		p = UniLinkedList_NewFromMap(&map);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( struct UniListNode *n=p->Head ; n ; n = n->Next )
				fprintf(DSC_debug_output, "map value : %lli\n", n->Data.Int64);
			UniLinkedList_Free(&p);
		}
		
		p = UniLinkedList_NewFromVector(&vec);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( struct UniListNode *n=p->Head ; n ; n = n->Next )
				fprintf(DSC_debug_output, "vec value : %lli\n", n->Data.Int64);
			UniLinkedList_Free(&p);
		}
		
		p = UniLinkedList_NewFromTuple(&tuple);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( struct UniListNode *n=p->Head ; n ; n = n->Next )
				fprintf(DSC_debug_output, "tuple value : %lli\n", n->Data.Int64);
			UniLinkedList_Free(&p);
		}
		
		p = UniLinkedList_NewFromGraph(&graph);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( struct UniListNode *n=p->Head ; n ; n = n->Next )
				fprintf(DSC_debug_output, "graph value : %lli\n", n->Data.Int64);
			UniLinkedList_Free(&p);
		}
	}
	
	// test bi linked list conversion.
	fputs("\ndata struct conversions :: test doubly linked list conversions.\n", DSC_debug_output);
	{
		struct BiLinkedList *p = NULL;
		
		p = BiLinkedList_NewFromUniLinkedList(&unilist);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( struct BiListNode *n=p->Head ; n ; n = n->Next )
				fprintf(DSC_debug_output, "unilist value : %lli\n", n->Data.Int64);
			BiLinkedList_Free(&p);
		}
		
		p = BiLinkedList_NewFromMap(&map);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( struct BiListNode *n=p->Head ; n ; n = n->Next )
				fprintf(DSC_debug_output, "map value : %lli\n", n->Data.Int64);
			BiLinkedList_Free(&p);
		}
		
		p = BiLinkedList_NewFromVector(&vec);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( struct BiListNode *n=p->Head ; n ; n = n->Next )
				fprintf(DSC_debug_output, "vec value : %lli\n", n->Data.Int64);
			BiLinkedList_Free(&p);
		}
		
		p = BiLinkedList_NewFromTuple(&tuple);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( struct BiListNode *n=p->Head ; n ; n = n->Next )
				fprintf(DSC_debug_output, "tuple value : %lli\n", n->Data.Int64);
			BiLinkedList_Free(&p);
		}
		
		p = BiLinkedList_NewFromGraph(&graph);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( struct BiListNode *n=p->Head ; n ; n = n->Next )
				fprintf(DSC_debug_output, "graph value : %lli\n", n->Data.Int64);
			BiLinkedList_Free(&p);
		}
	}
	
	// test tuple conversion.
	fputs("\ndata struct conversions :: test tuple conversions.\n", DSC_debug_output);
	{
		struct Tuple *p = NULL;
		
		p = Tuple_NewFromUniLinkedList(&unilist);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( size_t i=0 ; i<p->Len ; i++ )
				fprintf(DSC_debug_output, "unilist value : %lli\n", p->Items[i].Int64);
			Tuple_Free(&p);
		}
		
		p = Tuple_NewFromMap(&map);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( size_t i=0 ; i<p->Len ; i++ )
				fprintf(DSC_debug_output, "map value : %lli\n", p->Items[i].Int64);
			Tuple_Free(&p);
		}
		
		p = Tuple_NewFromVector(&vec);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( size_t i=0 ; i<p->Len ; i++ )
				fprintf(DSC_debug_output, "vec value : %lli\n", p->Items[i].Int64);
			Tuple_Free(&p);
		}
		
		p = Tuple_NewFromBiLinkedList(&bilist);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( size_t i=0 ; i<p->Len ; i++ )
				fprintf(DSC_debug_output, "bilist value : %lli\n", p->Items[i].Int64);
			Tuple_Free(&p);
		}
		
		p = Tuple_NewFromGraph(&graph);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( size_t i=0 ; i<p->Len ; i++ )
				fprintf(DSC_debug_output, "graph value : %lli\n", p->Items[i].Int64);
			Tuple_Free(&p);
		}
	}
	
	// test graph conversion.
	fputs("\ndata struct conversions :: test graph conversions.\n", DSC_debug_output);
	{
		struct Graph *p = NULL;
		
		p = Graph_NewFromUniLinkedList(&unilist);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( size_t i=0 ; i<p->VertexCount ; i++ )
				fprintf(DSC_debug_output, "unilist value : %lli\n", p->Vertices[i].Data.Int64);
			Graph_Free(&p);
		}
		
		p = Graph_NewFromMap(&map);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( size_t i=0 ; i<p->VertexCount ; i++ )
				fprintf(DSC_debug_output, "map value : %lli\n", p->Vertices[i].Data.Int64);
			Graph_Free(&p);
		}
		
		p = Graph_NewFromVector(&vec);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( size_t i=0 ; i<p->VertexCount ; i++ )
				fprintf(DSC_debug_output, "vec value : %lli\n", p->Vertices[i].Data.Int64);
			Graph_Free(&p);
		}
		
		p = Graph_NewFromBiLinkedList(&bilist);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( size_t i=0 ; i<p->VertexCount ; i++ )
				fprintf(DSC_debug_output, "bilist value : %lli\n", p->Vertices[i].Data.Int64);
			Graph_Free(&p);
		}
		
		p = Graph_NewFromTuple(&tuple);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( size_t i=0 ; i<p->VertexCount ; i++ )
				fprintf(DSC_debug_output, "tuple value : %lli\n", p->Vertices[i].Data.Int64);
			Graph_Free(&p);
		}
	}
	
	Vector_Del(&vec);
	Map_Del(&map);
	UniLinkedList_Del(&unilist);
	BiLinkedList_Del(&bilist);
	Tuple_Del(&tuple);
	Graph_Del(&graph);
}
