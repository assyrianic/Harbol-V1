#include <assert.h>
#include <stdalign.h>
#include "harbol.h"

void test_harbol_string(void);
void test_harbol_vector(void);
void test_harbol_hashmap(void);
void test_harbol_unilist(void);
void test_harbol_bilist(void);
void test_harbol_bytebuffer(void);
void test_harbol_tuple(void);
void test_harbol_mempool(void);
void test_harbol_graph(void);
void test_harbol_tree(void);
void test_harbol_linkmap(void);
void test_conversions(void);
void test_harbol_cfg(void);
void test_harbol_plugins(void);
void test_harbol_multithreading(void);

FILE *g_harbol_debug_stream = NULL;


int main()
{
	g_harbol_debug_stream = fopen("data_structure_debug_output.txt", "wa+");
	if( !g_harbol_debug_stream )
		return -1;
	
	test_harbol_string();
	test_harbol_vector();
	test_harbol_hashmap();
	test_harbol_unilist();
	test_harbol_bilist();
	test_harbol_bytebuffer();
	test_harbol_tuple();
	test_harbol_mempool();
	test_harbol_graph();
	test_harbol_tree();
	test_harbol_linkmap();
	test_conversions();
	test_harbol_cfg();
	test_harbol_plugins();
	test_harbol_multithreading();
	
	fclose(g_harbol_debug_stream), g_harbol_debug_stream=NULL;
}

void test_harbol_string(void)
{
	if( !g_harbol_debug_stream )
		return;
	
	// Test allocation and initializations
	fputs("string :: test allocation/initialization.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	struct HarbolString *p = harbol_string_new_cstr("test ptr with cstr!");
	assert( p );
	fputs(harbol_string_get_cstr(p), g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	
	struct HarbolString i = (struct HarbolString){0};
	harbol_string_copy_cstr(&i, "test stk with cstr!");
	fputs(harbol_string_get_cstr(&i), g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	
	// test appending individual chars.
	fputs("string :: test appending individual chars.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_string_add_char(p, '6');
	fputs(harbol_string_get_cstr(p), g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	
	harbol_string_add_char(&i, '6');
	fputs(harbol_string_get_cstr(&i), g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	
	// test appending strings.
	fputs("string :: test appending C strings.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_string_add_cstr(p, "\'new string!\'");
	fputs(harbol_string_get_cstr(p), g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	
	harbol_string_add_cstr(&i, "\'new string!\'");
	fputs(harbol_string_get_cstr(&i), g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	
	// test appending string objects.
	fputs("\nstring :: test appending string objects.\n", g_harbol_debug_stream);
	harbol_string_copy_cstr(p, "A");
	harbol_string_copy_cstr(&i, "B");
	harbol_string_add_str(p, &i);
	harbol_string_add_str(&i, p);
	fputs(harbol_string_get_cstr(p), g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	fputs(harbol_string_get_cstr(&i), g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	
	// test copying string objects.
	fputs("\nstring :: test copying string objects.\n", g_harbol_debug_stream);
	harbol_string_copy_cstr(p, "copied from ptr!");
	harbol_string_add_str(&i, p);
	fputs(harbol_string_get_cstr(p), g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	fputs(harbol_string_get_cstr(&i), g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	
	// test string formatting.
	fputs("\nstring :: test string formatting.\n", g_harbol_debug_stream);
	harbol_string_del(&i);
	//harbol_string_reserve(&i, 100);
	harbol_string_format(&i, "%i + %f%% + %i", 900, 4242.2, 10);
	fputs(harbol_string_get_cstr(&i), g_harbol_debug_stream);
	fprintf(g_harbol_debug_stream, "\ni's string len '%zu' | strlen val '%zu'\n", i.Len, strlen(i.CStr));
	fputs("\n", g_harbol_debug_stream);
	
	// free data
	fputs("string :: test destruction.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_string_del(&i);
	fprintf(g_harbol_debug_stream, "i's string is null? '%s'\n", i.CStr ? "no" : "yes");
	
	harbol_string_del(p);
	fprintf(g_harbol_debug_stream, "p's string is null? '%s'\n", p->CStr ? "no" : "yes");
	harbol_string_free(&p);
	fprintf(g_harbol_debug_stream, "p is null? '%s'\n", p ? "no" : "yes");
}

void test_harbol_vector(void)
{
	// Test allocation and initializations
	fputs("vector :: test allocation/initialization.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	struct HarbolVector *p = harbol_vector_new();
	assert( p );
	
	struct HarbolVector i = (struct HarbolVector){0};
	
	// test data inserting
	fputs("vector :: test insertion.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_vector_insert(p, (union HarbolValue){.Int64=100});
	harbol_vector_insert(&i, (union HarbolValue){.Int64=100});
	
	// test data retrieval
	fputs("vector :: test retrieval.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	fprintf(g_harbol_debug_stream, "ptr[0] == %" PRIi64 "\n", harbol_vector_get(p, 0).Int64);
	fprintf(g_harbol_debug_stream, "stk[0] == %" PRIi64 "\n", harbol_vector_get(&i, 0).Int64);
	
	// test data setting
	fputs("vector :: test setting data.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_vector_set(p, 0, (union HarbolValue){.Int64=10});
	harbol_vector_set(&i, 0, (union HarbolValue){.Int64=9});
	fprintf(g_harbol_debug_stream, "ptr[0] == %" PRIi64 "\n", harbol_vector_get(p, 0).Int64);
	fprintf(g_harbol_debug_stream, "stk[0] == %" PRIi64 "\n", harbol_vector_get(&i, 0).Int64);
	
	// append the vectors
	fputs("vector :: test vector appending.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_vector_add(p, &i);
	fprintf(g_harbol_debug_stream, "ptr[1] == %" PRIi64 "\n", harbol_vector_get(p, 1).Int64);
	fprintf(g_harbol_debug_stream, "stk[1] == %" PRIi64 "\n", harbol_vector_get(&i, 1).Int64);
	
	// test vector copying.
	fputs("vector :: test vector copying.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_vector_del(&i, NULL);
	harbol_vector_insert(&i, (union HarbolValue){.Int64=100});
	harbol_vector_insert(&i, (union HarbolValue){.Int64=101});
	harbol_vector_insert(&i, (union HarbolValue){.Int64=102});
	harbol_vector_copy(p, &i);
	fprintf(g_harbol_debug_stream, "ptr[1] == %" PRIi64 "\n", harbol_vector_get(p, 1).Int64);
	fprintf(g_harbol_debug_stream, "ptr[2] == %" PRIi64 "\n", harbol_vector_get(p, 2).Int64);
	fprintf(g_harbol_debug_stream, "stk[1] == %" PRIi64 "\n", harbol_vector_get(&i, 1).Int64);
	fprintf(g_harbol_debug_stream, "stk[2] == %" PRIi64 "\n", harbol_vector_get(&i, 2).Int64);
	
	// test vector deleting and truncating.
	fputs("vector :: test item deletion and vector truncation.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_vector_del(&i, NULL);
	harbol_vector_del(p, NULL);
	harbol_vector_insert(p, (union HarbolValue){.Int64=100});
	harbol_vector_insert(p, (union HarbolValue){.Int64=101});
	harbol_vector_insert(p, (union HarbolValue){.Int64=102});
	harbol_vector_insert(p, (union HarbolValue){.Int64=103});
	harbol_vector_insert(p, (union HarbolValue){.Int64=104});
	harbol_vector_insert(p, (union HarbolValue){.Int64=105});
	for( size_t i=0; i<p->Count; i++ )
		fprintf(g_harbol_debug_stream, "ptr[%zu] == %" PRIi64 "\n", i, harbol_vector_get(p, i).Int64);
	fputs("\n", g_harbol_debug_stream);
	harbol_vector_delete(p, 0, NULL); // deletes 100
	harbol_vector_delete(p, 1, NULL); // deletes 102 since 101 because new 0 index
	harbol_vector_delete(p, 2, NULL); // deletes 104
	for( size_t i=0; i<p->Count; i++ )
		fprintf(g_harbol_debug_stream, "ptr[%zu] == %" PRIi64 "\n", i, harbol_vector_get(p, i).Int64);
	fprintf(g_harbol_debug_stream, "\nptr[] len == %zu\n", harbol_vector_get_len(p));
	harbol_vector_truncate(p);
	fprintf(g_harbol_debug_stream, "ptr[] len == %zu\n\n", harbol_vector_get_len(p));
	for( size_t i=0; i<p->Count; i++ )
		fprintf(g_harbol_debug_stream, "ptr[%zu] == %" PRIi64 "\n", i, harbol_vector_get(p, i).Int64);
	
	fputs("vector :: test vector popping.\n", g_harbol_debug_stream);
	for( size_t i=0; i<p->Count; i++ )
		fprintf(g_harbol_debug_stream, "prepop ptr[%zu] == %" PRIi64 "\n", i, harbol_vector_get(p, i).Int64);
	const union HarbolValue vec_item_2 = harbol_vector_pop(p);
	for( size_t i=0; i<p->Count; i++ )
		fprintf(g_harbol_debug_stream, "postpop ptr[%zu] == %" PRIi64 "\n", i, harbol_vector_get(p, i).Int64);
	
	fprintf(g_harbol_debug_stream, "popped val == %" PRIi64 "\n", vec_item_2.Int64);
	// free data
	fputs("vector :: test destruction.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_vector_del(&i, NULL);
	fprintf(g_harbol_debug_stream, "i's table is null? '%s'\n", i.Table ? "no" : "yes");
	
	harbol_vector_del(p, NULL);
	fprintf(g_harbol_debug_stream, "p's table is null? '%s'\n", p->Table ? "no" : "yes");
	harbol_vector_free(&p, NULL);
	fprintf(g_harbol_debug_stream, "p is null? '%s'\n", p ? "no" : "yes");
}

void test_harbol_hashmap(void)
{
	// Test allocation and initializations
	fputs("hashmap :: test allocation / initialization.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	struct HarbolHashMap *p = harbol_hashmap_new();
	assert( p );
	
	struct HarbolHashMap i = (struct HarbolHashMap){0};
	
	// test insertion
	fputs("hashmap :: test insertion.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_hashmap_insert(p, "1", (union HarbolValue){.Int64=1});
	harbol_hashmap_insert(p, "2", (union HarbolValue){.Int64=2});
	harbol_hashmap_insert(&i, "1", (union HarbolValue){.Int64=1});
	harbol_hashmap_insert(&i, "2", (union HarbolValue){.Int64=2});
	
	// test retrieval.
	fputs("hashmap :: test data retrieval.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	fprintf(g_harbol_debug_stream, "ptr[\"1\"] == %" PRIi64 "\n", harbol_hashmap_get(p, "1").Int64);
	fprintf(g_harbol_debug_stream, "ptr[\"2\"] == %" PRIi64 "\n", harbol_hashmap_get(p, "2").Int64);
	fprintf(g_harbol_debug_stream, "stk[\"1\"] == %" PRIi64 "\n", harbol_hashmap_get(&i, "1").Int64);
	fprintf(g_harbol_debug_stream, "stk[\"2\"] == %" PRIi64 "\n", harbol_hashmap_get(&i, "2").Int64);
	
	// test setting.
	fputs("hashmap :: test data setting.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_hashmap_set(p, "2", (union HarbolValue){.Int64=20});
	harbol_hashmap_set(&i, "2", (union HarbolValue){.Int64=200});
	fprintf(g_harbol_debug_stream, "ptr[\"1\"] == %" PRIi64 "\n", harbol_hashmap_get(p, "1").Int64);
	fprintf(g_harbol_debug_stream, "ptr[\"2\"] == %" PRIi64 "\n", harbol_hashmap_get(p, "2").Int64);
	fprintf(g_harbol_debug_stream, "stk[\"1\"] == %" PRIi64 "\n", harbol_hashmap_get(&i, "1").Int64);
	fprintf(g_harbol_debug_stream, "stk[\"2\"] == %" PRIi64 "\n", harbol_hashmap_get(&i, "2").Int64);
	
	// test deletion
	fputs("hashmap :: test item deletion.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_hashmap_delete(p, "2", NULL);
	fprintf(g_harbol_debug_stream, "ptr[\"2\"] == %" PRIi64 "\n", harbol_hashmap_get(p, "2").Int64);
	
	// free data
	fputs("hashmap :: test destruction.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_hashmap_del(&i, NULL);
	fprintf(g_harbol_debug_stream, "i's buckets are null? '%s'\n", i.Table ? "no" : "yes");
	
	harbol_hashmap_del(p, NULL);
	fprintf(g_harbol_debug_stream, "p's buckets are null? '%s'\n", p->Table ? "no" : "yes");
	harbol_hashmap_free(&p, NULL);
	fprintf(g_harbol_debug_stream, "p is null? '%s'\n", p ? "no" : "yes");
}

void test_harbol_unilist(void)
{
	// Test allocation and initializations
	fputs("unilist :: test allocation / initialization.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	struct HarbolUniList *p = harbol_unilist_new();
	assert( p );
	
	struct HarbolUniList i = (struct HarbolUniList){0};
	
	// test insertion.
	// test insertion at tail.
	fputs("unilist :: test tail insertion.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_unilist_insert_at_tail(p, (union HarbolValue){.Int64=1});
	for( struct HarbolUniListNode *n=harbol_unilist_get_head_node(p); n; n = harbol_unilistnode_get_next_node(n) )
		fprintf(g_harbol_debug_stream, "unilist value : %" PRIi64 "\n", harbol_unilistnode_get_val(n).Int64);
	
	harbol_unilist_insert_at_tail(p, (union HarbolValue){.Int64=2});
	harbol_unilist_insert_at_tail(p, (union HarbolValue){.Int64=3});
	for( struct HarbolUniListNode *n=harbol_unilist_get_head_node(p); n; n = harbol_unilistnode_get_next_node(n) )
		fprintf(g_harbol_debug_stream, "unilist value : %" PRIi64 "\n", harbol_unilistnode_get_val(n).Int64);
	
	// test insertion at head.
	fputs("unilist :: test head insertion.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_unilist_insert_at_head(p, (union HarbolValue){.Int64=0});
	harbol_unilist_insert_at_head(p, (union HarbolValue){.Int64=-1});
	harbol_unilist_insert_at_head(p, (union HarbolValue){.Int64=-2});
	for( struct HarbolUniListNode *n=harbol_unilist_get_head_node(p); n; n = harbol_unilistnode_get_next_node(n) )
		fprintf(g_harbol_debug_stream, "unilist value : %" PRIi64 "\n", harbol_unilistnode_get_val(n).Int64);
	
	// test retrieval.
	fputs("unilist :: test node retrival by index.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	{
		struct HarbolUniListNode *n = harbol_unilist_get_node_by_index(p, 1);
		fprintf(g_harbol_debug_stream, "unilist value : %" PRIi64 "\n", harbol_unilistnode_get_val(n).Int64);
		
		// test item destruction.
		fputs("unilist :: test node removal & destruction by reference.", g_harbol_debug_stream);
		fputs("\n", g_harbol_debug_stream);
		harbol_unilist_del_node_by_ref(p, &n, NULL);
		for( struct HarbolUniListNode *k=harbol_unilist_get_head_node(p); k; k = harbol_unilistnode_get_next_node(k) )
			fprintf(g_harbol_debug_stream, "unilist value : %" PRIi64 "\n", harbol_unilistnode_get_val(k).Int64);
		
		fputs("unilist :: test node removal & destruction by index.", g_harbol_debug_stream);
		fputs("\n", g_harbol_debug_stream);
		harbol_unilist_del_node_by_index(p, 1, NULL);
		for( struct HarbolUniListNode *k=harbol_unilist_get_head_node(p); k; k = harbol_unilistnode_get_next_node(k) )
			fprintf(g_harbol_debug_stream, "unilist value : %" PRIi64 "\n", harbol_unilistnode_get_val(k).Int64);
	}
	
	// test setting data by index
	fputs("unilist :: test setting data by index (index of 0 is head node).", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_unilist_set_val(p, 0, (union HarbolValue){.Int64=222}); // 0 is head
	for( struct HarbolUniListNode *n=harbol_unilist_get_head_node(p); n; n = harbol_unilistnode_get_next_node(n) )
		fprintf(g_harbol_debug_stream, "unilist value : %" PRIi64 "\n", harbol_unilistnode_get_val(n).Int64);
	
	// test deleting head node after freeing.
	fputs("unilist :: test deleting head node after freeing and new insertion.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_unilist_del(p, NULL);
	harbol_unilist_insert_at_head(p, (union HarbolValue){.Int64=100});
	for( struct HarbolUniListNode *n=harbol_unilist_get_head_node(p); n; n = harbol_unilistnode_get_next_node(n) )
		fprintf(g_harbol_debug_stream, "unilist value : %" PRIi64 "\n", harbol_unilistnode_get_val(n).Int64);
	harbol_unilist_del_node_by_index(p, 0, NULL);
	fprintf(g_harbol_debug_stream, "unilist head node ptr : %p, tail node ptr : %p\n", (void*)harbol_unilist_get_head_node(p), (void*)harbol_unilist_get_tail_node(p));
	
	
	// test deleting items by index on a list size of 2.
	fputs("unilist :: test deleting items by index on a list size of 2.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_unilist_del(p, NULL);
	harbol_unilist_insert_at_tail(p, (union HarbolValue){.Int64=100});
	harbol_unilist_insert_at_tail(p, (union HarbolValue){.Int64=101});
	for( struct HarbolUniListNode *n=harbol_unilist_get_head_node(p); n; n = harbol_unilistnode_get_next_node(n) )
		fprintf(g_harbol_debug_stream, "unilist value : %" PRIi64 "\n", harbol_unilistnode_get_val(n).Int64);
	harbol_unilist_del_node_by_index(p, 0, NULL);
	for( struct HarbolUniListNode *n=harbol_unilist_get_head_node(p); n; n = harbol_unilistnode_get_next_node(n) )
		fprintf(g_harbol_debug_stream, "unilist value : %" PRIi64 "\n", harbol_unilistnode_get_val(n).Int64);
	
	// test insertion in middle of list
	fputs("unilist :: test middle insertion.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_unilist_del(p, NULL);
	harbol_unilist_insert_at_index(p, (union HarbolValue){.Int64=100}, 0); // have it insert at head by inserting at index 0.
	harbol_unilist_insert_at_index(p, (union HarbolValue){.Int64=101}, 10); // have it insert at tail by having the index exceed the size of list
	harbol_unilist_insert_at_index(p, (union HarbolValue){.Int64=102}, 1);
	harbol_unilist_insert_at_index(p, (union HarbolValue){.Int64=102}, 1);
	harbol_unilist_insert_at_index(p, (union HarbolValue){.Int64=102}, 1);
	harbol_unilist_insert_at_index(p, (union HarbolValue){.Int64=102}, 1);
	harbol_unilist_insert_at_index(p, (union HarbolValue){.Int64=103}, 1);
	for( struct HarbolUniListNode *n=harbol_unilist_get_head_node(p); n; n = harbol_unilistnode_get_next_node(n) )
		fprintf(g_harbol_debug_stream, "unilist value : %" PRIi64 "\n", harbol_unilistnode_get_val(n).Int64);
	
	// test deleting the tail through an index.
	fputs("unilist :: test deleting tail by index.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_unilist_del(p, NULL);
	harbol_unilist_insert_at_tail(p, (union HarbolValue){.Int64=100});
	harbol_unilist_insert_at_tail(p, (union HarbolValue){.Int64=101});
	harbol_unilist_insert_at_tail(p, (union HarbolValue){.Int64=102});
	for( struct HarbolUniListNode *n=harbol_unilist_get_head_node(p); n; n = harbol_unilistnode_get_next_node(n) )
		fprintf(g_harbol_debug_stream, "unilist value : %" PRIi64 "\n", harbol_unilistnode_get_val(n).Int64);
	harbol_unilist_del_node_by_index(p, 2, NULL);
	for( struct HarbolUniListNode *n=harbol_unilist_get_head_node(p); n; n = harbol_unilistnode_get_next_node(n) )
		fprintf(g_harbol_debug_stream, "unilist value : %" PRIi64 "\n", harbol_unilistnode_get_val(n).Int64);
	
	// test finding a node by value.
	
	// free data
	fputs("unilist :: test destruction.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_unilist_del(&i, NULL);
	harbol_unilist_del(&i, NULL);
	fprintf(g_harbol_debug_stream, "i's Head is null? '%s'\n", i.Head ? "no" : "yes");
	
	harbol_unilist_del(p, NULL);
	fprintf(g_harbol_debug_stream, "p's Head is null? '%s', but is p null? '%s'\n", p->Head ? "no" : "yes", p ? "no" : "yes");
	harbol_unilist_free(&p, NULL);
	fprintf(g_harbol_debug_stream, "p is null? '%s'\n", p ? "no" : "yes");
}

void test_harbol_bilist(void)
{
	// Test allocation and initializations
	fputs("bilist :: test allocation / initialization.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	struct HarbolBiList *p = harbol_bilist_new();
	assert( p );
	
	struct HarbolBiList i = (struct HarbolBiList){0};
	
	// test insertion.
	// test insertion at tail.
	fputs("bilist :: test tail insertion.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_bilist_insert_at_tail(p, (union HarbolValue){.Int64=1});
	for( struct HarbolBiListNode *n=harbol_bilist_get_head_node(p); n; n = harbol_bilist_node_get_next_node(n) )
		fprintf(g_harbol_debug_stream, "bilist value : %" PRIi64 "\n", harbol_bilist_node_get_val(n).Int64);
	
	harbol_bilist_insert_at_tail(p, (union HarbolValue){.Int64=2});
	harbol_bilist_insert_at_tail(p, (union HarbolValue){.Int64=3});
	for( struct HarbolBiListNode *n=harbol_bilist_get_head_node(p); n; n = harbol_bilist_node_get_next_node(n) )
		fprintf(g_harbol_debug_stream, "bilist value : %" PRIi64 "\n", harbol_bilist_node_get_val(n).Int64);
	
	// test insertion at head.
	fputs("bilist :: test head insertion.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_bilist_insert_at_head(p, (union HarbolValue){.Int64=0});
	harbol_bilist_insert_at_head(p, (union HarbolValue){.Int64=-1});
	harbol_bilist_insert_at_head(p, (union HarbolValue){.Int64=-2});
	for( struct HarbolBiListNode *n=harbol_bilist_get_head_node(p); n; n = harbol_bilist_node_get_next_node(n) )
		fprintf(g_harbol_debug_stream, "bilist value : %" PRIi64 "\n", harbol_bilist_node_get_val(n).Int64);
	
	// test retrieval.
	fputs("bilist :: test node retrival by index.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	{
		struct HarbolBiListNode *n = harbol_bilist_get_node_by_index(p, 1);
		fprintf(g_harbol_debug_stream, "bilist value : %" PRIi64 "\n", harbol_bilist_node_get_val(n).Int64);
		
		// test item destruction.
		fputs("bilist :: test node removal & destruction by reference.", g_harbol_debug_stream);
		fputs("\n", g_harbol_debug_stream);
		harbol_bilist_del_node_by_ref(p, &n, NULL);
		for( struct HarbolBiListNode *n=harbol_bilist_get_head_node(p); n; n = harbol_bilist_node_get_next_node(n) )
			fprintf(g_harbol_debug_stream, "bilist value : %" PRIi64 "\n", harbol_bilist_node_get_val(n).Int64);
		
		fputs("bilist :: test node removal & destruction by index.", g_harbol_debug_stream);
		fputs("\n", g_harbol_debug_stream);
		harbol_bilist_del_node_by_index(p, 1, NULL);
		for( struct HarbolBiListNode *n=harbol_bilist_get_head_node(p); n; n = harbol_bilist_node_get_next_node(n) )
			fprintf(g_harbol_debug_stream, "bilist value : %" PRIi64 "\n", harbol_bilist_node_get_val(n).Int64);
	}
	
	// test setting data by index
	fputs("bilist :: test setting data by index (index of 0 is head node).", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_bilist_set_val(p, 0, (union HarbolValue){.Int64=222}); // 0 is head
	for( struct HarbolBiListNode *n=harbol_bilist_get_head_node(p); n; n = harbol_bilist_node_get_next_node(n) )
		fprintf(g_harbol_debug_stream, "bilist value : %" PRIi64 "\n", harbol_bilist_node_get_val(n).Int64);
	
	// test deleting head node after freeing.
	fputs("bilist :: test deleting head node after freeing and new insertion.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_bilist_del(p, NULL);
	harbol_bilist_insert_at_head(p, (union HarbolValue){.Int64=100});
	for( struct HarbolBiListNode *n=harbol_bilist_get_head_node(p); n; n = harbol_bilist_node_get_next_node(n) )
		fprintf(g_harbol_debug_stream, "bilist value : %" PRIi64 "\n", harbol_bilist_node_get_val(n).Int64);
	harbol_bilist_del_node_by_index(p, 0, NULL);
	fprintf(g_harbol_debug_stream, "bilist head node ptr : %p, tail node ptr : %p\n", (void*)harbol_bilist_get_head_node(p), (void*)harbol_bilist_get_tail_node(p));
	
	
	// test deleting items by index on a list size of 2.
	fputs("bilist :: test deleting items by index on a list size of 2.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_bilist_del(p, NULL);
	harbol_bilist_insert_at_tail(p, (union HarbolValue){.Int64=100});
	harbol_bilist_insert_at_tail(p, (union HarbolValue){.Int64=101});
	for( struct HarbolBiListNode *n=harbol_bilist_get_head_node(p); n; n = harbol_bilist_node_get_next_node(n) )
		fprintf(g_harbol_debug_stream, "bilist value : %" PRIi64 "\n", harbol_bilist_node_get_val(n).Int64);
	harbol_bilist_del_node_by_index(p, 0, NULL);
	for( struct HarbolBiListNode *n=harbol_bilist_get_head_node(p); n; n = harbol_bilist_node_get_next_node(n) )
		fprintf(g_harbol_debug_stream, "bilist value : %" PRIi64 "\n", harbol_bilist_node_get_val(n).Int64);
	
	// test insertion in middle of list
	fputs("bilist :: test middle insertion.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_bilist_del(p, NULL);
	harbol_bilist_insert_at_index(p, (union HarbolValue){.Int64=100}, 0); // have it insert at head by inserting at index 0.
	harbol_bilist_insert_at_index(p, (union HarbolValue){.Int64=101}, 10); // have it insert at tail by having the index exceed the size of list
	harbol_bilist_insert_at_index(p, (union HarbolValue){.Int64=102}, 1);
	harbol_bilist_insert_at_index(p, (union HarbolValue){.Int64=102}, 1);
	harbol_bilist_insert_at_index(p, (union HarbolValue){.Int64=102}, 1);
	harbol_bilist_insert_at_index(p, (union HarbolValue){.Int64=102}, 1);
	harbol_bilist_insert_at_index(p, (union HarbolValue){.Int64=103}, 1);
	for( struct HarbolBiListNode *n=harbol_bilist_get_head_node(p); n; n = harbol_bilist_node_get_next_node(n) )
		fprintf(g_harbol_debug_stream, "bilist value : %" PRIi64 "\n", harbol_bilist_node_get_val(n).Int64);
	
	
	// free data
	fputs("bilist :: test destruction.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_bilist_del(&i, NULL);
	harbol_bilist_del(&i, NULL);
	fprintf(g_harbol_debug_stream, "i's Head is null? '%s'\n", i.Head ? "no" : "yes");
	
	harbol_bilist_del(p, NULL);
	fprintf(g_harbol_debug_stream, "p's Head is null? '%s', but is p null? '%s'\n", p->Head ? "no" : "yes", p ? "no" : "yes");
	harbol_bilist_free(&p, NULL);
	fprintf(g_harbol_debug_stream, "p is null? '%s'\n", p ? "no" : "yes");
}

void test_harbol_bytebuffer(void)
{
	if( !g_harbol_debug_stream )
		return;
	
	// Test allocation and initializations
	fputs("bytebuffer :: test allocation/initialization.\n", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	struct HarbolByteBuffer *p = harbol_bytebuffer_new();
	assert( p );
	
	struct HarbolByteBuffer i = (struct HarbolByteBuffer){0};
	
	// test adding a byte.
	fputs("bytebuffer :: test byte appending.\n", g_harbol_debug_stream);
	harbol_bytebuffer_insert_byte(p, 5);
	harbol_bytebuffer_insert_byte(&i, 6);
	for( size_t n=0; n<harbol_bytebuffer_get_count(p); n++ )
		fprintf(g_harbol_debug_stream, "bytebuffer value : %zu\n", harbol_bytebuffer_get_raw_buffer(p)[n]);
	for( size_t n=0; n<harbol_bytebuffer_get_count(p); n++ )
		fprintf(g_harbol_debug_stream, "bytebuffer value : %zu\n", harbol_bytebuffer_get_raw_buffer(&i)[n]);
	
	// test integer appending
	harbol_bytebuffer_del(&i);
	harbol_bytebuffer_del(p);
	uint16_t ush = 50;
	harbol_bytebuffer_insert_integer(p, ush, sizeof ush);
	harbol_bytebuffer_insert_integer(&i, ush, sizeof ush);
	for( size_t n=0; n<harbol_bytebuffer_get_count(p); n++ )
		fprintf(g_harbol_debug_stream, "bytebuffer value : %zu\n", harbol_bytebuffer_get_raw_buffer(p)[n]);
	for( size_t n=0; n<harbol_bytebuffer_get_count(p); n++ )
		fprintf(g_harbol_debug_stream, "bytebuffer value : %zu\n", harbol_bytebuffer_get_raw_buffer(&i)[n]);
	
	harbol_bytebuffer_del(&i);
	harbol_bytebuffer_del(p);
	uint64_t ull = 0xabcdef;
	harbol_bytebuffer_insert_integer(p, ull, sizeof ull);
	harbol_bytebuffer_insert_integer(&i, ull, sizeof ull);
	for( size_t n=0; n<harbol_bytebuffer_get_count(p); n++ )
		fprintf(g_harbol_debug_stream, "bytebuffer value : %zu\n", harbol_bytebuffer_get_raw_buffer(p)[n]);
	for( size_t n=0; n<harbol_bytebuffer_get_count(p); n++ )
		fprintf(g_harbol_debug_stream, "bytebuffer value : %zu\n", harbol_bytebuffer_get_raw_buffer(&i)[n]);
	
	
	// free data
	fputs("bytebuffer :: test destruction.", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	harbol_bytebuffer_del(&i);
	fprintf(g_harbol_debug_stream, "i's buffer is null? '%s'\n", i.Buffer ? "no" : "yes");
	
	harbol_bytebuffer_del(p);
	fprintf(g_harbol_debug_stream, "p's buffer is null? '%s'\n", p->Buffer ? "no" : "yes");
	harbol_bytebuffer_free(&p);
	fprintf(g_harbol_debug_stream, "p is null? '%s'\n", p ? "no" : "yes");
}

void test_harbol_tuple(void)
{
	if( !g_harbol_debug_stream )
		return;
	
	// Test allocation and initializations
	fputs("tuple :: test allocation/initialization.\n", g_harbol_debug_stream);
	fputs("\n", g_harbol_debug_stream);
	
	size_t struc[] = { sizeof(char), sizeof(int), sizeof(short) };
	
	struct HarbolTuple *p = harbol_tuple_new(sizeof struc / sizeof struc[0], struc, false);
	assert( p );
	fprintf(g_harbol_debug_stream, "p's size: '%zu'\n", p->Len);
	
	fputs("\ntuple :: printing 3-tuple fields.\n", g_harbol_debug_stream);
	for( size_t i=0; i<p->Fields.Count; i++ ) {
		//fprintf(g_harbol_debug_stream, "offset: %zu -> p's ptr: '%" PRIuPTR "'\n", i, harbol_vector_get(&p->Fields, i).UIntNative);
		fprintf(g_harbol_debug_stream, "index: %zu -> p's offset: '%" PRIu64 "' | size: '%" PRIu64 "'\n", i, harbol_vector_get(&p->Fields, i).UInt64 & 0xffFFffFF, harbol_vector_get(&p->Fields, i).UInt64 >> 32);
	}
	fputs("\n", g_harbol_debug_stream);
	
	harbol_tuple_free(&p);
	
	p = harbol_tuple_new(sizeof struc / sizeof struc[0], struc, true);
	assert( p );
	fprintf(g_harbol_debug_stream, "packed p's size: '%zu'\n", p->Len);
	
	fputs("\ntuple :: printing packed 3-tuple fields.\n", g_harbol_debug_stream);
	for( size_t i=0; i<p->Fields.Count; i++ ) {
		//fprintf(g_harbol_debug_stream, "offset: %zu -> p's ptr: '%" PRIuPTR "'\n", i, harbol_vector_get(&p->Fields, i).UIntNative);
		fprintf(g_harbol_debug_stream, "index: %zu -> p's offset: '%" PRIu64 "' | size: '%" PRIu64 "'\n", i, harbol_vector_get(&p->Fields, i).UInt64 & 0xffFFffFF, harbol_vector_get(&p->Fields, i).UInt64 >> 32);
	}
	fputs("\n", g_harbol_debug_stream);
	
	harbol_tuple_free(&p);
	
	p = harbol_tuple_new(4, (size_t[]){ sizeof(char), sizeof(char), sizeof(char), sizeof(char) }, false);
	assert( p );
	fprintf(g_harbol_debug_stream, "char p's size: '%zu'\n", p->Len);
	
	fputs("\ntuple :: printing byte 4-tuple fields.\n", g_harbol_debug_stream);
	for( size_t i=0; i<p->Fields.Count; i++ ) {
		//fprintf(g_harbol_debug_stream, "offset: %zu -> p's ptr: '%" PRIuPTR "'\n", i, harbol_vector_get(&p->Fields, i).UIntNative);
		fprintf(g_harbol_debug_stream, "index: %zu -> p's offset: '%" PRIu64 "' | size: '%" PRIu64 "'\n", i, harbol_vector_get(&p->Fields, i).UInt64 & 0xffFFffFF, harbol_vector_get(&p->Fields, i).UInt64 >> 32);
	}
	fputs("\n", g_harbol_debug_stream);
	
	harbol_tuple_free(&p);
	
	p = harbol_tuple_new(4, (size_t[]){ sizeof(int64_t), sizeof(char), sizeof(int), sizeof(short) }, false);
	assert( p );
	fprintf(g_harbol_debug_stream, "p's size: '%zu'\n", p->Len);
	
	fputs("\ntuple :: printing 4-tuple fields.\n", g_harbol_debug_stream);
	for( size_t i=0; i<p->Fields.Count; i++ ) {
		fprintf(g_harbol_debug_stream, "index: %zu -> p's offset: '%" PRIu64 "' | size: '%" PRIu64 "'\n", i, harbol_vector_get(&p->Fields, i).UInt64 & 0xffFFffFF, harbol_vector_get(&p->Fields, i).UInt64 >> 32);
	}
	fputs("\n", g_harbol_debug_stream);
	harbol_tuple_free(&p);
	
	p = harbol_tuple_new(3, (size_t[]){ sizeof(int), sizeof(char), sizeof(short) }, false);
	assert( p );
	fprintf(g_harbol_debug_stream, "p's size: '%zu'\n", p->Len);
	
	fputs("\ntuple :: printing newly aligned 3-tuple fields.\n", g_harbol_debug_stream);
	for( size_t i=0; i<p->Fields.Count; i++ ) {
		fprintf(g_harbol_debug_stream, "index: %zu -> p's offset: '%" PRIu64 "' | size: '%" PRIu64 "'\n", i, harbol_vector_get(&p->Fields, i).UInt64 & 0xffFFffFF, harbol_vector_get(&p->Fields, i).UInt64 >> 32);
	}
	fputs("\n", g_harbol_debug_stream);
	harbol_tuple_free(&p);
	
	p = harbol_tuple_new(3, (size_t[]){ sizeof(HarbolTuple), sizeof(HarbolLinkMap), sizeof(HarbolString) }, false);
	assert( p );
	fprintf(g_harbol_debug_stream, "p's size: '%zu'\n", p->Len);
	
	fputs("\ntuple :: printing structure-tuple fields.\n", g_harbol_debug_stream);
	for( size_t i=0; i<p->Fields.Count; i++ ) {
		//fprintf(g_harbol_debug_stream, "offset: %zu -> p's ptr: '%" PRIuPTR "'\n", i, harbol_vector_get(&p->Fields, i).UIntNative);
		fprintf(g_harbol_debug_stream, "index: %zu -> p's offset: '%" PRIu64 "' | size: '%" PRIu64 "'\n", i, harbol_vector_get(&p->Fields, i).UInt64 & 0xffFFffFF, harbol_vector_get(&p->Fields, i).UInt64 >> 32);
	}
	fputs("\n", g_harbol_debug_stream);
	
	{
		struct { HarbolTuple tup; HarbolLinkMap linkmap; HarbolString str; } testStruct = {0};
		memset(&testStruct.tup, 0xa, sizeof testStruct.tup);
		memset(&testStruct.linkmap, 0xb, sizeof testStruct.linkmap);
		memset(&testStruct.str, 0xc, sizeof testStruct.str);
		
		fprintf(g_harbol_debug_stream, "sizeof of testStruct: %zu\nalignment of testStruct: %zu\n", sizeof testStruct, alignof(testStruct));
		uint8_t *n = (uint8_t *)&testStruct;
		for(size_t i=0;i<sizeof testStruct;i++)
			fprintf(g_harbol_debug_stream, "testStruct[%zu]: '%u'\n", i, n[i]);
	}
	// free tuple
	harbol_tuple_del(p);
	fprintf(g_harbol_debug_stream, "p's item is null? '%s'\n", p->Datum ? "no" : "yes");
	harbol_tuple_free(&p);
	fprintf(g_harbol_debug_stream, "p is null? '%s'\n\n", p ? "no" : "yes");
}

void test_harbol_mempool(void)
{
	if( !g_harbol_debug_stream )
		return;
	
	// Test allocation and initializations
	fputs("mempool :: test allocation/initialization.\n", g_harbol_debug_stream);
	
	struct HarbolMemoryPool i;
#ifdef HARBOL_HEAP_NO_MALLOC
	harbol_mempool_init(&i);
#else
	harbol_mempool_init(&i, 1000);
#endif
	fprintf(g_harbol_debug_stream, "remaining heap mem: '%zu'\n", harbol_mempool_get_remaining(&i));
	
	// test giving memory
	fputs("mempool :: test giving memory.\n", g_harbol_debug_stream);
	fputs("\nmempool :: allocating int ptr.\n", g_harbol_debug_stream);
	int *p = harbol_mempool_alloc(&i, sizeof *p);
	fprintf(g_harbol_debug_stream, "p is null? '%s'\n", p ? "no" : "yes");
	if( p ) {
		*p = 500;
		fprintf(g_harbol_debug_stream, "p's value: %i\n", *p);
	}
	
	fputs("\nmempool :: allocating float ptr.\n", g_harbol_debug_stream);
	float *f = harbol_mempool_alloc(&i, sizeof *f);
	fprintf(g_harbol_debug_stream, "f is null? '%s'\n", f ? "no" : "yes");
	if( f ) {
		*f = 500.5f;
		fprintf(g_harbol_debug_stream, "f's value: %f\n", *f);
	}
	fprintf(g_harbol_debug_stream, "remaining heap mem: '%zu'\n", harbol_mempool_get_remaining(&i));
	
	// test releasing memory
	fputs("mempool :: test releasing memory.\n", g_harbol_debug_stream);
	harbol_mempool_dealloc(&i, f), f=NULL;
	harbol_mempool_dealloc(&i, p), p=NULL;
	
	// test re-giving memory
	fputs("mempool :: test regiving memory.\n", g_harbol_debug_stream);
	p = harbol_mempool_alloc(&i, sizeof *p);
	fprintf(g_harbol_debug_stream, "p is null? '%s'\n", p ? "no" : "yes");
	if( p ) {
		*p = 532;
		fprintf(g_harbol_debug_stream, "p's value: %i\n", *p);
	}
	f = harbol_mempool_alloc(&i, sizeof *f);
	fprintf(g_harbol_debug_stream, "f is null? '%s'\n", f ? "no" : "yes");
	if( f ) {
		*f = 466.5f;
		fprintf(g_harbol_debug_stream, "f's value: %f\n", *f);
	}
	fprintf(g_harbol_debug_stream, "remaining heap mem: '%zu'\n", harbol_mempool_get_remaining(&i));
	harbol_mempool_dealloc(&i, p), p=NULL; // release memory that's from different region.
	harbol_mempool_dealloc(&i, f), f=NULL;
	fprintf(g_harbol_debug_stream, "remaining heap mem: '%zu'\n", harbol_mempool_get_remaining(&i));
	
	// test giving array memory
	fputs("\nmempool :: test giving array memory.\n", g_harbol_debug_stream);
	const size_t arrsize = 100;
	p = harbol_mempool_alloc(&i, sizeof *p * arrsize);
	fprintf(g_harbol_debug_stream, "p is null? '%s'\n", p ? "no" : "yes");
	if( p ) {
		for( size_t i=0; i<arrsize; i++ ) {
			p[i] = i+1;
			fprintf(g_harbol_debug_stream, "p[%zu] value: %i\n", i, p[i]);
		}
	}
	fprintf(g_harbol_debug_stream, "remaining heap mem: '%zu'\n", harbol_mempool_get_remaining(&i));
	f = harbol_mempool_alloc(&i, sizeof *f * arrsize);
	fprintf(g_harbol_debug_stream, "f is null? '%s'\n", f ? "no" : "yes");
	if( f ) {
		for( size_t i=0; i<arrsize; i++ ) {
			f[i] = i+1.15f;
			fprintf(g_harbol_debug_stream, "f[%zu] value: %f\n", i, f[i]);
		}
	}
	harbol_mempool_dealloc(&i, p), p=NULL;
	harbol_mempool_dealloc(&i, f), f=NULL;
	fprintf(g_harbol_debug_stream, "remaining heap mem: '%zu'\n", harbol_mempool_get_remaining(&i));
	
	
	// test using heap to make a unilinked list!
	fputs("\nmempool :: test using heap for unilinked list.\n", g_harbol_debug_stream);
	struct HarbolUniList *list = harbol_mempool_alloc(&i, sizeof *list);
	assert( list );
	
	struct HarbolUniListNode *node1 = harbol_mempool_alloc(&i, sizeof *node1);
	assert( node1 );
	node1->Data = (union HarbolValue){.Int64 = 1};
	harbol_unilist_insert_node_at_tail(list, node1);
	
	struct HarbolUniListNode *node2 = harbol_mempool_alloc(&i, sizeof *node2);
	assert( node2 );
	node2->Data = (union HarbolValue){.Int64 = 2};
	harbol_unilist_insert_node_at_tail(list, node2);
	
	struct HarbolUniListNode *node3 = harbol_mempool_alloc(&i, sizeof *node3);
	assert( node3 );
	node3->Data = (union HarbolValue){.Int64 = 3};
	harbol_unilist_insert_node_at_tail(list, node3);
	
	struct HarbolUniListNode *node4 = harbol_mempool_alloc(&i, sizeof *node4);
	assert( node4 );
	node4->Data = (union HarbolValue){.Int64 = 4};
	harbol_unilist_insert_node_at_tail(list, node4);
	
	struct HarbolUniListNode *node5 = harbol_mempool_alloc(&i, sizeof *node5);
	assert( node5 );
	node5->Data = (union HarbolValue){.Int64 = 5};
	harbol_unilist_insert_node_at_tail(list, node5);
	
	for( struct HarbolUniListNode *n=list->Head; n; n = n->Next )
		fprintf(g_harbol_debug_stream, "unilist value : %" PRIi64 "\n", n->Data.Int64);
	
	harbol_mempool_dealloc(&i, node1), node1=NULL;
	harbol_mempool_dealloc(&i, node2), node2=NULL;
	harbol_mempool_dealloc(&i, node3), node3=NULL;
	harbol_mempool_dealloc(&i, node4), node4=NULL;
	harbol_mempool_dealloc(&i, node5), node5=NULL;
	harbol_mempool_dealloc(&i, list), list=NULL;
	
	// test "double freeing"
	fputs("\nmempool :: test double freeing.\n", g_harbol_debug_stream);
	p = harbol_mempool_alloc(&i, sizeof *p);
	fprintf(g_harbol_debug_stream, "p is null? '%s'\n", p ? "no" : "yes");
	if( p ) {
		*p = 500;
		fprintf(g_harbol_debug_stream, "p's value: %i\n", *p);
	}
	
	harbol_mempool_dealloc(&i, p);
	harbol_mempool_dealloc(&i, p);
	
	fprintf(g_harbol_debug_stream, "\nmempool :: pool size == %zu.\n", harbol_mempool_get_remaining(&i));
	for( struct HarbolAllocNode *n = i.FreeList; n; n = n->Next )
		fprintf(g_harbol_debug_stream, "mempool :: n (%zu) size == %zu.\n", (uintptr_t)n, n->Size);
	
	float *hk = harbol_mempool_alloc(&i, sizeof *hk * 99);
	double *fg = harbol_mempool_alloc(&i, sizeof *fg * 10);
	char *fff = harbol_mempool_alloc(&i, sizeof *fff * 50);
	float *f32 = harbol_mempool_alloc(&i, sizeof *f32 * 23);
	char *jj = harbol_mempool_alloc(&i, sizeof *jj * 100);
	struct HarbolAllocNode *ac = harbol_mempool_alloc(&i, sizeof *ac * 31);
	harbol_mempool_dealloc(&i, fff);
	harbol_mempool_dealloc(&i, fg);
	harbol_mempool_dealloc(&i, ac);
	harbol_mempool_dealloc(&i, f32);
	fprintf(g_harbol_debug_stream, "\nmempool :: pool size == %zu.\n", harbol_mempool_get_remaining(&i));
	for( struct HarbolAllocNode *n = i.FreeList; n; n = n->Next )
		fprintf(g_harbol_debug_stream, "mempool :: n (%zu) size == %zu.\n", (uintptr_t)n, n->Size);
	fprintf(g_harbol_debug_stream, "mempool :: heap bottom (%zu).\n", (uintptr_t)i.HeapBottom);
	
	harbol_mempool_dealloc(&i, hk);
	fprintf(g_harbol_debug_stream, "\ncrazy mempool :: pool size == %zu.\n", harbol_mempool_get_remaining(&i));
	for( struct HarbolAllocNode *n = i.FreeList; n; n = n->Next )
		fprintf(g_harbol_debug_stream, "crazy mempool :: n (%zu) size == %zu.\n", (uintptr_t)n, n->Size);
		
	harbol_mempool_dealloc(&i, jj);
	
	fprintf(g_harbol_debug_stream, "\nlast mempool :: pool size == %zu.\n", harbol_mempool_get_remaining(&i));
	for( struct HarbolAllocNode *n = i.FreeList; n; n = n->Next )
		fprintf(g_harbol_debug_stream, "last mempool :: n (%zu) size == %zu.\n", (uintptr_t)n, n->Size);
	//fprintf(g_harbol_debug_stream, "mempool :: heap bottom (%zu).\n", (uintptr_t)i.HeapBottom);
	
	fputs("\nmempool :: test reallocating jj to a single value.\n", g_harbol_debug_stream);
	jj = harbol_mempool_alloc(&i, sizeof *jj);
	*jj = 50;
	fprintf(g_harbol_debug_stream, "mempool :: jj == %i.\n", *jj);
	
	int *newer = harbol_mempool_realloc(&i, jj, sizeof *newer);
	fputs("\nmempool :: test reallocating jj to int ptr 'newer'.\n", g_harbol_debug_stream);
	fprintf(g_harbol_debug_stream, "mempool :: newer == %i.\n", *newer);
	
	jj = harbol_mempool_realloc(&i, newer, sizeof *jj);
	fputs("\nmempool :: test reallocating newer back to jj.\n", g_harbol_debug_stream);
	fprintf(g_harbol_debug_stream, "mempool :: jj == %i.\n", *jj);
	
	newer = harbol_mempool_realloc(&i, jj, sizeof *newer * 10);
	fputs("\nmempool :: test reallocating jj back to newer as an array of int[10].\n", g_harbol_debug_stream);
	for( size_t i=0; i<10; i++ ) {
		newer[i] = i+1;
		fprintf(g_harbol_debug_stream, "mempool :: newer[%zu] == %i.\n", i, newer[i]);
	}
	fputs("\n", g_harbol_debug_stream);
	newer = harbol_mempool_realloc(&i, newer, sizeof *newer * 5);
	for( size_t i=0; i<5; i++ )
		fprintf(g_harbol_debug_stream, "mempool :: reallocated newer[%zu] == %i.\n", i, newer[i]);
	harbol_mempool_dealloc(&i, newer);
	
	// free data
	fputs("\nmempool :: test destruction.\n", g_harbol_debug_stream);
	harbol_mempool_del(&i);
	fprintf(g_harbol_debug_stream, "i's heap bottom is null? '%s'\n", i.HeapBottom ? "no" : "yes");
	fprintf(g_harbol_debug_stream, "i's FreeList is null? '%s'\n", i.FreeList ? "no" : "yes");
}

void test_harbol_graph(void)
{
	if( !g_harbol_debug_stream )
		return;
	
	// Test allocation and initializations
	fputs("graph :: test allocation/initialization.\n", g_harbol_debug_stream);
	
	struct HarbolGraph g = (struct HarbolGraph){0};
	harbol_graph_init(&g);
	struct HarbolGraph *p = harbol_graph_new();
	if( p )
		fputs("graph :: allocation/initialization of p is GOOD.\n", g_harbol_debug_stream);
	
	// Test adding Vertices
	fputs("\ngraph :: test adding Vertices.\n", g_harbol_debug_stream);
	for( size_t c=0; c<5; c++ ) {
		harbol_graph_insert_val(&g, (union HarbolValue){.Int64=c+1});
		harbol_graph_insert_val(p, (union HarbolValue){.Int64=c+1});
	}
	for( size_t i=0; i<g.Vertices.Count; i++ ) {
		struct HarbolGraphVertex *vert = harbol_graph_get_vertex_by_index(&g, i);
		fprintf(g_harbol_debug_stream, "Vertex Data: '%" PRIi64 "'\n", vert->Data.Int64);
	}
	// test linking two Vertices
	fputs("\ngraph :: test linking Vertices.\n", g_harbol_debug_stream);
	fprintf(g_harbol_debug_stream, "linking was success?: '%u'\n", harbol_graph_insert_edge(&g, 0, 1, (union HarbolValue){0}));
	fprintf(g_harbol_debug_stream, "Edge count in graph: '%u'\n", harbol_graph_get_edge_count(&g));
	
	for( size_t i=0; i<g.Vertices.Count; i++ ) {
		struct HarbolGraphVertex *vert = g.Vertices.Table[i].Ptr;
		fprintf(g_harbol_debug_stream, "Vertex Data: '%" PRIi64 "'\n", vert->Data.Int64);
		for( size_t n=0; n<vert->Edges.Count; n++ ) {
			struct HarbolGraphEdge *k = vert->Edges.Table[i].Ptr;
			fprintf(g_harbol_debug_stream, "Edge Data: '%" PRIi64 "'\n", k->Weight.Int64);
			if( k->Link )
				fprintf(g_harbol_debug_stream, "Vertex Socket Data: '%" PRIi64 "'\n", k->Link->Data.Int64);
		}
	}
	
	// test unlinking the two previous Vertices
	fputs("\ngraph :: test unlinking the two previous Vertices.\n", g_harbol_debug_stream);
	fprintf(g_harbol_debug_stream, "unlinking was success?: '%u'\n", harbol_graph_delete_edge(&g, 0, 1, NULL));
	fprintf(g_harbol_debug_stream, "Edge count in graph: '%u'\n", harbol_graph_get_edge_count(&g));
	
	// test linking ALL available Vertices!
	fputs("\ngraph :: test linking all Vertices in graph.\n", g_harbol_debug_stream);
	// link 1 to 2
	harbol_graph_insert_edge(&g, 0, 1, (union HarbolValue){5});
	// link 2 to 3
	harbol_graph_insert_edge(&g, 1, 2, (union HarbolValue){2});
	// link 3 to 4
	harbol_graph_insert_edge(&g, 2, 3, (union HarbolValue){24});
	// link 4 to 5
	harbol_graph_insert_edge(&g, 3, 4, (union HarbolValue){62});
	// link 5 to 3
	harbol_graph_insert_edge(&g, 4, 2, (union HarbolValue){77});
	fprintf(g_harbol_debug_stream, "Edge count in graph: '%u'\n", harbol_graph_get_edge_count(&g));
	
	for( size_t i=0; i<g.Vertices.Count; i++ ) {
		struct HarbolGraphVertex *vert = g.Vertices.Table[i].Ptr;
		fprintf(g_harbol_debug_stream, "Vertex Data: '%" PRIi64 "'\n", vert->Data.Int64);
		for( size_t n=0; n<vert->Edges.Count; n++ ) {
			struct HarbolGraphEdge *k = vert->Edges.Table[n].Ptr;
			fprintf(g_harbol_debug_stream, "Edge Data: '%" PRIi64 "'\n", k->Weight.Int64);
			if( k->Link )
				fprintf(g_harbol_debug_stream, "Edge Vertex Socket Data: '%" PRIi64 "'\n", k->Link->Data.Int64);
		}
	}
	fprintf(g_harbol_debug_stream, "\nRemoving 5th value success?: '%u'\n", harbol_graph_delete_val_by_index(&g, 4, NULL, NULL));
	for( size_t i=0; i<g.Vertices.Count; i++ ) {
		struct HarbolGraphVertex *vert = g.Vertices.Table[i].Ptr;
		fprintf(g_harbol_debug_stream, "Vertex Data: '%" PRIi64 "'\n", vert->Data.Int64);
		for( size_t n=0; n<vert->Edges.Count; n++ ) {
			struct HarbolGraphEdge *k = vert->Edges.Table[n].Ptr;
			fprintf(g_harbol_debug_stream, "Edge Data: '%" PRIi64 "'\n", k->Weight.Int64);
			if( k->Link )
				fprintf(g_harbol_debug_stream, "Edge Vertex Socket Data: '%" PRIi64 "'\n", k->Link->Data.Int64);
		}
	}
	
	// test changing vertex data through an index!
	fprintf(g_harbol_debug_stream, "\nTest changing vertex data by index\n");
	harbol_graph_set_val_by_index(&g, 0, (union HarbolValue){.Int64 = 100});
	for( size_t i=0; i<g.Vertices.Count; i++ ) {
		struct HarbolGraphVertex *vert = g.Vertices.Table[i].Ptr;
		fprintf(g_harbol_debug_stream, "Vertex Data: '%" PRIi64 "'\n", vert->Data.Int64);
		for( size_t n=0; n<vert->Edges.Count; n++ ) {
			struct HarbolGraphEdge *k = vert->Edges.Table[n].Ptr;
			fprintf(g_harbol_debug_stream, "Edge Data: '%" PRIi64 "'\n", k->Weight.Int64);
			if( k->Link )
				fprintf(g_harbol_debug_stream, "Vertex Socket Data: '%" PRIi64 "'\n", k->Link->Data.Int64);
		}
	}
	
	// test getting edge pointer
	fputs("\ngraph :: test getting edge pointer.\n", g_harbol_debug_stream);
	struct HarbolGraphEdge *edge = harbol_graph_get_edge(&g, 0, 1);
	if( edge ) {
		fputs("edge ptr is VALID.\n", g_harbol_debug_stream);
		if( edge->Link )
			fprintf(g_harbol_debug_stream, "edge Vertex Socket Data: '%" PRIi64 "'\n", edge->Link->Data.Int64);
	}
	
	// test adjacency function
	fprintf(g_harbol_debug_stream, "\nindex 0 is adjacent to index 1?: '%u'\n", harbol_graph_is_vertex_adjacent_by_index(&g, 0, 1));
	
	// free data
	fputs("\ngraph :: test destruction.\n", g_harbol_debug_stream);
	harbol_graph_del(&g, NULL, NULL);
	fprintf(g_harbol_debug_stream, "i's Vertices vector is null? '%s'\n", g.Vertices.Table ? "no" : "yes");
	
	harbol_graph_del(p, NULL, NULL);
	fprintf(g_harbol_debug_stream, "p's Vertices vector is null? '%s'\n", p->Vertices.Table ? "no" : "yes");
	harbol_graph_free(&p, NULL, NULL);
	fprintf(g_harbol_debug_stream, "p is null? '%s'\n", p ? "no" : "yes");
}

void test_harbol_tree(void)
{
	if( !g_harbol_debug_stream )
		return;
	
	// Test allocation and initializations
	fputs("tree :: test allocation/initialization.\n", g_harbol_debug_stream);
	struct HarbolTree i = (struct HarbolTree){0};
	harbol_tree_init(&i);
	struct HarbolTree *p = harbol_tree_new((union HarbolValue){0});
	if( p )
		fputs("graph :: allocation/initialization of p is GOOD.\n", g_harbol_debug_stream);
	
	
	// Test insertions
	fputs("\ntree :: test insertions.\n", g_harbol_debug_stream);
	fprintf(g_harbol_debug_stream, "insertion by node success?: '%u'\n", harbol_tree_insert_child_node(p, harbol_tree_new((union HarbolValue){.Int64=20})));
	fprintf(g_harbol_debug_stream, "insertion by value success?: '%u'\n", harbol_tree_insert_child_val(p, (union HarbolValue){.Int64=10}));
	harbol_tree_del(p, NULL);
	
	// Test deletions
	fputs("\ntree :: test deletions by first adding 5 children.\n", g_harbol_debug_stream);
	harbol_tree_insert_child_val(p, (union HarbolValue){.Int64=1});
	harbol_tree_insert_child_val(p, (union HarbolValue){.Int64=2});
	harbol_tree_insert_child_val(p, (union HarbolValue){.Int64=3});
	harbol_tree_insert_child_val(p, (union HarbolValue){.Int64=4});
	harbol_tree_insert_child_val(p, (union HarbolValue){.Int64=5});
	for( size_t n=0; n<p->Children.Count; n++ ) {
		struct HarbolTree *child = p->Children.Table[n].Ptr;
		fprintf(g_harbol_debug_stream, "child #%zu value: '%" PRIi64 "'\n", n, child->Data.Int64);
	}
	fputs("\ndeleting index 1\n", g_harbol_debug_stream);
	harbol_tree_delete_child_by_index(p, 1, NULL);
	for( size_t n=0; n<p->Children.Count; n++ ) {
		struct HarbolTree *child = p->Children.Table[n].Ptr;
		fprintf(g_harbol_debug_stream, "child #%zu value: '%" PRIi64 "'\n", n, child->Data.Int64);
	}
	// Test delete by node reference
	fputs("\ntree :: test deletion by node reference.\n", g_harbol_debug_stream);
	// delete first child!
	fputs("\ndeleting index 0\n", g_harbol_debug_stream);
	harbol_tree_delete_child_by_ref(p, (struct HarbolTree **)&p->Children.Table[0].Ptr, NULL);
	for( size_t n=0; n<p->Children.Count; n++ ) {
		struct HarbolTree *child = p->Children.Table[n].Ptr;
		fprintf(g_harbol_debug_stream, "child #%zu value: '%" PRIi64 "'\n", n, child->Data.Int64);
	}
	// Test creating something of an abstract syntax tree.
	fputs("\ntree :: test creating something of an abstract syntax tree.\n", g_harbol_debug_stream);
	harbol_tree_del(p, NULL);
	harbol_tree_set_val(p, (union HarbolValue){.StrObjPtr=harbol_string_new_cstr("program")});
	harbol_tree_insert_child_val(p, (union HarbolValue){.StrObjPtr=harbol_string_new_cstr("stmt")});
	harbol_tree_insert_child_val(p->Children.Table[0].Ptr, (union HarbolValue){.StrObjPtr=harbol_string_new_cstr("if")});
	harbol_tree_insert_child_val(p->Children.Table[0].Ptr, (union HarbolValue){.StrObjPtr=harbol_string_new_cstr("cond")});
	harbol_tree_insert_child_val(p->Children.Table[0].Ptr, (union HarbolValue){.StrObjPtr=harbol_string_new_cstr("stmt")});
	harbol_tree_insert_child_val(p->Children.Table[0].Ptr, (union HarbolValue){.StrObjPtr=harbol_string_new_cstr("else")});
	fprintf(g_harbol_debug_stream, "p's data: '%s'\n", p->Data.StrObjPtr->CStr);
	struct HarbolTree *kid = p->Children.Table[0].Ptr;
	fprintf(g_harbol_debug_stream, "p's child data: '%s'\n", kid->Data.StrObjPtr->CStr);
	for( size_t n=0; n<kid->Children.Count; n++ ) {
		struct HarbolTree *child = kid->Children.Table[n].Ptr;
		fprintf(g_harbol_debug_stream, "p's child's children data: '%s'\n", child->Data.StrObjPtr->CStr);
	}
	fputs("\nfreeing string data.\n", g_harbol_debug_stream);
	harbol_tree_del(p, (fnHarbolDestructor *)harbol_string_free);
	
	// free data
	fputs("\ntree :: test destruction.\n", g_harbol_debug_stream);
	harbol_tree_del(&i, NULL);
	fprintf(g_harbol_debug_stream, "i's Children vector is null? '%s'\n", i.Children.Table ? "no" : "yes");
	
	harbol_tree_del(p, NULL);
	fprintf(g_harbol_debug_stream, "p's Children vector is null? '%s'\n", p->Children.Table ? "no" : "yes");
	harbol_tree_free(&p, NULL);
	fprintf(g_harbol_debug_stream, "p is null? '%s'\n", p ? "no" : "yes");
}

void test_harbol_linkmap(void)
{
	// Test allocation and initializations
	fputs("linkmap :: test allocation / initialization.\n", g_harbol_debug_stream);
	struct HarbolLinkMap *p = harbol_linkmap_new();
	assert( p );
	
	struct HarbolLinkMap i = (struct HarbolLinkMap){0};
	
	// test insertion
	fputs("\nlinkmap :: test insertion.\n", g_harbol_debug_stream);
	harbol_linkmap_insert(p, "1", (union HarbolValue){.Int64=1});
	harbol_linkmap_insert(p, "2", (union HarbolValue){.Int64=2});
	harbol_linkmap_insert(p, "3", (union HarbolValue){.Int64=3});
	harbol_linkmap_insert(p, "4", (union HarbolValue){.Int64=4});
	harbol_linkmap_insert(p, "5", (union HarbolValue){.Int64=5});
	harbol_linkmap_insert(p, "6", (union HarbolValue){.Int64=6});
	harbol_linkmap_insert(p, "7", (union HarbolValue){.Int64=7});
	harbol_linkmap_insert(p, "8", (union HarbolValue){.Int64=8});
	harbol_linkmap_insert(p, "9", (union HarbolValue){.Int64=9});
	harbol_linkmap_insert(p, "10", (union HarbolValue){.Int64=10});
	
	harbol_linkmap_insert(&i, "1", (union HarbolValue){.Int64=1});
	harbol_linkmap_insert(&i, "2", (union HarbolValue){.Int64=2});
	harbol_linkmap_insert(&i, "3", (union HarbolValue){.Int64=3});
	harbol_linkmap_insert(&i, "4", (union HarbolValue){.Int64=4});
	harbol_linkmap_insert(&i, "5", (union HarbolValue){.Int64=5});
	harbol_linkmap_insert(&i, "6", (union HarbolValue){.Int64=6});
	harbol_linkmap_insert(&i, "7", (union HarbolValue){.Int64=7});
	harbol_linkmap_insert(&i, "8", (union HarbolValue){.Int64=8});
	harbol_linkmap_insert(&i, "9", (union HarbolValue){.Int64=9});
	harbol_linkmap_insert(&i, "10", (union HarbolValue){.Int64=10});
	
	// test retrieval.
	fputs("\nlinkmap :: test data retrieval.\n", g_harbol_debug_stream);
	fprintf(g_harbol_debug_stream, "ptr[\"1\"] == %" PRIi64 "\n", harbol_linkmap_get(p, "1").Int64);
	fprintf(g_harbol_debug_stream, "ptr[\"2\"] == %" PRIi64 "\n", harbol_linkmap_get(p, "2").Int64);
	fprintf(g_harbol_debug_stream, "stk[\"1\"] == %" PRIi64 "\n", harbol_linkmap_get(&i, "1").Int64);
	fprintf(g_harbol_debug_stream, "stk[\"2\"] == %" PRIi64 "\n", harbol_linkmap_get(&i, "2").Int64);
	
	fputs("\nlinkmap :: looping through all data.\n", g_harbol_debug_stream);
	for( size_t n=0; n<i.Order.Count; n++ ) {
		struct HarbolKeyValPair *l = i.Order.Table[n].Ptr;
		fprintf(g_harbol_debug_stream, "l's value == %" PRIi64 "\n", l->Data.Int64);
	}
	// test setting.
	fputs("\nlinkmap :: test data setting.\n", g_harbol_debug_stream);
	harbol_linkmap_set(p, "2", (union HarbolValue){.Int64=20});
	harbol_linkmap_set(&i, "2", (union HarbolValue){.Int64=200});
	fprintf(g_harbol_debug_stream, "ptr[\"1\"] == %" PRIi64 "\n", harbol_linkmap_get(p, "1").Int64);
	fprintf(g_harbol_debug_stream, "ptr[\"2\"] == %" PRIi64 "\n", harbol_linkmap_get(p, "2").Int64);
	fprintf(g_harbol_debug_stream, "stk[\"1\"] == %" PRIi64 "\n", harbol_linkmap_get(&i, "1").Int64);
	fprintf(g_harbol_debug_stream, "stk[\"2\"] == %" PRIi64 "\n", harbol_linkmap_get(&i, "2").Int64);
	fputs("\nlinkmap :: looping through all data.\n", g_harbol_debug_stream);
	for( size_t n=0; n<i.Order.Count; n++ ) {
		struct HarbolKeyValPair *l = i.Order.Table[n].Ptr;
		fprintf(g_harbol_debug_stream, "l's value == %" PRIi64 "\n", l->Data.Int64);
	}
	fputs("\n", g_harbol_debug_stream);
	for( size_t n=0; n<p->Order.Count; n++ ) {
		struct HarbolKeyValPair *l = p->Order.Table[n].Ptr;
		fprintf(g_harbol_debug_stream, "l's value == %" PRIi64 "\n", l->Data.Int64);
	}
	
	// test deletion
	fputs("\nlinkmap :: test item deletion.\n", g_harbol_debug_stream);
	harbol_linkmap_delete(p, "2", NULL);
	fprintf(g_harbol_debug_stream, "ptr[\"2\"] == %" PRIi64 "\n", harbol_linkmap_get(p, "2").Int64);
	fputs("\nlinkmap :: looping through all data.\n", g_harbol_debug_stream);
	for( size_t n=0; n<p->Order.Count; n++ ) {
		struct HarbolKeyValPair *l = p->Order.Table[n].Ptr;
		fprintf(g_harbol_debug_stream, "l's value == %" PRIi64 "\n", l->Data.Int64);
	}
	fputs("\nlinkmap :: test item deletion by index.\n", g_harbol_debug_stream);
	harbol_linkmap_delete_by_index(p, 2, NULL);
	for( size_t n=0; n<p->Order.Count; n++ ) {
		struct HarbolKeyValPair *l = p->Order.Table[n].Ptr;
		fprintf(g_harbol_debug_stream, "l's value == %" PRIi64 "\n", l->Data.Int64);
	}
	// test setting by index
	fputs("\nlinkmap :: test item setting by index.\n", g_harbol_debug_stream);
	harbol_linkmap_set_by_index(p, 2, (union HarbolValue){.Int64=500});
	for( size_t n=0; n<p->Order.Count; n++ ) {
		struct HarbolKeyValPair *l = p->Order.Table[n].Ptr;
		fprintf(g_harbol_debug_stream, "l's value == %" PRIi64 "\n", l->Data.Int64);
	}
	// free data
	fputs("\nlinkmap :: test destruction.\n", g_harbol_debug_stream);
	harbol_linkmap_del(&i, NULL);
	fprintf(g_harbol_debug_stream, "i's buckets are null? '%s'\n", i.Order.Table ? "no" : "yes");
	
	harbol_linkmap_del(p, NULL);
	fprintf(g_harbol_debug_stream, "p's buckets are null? '%s'\n", p->Order.Table ? "no" : "yes");
	harbol_linkmap_free(&p, NULL);
	fprintf(g_harbol_debug_stream, "p is null? '%s'\n", p ? "no" : "yes");
}

void test_conversions(void)
{
	if( !g_harbol_debug_stream )
		return;
	
	// inserting five items to all 6 of these collection type data structures.
	struct HarbolVector vec = (struct HarbolVector){0};
	harbol_vector_insert(&vec, (union HarbolValue){.Int64 = 1});
	harbol_vector_insert(&vec, (union HarbolValue){.Int64 = 2});
	harbol_vector_insert(&vec, (union HarbolValue){.Int64 = 3});
	harbol_vector_insert(&vec, (union HarbolValue){.Int64 = 4});
	harbol_vector_insert(&vec, (union HarbolValue){.Int64 = 5});
	
	struct HarbolHashMap map = (struct HarbolHashMap){0};
	harbol_hashmap_insert(&map, "1", (union HarbolValue){.Int64 = 1});
	harbol_hashmap_insert(&map, "2", (union HarbolValue){.Int64 = 2});
	harbol_hashmap_insert(&map, "3", (union HarbolValue){.Int64 = 3});
	harbol_hashmap_insert(&map, "4", (union HarbolValue){.Int64 = 4});
	harbol_hashmap_insert(&map, "5", (union HarbolValue){.Int64 = 5});
	
	struct HarbolUniList unilist = (struct HarbolUniList){0};
	harbol_unilist_insert_at_tail(&unilist, (union HarbolValue){.Int64 = 1});
	harbol_unilist_insert_at_tail(&unilist, (union HarbolValue){.Int64 = 2});
	harbol_unilist_insert_at_tail(&unilist, (union HarbolValue){.Int64 = 3});
	harbol_unilist_insert_at_tail(&unilist, (union HarbolValue){.Int64 = 4});
	harbol_unilist_insert_at_tail(&unilist, (union HarbolValue){.Int64 = 5});
	
	struct HarbolBiList bilist = (struct HarbolBiList){0};
	harbol_bilist_insert_at_tail(&bilist, (union HarbolValue){.Int64 = 1});
	harbol_bilist_insert_at_tail(&bilist, (union HarbolValue){.Int64 = 2});
	harbol_bilist_insert_at_tail(&bilist, (union HarbolValue){.Int64 = 3});
	harbol_bilist_insert_at_tail(&bilist, (union HarbolValue){.Int64 = 4});
	harbol_bilist_insert_at_tail(&bilist, (union HarbolValue){.Int64 = 5});
	
	struct HarbolGraph graph = (struct HarbolGraph){0};
	harbol_graph_insert_val(&graph, (union HarbolValue){.Int64 = 1});
	harbol_graph_insert_val(&graph, (union HarbolValue){.Int64 = 2});
	harbol_graph_insert_val(&graph, (union HarbolValue){.Int64 = 3});
	harbol_graph_insert_val(&graph, (union HarbolValue){.Int64 = 4});
	harbol_graph_insert_val(&graph, (union HarbolValue){.Int64 = 5});
	
	struct HarbolLinkMap linkmap = (struct HarbolLinkMap){0};
	harbol_linkmap_insert(&linkmap, "1", (union HarbolValue){.Int64 = 1});
	harbol_linkmap_insert(&linkmap, "2", (union HarbolValue){.Int64 = 2});
	harbol_linkmap_insert(&linkmap, "3", (union HarbolValue){.Int64 = 3});
	harbol_linkmap_insert(&linkmap, "4", (union HarbolValue){.Int64 = 4});
	harbol_linkmap_insert(&linkmap, "5", (union HarbolValue){.Int64 = 5});
	
	
	// test vector conversion.
	fputs("\ndata struct conversions :: test vector conversions.\n", g_harbol_debug_stream);
	{
		struct HarbolVector *p = NULL;
		
		p = harbol_vector_new_from_unilist(&unilist);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( size_t i=0; i<p->Count; i++ )
				fprintf(g_harbol_debug_stream, "unilist -> ptr[%zu] == %" PRIi64 "\n", i, p->Table[i].Int64);
			harbol_vector_free(&p, NULL);
		}
		
		p = harbol_vector_new_from_bilist(&bilist);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( size_t i=0; i<p->Count; i++ )
				fprintf(g_harbol_debug_stream, "bilist -> ptr[%zu] == %" PRIi64 "\n", i, p->Table[i].Int64);
			harbol_vector_free(&p, NULL);
		}
		
		p = harbol_vector_new_from_hashmap(&map);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( size_t i=0; i<p->Count; i++ )
				fprintf(g_harbol_debug_stream, "map -> ptr[%zu] == %" PRIi64 "\n", i, p->Table[i].Int64);
			harbol_vector_free(&p, NULL);
		}
		
		p = harbol_vector_new_from_graph(&graph);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( size_t i=0; i<p->Count; i++ )
				fprintf(g_harbol_debug_stream, "graph -> ptr[%zu] == %" PRIi64 "\n", i, p->Table[i].Int64);
			harbol_vector_free(&p, NULL);
		}
		
		p = harbol_vector_new_from_linkmap(&linkmap);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( size_t i=0; i<p->Count; i++ )
				fprintf(g_harbol_debug_stream, "linked map -> ptr[%zu] == %" PRIi64 "\n", i, p->Table[i].Int64);
			harbol_vector_free(&p, NULL);
		}
	}
	
	// test map conversion.
	fputs("\ndata struct conversions :: test map conversions.\n", g_harbol_debug_stream);
	{
		struct HarbolHashMap *p = NULL;
		
		p = harbol_hashmap_new_from_unilist(&unilist);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( size_t i=0; i<p->Count; i++ ) {
				struct HarbolVector *vec = p->Table + i;
				for( size_t n=0; n<vec->Count; n++ ) {
					struct HarbolKeyValPair *node = vec->Table[n].Ptr;
					fprintf(g_harbol_debug_stream, "unilist -> ptr[\"%s\"] == %" PRIi64 "\n", node->KeyName.CStr, node->Data.Int64);
				}
			}
			harbol_hashmap_free(&p, NULL);
		}
		
		p = harbol_hashmap_new_from_bilist(&bilist);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( size_t i=0; i<p->Count; i++ ) {
				struct HarbolVector *vec = p->Table + i;
				for( size_t n=0; n<vec->Count; n++ ) {
					struct HarbolKeyValPair *node = vec->Table[n].Ptr;
					fprintf(g_harbol_debug_stream, "bilist -> ptr[\"%s\"] == %" PRIi64 "\n", node->KeyName.CStr, node->Data.Int64);
				}
			}
			harbol_hashmap_free(&p, NULL);
		}
		
		p = harbol_hashmap_new_from_vector(&vec);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( size_t i=0; i<p->Count; i++ ) {
				struct HarbolVector *vec = p->Table + i;
				for( size_t n=0; n<vec->Count; n++ ) {
					struct HarbolKeyValPair *node = vec->Table[n].Ptr;
					fprintf(g_harbol_debug_stream, "vec -> ptr[\"%s\"] == %" PRIi64 "\n", node->KeyName.CStr, node->Data.Int64);
				}
			}
			harbol_hashmap_free(&p, NULL);
		}
		
		p = harbol_hashmap_new_from_graph(&graph);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( size_t i=0; i<p->Count; i++ ) {
				struct HarbolVector *vec = p->Table + i;
				for( size_t n=0; n<vec->Count; n++ ) {
					struct HarbolKeyValPair *node = vec->Table[n].Ptr;
					fprintf(g_harbol_debug_stream, "graph -> ptr[\"%s\"] == %" PRIi64 "\n", node->KeyName.CStr, node->Data.Int64);
				}
			}
			harbol_hashmap_free(&p, NULL);
		}
		
		p = harbol_hashmap_new_from_linkmap(&linkmap);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( size_t i=0; i<p->Count; i++ ) {
				struct HarbolVector *vec = p->Table + i;
				for( size_t n=0; n<vec->Count; n++ ) {
					struct HarbolKeyValPair *node = vec->Table[n].Ptr;
					fprintf(g_harbol_debug_stream, "linked map -> ptr[\"%s\"] == %" PRIi64 "\n", node->KeyName.CStr, node->Data.Int64);
				}
			}
			harbol_hashmap_free(&p, NULL);
		}
	}
	
	// test uni linked list conversion.
	fputs("\ndata struct conversions :: test singly linked list conversions.\n", g_harbol_debug_stream);
	{
		struct HarbolUniList *p = NULL;
		
		p = harbol_unilist_new_from_bilist(&bilist);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( struct HarbolUniListNode *n=p->Head; n; n = n->Next )
				fprintf(g_harbol_debug_stream, "bilist value : %" PRIi64 "\n", n->Data.Int64);
			harbol_unilist_free(&p, NULL);
		}
		
		p = harbol_unilist_new_from_hashmap(&map);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( struct HarbolUniListNode *n=p->Head; n; n = n->Next )
				fprintf(g_harbol_debug_stream, "map value : %" PRIi64 "\n", n->Data.Int64);
			harbol_unilist_free(&p, NULL);
		}
		
		p = harbol_unilist_new_from_vector(&vec);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( struct HarbolUniListNode *n=p->Head; n; n = n->Next )
				fprintf(g_harbol_debug_stream, "vec value : %" PRIi64 "\n", n->Data.Int64);
			harbol_unilist_free(&p, NULL);
		}
		
		p = harbol_unilist_new_from_graph(&graph);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( struct HarbolUniListNode *n=p->Head; n; n = n->Next )
				fprintf(g_harbol_debug_stream, "graph value : %" PRIi64 "\n", n->Data.Int64);
			harbol_unilist_free(&p, NULL);
		}
		
		p = harbol_unilist_new_from_linkmap(&linkmap);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( struct HarbolUniListNode *n=p->Head; n; n = n->Next )
				fprintf(g_harbol_debug_stream, "linked map value : %" PRIi64 "\n", n->Data.Int64);
			harbol_unilist_free(&p, NULL);
		}
	}
	
	// test bi linked list conversion.
	fputs("\ndata struct conversions :: test doubly linked list conversions.\n", g_harbol_debug_stream);
	{
		struct HarbolBiList *p = NULL;
		
		p = harbol_bilist_new_from_unilist(&unilist);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( struct HarbolBiListNode *n=p->Head; n; n = n->Next )
				fprintf(g_harbol_debug_stream, "unilist value : %" PRIi64 "\n", n->Data.Int64);
			harbol_bilist_free(&p, NULL);
		}
		
		p = harbol_bilist_new_from_hashmap(&map);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( struct HarbolBiListNode *n=p->Head; n; n = n->Next )
				fprintf(g_harbol_debug_stream, "map value : %" PRIi64 "\n", n->Data.Int64);
			harbol_bilist_free(&p, NULL);
		}
		
		p = harbol_bilist_new_from_vector(&vec);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( struct HarbolBiListNode *n=p->Head; n; n = n->Next )
				fprintf(g_harbol_debug_stream, "vec value : %" PRIi64 "\n", n->Data.Int64);
			harbol_bilist_free(&p, NULL);
		}
		
		p = harbol_bilist_new_from_graph(&graph);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( struct HarbolBiListNode *n=p->Head; n; n = n->Next )
				fprintf(g_harbol_debug_stream, "graph value : %" PRIi64 "\n", n->Data.Int64);
			harbol_bilist_free(&p, NULL);
		}
		
		p = harbol_bilist_new_from_linkmap(&linkmap);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( struct HarbolBiListNode *n=p->Head; n; n = n->Next )
				fprintf(g_harbol_debug_stream, "linked map value : %" PRIi64 "\n", n->Data.Int64);
			harbol_bilist_free(&p, NULL);
		}
	}
	
	// test graph conversion.
	fputs("\ndata struct conversions :: test graph conversions.\n", g_harbol_debug_stream);
	{
		struct HarbolGraph *p = NULL;
		
		p = harbol_graph_new_from_unilist(&unilist);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( size_t i=0; i<p->Vertices.Count; i++ ) {
				struct HarbolGraphVertex *v = harbol_graph_get_vertex_by_index(p, i);
				fprintf(g_harbol_debug_stream, "unilist value : %" PRIi64 "\n", v->Data.Int64);
			}
			harbol_graph_free(&p, NULL, NULL);
		}
		
		p = harbol_graph_new_from_hashmap(&map);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( size_t i=0; i<p->Vertices.Count; i++ ) {
				struct HarbolGraphVertex *v = harbol_graph_get_vertex_by_index(p, i);
				fprintf(g_harbol_debug_stream, "map value : %" PRIi64 "\n", v->Data.Int64);
			}
			harbol_graph_free(&p, NULL, NULL);
		}
		
		p = harbol_graph_new_from_vector(&vec);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( size_t i=0; i<p->Vertices.Count; i++ ) {
				struct HarbolGraphVertex *v = harbol_graph_get_vertex_by_index(p, i);
				fprintf(g_harbol_debug_stream, "vec value : %" PRIi64 "\n", v->Data.Int64);
			}
			harbol_graph_free(&p, NULL, NULL);
		}
		
		p = harbol_graph_new_from_bilist(&bilist);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( size_t i=0; i<p->Vertices.Count; i++ ) {
				struct HarbolGraphVertex *v = harbol_graph_get_vertex_by_index(p, i);
				fprintf(g_harbol_debug_stream, "bilist value : %" PRIi64 "\n", v->Data.Int64);
			}
			harbol_graph_free(&p, NULL, NULL);
		}
		
		p = harbol_graph_new_from_linkmap(&linkmap);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( size_t i=0; i<p->Vertices.Count; i++ ) {
				struct HarbolGraphVertex *v = harbol_graph_get_vertex_by_index(p, i);
				fprintf(g_harbol_debug_stream, "linked map value : %" PRIi64 "\n", v->Data.Int64);
			}
			harbol_graph_free(&p, NULL, NULL);
		}
	}
	
	// test linked map conversion.
	fputs("\ndata struct conversions :: test linked map conversions.\n", g_harbol_debug_stream);
	{
		struct HarbolLinkMap *p = NULL;
		
		p = harbol_linkmap_new_from_unilist(&unilist);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( size_t n=0; n<p->Order.Count; n++ ) {
				struct HarbolKeyValPair *l = p->Order.Table[n].Ptr;
				fprintf(g_harbol_debug_stream, "unilist value : %" PRIi64 "\n", l->Data.Int64);
			}
			harbol_linkmap_free(&p, NULL);
		}
		
		p = harbol_linkmap_new_from_hashmap(&map);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( size_t n=0; n<p->Order.Count; n++ ) {
				struct HarbolKeyValPair *l = p->Order.Table[n].Ptr;
				fprintf(g_harbol_debug_stream, "map value : %" PRIi64 "\n", l->Data.Int64);
			}
			harbol_linkmap_free(&p, NULL);
		}
		
		p = harbol_linkmap_new_from_vector(&vec);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( size_t n=0; n<p->Order.Count; n++ ) {
				struct HarbolKeyValPair *l = p->Order.Table[n].Ptr;
				fprintf(g_harbol_debug_stream, "vec value : %" PRIi64 "\n", l->Data.Int64);
			}
			harbol_linkmap_free(&p, NULL);
		}
		
		p = harbol_linkmap_new_from_bilist(&bilist);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( size_t n=0; n<p->Order.Count; n++ ) {
				struct HarbolKeyValPair *l = p->Order.Table[n].Ptr;
				fprintf(g_harbol_debug_stream, "bilist value : %" PRIi64 "\n", l->Data.Int64);
			}
			harbol_linkmap_free(&p, NULL);
		}
		
		p = harbol_linkmap_new_from_graph(&graph);
		if( p ) {
			fputs("ptr is valid\n", g_harbol_debug_stream);
			for( size_t n=0; n<p->Order.Count; n++ ) {
				struct HarbolKeyValPair *l = p->Order.Table[n].Ptr;
				fprintf(g_harbol_debug_stream, "graph value : %" PRIi64 "\n", l->Data.Int64);
			}
			harbol_linkmap_free(&p, NULL);
		}
	}
	
	harbol_vector_del(&vec, NULL);
	harbol_hashmap_del(&map, NULL);
	harbol_unilist_del(&unilist, NULL);
	harbol_bilist_del(&bilist, NULL);
	harbol_graph_del(&graph, NULL, NULL);
	harbol_linkmap_del(&linkmap, NULL);
}

void test_harbol_cfg(void)
{
	if( !g_harbol_debug_stream )
		return;
	
	// Test allocation and initializations
	fputs("cfg :: test allocation/initialization.\n", g_harbol_debug_stream);
	struct HarbolLinkMap *cfg = harbol_cfg_parse_cstr("'section': { 'lel': null }");
	fprintf(g_harbol_debug_stream, "cfg ptr valid?: '%s'\n", cfg ? "yes" : "no");
	
	if( cfg ) {
		fputs("\ncfg :: iterating entire config.\n", g_harbol_debug_stream);
		struct HarbolString stringcfg = {NULL, 0};
		harbol_cfg_to_str(cfg, &stringcfg);
		fprintf(g_harbol_debug_stream, "\ncfg :: testing config to string conversion: \n%s\n", stringcfg.CStr);
		harbol_string_del(&stringcfg);
	}
	
	// Test realistic config file string
	fputs("cfg :: test realistic config file string.\n", g_harbol_debug_stream);
	const char *restrict test_cfg = "'root': { \
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
		'phoneNumbers.': { \
			'1' { \
				'type': 'home \\x5c', \
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
		'colors': c[ 0xff, 0xff, 0xff, 0xff ], \
		'origin': v[0.0, 24.43, 25.0], \
		'children': {}, \
		'spouse': null \
	}";
	const clock_t start = clock();
	struct HarbolLinkMap *larger_cfg = harbol_cfg_parse_cstr(test_cfg);
	const clock_t end = clock();
	printf("cfg parsing time: %f\n", (end-start)/(double)CLOCKS_PER_SEC);
	fprintf(g_harbol_debug_stream, "larger_cfg ptr valid?: '%s'\n", larger_cfg ? "yes" : "no");
	if( larger_cfg ) {
		fputs("\ncfg :: iterating realistic config.\n", g_harbol_debug_stream);
		struct HarbolString stringcfg = {NULL, 0};
		harbol_cfg_to_str(larger_cfg, &stringcfg);
		fprintf(g_harbol_debug_stream, "\ncfg :: test config to string conversion:\n%s\n", stringcfg.CStr);
		harbol_string_del(&stringcfg);
		
		fputs("\ncfg :: test retrieving sub section of realistic config.\n", g_harbol_debug_stream);
		struct HarbolLinkMap *phone_numbers1 = harbol_cfg_get_section_by_key(larger_cfg, "root.phoneNumbers/..1");
		harbol_cfg_to_str(phone_numbers1, &stringcfg);
		fprintf(g_harbol_debug_stream, "\nphone_numbers to string conversion: \n%s\n", stringcfg.CStr);
		harbol_string_del(&stringcfg);
		if( phone_numbers1 ) {
			fputs("\ncfg :: iterating phone_numbers1 subsection.\n", g_harbol_debug_stream);
			struct HarbolString stringcfg = {NULL, 0};
			harbol_cfg_to_str(phone_numbers1, &stringcfg);
			fprintf(g_harbol_debug_stream, "\nphone_numbers1 to string conversion: \n%s\n", stringcfg.CStr);
			harbol_string_del(&stringcfg);
		}
		
		fputs("\ncfg :: test retrieving string value from subsection of realistic config.\n", g_harbol_debug_stream);
		char *restrict type = harbol_cfg_get_str_by_key(larger_cfg, "root.phoneNumbers\..1.type");
		fprintf(g_harbol_debug_stream, "root.phoneNumbers\..1.type type valid?: '%s'\n", type ? "yes" : "no");
		if( type ) {
			fprintf(g_harbol_debug_stream, "root.phoneNumbers\..1.type: %s\n", type);
		}
		type = harbol_cfg_get_str_by_key(larger_cfg, "root.age");
		fprintf(g_harbol_debug_stream, "root.age string type valid?: '%s'\n", type ? "yes" : "no");
		if( type ) {
			fprintf(g_harbol_debug_stream, "root.age: %s\n", type);
		}
		int64_t age; harbol_cfg_get_int_by_key(larger_cfg, "root.age", &age);
		fprintf(g_harbol_debug_stream, "root.age int?: '%" PRIi64 "'\n", age);
		double money; harbol_cfg_get_float_by_key(larger_cfg, "root.money", &money);
		fprintf(g_harbol_debug_stream, "root.money float?: '%f'\n", money);
		
		union HarbolColor color = {0};
		const bool gotcolor = harbol_cfg_get_color_by_key(larger_cfg, "root.colors", &color);
		fprintf(g_harbol_debug_stream, "root.colors color?: '%s': '[%u, %u, %u, %u]'\n", gotcolor ? "yes" : "no", color.Struc.R, color.Struc.G, color.Struc.B, color.Struc.A);
		
		fputs("\ncfg :: test override setting an existing key-value from null to a string type.\n", g_harbol_debug_stream);
		harbol_cfg_set_str_by_key(larger_cfg, "root.spouse", "Jane Smith", true);
		{
			struct HarbolString stringcfg = {NULL, 0};
			harbol_cfg_to_str(larger_cfg, &stringcfg);
			fprintf(g_harbol_debug_stream, "\nadded spouse!: \n%s\n", stringcfg.CStr);
			harbol_string_del(&stringcfg);
		}
		
		fputs("\ncfg :: test building cfg file!\n", g_harbol_debug_stream);
		fprintf(g_harbol_debug_stream, "\nconfig construction result: '%s'\n", harbol_cfg_build_file(larger_cfg, "large_cfg.ini", true) ? "success" : "failure");
		
		fputs("\ncfg :: test setting a key back to null\n", g_harbol_debug_stream);
		harbol_cfg_set_key_to_null(larger_cfg, "root.spouse");
		{
			struct HarbolString stringcfg = {NULL, 0};
			harbol_cfg_to_str(larger_cfg, &stringcfg);
			fprintf(g_harbol_debug_stream, "\nremoved spouse!: \n%s\n", stringcfg.CStr);
			harbol_string_del(&stringcfg);
		}
		
		fputs("\ncfg :: test getting the type of a key\n", g_harbol_debug_stream);
		enum HarbolCfgType n = 0;
		harbol_cfg_get_key_type(larger_cfg, "root.phoneNumbers/.", &n);
		fprintf(g_harbol_debug_stream, "Type of root.phoneNumbers/.: %i\n", n);
		harbol_cfg_get_key_type(larger_cfg, "root.spouse", &n);
		fprintf(g_harbol_debug_stream, "Type of root.spouse: %i\n", n);
		harbol_cfg_get_key_type(larger_cfg, "root.money", &n);
		fprintf(g_harbol_debug_stream, "Type of root.money: %i\n", n);
		harbol_cfg_get_key_type(larger_cfg, "root.origin", &n);
		fprintf(g_harbol_debug_stream, "Type of root.origin: %i\n", n);
		
		fputs("\ncfg :: test adding other cfg as a new section\n", g_harbol_debug_stream);
		{
			harbol_linkmap_insert(larger_cfg, "former lovers", (union HarbolValue){ .VarPtr=harbol_variant_new((union HarbolValue){ .LinkMapPtr=cfg }, HarbolTypeLinkMap) });
			struct HarbolString stringcfg = {NULL, 0};
			harbol_cfg_to_str(larger_cfg, &stringcfg);
			fprintf(g_harbol_debug_stream, "\nremoved spouse!: \n%s\n", stringcfg.CStr);
			harbol_string_del(&stringcfg);
		}
		fputs("\ncfg :: test building newer cfg file!\n", g_harbol_debug_stream);
		fprintf(g_harbol_debug_stream, "\nconfig construction result: '%s'\n", harbol_cfg_build_file(larger_cfg, "large_cfg_new_sect.ini", true) ? "success" : "failure");
		harbol_cfg_free(&larger_cfg);
		fprintf(g_harbol_debug_stream, "cfg ptr valid?: '%s'\n", cfg ? "yes" : "no");
	}
	cfg = NULL;
	fputs("\ncfg :: test destruction.\n", g_harbol_debug_stream);
	harbol_cfg_free(&cfg);
	fprintf(g_harbol_debug_stream, "cfg ptr valid?: '%s'\n", cfg ? "yes" : "no");
}



void on_plugin_load(struct HarbolPluginManager *manager, struct HarbolPlugin **const pluginref)
{
	(void)manager;
	/* no way manager nor the plugin reference would be invalid... */
	struct HarbolPlugin *const plugin = *pluginref;
	struct HarbolString load_func_name; harbol_string_init_cstr(&load_func_name, harbol_plugin_get_name(plugin));
	harbol_string_add_cstr(&load_func_name, "_load");
	fprintf(g_harbol_debug_stream, "\nplugin manager :: on_plugin_load - plugin name :: '%s'\n", harbol_plugin_get_name(plugin));
	fprintf(g_harbol_debug_stream, "\nplugin manager :: on_plugin_load :: getting function '%s'\n", harbol_string_get_cstr(&load_func_name));
	int32_t (*const onload)() = (int32_t(*)())(intptr_t)harbol_plugin_get_sym(plugin, harbol_string_get_cstr(&load_func_name));
	fprintf(g_harbol_debug_stream, "\nplugin manager :: on_plugin_load :: function ptr valid? '%s'\n", onload ? "yes" : "no");
	if( onload )
		fprintf(g_harbol_debug_stream, "\nplugin manager :: on_plugin_load :: function return value: '%i'\n", (*onload)());
	
	harbol_string_del(&load_func_name);
}

void on_plugin_unload(struct HarbolPluginManager *manager, struct HarbolPlugin **const pluginref)
{
	(void)manager;
	/* no way manager nor the plugin reference would be invalid... */
	struct HarbolPlugin *const plugin = *pluginref;
	fprintf(g_harbol_debug_stream, "\nplugin manager :: on_plugin_unload - plugin name :: '%s'\n", harbol_plugin_get_name(plugin));
	
	struct HarbolString unload_func_name; harbol_string_init_cstr(&unload_func_name, harbol_plugin_get_name(plugin));
	harbol_string_add_cstr(&unload_func_name, "_unload");
	fprintf(g_harbol_debug_stream, "\nplugin manager :: on_plugin_unload :: getting function '%s'\n", harbol_string_get_cstr(&unload_func_name));
	void (*const onunload)() = (void(*)())(intptr_t)harbol_plugin_get_sym(plugin, harbol_string_get_cstr(&unload_func_name));
	fprintf(g_harbol_debug_stream, "\nplugin manager :: on_plugin_unload :: function ptr valid? '%s'\n", onunload ? "yes" : "no");
	if( onunload )
		(*onunload)();
	harbol_string_del(&unload_func_name);
}

void test_harbol_plugins(void)
{
	if( !g_harbol_debug_stream )
		return;
	fputs("plugin manager :: test init.\n", g_harbol_debug_stream);
	struct HarbolPluginManager pm = (struct HarbolPluginManager){0};
	fprintf(g_harbol_debug_stream, "\nplugin manager :: initialization good?: '%s'\n", harbol_plugin_manager_init(&pm, "test_harbol_plugins/", true, on_plugin_load) ? "yes" : "no");
	
	fputs("\nplugin manager :: test deleting plugin by name.\n", g_harbol_debug_stream);
	harbol_plugin_manager_delete_plugin_by_name(&pm, "test_plugin", on_plugin_unload);
	
	fputs("\nplugin manager :: test reloading all loaded plugins.\n", g_harbol_debug_stream);
	harbol_plugin_manager_reload_plugins(&pm, NULL, NULL);
	
	fputs("\nplugin manager :: test destruction.\n", g_harbol_debug_stream);
	harbol_plugin_manager_del(&pm, on_plugin_unload);
}


static int first_thread_test(void *const vargp) 
{
	(void)vargp;
	puts("Hello from Thread!"); 
	return 0;
}

static HarbolMutex g_mutex;

static int second_thread_test_mutex(void *const vargp) 
{
	harbol_mutex_lock(&g_mutex);
	struct {
		int i[3];
		double g;
	} *const restrict p = vargp;
	p->i[0] = 2;
	p->i[1] = p->i[0] * 2;
	p->i[2] = p->i[1] * 2;
	p->g = 5.0;
	harbol_mutex_unlock(&g_mutex);
	return 0;
}

static int second_thread_test_mutex2(void *const vargp) 
{
	harbol_mutex_lock(&g_mutex);
	struct {
		int i[3];
		double g;
	} *const restrict p = vargp;
	p->i[0] = 5;
	p->i[1] = p->i[0] * 2;
	p->i[2] = p->i[1] * 2;
	p->g = 1.0;
	harbol_mutex_unlock(&g_mutex);
	return 0;
}

void test_harbol_multithreading(void)
{
	if( !g_harbol_debug_stream )
		return;
	
	fputs("multithreading :: test thread init.\n", g_harbol_debug_stream);
	struct HarbolThread thread = {0};
	const enum HarbolThreadRes res = harbol_thread_init(&thread, first_thread_test, NULL);
	fprintf(g_harbol_debug_stream, "\nmultithreading :: initialization good?: '%s'\n", res==HarbolThreadResSuccess ? "yes" : "no");
	fprintf(g_harbol_debug_stream, "\nmultithreading :: thread joining good?: '%s'\n", harbol_thread_join(&thread, NULL)==HarbolThreadResSuccess ? "yes" : "no");
	
	fputs("\nmultithreading :: test mutex init.\n", g_harbol_debug_stream);
	struct { int i[3]; double g; } f = {0};
	harbol_mutex_init(&g_mutex, HarbolMutexTypeRecursive);
	struct HarbolThread thread2 = {0};
	harbol_thread_init(&thread2, second_thread_test_mutex2, &f);
	harbol_thread_init(&thread, second_thread_test_mutex, &f);
	
	harbol_thread_join(&thread, NULL);
	fprintf(g_harbol_debug_stream, "\nmultithreading :: mutex data: f.i[0] = '%i', f.i[1] = '%i', f.i[2] = '%i', f.g = '%f'\n", f.i[0], f.i[1], f.i[2], f.g);
	harbol_thread_join(&thread2, NULL);
	fprintf(g_harbol_debug_stream, "\nmultithreading :: mutex data: f.i[0] = '%i', f.i[1] = '%i', f.i[2] = '%i', f.g = '%f'\n", f.i[0], f.i[1], f.i[2], f.g);
}
