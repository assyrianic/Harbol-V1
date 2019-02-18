# union HarbolValue

```c
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
	struct HarbolHashmap *MapPtr;
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
```

## Bool
`bool` value.

## BoolArray
`bool` array of 8 cells.

## BoolPtr
pointer to `bool` value.

## Int8
`signed byte integer` value.

## Int8Array
`signed byte integer` array of 8 cells.

## Int8Ptr
ptr to `signed byte integer` value.

## Int16
`signed 2-byte integer` value.

## Int16Array
`signed 2-byte integer` array of 4 cells.

## Int16Ptr
ptr to `signed 2-byte integer` value.

## Int32
`signed 4-byte integer` value.

## Int32Array
`signed 4-byte integer` array of 2 cells.

## Int32Ptr
ptr to `signed 4-byte integer` value.

## Int64
`signed 8-byte integer` value.

## Int64Ptr
ptr to `signed 8-byte integer` value.

## UInt8
`unsigned byte integer` value.

## UInt8Array
`unsigned byte integer` array of 8 cells.

## UInt8Ptr
ptr to `unsigned byte integer` value.

## UInt16
`unsigned 2-byte integer` value.

## UInt16Array
`unsigned 2-byte integer` array of 4 cells.

## UInt16Ptr
ptr to `unsigned 2-byte integer` value.

## UInt32
`unsigned 4-byte integer` value.

## UInt32Array
`unsigned 4-byte integer` array of 2 cells.

## UInt32Ptr
ptr to `unsigned 4-byte integer` value.

## UInt64
`unsigned 8-byte integer` value.

## UInt64Ptr
ptr to `unsigned 8-byte integer` value.

## IntSize
`size_t` value.

## IntSizePtr
pointer to `size_t` value.

## IntNative
native sized signed integer value.

## UIntNative
native sized unsigned integer value.

## Float
`4-byte floating point` value.

## FloatArray
`4-byte floating point` array of 2 cells.

## FloatPtr
pointer to `4-byte floating point` value.

## Double
`8-byte floating point` value.

## DoublePtr
pointer to `8-byte floating point` value.

## Ptr
pointer to unknown type value.

## SelfPtr
pointer to a `union HarbolValue` value.

## VarPtr
pointer to a `struct HarbolVariant` value.

## StrObjPtr
pointer to a `struct HarbolString` value.

## VecPtr
pointer to a `struct HarbolVector` value.

## KvPairPtr
pointer to a `struct HarbolKeyValPair` value.

## MapPtr
pointer to a `struct HarbolHashmap` value.

## UniListPtr
pointer to a `struct HarbolUniList` value.

## BiListPtr
pointer to a `struct HarbolBiList` value.

## ByteBufferPtr
pointer to a `struct HarbolByteBuffer` value.

## TuplePtr
pointer to a `struct HarbolTuple` value.

## GraphEdgePtr
pointer to a `struct HarbolGraphEdge` value.

## GraphVertPtr
pointer to a `struct HarbolGraphVertex` value.

## GraphPtr
pointer to a `struct HarbolGraph` value.

## TreePtr
pointer to a `struct HarbolTree` value.

## LinkMapPtr
pointer to a `struct HarbolLinkMap` value.
