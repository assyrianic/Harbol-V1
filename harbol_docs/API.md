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

### CStr
allocated array of char values as a null-terminated string.

### Len
size_t member that stores the size of `CStr`. Is set to 0 if `CStr` is `NULL` or empty "".


## struct HarbolVector

### Table
allocated array of `union HarbolValue`.

### Len
size_t member that stores the size of `Table`.

### Count
size_t member that stores how many items populate `Table`.


## struct HarbolKeyValPair

### KeyName
member of type `struct HarbolString` used to store keys that pair to data.

### Data
member of type `union HarbolValue` used to store data that is associated to a key.


## struct HarbolHashmap

### Table
allocated array of `struct HarbolVector`'s that store pointers of type `struct HarbolKeyValPair`.

### Len
size_t member that stores the size of `Table`.

### Count
size_t member that stores how many items populate `Table`.


## struct HarbolUniListNode

### Data
member of type `union HarbolValue` for storing node data.

### Next
pointer to type `struct HarbolUniListNode`.


## struct HarbolUniList

### Head, Tail
pointers to type `struct HarbolUniListNode`.
`Head` tracks the first node while `Tail` tracks the last node.

### Len
size_t member that tracks how many nodes the list contains.


## struct HarbolBiListNode

### Data
member of type `union HarbolValue` for storing node data.

### Next, Prev
pointers to type `struct HarbolBiListNode`.


## struct HarbolBiList

### Head, Tail
pointers to type `struct HarbolBiListNode`.
`Head` tracks the first node while `Tail` tracks the last node.

### Len
size_t member that tracks how many nodes the list contains.


## struct HarbolByteBuffer

### Buffer
allocated unsigned char array.

### Len
size_t member that tracks the size of `Buffer`.

### Count
size_t member that tracks the amount of items in `Buffer`.


## struct HarbolTuple

### Items
allocated array of `union HarbolValue` values.

### Len
size_t member that tracks the size of `Items`.


## struct HarbolAllocNode

### Size
size_t member that tracks the memory size of `NextFree`.

### NextFree
pointer to type `struct HarbolAllocNode`.


## struct HarbolMemoryPool

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

### Weight
`union HarbolValue` member for storing edge data.

### VertexSocket
pointer to `struct HarbolGraphVertex`. Used to connect the edge to the receiving socket.


## struct HarbolGraphVertex

### Edges
member of `struct HarbolVector` which stores pointers to `struct HarbolGraphEdge`.

### Data
`union HarbolValue` member for storing vertex data.


## struct HarbolGraph

### Vertices
member of `struct HarbolVector` which stores pointers to `struct HarbolGraphVertex`.


## struct HarbolTree

### Children
member of `struct HarbolVector` which stores pointers to `struct HarbolTree`.

### Data
member of `union HarbolValue` for storing node data.


## struct HarbolLinkMap

### Map
member of `struct HarbolHashmap` which implements the hash map portion of the structure.

### Order
member of `struct HarbolVector` which preserves insertion order of items.


## struct HarbolVariant

### Val
member of `union HarbolValue` for storing data.

### TypeTag
member of `int32_t` for storing an integer ID for denoting user types.



# Functions/Methods

## FuncName
```c
type *FuncName(void *param1);
```

### Description
how now brown cow.

### Parameters
`param1` - param1 info.

### Return Value
return value information.
