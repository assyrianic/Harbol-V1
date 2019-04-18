#ifndef HARBOL_INCLUDED
#	define HARBOL_INCLUDED
#	define HARBOL_VERSION_MAJOR    1
#	define HARBOL_VERSION_MINOR    0
#	define HARBOL_VERSION_PATCH    0
#	define STR_HELPER(x)    #x
#	define STR(x)    STR_HELPER(x)
#	define HARBOL_VERSION_STRING \
			STR(HARBOL_VERSION_MAJOR) STR(HARBOL_VERSION_MINOR) STR(HARBOL_VERSION_PATCH)


/* Check if Windows */
#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#	ifndef OS_WINDOWS
#		define OS_WINDOWS 1
#	endif

/* Check if Linux/UNIX & FreeBSD */
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__linux__) || defined(linux) || defined(__linux) || defined(__freeBSD__)
#	ifndef OS_LINUX_UNIX
#		define OS_LINUX_UNIX 1
#	endif

/* Check if Android */
#elif defined(__ANDROID__)
#	ifndef OS_ANDROID
#		define OS_ANDROID 1
#	endif

/* Check if Solaris/SunOS */
#elif defined(sun) || defined(__sun)
#	if defined(__SVR4) || defined(__svr4__)
#		ifndef OS_SOLARIS
#			define OS_SOLARIS 1
#		endif
#	else
#		ifndef OS_SUNOS
#			define OS_SUNOS 1
#		endif
#	endif

/* Check if Macintosh/MacOS */
#elif defined(macintosh) || defined(Macintosh) || defined(__APPLE__)
#	ifndef OS_MAC
#		define OS_MAC 1
#	endif

#endif /* end OS checks */

/* check what compiler we got */
#if defined(__clang__)
#	ifndef COMPILER_CLANG
#		define COMPILER_CLANG
#	endif
#elif defined(__GNUC__) || defined(__GNUG__)
#	ifndef COMPILER_GCC
#		define COMPILER_GCC
#	endif
#elif defined(_MSC_VER)
#	ifndef COMPILER_MSVC
#		define COMPILER_MSVC
#	endif
#elif defined(__INTEL_COMPILER)
#	ifndef COMPILER_INTEL
#		define COMPILER_INTEL
#	endif
#endif /* end compiler check macros */

/* set up the C standard macros! */
#ifdef __STDC__
#	ifndef C89
#		define C89
#	endif
#	ifdef __STDC_VERSION__
#		ifndef C90
#			define C90
#		endif
#		if (__STDC_VERSION__ >= 199409L)
#			ifndef C94
#				define C94
#			endif
#		endif
#		if (__STDC_VERSION__ >= 199901L)
#			ifndef C99
#				define C99
#			endif
#		endif
#		if (__STDC_VERSION__ >= 201112L)
#			ifndef C11
#				define C11
#			endif
#		endif
#		if (__STDC_VERSION__ >= 201710L)
#			ifndef C18
#				define C18
#			endif
#		endif
#	endif
#endif


/* setup RAII destructor macro if possibru to mark functions as cleaner-uppers. */
#ifndef RAII_DTOR
#	if defined(COMPILER_CLANG) || defined(COMPILER_GCC)
#		define RAII_DTOR(func) __attribute__ ((cleanup((func))))
#	else
#		define RAII_DTOR(func)
#	endif
#endif

/* setup macro to mark a param as "cannot or can never be NULL". */
#ifndef NEVER_NULL
#	if defined(COMPILER_CLANG) || defined(COMPILER_GCC)
#		define NEVER_NULL(...) __attribute__( (nonnull(__VA_ARGS__)) )
#	else
#		define NEVER_NULL(...)
#	endif
#endif
#ifndef NO_NULL
#	if defined(COMPILER_CLANG) || defined(COMPILER_GCC)
#		define NO_NULL __attribute__((nonnull))
#	else
#		define NO_NULL
#	endif
#endif

/* setup macro to mark a function as never returning a null pointer. */
#ifndef NONNULL_RET
#	if defined(COMPILER_CLANG) || defined(COMPILER_GCC)
#		define NONNULL_RET __attribute__((returns_nonnull))
#	else
#		define NONNULL_RET
#	endif
#endif

/* DLL crap to deal with Windows asinine poppycock DLL construction. */
#ifdef HARBOL_DLL
#	ifndef HARBOL_LIB 
#		define HARBOL_EXPORT __declspec(dllimport)
#	else
#		define HARBOL_EXPORT __declspec(dllexport)
#	endif
#else
#	define HARBOL_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#	ifdef OS_WINDOWS
#		ifndef restrict
#			define restrict __restrict
#		endif
#	else
#		ifndef restrict
#			define restrict __restrict__
#		endif
#	endif
#endif

/* includes, duh. */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdalign.h>
#include <time.h>
#include <iso646.h>

#ifdef OS_WINDOWS
#	include <windows.h>
#else
#	include <pthread.h>
#endif


typedef bool fnHarbolDestructor(void *ptr_ref); /* use pointer to a pointer. */

struct HarbolVariant;
struct HarbolString;
struct HarbolVector;
struct HarbolKeyValPair;
struct HarbolHashMap;
struct HarbolUniList;
struct HarbolBiList;
struct HarbolByteBuffer;
struct HarbolTuple;
struct HarbolGraphEdge;
struct HarbolGraphVertex;
struct HarbolGraph;
struct HarbolTree;
struct HarbolLinkMap;
struct HarbolMemoryPool;

typedef union HarbolValue {
	bool Bool, BoolArray[8], *BoolPtr;
	int8_t Int8, Int8Array[8], *Int8Ptr;
	int16_t Int16, Int16Array[4], *Int16Ptr;
	int32_t Int32, Int32Array[2], *Int32Ptr;
	int64_t Int64, *Int64Ptr;
	
	uint8_t UInt8, UInt8Array[8], *UInt8Ptr;
	uint16_t UInt16, UInt16Array[4], *UInt16Ptr;
	uint32_t UInt32, UInt32Array[2], *UInt32Ptr;
	uint64_t UInt64, *UInt64Ptr;
	size_t IntSize, *IntSizePtr;
	
	intptr_t IntNative;
	uintptr_t UIntNative;
	
	float Float, FloatArray[2], *FloatPtr;
	double Double, *DoublePtr;
	
	void *Ptr;
	union HarbolValue *SelfPtr;
	struct HarbolVariant *VarPtr;
	struct HarbolString *StrObjPtr;
	struct HarbolVector *VecPtr;
	struct HarbolKeyValPair *KvPairPtr;
	struct HarbolHashMap *MapPtr;
	struct HarbolUniList *UniListPtr;
	struct HarbolBiList *BiListPtr;
	struct HarbolByteBuffer *ByteBufferPtr;
	struct HarbolTuple *TuplePtr;
	struct HarbolGraphEdge *GraphEdgePtr;
	struct HarbolGraphVertex *GraphVertPtr;
	struct HarbolGraph *GraphPtr;
	struct HarbolTree *TreePtr;
	struct HarbolLinkMap *LinkMapPtr;
} HarbolValue;

#ifdef C11
#	ifndef harbol_value_set_val
#		define harbol_value_set_val(val, data) \
			_Generic((data), \
				bool: val.Bool=data, bool*: val.BoolPtr=data, \
				int8_t: val.Int8=data, int8_t*: val.Int8Ptr=data, \
				int16_t: val.Int16=data, int16_t*: val.Int16Ptr=data, \
				int32_t: val.Int32=data, int32_t*: val.Int32Ptr=data, \
				int64_t: val.Int64=data, int64_t*: val.Int64Ptr=data, \
				\
				uint8_t: val.UInt8=data, int8_t*: val.UInt8Ptr=data, \
				uint16_t: val.UInt16=data, int16_t*: val.UInt16Ptr=data, \
				uint32_t: val.UInt32=data, int32_t*: val.UInt32Ptr=data, \
				uint64_t: val.UInt64=data, uint64_t*: val.UInt64Ptr=data, \
				\
				size_t: val.IntSize=data, size_t*: val.IntSizePtr=data, \
				intptr_t: val.IntNative=data, uintptr_t: val.UIntNative=data, \
				\
				float: val.Float=data, float*: val.FloatPtr=data, \
				double: val.Double=data, double*: val.DoublePtr=data, \
				\
				void*: val.Ptr=data, \
				union HarbolValue*: val.SelfPtr=data, \
				struct HarbolVariant*: val.VarPtr=data, \
				struct HarbolString*: val.StrObjPtr=data, \
				struct HarbolVector*: val.VecPtr=data, \
				struct HarbolKeyValPair*: val.KvPairPtr=data, \
				struct HarbolHashMap*: val.MapPtr=data, \
				struct HarbolUniList*: val.UniListPtr=data, \
				struct HarbolBiList*: val.BiListPtr=data, \
				struct HarbolByteBuffer*: val.ByteBufferPtr=data, \
				struct HarbolTuple*: val.TuplePtr=data, \
				struct HarbolGraphEdge*: val.GraphEdgePtr=data, \
				struct HarbolGraphVertex*: val.GraphVertPtr=data, \
				struct HarbolGraph*: val.GraphPtr=data, \
				struct HarbolTree*: val.TreePtr=data, \
				struct HarbolLinkMap*: val.LinkMapPtr=data, \
				default: (void)val \
			)
#	endif
#	ifndef harbol_value_get_val
#		define harbol_value_get_val(val, var) \
			var = _Generic((var), \
				bool: val.Bool, bool*: val.BoolPtr, \
				int8_t: val.Int8, int8_t*: val.Int8Ptr, \
				int16_t: val.Int16, int16_t*: val.Int16Ptr, \
				int32_t: val.Int32, int32_t*: val.Int32Ptr, \
				int64_t: val.Int64, int64_t*: val.Int64Ptr, \
				\
				uint8_t: val.UInt8, uint8_t*: val.UInt8Ptr, \
				uint16_t: val.UInt16, uint16_t*: val.UInt16Ptr, \
				uint32_t: val.UInt32, uint32_t*: val.UInt32Ptr, \
				uint64_t: val.UInt64, uint64_t*: val.UInt64Ptr, \
				\
				size_t: val.IntSize, size_t*: val.IntSizePtr, \
				intptr_t: val.IntNative, uintptr_t: val.UIntNative, \
				\
				float: val.Float, float*: val.FloatPtr, \
				double: val.Double, double*: val.DoublePtr, \
				\
				void*: val.Ptr, \
				union HarbolValue*: val.SelfPtr, \
				struct HarbolVariant*: val.VarPtr, \
				struct HarbolString*: val.StrObjPtr, \
				struct HarbolVector*: val.VecPtr, \
				struct HarbolKeyValPair*: val.KvPairPtr, \
				struct HarbolHashMap*: val.MapPtr, \
				struct HarbolUniList*: val.UniListPtr, \
				struct HarbolBiList*: val.BiListPtr, \
				struct HarbolByteBuffer*: val.ByteBufferPtr, \
				struct HarbolTuple*: val.TuplePtr, \
				struct HarbolGraphEdge*: val.GraphEdgePtr, \
				struct HarbolGraphVertex*: val.GraphVertPtr, \
				struct HarbolGraph*: val.GraphPtr, \
				struct HarbolTree*: val.TreePtr, \
				struct HarbolLinkMap*: val.LinkMapPtr, \
				default: (void)val \
			)
#	endif
#endif

/************* C++ Style Automated String (stringobj.c) *************/
typedef struct HarbolString {
	char *CStr;
	size_t Len;
} HarbolString;

HARBOL_EXPORT struct HarbolString *harbol_string_new(void);
HARBOL_EXPORT struct HarbolString *harbol_string_new_cstr(const char cstr[]);
HARBOL_EXPORT void harbol_string_del(struct HarbolString *str);
HARBOL_EXPORT bool harbol_string_free(struct HarbolString **strref);
HARBOL_EXPORT void harbol_string_init(struct HarbolString *str);
HARBOL_EXPORT void harbol_string_init_cstr(struct HarbolString *str, const char cstr[]);
HARBOL_EXPORT void harbol_string_add_char(struct HarbolString *str, char chr);
HARBOL_EXPORT void harbol_string_add_str(struct HarbolString *strA, const struct HarbolString *strB);
HARBOL_EXPORT void harbol_string_add_cstr(struct HarbolString *str, const char cstr[]);
HARBOL_EXPORT char *harbol_string_get_cstr(const struct HarbolString *str);
HARBOL_EXPORT size_t harbol_string_get_len(const struct HarbolString *str);
HARBOL_EXPORT void harbol_string_copy_str(struct HarbolString *strA, const struct HarbolString *strB);
HARBOL_EXPORT void harbol_string_copy_cstr(struct HarbolString *str, const char cstr[]);
HARBOL_EXPORT int32_t harbol_string_format(struct HarbolString *str, const char fmt[], ...);
HARBOL_EXPORT int32_t harbol_string_cmpcstr(const struct HarbolString *str, const char cstr[]);
HARBOL_EXPORT int32_t harbol_string_cmpstr(const struct HarbolString *strA, const struct HarbolString *strB);
HARBOL_EXPORT int32_t harbol_string_ncmpcstr(const struct HarbolString *str, const char cstr[], size_t len);
HARBOL_EXPORT int32_t harbol_string_ncmpstr(const struct HarbolString *strA, const struct HarbolString *strB, size_t len);
HARBOL_EXPORT bool harbol_string_is_empty(const struct HarbolString *str);
HARBOL_EXPORT bool harbol_string_reserve(struct HarbolString *str, size_t size);
HARBOL_EXPORT char *harbol_string_fgets(struct HarbolString *str, FILE *file);
HARBOL_EXPORT void harbol_string_clear(struct HarbolString *str);
HARBOL_EXPORT bool harbol_string_read_file(struct HarbolString *str, FILE *file);
HARBOL_EXPORT bool harbol_string_replace(struct HarbolString *str, char to_replace, char with);
/***************/


/************* Vector / Dynamic Array (vector.c) *************/
typedef struct HarbolVector {
	union HarbolValue *Table;
	size_t Len, Count;
} HarbolVector;

HARBOL_EXPORT void harbol_generic_vector_resizer(void *table_ref, size_t *curr_len, size_t element_size);
HARBOL_EXPORT void harbol_generic_vector_truncater(void *table_ref, size_t *curr_len, size_t element_size);

HARBOL_EXPORT struct HarbolVector *harbol_vector_new(void);
HARBOL_EXPORT void harbol_vector_init(struct HarbolVector *vec);
HARBOL_EXPORT void harbol_vector_del(struct HarbolVector *vec, fnHarbolDestructor *dtor);
HARBOL_EXPORT void harbol_vector_free(struct HarbolVector **vecref, fnHarbolDestructor *dtor);

HARBOL_EXPORT size_t harbol_vector_get_len(const struct HarbolVector *vec);
HARBOL_EXPORT size_t harbol_vector_get_count(const struct HarbolVector *vec);
HARBOL_EXPORT union HarbolValue *harbol_vector_get_iter(const struct HarbolVector *vec);
HARBOL_EXPORT union HarbolValue *harbol_vector_get_iter_end_len(const struct HarbolVector *vec);
HARBOL_EXPORT union HarbolValue *harbol_vector_get_iter_end_count(const struct HarbolVector *vec);
HARBOL_EXPORT void harbol_vector_resize(struct HarbolVector *vec);
HARBOL_EXPORT void harbol_vector_truncate(struct HarbolVector *vec);

HARBOL_EXPORT bool harbol_vector_insert(struct HarbolVector *vec, union HarbolValue val);
HARBOL_EXPORT union HarbolValue harbol_vector_pop(struct HarbolVector *vec);
HARBOL_EXPORT union HarbolValue harbol_vector_get(const struct HarbolVector *vec, size_t index);
HARBOL_EXPORT void harbol_vector_set(struct HarbolVector *vec, size_t index, union HarbolValue val);

HARBOL_EXPORT void harbol_vector_delete(struct HarbolVector *vec, size_t index, fnHarbolDestructor *dtor);
HARBOL_EXPORT void harbol_vector_add(struct HarbolVector *vecA, const struct HarbolVector *vecB);
HARBOL_EXPORT void harbol_vector_copy(struct HarbolVector *vecA, const struct HarbolVector *vecB);

HARBOL_EXPORT void harbol_vector_from_unilist(struct HarbolVector *vec, const struct HarbolUniList *list);
HARBOL_EXPORT void harbol_vector_from_bilist(struct HarbolVector *vec, const struct HarbolBiList *list);
HARBOL_EXPORT void harbol_vector_from_hashmap(struct HarbolVector *vec, const struct HarbolHashMap *map);
HARBOL_EXPORT void harbol_vector_from_graph(struct HarbolVector *vec, const struct HarbolGraph *graph);
HARBOL_EXPORT void harbol_vector_from_linkmap(struct HarbolVector *vec, const struct HarbolLinkMap *map);

HARBOL_EXPORT struct HarbolVector *harbol_vector_new_from_unilist(const struct HarbolUniList *list);
HARBOL_EXPORT struct HarbolVector *harbol_vector_new_from_bilist(const struct HarbolBiList *list);
HARBOL_EXPORT struct HarbolVector *harbol_vector_new_from_hashmap(const struct HarbolHashMap *map);
HARBOL_EXPORT struct HarbolVector *harbol_vector_new_from_graph(const struct HarbolGraph *graph);
HARBOL_EXPORT struct HarbolVector *harbol_vector_new_from_linkmap(const struct HarbolLinkMap *map);
/***************/


/************* String Key Hashmap (hashmap.c) *************/
typedef struct HarbolKeyValPair {
	struct HarbolString KeyName;
	union HarbolValue Data;
} HarbolKeyValPair;

HARBOL_EXPORT struct HarbolKeyValPair *harbol_kvpair_new(void);
HARBOL_EXPORT struct HarbolKeyValPair *harbol_kvpair_new_strval(const char cstr[], union HarbolValue val);

HARBOL_EXPORT void harbol_kvpair_del(struct HarbolKeyValPair *kv, fnHarbolDestructor *dtor);
HARBOL_EXPORT void harbol_kvpair_free(struct HarbolKeyValPair **kvref, fnHarbolDestructor *dtor);


typedef struct HarbolHashMap {
	struct HarbolVector *Table; /* a vector of vectors! */
	size_t Len, Count;
} HarbolHashMap;

HARBOL_EXPORT size_t generic_hash(const char key[]);
HARBOL_EXPORT struct HarbolHashMap *harbol_hashmap_new(void);
HARBOL_EXPORT void harbol_hashmap_init(struct HarbolHashMap *map);
HARBOL_EXPORT void harbol_hashmap_del(struct HarbolHashMap *map, fnHarbolDestructor *dtor);
HARBOL_EXPORT void harbol_hashmap_free(struct HarbolHashMap **mapref, fnHarbolDestructor *dtor);
HARBOL_EXPORT size_t harbol_hashmap_get_count(const struct HarbolHashMap *map);
HARBOL_EXPORT size_t harbol_hashmap_get_len(const struct HarbolHashMap *map);
HARBOL_EXPORT bool harbol_hashmap_rehash(struct HarbolHashMap *map);

HARBOL_EXPORT bool harbol_hashmap_insert_node(struct HarbolHashMap *map, struct HarbolKeyValPair *kv);
HARBOL_EXPORT bool harbol_hashmap_insert(struct HarbolHashMap *map, const char key[], union HarbolValue val);

HARBOL_EXPORT union HarbolValue harbol_hashmap_get(const struct HarbolHashMap *map, const char key[]);
HARBOL_EXPORT void harbol_hashmap_set(struct HarbolHashMap *map, const char key[], union HarbolValue val);

HARBOL_EXPORT void harbol_hashmap_delete(struct HarbolHashMap *map, const char key[], fnHarbolDestructor *dtor);
HARBOL_EXPORT bool harbol_hashmap_has_key(const struct HarbolHashMap *map, const char key[]);
HARBOL_EXPORT struct HarbolKeyValPair *harbol_hashmap_get_node(const struct HarbolHashMap *map, const char key[]);
struct HarbolVector *harbol_hashmap_get_buckets(const struct HarbolHashMap *map);

HARBOL_EXPORT void harbol_hashmap_from_unilist(struct HarbolHashMap *map, const struct HarbolUniList *list);
HARBOL_EXPORT void harbol_hashmap_from_bilist(struct HarbolHashMap *map, const struct HarbolBiList *list);
HARBOL_EXPORT void harbol_hashmap_from_vector(struct HarbolHashMap *map, const struct HarbolVector *vec);
HARBOL_EXPORT void harbol_hashmap_from_graph(struct HarbolHashMap *map, const struct HarbolGraph *graph);
HARBOL_EXPORT void harbol_hashmap_from_linkmap(struct HarbolHashMap *map, const struct HarbolLinkMap *linkmap);

HARBOL_EXPORT struct HarbolHashMap *harbol_hashmap_new_from_unilist(const struct HarbolUniList *list);
HARBOL_EXPORT struct HarbolHashMap *harbol_hashmap_new_from_bilist(const struct HarbolBiList *list);
HARBOL_EXPORT struct HarbolHashMap *harbol_hashmap_new_from_vector(const struct HarbolVector *vec);
HARBOL_EXPORT struct HarbolHashMap *harbol_hashmap_new_from_graph(const struct HarbolGraph *graph);
HARBOL_EXPORT struct HarbolHashMap *harbol_hashmap_new_from_linkmap(const struct HarbolLinkMap *linkmap);
/***************/


/************* Singly Linked List (unilist.c) *************/
typedef struct HarbolUniListNode {
	union HarbolValue Data;
	struct HarbolUniListNode *Next;
} HarbolUniListNode;

HARBOL_EXPORT struct HarbolUniListNode *harbol_unilistnode_new(void);
HARBOL_EXPORT struct HarbolUniListNode *harbol_unilistnode_new_val(union HarbolValue val);
HARBOL_EXPORT void harbol_unilistnode_del(struct HarbolUniListNode *node, fnHarbolDestructor *dtor);
HARBOL_EXPORT void harbol_unilistnode_free(struct HarbolUniListNode **noderef, fnHarbolDestructor *dtor);
HARBOL_EXPORT struct HarbolUniListNode *harbol_unilistnode_get_next_node(const struct HarbolUniListNode *node);
HARBOL_EXPORT union HarbolValue harbol_unilistnode_get_val(const struct HarbolUniListNode *node);


typedef struct HarbolUniList {
	struct HarbolUniListNode *Head, *Tail;
	size_t Len;
} HarbolUniList;

HARBOL_EXPORT struct HarbolUniList *harbol_unilist_new(void);
HARBOL_EXPORT void harbol_unilist_del(struct HarbolUniList *list, fnHarbolDestructor *dtor);
HARBOL_EXPORT void harbol_unilist_free(struct HarbolUniList **listref, fnHarbolDestructor *dtor);
HARBOL_EXPORT void harbol_unilist_init(struct HarbolUniList *list);

HARBOL_EXPORT size_t harbol_unilist_get_len(const struct HarbolUniList *list);
HARBOL_EXPORT bool harbol_unilist_insert_node_at_head(struct HarbolUniList *list, struct HarbolUniListNode *node);
HARBOL_EXPORT bool harbol_unilist_insert_node_at_tail(struct HarbolUniList *list, struct HarbolUniListNode *node);
HARBOL_EXPORT bool harbol_unilist_insert_node_at_index(struct HarbolUniList *list, struct HarbolUniListNode *node, size_t index);
HARBOL_EXPORT bool harbol_unilist_insert_at_head(struct HarbolUniList *list, union HarbolValue val);
HARBOL_EXPORT bool harbol_unilist_insert_at_tail(struct HarbolUniList *list, union HarbolValue val);
HARBOL_EXPORT bool harbol_unilist_insert_at_index(struct HarbolUniList *list, union HarbolValue val, size_t index);

HARBOL_EXPORT struct HarbolUniListNode *harbol_unilist_get_node_by_index(const struct HarbolUniList *list, size_t index);
HARBOL_EXPORT struct HarbolUniListNode *harbol_unilist_get_node_by_val(const struct HarbolUniList *list, union HarbolValue val);
HARBOL_EXPORT union HarbolValue harbol_unilist_get_val(const struct HarbolUniList *list, size_t index);
HARBOL_EXPORT void harbol_unilist_set_val(struct HarbolUniList *list, size_t index, union HarbolValue val);
HARBOL_EXPORT bool harbol_unilist_del_node_by_index(struct HarbolUniList *list, size_t index, fnHarbolDestructor *dtor);
HARBOL_EXPORT bool harbol_unilist_del_node_by_ref(struct HarbolUniList *list, struct HarbolUniListNode **noderef, fnHarbolDestructor *dtor);

HARBOL_EXPORT struct HarbolUniListNode *harbol_unilist_get_head_node(const struct HarbolUniList *list);
HARBOL_EXPORT struct HarbolUniListNode *harbol_unilist_get_tail_node(const struct HarbolUniList *list);

HARBOL_EXPORT void harbol_unilist_from_bilist(struct HarbolUniList *list, const struct HarbolBiList *bilist);
HARBOL_EXPORT void harbol_unilist_from_hashmap(struct HarbolUniList *list, const struct HarbolHashMap *map);
HARBOL_EXPORT void harbol_unilist_from_vector(struct HarbolUniList *list, const struct HarbolVector *vec);
HARBOL_EXPORT void harbol_unilist_from_graph(struct HarbolUniList *list, const struct HarbolGraph *graph);
HARBOL_EXPORT void harbol_unilist_from_linkmap(struct HarbolUniList *list, const struct HarbolLinkMap *map);

HARBOL_EXPORT struct HarbolUniList *harbol_unilist_new_from_bilist(const struct HarbolBiList *bilist);
HARBOL_EXPORT struct HarbolUniList *harbol_unilist_new_from_hashmap(const struct HarbolHashMap *map);
HARBOL_EXPORT struct HarbolUniList *harbol_unilist_new_from_vector(const struct HarbolVector *vec);
HARBOL_EXPORT struct HarbolUniList *harbol_unilist_new_from_graph(const struct HarbolGraph *graph);
HARBOL_EXPORT struct HarbolUniList *harbol_unilist_new_from_linkmap(const struct HarbolLinkMap *map);
/***************/


/************* Doubly Linked List (bilist.c) *************/
typedef struct HarbolBiListNode {
	union HarbolValue Data;
	struct HarbolBiListNode *Next, *Prev;
} HarbolBiListNode;

HARBOL_EXPORT struct HarbolBiListNode *harbol_bilist_node_new(void);
HARBOL_EXPORT struct HarbolBiListNode *harbol_bilist_node_new_val(union HarbolValue val);
HARBOL_EXPORT void harbol_bilist_node_del(struct HarbolBiListNode *node, fnHarbolDestructor *dtor);
HARBOL_EXPORT void harbol_bilist_node_free(struct HarbolBiListNode **noderef, fnHarbolDestructor *dtor);
HARBOL_EXPORT struct HarbolBiListNode *harbol_bilist_node_get_next_node(const struct HarbolBiListNode *node);
HARBOL_EXPORT struct HarbolBiListNode *harbol_bilist_node_get_prev_node(const struct HarbolBiListNode *node);
HARBOL_EXPORT union HarbolValue harbol_bilist_node_get_val(const struct HarbolBiListNode *node);


typedef struct HarbolBiList {
	struct HarbolBiListNode *Head, *Tail;
	size_t Len;
} HarbolBiList;

HARBOL_EXPORT struct HarbolBiList *harbol_bilist_new(void);
HARBOL_EXPORT void harbol_bilist_del(struct HarbolBiList *list, fnHarbolDestructor *dtor);
HARBOL_EXPORT void harbol_bilist_free(struct HarbolBiList **listref, fnHarbolDestructor *dtor);
HARBOL_EXPORT void harbol_bilist_init(struct HarbolBiList *list);

HARBOL_EXPORT size_t harbol_bilist_get_len(const struct HarbolBiList *list);
HARBOL_EXPORT bool harbol_bilist_insert_node_at_head(struct HarbolBiList *list, struct HarbolBiListNode *node);
HARBOL_EXPORT bool harbol_bilist_insert_node_at_tail(struct HarbolBiList *list, struct HarbolBiListNode *node);
HARBOL_EXPORT bool harbol_bilist_insert_node_at_index(struct HarbolBiList *list, struct HarbolBiListNode *, size_t index);
HARBOL_EXPORT bool harbol_bilist_insert_at_head(struct HarbolBiList *list, union HarbolValue val);
HARBOL_EXPORT bool harbol_bilist_insert_at_tail(struct HarbolBiList *list, union HarbolValue val);
HARBOL_EXPORT bool harbol_bilist_insert_at_index(struct HarbolBiList *list, union HarbolValue val, size_t index);

HARBOL_EXPORT struct HarbolBiListNode *harbol_bilist_get_node_by_index(const struct HarbolBiList *list, size_t index);
HARBOL_EXPORT struct HarbolBiListNode *harbol_bilist_get_node_by_val(const struct HarbolBiList *list, union HarbolValue val);
HARBOL_EXPORT union HarbolValue harbol_bilist_get_val(const struct HarbolBiList *list, size_t index);
HARBOL_EXPORT void harbol_bilist_set_val(struct HarbolBiList *list, size_t index, union HarbolValue val);
HARBOL_EXPORT bool harbol_bilist_del_node_by_index(struct HarbolBiList *list, size_t index, fnHarbolDestructor *dtor);
HARBOL_EXPORT bool harbol_bilist_del_node_by_ref(struct HarbolBiList *list, struct HarbolBiListNode **noderef, fnHarbolDestructor *dtor);

HARBOL_EXPORT struct HarbolBiListNode *harbol_bilist_get_head_node(const struct HarbolBiList *list);
HARBOL_EXPORT struct HarbolBiListNode *harbol_bilist_get_tail_node(const struct HarbolBiList *list);

HARBOL_EXPORT void harbol_bilist_from_unilist(struct HarbolBiList *bilist, const struct HarbolUniList *unilist);
HARBOL_EXPORT void harbol_bilist_from_hashmap(struct HarbolBiList *bilist, const struct HarbolHashMap *map);
HARBOL_EXPORT void harbol_bilist_from_vector(struct HarbolBiList *bilist, const struct HarbolVector *vec);
HARBOL_EXPORT void harbol_bilist_from_graph(struct HarbolBiList *bilist, const struct HarbolGraph *graph);
HARBOL_EXPORT void harbol_bilist_from_linkmap(struct HarbolBiList *bilist, const struct HarbolLinkMap *linkmap);

HARBOL_EXPORT struct HarbolBiList *harbol_bilist_new_from_unilist(const struct HarbolUniList *unilist);
HARBOL_EXPORT struct HarbolBiList *harbol_bilist_new_from_hashmap(const struct HarbolHashMap *map);
HARBOL_EXPORT struct HarbolBiList *harbol_bilist_new_from_vector(const struct HarbolVector *vec);
HARBOL_EXPORT struct HarbolBiList *harbol_bilist_new_from_graph(const struct HarbolGraph *graph);
HARBOL_EXPORT struct HarbolBiList *harbol_bilist_new_from_linkmap(const struct HarbolLinkMap *linkmap);
/***************/


/************* Byte Buffer (bytebuffer.c) *************/
typedef struct HarbolByteBuffer {
	uint8_t *Buffer;
	size_t Len, Count;
} HarbolByteBuffer;

HARBOL_EXPORT struct HarbolByteBuffer *harbol_bytebuffer_new(void);
HARBOL_EXPORT void harbol_bytebuffer_init(struct HarbolByteBuffer *buf);
HARBOL_EXPORT void harbol_bytebuffer_del(struct HarbolByteBuffer *buf);
HARBOL_EXPORT void harbol_bytebuffer_free(struct HarbolByteBuffer **bufref);
HARBOL_EXPORT size_t harbol_bytebuffer_get_len(const struct HarbolByteBuffer *buf);
HARBOL_EXPORT size_t harbol_bytebuffer_get_count(const struct HarbolByteBuffer *buf);
HARBOL_EXPORT uint8_t *harbol_bytebuffer_get_raw_buffer(const struct HarbolByteBuffer *buf);
HARBOL_EXPORT void harbol_bytebuffer_insert_byte(struct HarbolByteBuffer *buf, uint8_t byte);
HARBOL_EXPORT void harbol_bytebuffer_insert_integer(struct HarbolByteBuffer *buf, uint64_t integer, size_t size);
HARBOL_EXPORT void harbol_bytebuffer_insert_float32(struct HarbolByteBuffer *buf, float flt);
HARBOL_EXPORT void harbol_bytebuffer_insert_float64(struct HarbolByteBuffer *buf, double flt);
HARBOL_EXPORT void harbol_bytebuffer_insert_cstr(struct HarbolByteBuffer *buf, const char cstr[], size_t cstrlen);
HARBOL_EXPORT void harbol_bytebuffer_insert_obj(struct HarbolByteBuffer *buf, const void *obj, size_t objlen);
HARBOL_EXPORT void harbol_bytebuffer_insert_zeros(struct HarbolByteBuffer *buf, size_t amount);
HARBOL_EXPORT void harbol_bytebuffer_delete_byte(struct HarbolByteBuffer *buf, size_t index);
HARBOL_EXPORT void harbol_bytebuffer_resize(struct HarbolByteBuffer *buf);
HARBOL_EXPORT void harbol_bytebuffer_to_file(const struct HarbolByteBuffer *buf, FILE *file);
HARBOL_EXPORT size_t harbol_bytebuffer_read_from_file(struct HarbolByteBuffer *buf, FILE *file);
HARBOL_EXPORT void harbol_bytebuffer_append(struct HarbolByteBuffer *bufA, struct HarbolByteBuffer *bufB);
/***************/


/************* Memory-aligned, Packed Data Structure (tuple.c) *************/
/* Tuples act like constant structs but use indexes instead of named fields. */
typedef struct HarbolTuple {
	struct HarbolVector Fields; /* contains the offsets of each member */
	uint8_t *Datum;
	size_t Len;
	bool Packed : 1;
} HarbolTuple;

HARBOL_EXPORT struct HarbolTuple *harbol_tuple_new(size_t member_count, const size_t sizes[], bool packed);
HARBOL_EXPORT bool harbol_tuple_free(struct HarbolTuple **tupleref);

HARBOL_EXPORT void harbol_tuple_init(struct HarbolTuple *tuple, size_t member_count, const size_t sizes[], bool packed);
HARBOL_EXPORT void harbol_tuple_del(struct HarbolTuple *tuple);

HARBOL_EXPORT size_t harbol_tuple_get_len(const struct HarbolTuple *tuple);
HARBOL_EXPORT void *harbol_tuple_get_field(const struct HarbolTuple *tuple, size_t index);
HARBOL_EXPORT void *harbol_tuple_set_field(const struct HarbolTuple *tuple, size_t index, void *field);

HARBOL_EXPORT size_t harbol_tuple_get_field_size(const struct HarbolTuple *tuple, size_t index);
HARBOL_EXPORT bool harbol_tuple_is_packed(const struct HarbolTuple *tuple);
HARBOL_EXPORT bool harbol_tuple_to_struct(const struct HarbolTuple *tuple, void *struc);
/***************/


/************* Memory Pool (mempool.c) *************/
// uncomment 'POOL_NO_MALLOC' if you can't or don't want to use 'malloc/calloc'.
// library will need recompiling though.

//#define POOL_NO_MALLOC

#ifdef POOL_NO_MALLOC
#	ifndef POOL_HEAPSIZE
#		define POOL_HEAPSIZE	0xFFFF //(65535)
#	endif
#endif

typedef struct HarbolAllocNode {
	size_t Size;
	struct HarbolAllocNode *Next;
} HarbolAllocNode;

typedef struct HarbolMemoryPool {
	uint8_t
#ifdef POOL_NO_MALLOC
#	ifndef POOL_HEAPSIZE
#		error please define 'POOL_HEAPSIZE' with a valid size.
#	else
		HeapMem[POOL_HEAPSIZE+1],
#	endif
#else
		*HeapMem,
#endif
		*HeapBottom
	;
	size_t HeapSize, NodeCount;
	struct HarbolAllocNode *FreeList;
} HarbolMemoryPool;

HARBOL_EXPORT void harbol_mempool_init(struct HarbolMemoryPool *mempool, size_t bytes);
HARBOL_EXPORT void harbol_mempool_del(struct HarbolMemoryPool *mempool);
HARBOL_EXPORT void *harbol_mempool_alloc(struct HarbolMemoryPool *mempool, size_t bytes);
HARBOL_EXPORT void *harbol_mempool_realloc(struct HarbolMemoryPool *mempool, void *ptr, size_t bytes);
HARBOL_EXPORT void harbol_mempool_dealloc(struct HarbolMemoryPool *mempool, void *ptr);
HARBOL_EXPORT void harbol_mempool_destroy(struct HarbolMemoryPool *mempool, void *ptrref);
HARBOL_EXPORT size_t harbol_mempool_get_remaining(const struct HarbolMemoryPool *mempool);
HARBOL_EXPORT size_t harbol_mempool_get_heap_size(const struct HarbolMemoryPool *mempool);
HARBOL_EXPORT struct HarbolAllocNode *harbol_mempool_get_freelist(const struct HarbolMemoryPool *mempool);
HARBOL_EXPORT bool harbol_mempool_defrag(struct HarbolMemoryPool *mempool);
/***************/


/************* Weighted Graph (Adjacency List) (graph.c) *************/
struct HarbolGraphVertex;
typedef struct HarbolGraphEdge {
	union HarbolValue Weight;
	struct HarbolGraphVertex *Link;
} HarbolGraphEdge;

HARBOL_EXPORT struct HarbolGraphEdge *harbol_edge_new(void);
HARBOL_EXPORT struct HarbolGraphEdge *harbol_edge_new_val_vert(union HarbolValue val, struct HarbolGraphVertex *vert);
HARBOL_EXPORT void harbol_edge_del(struct HarbolGraphEdge *edge, fnHarbolDestructor *dtor);
HARBOL_EXPORT void harbol_edge_free(struct HarbolGraphEdge **edgeref, fnHarbolDestructor *dtor);

HARBOL_EXPORT union HarbolValue harbol_edge_get_weight(const struct HarbolGraphEdge *edge);
HARBOL_EXPORT void harbol_edge_set_weight(struct HarbolGraphEdge *edge, union HarbolValue val);
HARBOL_EXPORT struct HarbolGraphVertex *harbol_edge_get_vertex(const struct HarbolGraphEdge *edge);
HARBOL_EXPORT void harbol_edge_set_vertex(struct HarbolGraphEdge *edge, struct HarbolGraphVertex *vert);


typedef struct HarbolGraphVertex {
	struct HarbolVector Edges;
	union HarbolValue Data;
} HarbolGraphVertex;

HARBOL_EXPORT struct HarbolGraphVertex *harbol_vertex_new(union HarbolValue val);
HARBOL_EXPORT void harbol_vertex_init(struct HarbolGraphVertex *vert, union HarbolValue val);
HARBOL_EXPORT void harbol_vertex_del(struct HarbolGraphVertex *vert, fnHarbolDestructor *dtor_edge, fnHarbolDestructor *dtor_vert);
HARBOL_EXPORT void harbol_vertex_free(struct HarbolGraphVertex **vertref, fnHarbolDestructor *dtor_edge, fnHarbolDestructor *dtor_vert);
HARBOL_EXPORT bool harbol_vertex_add_edge(struct HarbolGraphVertex *vert, struct HarbolGraphEdge *edge);
HARBOL_EXPORT struct HarbolVector *harbol_vertex_get_edges(struct HarbolGraphVertex *vert);
HARBOL_EXPORT union HarbolValue harbol_vertex_get_val(const struct HarbolGraphVertex *vert);
HARBOL_EXPORT void harbol_vertex_set_val(struct HarbolGraphVertex *vert, union HarbolValue val);


typedef struct HarbolGraph {
	struct HarbolVector Vertices;
} HarbolGraph;

HARBOL_EXPORT struct HarbolGraph *harbol_graph_new(void);
HARBOL_EXPORT void harbol_graph_init(struct HarbolGraph *graph);
HARBOL_EXPORT void harbol_graph_del(struct HarbolGraph *graph, fnHarbolDestructor *dtor_edge, fnHarbolDestructor *dtor_vert);
HARBOL_EXPORT void harbol_graph_free(struct HarbolGraph **graphref, fnHarbolDestructor *dtor_edge, fnHarbolDestructor *dtor_vert);

HARBOL_EXPORT bool harbol_graph_insert_val(struct HarbolGraph *graph, union HarbolValue val);
HARBOL_EXPORT bool harbol_graph_delete_val(struct HarbolGraph *graph, union HarbolValue val, fnHarbolDestructor *dtor_edge, fnHarbolDestructor *dtor_vert);
HARBOL_EXPORT bool harbol_graph_delete_val_by_index(struct HarbolGraph *graph, size_t index, fnHarbolDestructor *dtor_edge, fnHarbolDestructor *dtor_vert);

HARBOL_EXPORT bool harbol_graph_insert_edge(struct HarbolGraph *graph, size_t index_vert1, size_t index_vert2, union HarbolValue weight);
HARBOL_EXPORT bool harbol_graph_delete_edge(struct HarbolGraph *graph, size_t index_vert1, size_t index_vert2, fnHarbolDestructor *dtor_edge);

HARBOL_EXPORT struct HarbolGraphVertex *harbol_graph_get_vertex_by_index(struct HarbolGraph *graph, size_t index);
HARBOL_EXPORT union HarbolValue harbol_graph_get_val_by_index(struct HarbolGraph *graph, size_t index);
HARBOL_EXPORT void harbol_graph_set_val_by_index(struct HarbolGraph *graph, size_t index, union HarbolValue val);
HARBOL_EXPORT struct HarbolGraphEdge *harbol_graph_get_edge(struct HarbolGraph *graph, size_t index_vert1, size_t index_vert2);

HARBOL_EXPORT bool harbol_graph_is_vertex_adjacent_by_index(struct HarbolGraph *graph, size_t index_vert1, size_t index_vert2);
HARBOL_EXPORT struct HarbolVector *harbol_graph_get_vertex_neighbors(struct HarbolGraph *graph, size_t index);

HARBOL_EXPORT struct HarbolVector *harbol_graph_get_vertex_vector(struct HarbolGraph *graph);
HARBOL_EXPORT size_t harbol_graph_get_vertex_count(const struct HarbolGraph *graph);
HARBOL_EXPORT size_t harbol_graph_get_edge_count(const struct HarbolGraph *graph);
HARBOL_EXPORT void harbol_graph_resize(struct HarbolGraph *graph);
HARBOL_EXPORT void harbol_graph_truncate(struct HarbolGraph *graph);

HARBOL_EXPORT void harbol_graph_from_vector(struct HarbolGraph *graph, const struct HarbolVector *vec);
HARBOL_EXPORT void harbol_graph_from_hashmap(struct HarbolGraph *graph, const struct HarbolHashMap *map);
HARBOL_EXPORT void harbol_graph_from_unilist(struct HarbolGraph *graph, const struct HarbolUniList *list);
HARBOL_EXPORT void harbol_graph_from_bilist(struct HarbolGraph *graph, const struct HarbolBiList *list);
HARBOL_EXPORT void harbol_graph_from_linkmap(struct HarbolGraph *graph, const struct HarbolLinkMap *map);

HARBOL_EXPORT struct HarbolGraph *harbol_graph_new_from_vector(const struct HarbolVector *vec);
HARBOL_EXPORT struct HarbolGraph *harbol_graph_new_from_hashmap(const struct HarbolHashMap *map);
HARBOL_EXPORT struct HarbolGraph *harbol_graph_new_from_unilist(const struct HarbolUniList *list);
HARBOL_EXPORT struct HarbolGraph *harbol_graph_new_from_bilist(const struct HarbolBiList *list);
HARBOL_EXPORT struct HarbolGraph *harbol_graph_new_from_linkmap(const struct HarbolLinkMap *map);
/***************/


/************* General Tree (tree.c) *************/
typedef struct HarbolTree {
	struct HarbolVector Children;
	union HarbolValue Data;
} HarbolTree;

HARBOL_EXPORT struct HarbolTree *harbol_tree_new(union HarbolValue val);
HARBOL_EXPORT void harbol_tree_init(struct HarbolTree *tree);
HARBOL_EXPORT void harbol_tree_init_val(struct HarbolTree *tree, union HarbolValue val);
HARBOL_EXPORT void harbol_tree_del(struct HarbolTree *tree, fnHarbolDestructor *dtor);
HARBOL_EXPORT void harbol_tree_free(struct HarbolTree **treeref, fnHarbolDestructor *dtor);

HARBOL_EXPORT bool harbol_tree_insert_child_node(struct HarbolTree *tree, struct HarbolTree *child);
HARBOL_EXPORT bool harbol_tree_insert_child_val(struct HarbolTree *tree, union HarbolValue val);

HARBOL_EXPORT bool harbol_tree_delete_child_by_ref(struct HarbolTree *tree, struct HarbolTree **childref, fnHarbolDestructor *dtor);
HARBOL_EXPORT bool harbol_tree_delete_child_by_index(struct HarbolTree *tree, size_t index, fnHarbolDestructor *dtor);
HARBOL_EXPORT bool harbol_tree_delete_child_by_val(struct HarbolTree *tree, union HarbolValue val, fnHarbolDestructor *dtor);

HARBOL_EXPORT struct HarbolTree *harbol_tree_get_child_by_index(const struct HarbolTree *tree, size_t index);
HARBOL_EXPORT struct HarbolTree *harbol_tree_get_child_by_val(const struct HarbolTree *tree, union HarbolValue val);
HARBOL_EXPORT union HarbolValue harbol_tree_get_val(const struct HarbolTree *tree);
HARBOL_EXPORT void harbol_tree_set_val(struct HarbolTree *tree, union HarbolValue val);
HARBOL_EXPORT struct HarbolVector *harbol_tree_get_children_vector(struct HarbolTree *tree);
HARBOL_EXPORT size_t harbol_tree_get_children_len(const struct HarbolTree *tree);
HARBOL_EXPORT size_t harbol_tree_get_children_count(const struct HarbolTree *tree);
/***************/


/************* Ordered String Key Hash Map (preserves insertion order) (linkmap.c) *************/
typedef struct HarbolLinkMap {
	struct HarbolHashMap Map;
	struct HarbolVector Order;
} HarbolLinkMap;

HARBOL_EXPORT struct HarbolLinkMap *harbol_linkmap_new(void);
HARBOL_EXPORT void harbol_linkmap_init(struct HarbolLinkMap *linkmap);
HARBOL_EXPORT void harbol_linkmap_del(struct HarbolLinkMap *linkmap, fnHarbolDestructor *dtor);
HARBOL_EXPORT void harbol_linkmap_free(struct HarbolLinkMap **linkmapref, fnHarbolDestructor *dtor);
HARBOL_EXPORT size_t harbol_linkmap_get_count(const struct HarbolLinkMap *linkmap);
HARBOL_EXPORT size_t harbol_linkmap_get_len(const struct HarbolLinkMap *linkmap);
HARBOL_EXPORT bool harbol_linkmap_rehash(struct HarbolLinkMap *linkmap);

HARBOL_EXPORT bool harbol_linkmap_insert(struct HarbolLinkMap *linkmap, const char key[], union HarbolValue val);
HARBOL_EXPORT bool harbol_linkmap_insert_node(struct HarbolLinkMap *linkmap, struct HarbolKeyValPair *kv);

HARBOL_EXPORT struct HarbolKeyValPair *harbol_linkmap_get_node_by_index(const struct HarbolLinkMap *linkmap, size_t index);
HARBOL_EXPORT union HarbolValue harbol_linkmap_get(const struct HarbolLinkMap *linkmap, const char key[]);
HARBOL_EXPORT void harbol_linkmap_set(struct HarbolLinkMap *linkmap, const char key[], union HarbolValue val);
HARBOL_EXPORT union HarbolValue harbol_linkmap_get_by_index(const struct HarbolLinkMap *linkmap, size_t index);
HARBOL_EXPORT void harbol_linkmap_set_by_index(struct HarbolLinkMap *linkmap, size_t index, union HarbolValue val);

HARBOL_EXPORT void harbol_linkmap_delete(struct HarbolLinkMap *linkmap, const char key[], fnHarbolDestructor *dtor);
HARBOL_EXPORT void harbol_linkmap_delete_by_index(struct HarbolLinkMap *linkmap, size_t, fnHarbolDestructor *dtor);
HARBOL_EXPORT bool harbol_linkmap_has_key(const struct HarbolLinkMap *, const char key[]);
HARBOL_EXPORT struct HarbolKeyValPair *harbol_linkmap_get_node_by_key(const struct HarbolLinkMap *linkmap, const char key[]);
HARBOL_EXPORT struct HarbolVector *harbol_linkmap_get_buckets(const struct HarbolLinkMap *linkmap);

HARBOL_EXPORT union HarbolValue *harbol_linkmap_get_iter(const struct HarbolLinkMap *linkmap);
HARBOL_EXPORT union HarbolValue *harbol_linkmap_get_iter_end_len(const struct HarbolLinkMap *linkmap);
HARBOL_EXPORT union HarbolValue *harbol_linkmap_get_iter_end_count(const struct HarbolLinkMap *linkmap);

HARBOL_EXPORT size_t harbol_linkmap_get_index_by_name(const struct HarbolLinkMap *linkmap, const char key[]);
HARBOL_EXPORT size_t harbol_linkmap_get_index_by_node(const struct HarbolLinkMap *linkmap, struct HarbolKeyValPair *kv);
HARBOL_EXPORT size_t harbol_linkmap_get_index_by_val(const struct HarbolLinkMap *linkmap, union HarbolValue val);

HARBOL_EXPORT void harbol_linkmap_from_hashmap(struct HarbolLinkMap *linkmap, const struct HarbolHashMap *map);
HARBOL_EXPORT void harbol_linkmap_from_unilist(struct HarbolLinkMap *linkmap, const struct HarbolUniList *list);
HARBOL_EXPORT void harbol_linkmap_from_bilist(struct HarbolLinkMap *linkmap, const struct HarbolBiList *list);
HARBOL_EXPORT void harbol_linkmap_from_vector(struct HarbolLinkMap *linkmap, const struct HarbolVector *vec);
HARBOL_EXPORT void harbol_linkmap_from_graph(struct HarbolLinkMap *linkmap, const struct HarbolGraph *graph);

HARBOL_EXPORT struct HarbolLinkMap *harbol_linkmap_new_from_hashmap(const struct HarbolHashMap *map);
HARBOL_EXPORT struct HarbolLinkMap *harbol_linkmap_new_from_unilist(const struct HarbolUniList *list);
HARBOL_EXPORT struct HarbolLinkMap *harbol_linkmap_new_from_bilist(const struct HarbolBiList *list);
HARBOL_EXPORT struct HarbolLinkMap *harbol_linkmap_new_from_vector(const struct HarbolVector *vec);
HARBOL_EXPORT struct HarbolLinkMap *harbol_linkmap_new_from_graph(const struct HarbolGraph *graph);
/***************/


/************* Tagged Union Type (variant.c) *************/
// discriminated union type
typedef struct HarbolVariant {
	union HarbolValue Val;
	int32_t TypeTag;
} HarbolVariant;

HARBOL_EXPORT struct HarbolVariant *harbol_variant_new(union HarbolValue val, int32_t typeval);
HARBOL_EXPORT void harbol_variant_free(struct HarbolVariant **varref, fnHarbolDestructor *dtor);
HARBOL_EXPORT void harbol_variant_init(struct HarbolVariant *var, union HarbolValue val, int32_t typeval);
HARBOL_EXPORT void harbol_variant_del(struct HarbolVariant *var, fnHarbolDestructor *dtor);

HARBOL_EXPORT union HarbolValue harbol_variant_get_val(const struct HarbolVariant *var);
HARBOL_EXPORT void harbol_variant_set_val(struct HarbolVariant *var, union HarbolValue val);

HARBOL_EXPORT int32_t harbol_variant_get_type(const struct HarbolVariant *var);
HARBOL_EXPORT void harbol_variant_set_type(struct HarbolVariant *var, int32_t typeval);
/***************/


/************* Minimal, JSON-like Configuration File Parser (cfg.c) *************/
typedef enum HarbolCfgType {
	HarbolTypeNull=0,
	HarbolTypeLinkMap,
	HarbolTypeString,
	HarbolTypeFloat,
	HarbolTypeInt,
	HarbolTypeBool,
	HarbolTypeColor,
	HarbolTypeVec4D,
} HarbolCfgType;

typedef union HarbolColor {
	uint32_t UIntColor;
	struct{ uint8_t R,G,B,A; } Struc;
	uint8_t RGBA[4];
} HarbolColor;

typedef union HarbolVec4D {
	struct{ float X,Y,Z,W; } Struc;
	float XYZW[4];
} HarbolVec4D;


HARBOL_EXPORT struct HarbolLinkMap *harbol_cfg_from_file(const char filename[]);
HARBOL_EXPORT struct HarbolLinkMap *harbol_cfg_parse_cstr(const char cstr[]);
HARBOL_EXPORT bool harbol_cfg_free(struct HarbolLinkMap **cfgref);
HARBOL_EXPORT bool harbol_cfg_to_str(const struct HarbolLinkMap *cfg, struct HarbolString *str);

HARBOL_EXPORT struct HarbolLinkMap *harbol_cfg_get_section_by_key(struct HarbolLinkMap *cfg, const char keypath[]);
HARBOL_EXPORT char *harbol_cfg_get_str_by_key(struct HarbolLinkMap *cfg, const char keypath[]);
HARBOL_EXPORT bool harbol_cfg_get_float_by_key(struct HarbolLinkMap *cfg, const char keypath[], double *dblref);
HARBOL_EXPORT bool harbol_cfg_get_int_by_key(struct HarbolLinkMap *cfg, const char keypath[], int64_t *intref);
HARBOL_EXPORT bool harbol_cfg_get_bool_by_key(struct HarbolLinkMap *cfg, const char keypath[], bool *boolref);
HARBOL_EXPORT bool harbol_cfg_get_color_by_key(struct HarbolLinkMap *cfg, const char keypath[], union HarbolColor *colorref);
HARBOL_EXPORT bool harbol_cfg_get_vec4D_by_key(struct HarbolLinkMap *cfg, const char keypath[], union HarbolVec4D *vec4dref);

HARBOL_EXPORT bool harbol_cfg_get_key_type(struct HarbolLinkMap *cfg, const char keypath[], enum HarbolCfgType *type);

HARBOL_EXPORT bool harbol_cfg_set_str_by_key(struct HarbolLinkMap *cfg, const char keypath[], const char cstr[], bool override_convert);
HARBOL_EXPORT bool harbol_cfg_set_float_by_key(struct HarbolLinkMap *cfg, const char keypath[], double fltval, bool override_convert);
HARBOL_EXPORT bool harbol_cfg_set_int_by_key(struct HarbolLinkMap *cfg, const char keypath[], int64_t ival, bool override_convert);
HARBOL_EXPORT bool harbol_cfg_set_bool_by_key(struct HarbolLinkMap *cfg, const char keypath[], bool boolval, bool override_convert);
HARBOL_EXPORT bool harbol_cfg_set_color_by_key(struct HarbolLinkMap *cfg, const char keypath[], const union HarbolColor *colorptr, bool override_convert);
HARBOL_EXPORT bool harbol_cfg_set_vec4D_by_key(struct HarbolLinkMap *cfg, const char keypath[], const union HarbolVec4D *vec4dptr, bool override_convert);
HARBOL_EXPORT bool harbol_cfg_set_key_to_null(struct HarbolLinkMap *cfg, const char keypath[]);

HARBOL_EXPORT bool harbol_cfg_build_file(const struct HarbolLinkMap *cfg, const char filename[], bool overwrite);
/***************/


/************* General Plugin System (plugins.c) *************/
#ifdef OS_WINDOWS
	typedef HMODULE HarbolModule;
#else
	typedef void *HarbolModule;
#endif

typedef struct HarbolPlugin {
	struct HarbolString LibPath;
	struct HarbolString *Name;
	HarbolModule SharedLib;
} HarbolPlugin;

struct HarbolPlugin *harbol_plugin_new(HarbolModule module);
bool harbol_plugin_free(struct HarbolPlugin **pluginref);

HARBOL_EXPORT HarbolModule harbol_plugin_get_module(const struct HarbolPlugin *plugin);
HARBOL_EXPORT const char *harbol_plugin_get_name(const struct HarbolPlugin *plugin);
HARBOL_EXPORT const char *harbol_plugin_get_path(const struct HarbolPlugin *plugin);
HARBOL_EXPORT void *harbol_plugin_get_sym(const struct HarbolPlugin *plugin, const char sym_name[]);

HARBOL_EXPORT bool harbol_plugin_reload(struct HarbolPlugin *plugin);


struct HarbolPluginManager;

/* When an individual plugin has been changed such as being loaded, reloading, or unloaded, this callback will fire.
 * You can use this callback to then individually do whatever you need to the plugin.
 */
typedef void fnHarbolPluginEvent(struct HarbolPluginManager *manager, struct HarbolPlugin **pluginref);

typedef struct HarbolPluginManager {
	struct HarbolLinkMap Plugins;
	struct HarbolString Directory;
} HarbolPluginManager;

HARBOL_EXPORT struct HarbolPluginManager *harbol_plugin_manager_new(const char directory[], bool load_plugins, fnHarbolPluginEvent *load_cb);
HARBOL_EXPORT bool harbol_plugin_manager_init(struct HarbolPluginManager *manager, const char directory[], bool load_plugins, fnHarbolPluginEvent *load_cb);
HARBOL_EXPORT bool harbol_plugin_manager_del(struct HarbolPluginManager *manager, fnHarbolPluginEvent *unload_cb);
HARBOL_EXPORT bool harbol_plugin_manager_free(struct HarbolPluginManager **managerref, fnHarbolPluginEvent *unload_cb);

HARBOL_EXPORT struct HarbolPlugin *harbol_plugin_manager_get_plugin_by_name(const struct HarbolPluginManager *manager, const char plugin_name[]);
HARBOL_EXPORT struct HarbolPlugin *harbol_plugin_manager_get_plugin_by_index(const struct HarbolPluginManager *manager, size_t index);

HARBOL_EXPORT size_t harbol_plugin_manager_get_plugin_count(const struct HarbolPluginManager *manager);
HARBOL_EXPORT const char *harbol_plugin_manager_get_plugin_dir(const struct HarbolPluginManager *manager);

HARBOL_EXPORT bool harbol_plugin_manager_load_plugin_by_name(struct HarbolPluginManager *manager, const char plugin_name[], fnHarbolPluginEvent *load_cb);
HARBOL_EXPORT bool harbol_plugin_manager_delete_plugin_by_name(struct HarbolPluginManager *manager, const char plugin_name[], fnHarbolPluginEvent *unload_cb);
HARBOL_EXPORT bool harbol_plugin_manager_delete_plugin_by_index(struct HarbolPluginManager *manager, size_t index, fnHarbolPluginEvent *unload_cb);
HARBOL_EXPORT bool harbol_plugin_manager_load_plugins(struct HarbolPluginManager *manager, fnHarbolPluginEvent *load_cb);

HARBOL_EXPORT bool harbol_plugin_manager_unload_plugins(struct HarbolPluginManager *manager, fnHarbolPluginEvent *unload_cb);
HARBOL_EXPORT bool harbol_plugin_manager_reload_plugins(struct HarbolPluginManager *manager, fnHarbolPluginEvent *prereload_cb, fnHarbolPluginEvent *postreload_cb);
/***************/


/************* Multi-Threading Library (threads.c) *************/
#ifndef TIME_UTC
#	define TIME_UTC 1
#	define HARBOL_TIMESPEC
#	ifdef OS_WINDOWS
#		ifndef HAVE_STRUCT_TIMESPEC
#			define HAVE_STRUCT_TIMESPEC
#		endif
#	endif
#endif

typedef enum HarbolThreadRes {
	HarbolThreadResError,
	HarbolThreadResSuccess,
	HarbolThreadResTimedOut,
	HarbolThreadResBusy,
} HarbolThreadRes;

typedef enum HarbolMutexType {
	HarbolMutexTypeNormal,
	HarbolMutexTypeRecursive,
	HarbolMutexTypeTimer,
} HarbolMutexType;

#ifdef OS_WINDOWS
typedef struct {
	union {
		CRITICAL_SECTION CriticalSect;
		HANDLE Mutex;
	} MutexHandle;
	bool
		Locked : 1,
		Recursive : 1,
		Timed : 1
	;
} HarbolMutex;
#else
typedef pthread_mutex_t HarbolMutex;
#endif

HARBOL_EXPORT enum HarbolThreadRes harbol_mutex_init(HarbolMutex *mutex, enum HarbolMutexType mutex_type);
HARBOL_EXPORT bool harbol_mutex_del(HarbolMutex *mutex);

HARBOL_EXPORT enum HarbolThreadRes harbol_mutex_lock(HarbolMutex *mutex);
HARBOL_EXPORT enum HarbolThreadRes harbol_mutex_timedlock(HarbolMutex *mutex, const struct timespec *t);
HARBOL_EXPORT enum HarbolThreadRes harbol_mutex_trylock(HarbolMutex *mutex);
HARBOL_EXPORT enum HarbolThreadRes harbol_mutex_unlock(HarbolMutex *mutex);


#ifdef OS_WINDOWS
typedef struct {
	HANDLE EventHandles[2];
	unsigned int CountWaiting;
	CRITICAL_SECTION CountWaitingLock;
} HarbolCond;
#else
typedef pthread_cond_t HarbolCond;
#endif

HARBOL_EXPORT enum HarbolThreadRes harbol_cond_init(HarbolCond *cond);
HARBOL_EXPORT void harbol_cond_del(HarbolCond *cond);

HARBOL_EXPORT enum HarbolThreadRes harbol_cond_signal(HarbolCond *cond);
HARBOL_EXPORT enum HarbolThreadRes harbol_cond_broadcast(HarbolCond *cond);
HARBOL_EXPORT enum HarbolThreadRes harbol_cond_wait(HarbolCond *cond, HarbolMutex *mutex);
HARBOL_EXPORT enum HarbolThreadRes harbol_cond_timedwait(HarbolCond *cond, HarbolMutex *mutex, const struct timespec *t);


#ifdef OS_WINDOWS
typedef HANDLE HarbolThreadCtxt;
#else
typedef pthread_t HarbolThreadCtxt;
#endif

#ifdef C11
#	ifndef HARBOL_THREAD_NORETURN
#		define HARBOL_THREAD_NORETURN _Noreturn
#	endif
#elif defined(COMPILER_GCC)
#	ifndef HARBOL_THREAD_NORETURN
#		define HARBOL_THREAD_NORETURN __attribute__((__noreturn__))
#	endif
#else
#	ifndef HARBOL_THREAD_NORETURN
#		define HARBOL_THREAD_NORETURN
#	endif
#endif

typedef int fnHarbolThread(void *arg);

typedef struct HarbolThread {
	fnHarbolThread *Func;
	void *Args;
	HarbolThreadCtxt Thread;
} HarbolThread;


HARBOL_EXPORT struct HarbolThread *harbol_thread_new(fnHarbolThread *func, void *arg);
HARBOL_EXPORT enum HarbolThreadRes harbol_thread_init(struct HarbolThread *thr, fnHarbolThread *func, void *arg);

HARBOL_EXPORT HarbolThreadCtxt harbol_thread_get_curr(void);
HARBOL_EXPORT enum HarbolThreadRes harbol_thread_detach(struct HarbolThread *thr);
HARBOL_EXPORT bool harbol_thread_equal(struct HarbolThread *thrA, struct HarbolThread *thrB);
HARBOL_EXPORT HARBOL_THREAD_NORETURN void harbol_thread_exit(int res, void *retval);
HARBOL_EXPORT enum HarbolThreadRes harbol_thread_join(struct HarbolThread *thr, int *res);
HARBOL_EXPORT int harbol_thread_sleep(const struct timespec *duration, struct timespec *remaining);
HARBOL_EXPORT void harbol_thread_yield(void);


#ifdef OS_WINDOWS
typedef uint32_t HarbolThreadStorage;
#else
typedef pthread_key_t HarbolThreadStorage;
#endif

typedef void fnHarbolStorageDtor(void *val);

typedef struct HarbolTSS {
#ifdef OS_WINDOWS
	fnHarbolStorageDtor *CacheDtor;
	void *CacheVal;
#endif
	HarbolThreadStorage Key;
} HarbolTSS;

HARBOL_EXPORT enum HarbolThreadRes harbol_tss_create(struct HarbolTSS *key, fnHarbolStorageDtor *dtor);
HARBOL_EXPORT void harbol_tss_delete(struct HarbolTSS *key);
HARBOL_EXPORT void *harbol_tss_get(struct HarbolTSS *key);
HARBOL_EXPORT enum HarbolThreadRes harbol_tss_set(struct HarbolTSS *key, void *val);
/***************/


#ifdef __cplusplus
}
#endif

#endif /* HARBOL_INCLUDED */
