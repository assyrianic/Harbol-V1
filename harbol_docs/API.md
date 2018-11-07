# Datatypes

## Union HarbolValue
### Bool
boolean value.

### BoolArray
boolean array of 8 cells.

### BoolPtr
ptr to a boolean value.

### Int8
signed char value.

### Int8Array
signed char array of 8 cells.

### Int8Ptr
ptr to a signed char value.

### Int16
signed two byte value.

### Int16Array
signed two byte array of 4 cells.

### Int16Ptr
ptr to a signed two byte value.

### Int32
signed four byte value.

### Int32Array
signed four byte array of 2 cells.

### Int32Ptr
ptr to a signed four byte value.

### Int64
signed eight byte value.

### Int64Ptr
ptr to a signed eight byte value.

### UInt8
unsigned char value.

### UInt8Array
unsigned char array of 8 cells.

### UInt8Ptr
ptr to a unsigned char value.

### UInt16
unsigned two byte value.

### UInt16Array
unsigned two byte array of 4 cells.

### UInt16Ptr
ptr to a unsigned two byte value.

### UInt32
unsigned four byte value.

### UInt32Array
unsigned four byte array of 2 cells.

### UInt32Ptr
ptr to a unsigned four byte value.

### UInt64
unsigned eight byte value.

### UInt64Ptr
ptr to a unsigned eight byte value.

### IntSize
size_t value.

### IntSizePtr
ptr to size_t value.

### IntNative
signed native (wide enough to fit a pointer) value.

### UIntNative
unsigned native (wide enough to fit a pointer) value.

### Float
32-bit floating point value.

### FloatArray
32-bit floating point array of 2 cells.

### FloatPtr
ptr to 32-bit floating point value.

### Double
64-bit floating point value.

### DoublePtr
ptr to 64-bit floating point value.

### Ptr
pointer to void type value.

### SelfPtr
pointer to `union HarbolValue` value.

### VarPtr
pointer to `struct HarbolVariant` value.

### StrObjPtr
pointer to `struct HarbolString` value.

### VecPtr
pointer to `struct HarbolVector` value.

### KvPairPtr
pointer to `struct HarbolKeyValPair` value.

### MapPtr
pointer to `struct HarbolHashmap` value.

### UniListPtr
pointer to `struct HarbolUniList` value.

### BiListPtr
pointer to `struct HarbolBiList` value.

### ByteBufferPtr
pointer to `struct HarbolByteBuffer` value.

### TuplePtr
pointer to `struct HarbolTuple` value.

### GraphPtr
pointer to `struct HarbolGraph` value.

### TreePtr
pointer to `struct HarbolTree` value.

### LinkMapPtr
pointer to `struct HarbolLinkMap` value.


## struct HarbolString
```c
typedef struct HarbolString {
	char *CStr;
	size_t Len;
} HarbolString;
```

### CStr
allocated array of char values as a null-terminated string.

### Len
size_t member that stores the size of `CStr`. Is set to 0 if `CStr` is `NULL` or empty "".


## struct HarbolVector
```c
typedef struct HarbolVector {
	union HarbolValue *Table;
	size_t Len, Count;
} HarbolVector;
```

### Table
allocated array of `union HarbolValue`.

### Len
size_t member that stores the size of `Table`.

### Count
size_t member that stores how many items populate `Table`.


## struct HarbolKeyValPair
```c
typedef struct HarbolKeyValPair {
	struct HarbolString KeyName;
	union HarbolValue Data;
} HarbolKeyValPair;
```

### KeyName
member of type `struct HarbolString` used to store keys that pair to data.

### Data
member of type `union HarbolValue` used to store data that is associated to a key.


## struct HarbolHashmap
```c
typedef struct HarbolHashmap {
	struct HarbolVector *Table;
	size_t Len, Count;
} HarbolHashmap;
```

### Table
allocated array of `struct HarbolVector`'s that store pointers of type `struct HarbolKeyValPair`.

### Len
size_t member that stores the size of `Table`.

### Count
size_t member that stores how many items populate `Table`.


## struct HarbolUniListNode
```c
typedef struct HarbolUniListNode {
	union HarbolValue Data;
	struct HarbolUniListNode *Next;
} HarbolUniListNode;
```

### Data
member of type `union HarbolValue` for storing node data.

### Next
pointer to type `struct HarbolUniListNode`.


## struct HarbolUniList
```c
typedef struct HarbolUniList {
	struct HarbolUniListNode *Head, *Tail;
	size_t Len;
} HarbolUniList;
```

### Head, Tail
pointers to type `struct HarbolUniListNode`.
`Head` tracks the first node while `Tail` tracks the last node.

### Len
size_t member that tracks how many nodes the list contains.


## struct HarbolBiListNode
```c
typedef struct HarbolBiListNode {
	union HarbolValue Data;
	struct HarbolBiListNode *Next, *Prev;
} HarbolBiListNode;
```

### Data
member of type `union HarbolValue` for storing node data.

### Next, Prev
pointers to type `struct HarbolBiListNode`.


## struct HarbolBiList
```c
typedef struct HarbolBiList {
	struct HarbolBiListNode *Head, *Tail;
	size_t Len;
} HarbolBiList;
```

### Head, Tail
pointers to type `struct HarbolBiListNode`.
`Head` tracks the first node while `Tail` tracks the last node.

### Len
size_t member that tracks how many nodes the list contains.


## struct HarbolByteBuffer
```c
typedef struct HarbolByteBuffer {
	uint8_t *Buffer;
	size_t Len, Count;
} HarbolByteBuffer;
```

### Buffer
allocated unsigned char array.

### Len
size_t member that tracks the size of `Buffer`.

### Count
size_t member that tracks the amount of items in `Buffer`.


## struct HarbolTuple
```c
typedef struct HarbolTuple {
	union HarbolValue *Items;
	size_t Len;
} HarbolTuple;
```

### Items
allocated array of `union HarbolValue` values.

### Len
size_t member that tracks the size of `Items`.


## struct HarbolAllocNode
```c
typedef struct HarbolAllocNode {
	size_t Size;
	struct HarbolAllocNode *NextFree;
} HarbolAllocNode;
```

### Size
size_t member that tracks the memory size of `NextFree`.

### NextFree
pointer to type `struct HarbolAllocNode`.


## struct HarbolMemoryPool
```c
typedef struct HarbolMemoryPool {
	uint8_t
#	ifdef POOL_NO_MALLOC
		HeapMem[POOL_HEAPSIZE],
#	else
		*HeapMem,
#	endif
		*HeapBottom
	;
	size_t HeapSize, FreeNodes;
	struct HarbolAllocNode *FreeList;
} HarbolMemoryPool;
```

### HeapMem
unsigned char pointer that holds the memory pool's heap.

### HeapBottom
unsigned char pointer that tracks the bottom of the memory pool's heap.

### HeapSize
size_t member that tracks the total size of the entire memory pool.

### FreeNodes
size_t member that tracks the amount of freelist nodes in `FreeList`.

### FreeList
pointer to type `struct HarbolAllocNode`.
Tracks the disjointed memory regions of the memory pool.


## struct HarbolGraphEdge
```c
typedef struct HarbolGraphEdge {
	union HarbolValue Weight;
	struct HarbolGraphVertex *VertexSocket;
} HarbolGraphEdge;
```

### Weight
`union HarbolValue` member for storing edge data.

### VertexSocket
pointer to `struct HarbolGraphVertex`. Used to connect the edge to the receiving socket.


## struct HarbolGraphVertex
```c
typedef struct HarbolGraphVertex {
	struct HarbolVector Edges;
	union HarbolValue Data;
} HarbolGraphVertex;
```

### Edges
member of `struct HarbolVector` which stores pointers to `struct HarbolGraphEdge`.

### Data
`union HarbolValue` member for storing vertex data.


## struct HarbolGraph
```c
typedef struct HarbolGraph {
	struct HarbolVector Vertices;
} HarbolGraph;
```

### Vertices
member of `struct HarbolVector` which stores pointers to `struct HarbolGraphVertex`.


## struct HarbolTree
```c
typedef struct HarbolTree {
	struct HarbolVector Children;
	union HarbolValue Data;
} HarbolTree;
```

### Children
member of `struct HarbolVector` which stores pointers to `struct HarbolTree`.

### Data
member of `union HarbolValue` for storing node data.


## struct HarbolLinkMap
```c
typedef struct HarbolLinkMap {
	struct HarbolHashmap Map;
	struct HarbolVector Order;
} HarbolLinkMap;
```

### Map
member of `struct HarbolHashmap` which implements the hash map portion of the structure.

### Order
member of `struct HarbolVector` which preserves insertion order of items.


## struct HarbolVariant
```c
typedef struct HarbolVariant {
	union HarbolValue Val;
	int32_t TypeTag;
} HarbolVariant;
```

### Val
member of `union HarbolValue` for storing data.

### TypeTag
member of `int32_t` for storing an integer ID for denoting user types.



# Functions/Methods

## HarbolString_New
```c
struct HarbolString *HarbolString_New(void);
```

### Description
allocates an empty string object.

### Parameters
no parameters.

### Return Value
returns a pointer to a dynamically allocated, empty string object. `NULL` if err occurred.


## HarbolString_NewStr
```c
struct HarbolString *HarbolString_NewStr(const char *cstr);
```

### Description
allocates an empty string object and initializes it with a C string.

### Parameters
`cstr` - pointer to a C string.

### Return Value
returns a pointer to a dynamically allocated, empty string object. `NULL` if err occurred.


## HarbolString_Del
```c
void HarbolString_Del(struct HarbolString *str);
```

### Description
cleans and clears a string object's data.

### Parameters
`str` - pointer to `struct HarbolString`.

### Return Value
returns nothing.


## HarbolString_Init
```c
void HarbolString_Init(struct HarbolString *str);
```

### Description
initializes a string object to zero.

### Parameters
`str` - pointer to `struct HarbolString`.

### Return Value
returns nothing.


## HarbolString_InitStr
```c
void HarbolString_InitStr(struct HarbolString *str, const char *cstr);
```

### Description
initializes a string object and copies a given C string.

### Parameters
`str` - pointer to `struct HarbolString` to initialize.
`cstr` - C string to copy to `str`.

### Return Value
returns nothing.


## HarbolString_AddChar
```c
void HarbolString_AddChar(struct HarbolString *str, char chr);
```

### Description
appends a single character to the end of the string object.

### Parameters
`str` - pointer to `struct HarbolString` to initialize.
`chr` - single signed char value to append to the string's data.

### Return Value
returns nothing.


## HarbolString_Add
```c
void HarbolString_Add(struct HarbolString *str1, const struct HarbolString *str2);
```

### Description
concatenates `str2` to `str1` string object.

### Parameters
`str1` - destination pointer to `struct HarbolString`.
`str2` - source pointer to `const struct HarbolString`.

### Return Value
returns nothing.


## HarbolString_AddStr
```c
void HarbolString_AddStr(struct HarbolString *str, const char *cstr);
```

### Description
concatenates a C string to a string object.

### Parameters
`str` - pointer to `struct HarbolString` to concatenate to.
`cstr` - pointer to a C string to copy to `str`.

### Return Value
returns nothing.


## HarbolString_GetStr
```c
char *HarbolString_GetStr(const struct HarbolString *str);
```

### Description
converts the string object to a C string.

### Parameters
`str` - pointer to `const struct HarbolString`.

### Return Value
returns a pointer to the allocated string data.


## HarbolString_Len
```c
size_t HarbolString_Len(const struct HarbolString *str);
```

### Description
gets the size of the string.

### Parameters
`str` - pointer to `const struct HarbolString`.

### Return Value
size of the string's allocated data.


## HarbolString_Copy
```c
void HarbolString_Copy(struct HarbolString *str1, const struct HarbolString *str2);
```

### Description
copies a string object's string data from another.

### Parameters
`str1` - destination pointer to `struct HarbolString`.
`str2` - source pointer to `struct HarbolString`.

### Return Value
returns nothing.
