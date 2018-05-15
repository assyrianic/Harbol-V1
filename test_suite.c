
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
void TestDSConversions(void);

FILE *DSC_debug_output = NULL;

int main()
{
	DSC_debug_output = fopen("data_structure_debug_output.txt", "w+");
	if( !DSC_debug_output )
		return -1;
	
	//TestStringObject();
	//TestVector();
	//TestHashmap();
	//TestUniList();
	TestBiList();
	//TestByteBuffer();
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
	
	Vector_Delete(p, 0);
	Vector_Delete(p, 1);
	Vector_Delete(p, 2);
	for( size_t i=0 ; i<p->Count ; i++ )
		fprintf(DSC_debug_output, "ptr[%zu] == %lli\n", i, Vector_Get(p, i).Int64);
	fprintf(DSC_debug_output, "ptr[] len == %zu\n", Vector_Len(p));
	Vector_Truncate(p);
	fprintf(DSC_debug_output, "ptr[] len == %zu\n", Vector_Len(p));
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
	
	
	// test vector conversion.
	fputs("data struct conversions :: test vector conversions.\n", DSC_debug_output);
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
	}
	
	// test map conversion.
	fputs("data struct conversions :: test map conversions.\n", DSC_debug_output);
	{
		struct Hashmap *p = NULL;
		
		p = Map_NewFromUniLinkedList(&unilist);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( size_t i=0 ; i<map.Len ; i++ )
				for( struct KeyNode *n = map.Table[i] ; n ; n=n->Next )
					fprintf(DSC_debug_output, "unilist -> ptr[\"%s\"] == %lli\n", n->KeyName.CStr, n->Data.Int64);
			Map_Free(&p);
		}
		
		p = Map_NewFromBiLinkedList(&bilist);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( size_t i=0 ; i<map.Len ; i++ )
				for( struct KeyNode *n = map.Table[i] ; n ; n=n->Next )
					fprintf(DSC_debug_output, "bilist -> ptr[\"%s\"] == %lli\n", n->KeyName.CStr, n->Data.Int64);
			Map_Free(&p);
		}
		
		p = Map_NewVector(&vec);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( size_t i=0 ; i<map.Len ; i++ )
				for( struct KeyNode *n = map.Table[i] ; n ; n=n->Next )
					fprintf(DSC_debug_output, "vec -> ptr[\"%s\"] == %lli\n", n->KeyName.CStr, n->Data.Int64);
			Map_Free(&p);
		}
	}
	
	// test uni linked list conversion.
	fputs("data struct conversions :: test singly linked list conversions.\n", DSC_debug_output);
	{
		struct UniLinkedList *p = NULL;
		
		p = UniLinkedList_NewFromBiLinkedList(&bilist);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( struct UniListNode *n=unilist.Head ; n ; n = n->Next )
				fprintf(DSC_debug_output, "bilist value : %lli\n", n->Data.Int64);
			UniLinkedList_Free(&p);
		}
		
		p = UniLinkedList_NewFromMap(&map);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( struct UniListNode *n=unilist.Head ; n ; n = n->Next )
				fprintf(DSC_debug_output, "map value : %lli\n", n->Data.Int64);
			UniLinkedList_Free(&p);
		}
		
		p = UniLinkedList_NewVector(&vec);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( struct UniListNode *n=unilist.Head ; n ; n = n->Next )
				fprintf(DSC_debug_output, "vec value : %lli\n", n->Data.Int64);
			UniLinkedList_Free(&p);
		}
	}
	
	// test bi linked list conversion.
	fputs("data struct conversions :: test doubly linked list conversions.\n", DSC_debug_output);
	{
		struct BiLinkedList *p = NULL;
		
		p = BiLinkedList_NewFromUniLinkedList(&unilist);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( struct BiListNode *n=bilist.Head ; n ; n = n->Next )
				fprintf(DSC_debug_output, "unilist value : %lli\n", n->Data.Int64);
			BiLinkedList_Free(&p);
		}
		
		p = BiLinkedList_NewFromMap(&map);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( struct BiListNode *n=bilist.Head ; n ; n = n->Next )
				fprintf(DSC_debug_output, "map value : %lli\n", n->Data.Int64);
			BiLinkedList_Free(&p);
		}
		
		p = BiLinkedList_NewVector(&vec);
		if( p ) {
			fputs("ptr is valid\n", DSC_debug_output);
			for( struct BiListNode *n=bilist.Head ; n ; n = n->Next )
				fprintf(DSC_debug_output, "vec value : %lli\n", n->Data.Int64);
			BiLinkedList_Free(&p);
		}
	}
	
	Vector_Del(&vec);
	Map_Del(&map);
	UniLinkedList_Del(&unilist);
	BiLinkedList_Del(&bilist);
	
	/*
	for( struct BiListNode *n=bilist.Head ; n ; n = n->Next )
		fprintf(DSC_debug_output, "bilist value : %lli\n", n->Data.Int64);
	*/
}
