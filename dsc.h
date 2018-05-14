#pragma once

#ifdef __cplusplus
extern "C" {
#endif


#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include <iso646.h>

struct Variant;
struct String;
struct Vector;
struct Hashmap;
struct UniLinkedList;
struct BiLinkedList;
struct ByteBuffer;

union Value {
	bool Bool, *BoolPtr, (*BoolFunc)(), *(*BoolPtrFunc)();
	int8_t Char, *CharPtr, (*CharFunc)(), *(*CharPtrFunc)();
	int16_t Short, *ShortPtr, (*ShortFunc)(), *(*ShortPtrFunc)();
	int32_t Int32, *Int32Ptr, (*Int32Func)(), *(*Int32PtrFunc)();
	int64_t Int64, *Int64Ptr, (*Int64Func)(), *(*Int64PtrFunc)();
	
	uint8_t UChar, *UCharPtr, (*UCharFunc)(), *(*UCharPtrFunc)();
	uint16_t UShort, *UShortPtr, (*UShortFunc)(), *(*UShortPtrFunc)();
	uint32_t UInt32, *UInt32Ptr, (*UInt32Func)(), *(*UInt32PtrFunc)();
	uint64_t UInt64, *UInt64Ptr, (*UInt64Func)(), *(*UInt64PtrFunc)();
	
	float Float, *FloatPtr, (*FloatFunc)(), *(*FloatPtrFunc)();
	double Double, *DoublePtr, (*DoubleFunc)(), *(*DoublePtrFunc)();
	
	void *Ptr, **PtrPtr, (*VoidFunc)(), *(*VoidPtrFunc)();
	union Value *SelfPtr, (*SelfFunc)(), *(*SelfPtrFunc)();
	struct Variant *VariantPtr, (*VariantFunc)(), *(*VariantPtrFunc)();
	struct String *StrObjPtr, (*StrObjFunc)(), *(*StrObjPtrFunc)();
	struct Vector *VecPtr, (*VecFunc)(), *(*VecPtrFunc)();
	struct Hashmap *MapPtr, (*MapFunc)(), *(*MapPtrFunc)();
	struct UniLinkedList *UniListPtr, (*UniListFunc)(), *(*UniListPtrFunc)();
	struct BiLinkedList *BiListPtr, (*BiListFunc)(), *(*BiListPtrFunc)();
	struct ByteBuffer *BufferPtr, (*BufferFunc)(), *(*BufferPtrFunc)();
};

typedef enum ValType {
	TypeInvalid=0,
	// integer types
	TypeBool, TypeBoolPtr, TypeBoolFunc, TypeBoolPtrFunc,
	TypeChar, TypeCharPtr, TypeCharFunc, TypeCharPtrFunc, 
	TypeShort, TypeShortPtr, TypeShortFunc, TypeShortPtrFunc, 
	TypeInt32, TypeInt32Ptr, TypeInt32Func, TypeInt32PtrFunc, 
	TypeInt64, TypeInt64Ptr, TypeInt64Func, TypeInt64PtrFunc, 
	TypeUChar, TypeUCharPtr, TypeUCharFunc, TypeUCharPtrFunc, 
	TypeUShort, TypeUShortPtr, TypeUShortFunc, TypeUShortPtrFunc, 
	TypeUInt32, TypeUInt32Ptr, TypeUInt32Func, TypeUInt32PtrFunc, 
	TypeUInt64, TypeUInt64Ptr, TypeUInt64Func, TypeUInt64PtrFunc, 
	
	// floating point types
	TypeFloat, TypeFloatPtr, TypeFloatFunc, TypeFloatPtrFunc,
	TypeDouble, TypeDoublePtr, TypeDoubleFunc, TypeDoublePtrFunc,
	// misc.
	TypePtr, TypePtrPtr, TypeVoidFunc, TypePtrFunc,
	TypeSelfPtr, TypeSelfFunc, TypeSelfPtrFunc,
	TypeVariantPtr, TypeVariantFunc, TypeVariantPtrFunc,
	// data structure oriented.
	TypeStrObjPtr, TypeStrObjFunc, TypeStrObjPtrFunc,
	TypeVecPtr, TypeVecFunc, TypeVecPtrFunc,
	TypeMapPtr, TypeMapFunc, TypeMapPtrFunc,
	TypeUniListPtr, TypeUniListFunc, TypeUniListPtrFunc,
	TypeBiListPtr, TypeBiListFunc, TypeBiListPtrFunc,
	TypeBufferPtr, TypeBufferFunc, TypeBufferPtrFunc,
} ValType;

// discriminated union type
struct Variant {
	union Value Val;
	enum ValType TypeTag;
};

inline struct Variant *Variant_New(const union Value val, const enum ValType typeID)
{
	struct Variant *var = calloc(1, sizeof *var);
	if( var )
		var->Val = val, var->TypeTag = typeID;
	return var;
}


/************* C++ Style Automated String (stringobj.c) *************/
struct String {
	char *CStr;
	size_t Len;
};

struct String *String_New(void);
struct String *String_NewStr(const char *);
void String_Del(struct String *);
void String_Free(struct String **);
void String_Init(struct String *);
void String_InitStr(struct String *, const char *);
void String_AddChar(struct String *, char);
void String_Add(struct String *, const struct String *);
void String_AddStr(struct String *, const char *);
char *String_GetStr(const struct String *);
size_t String_Len(const struct String *);
void String_Copy(struct String *, const struct String *);
void String_CopyStr(struct String *, const char *);
int32_t String_StrCmpCStr(const struct String *, const char *);
int32_t String_StrCmpStr(const struct String *, const struct String *);
int32_t String_StrnCmpCStr(const struct String *, const char *, size_t);
int32_t String_StrnCmpStr(const struct String *, const struct String *, size_t);
bool String_IsEmpty(const struct String *);
/***************/


/************* Vector / Dynamic Array (vector.c) *************/
struct Vector {
	union Value *Table;
	bool (*Destructor)(/* Type **Obj */);
	size_t Len, Count;
};

struct Vector *Vector_New(bool (*)());
void Vector_Init(struct Vector *, bool (*)());
void Vector_Del(struct Vector *);
void Vector_Free(struct Vector **);

size_t Vector_Len(const struct Vector *);
size_t Vector_Count(const struct Vector *);
union Value *Vector_GetTable(const struct Vector *);
void Vector_Resize(struct Vector *);
void Vector_Truncate(struct Vector *);

bool Vector_Insert(struct Vector *, union Value);
union Value Vector_Get(const struct Vector *, size_t);
void Vector_Set(struct Vector *, size_t, union Value);
void Vector_SetItemDestructor(struct Vector *, bool (*)());

void Vector_Delete(struct Vector *, size_t);
void Vector_Add(struct Vector *, const struct Vector *);
void Vector_Copy(struct Vector *, const struct Vector *);

/***************/

/************* Hashmap (hashmap.c) *************/
struct KeyNode {
	struct String KeyName;
	union Value Data;	// use variant on this one.
	struct KeyNode *Next;
};

struct KeyNode *KeyNode_New(void);
struct KeyNode *KeyNode_NewSP(const char *, union Value);

void KeyNode_Del(struct KeyNode *, bool(*)());
bool KeyNode_Free(struct KeyNode **, bool(*)());


struct Hashmap {
	struct KeyNode **Table;
	bool (*Destructor)(/* Type **Obj */);
	size_t Len, Count;
};


struct Hashmap *Map_New(bool (*)());
void Map_Init(struct Hashmap *, bool (*)());
void Map_Del(struct Hashmap *);
void Map_Free(struct Hashmap **);
size_t Map_Count(const struct Hashmap *);
size_t Map_Len(const struct Hashmap *);
bool Map_Rehash(struct Hashmap *);

bool Map_InsertNode(struct Hashmap *, struct KeyNode *);
bool Map_Insert(struct Hashmap *, const char *, union Value);

union Value Map_Get(const struct Hashmap *, const char *);
void Map_Set(struct Hashmap *, const char *, union Value);
void Map_SetItemDestructor(struct Hashmap *, bool (*)());

void Map_Delete(struct Hashmap *, const char *);
bool Map_HasKey(const struct Hashmap *, const char *);
struct KeyNode *Map_GetKeyNode(const struct Hashmap *, const char *);
struct KeyNode **Map_GetKeyTable(const struct Hashmap *);
size_t GenHash(const char *);
/***************/

#if 0
/************* Key Value Tree (kvtree.c) *************/
struct KeyVal {
	struct String KeyName;
	struct Variant Data;
	struct KeyVal *NextKey;
};

struct KeyVal *KeyVal_New(void);
struct KeyVal *KeyVal_NewS(char *);
void KeyVal_Free(struct KeyVal **);
void KeyVal_Init(struct KeyVal *);
void KeyVal_Print(struct KeyVal *);

uint64_t KeyVal_GetInt(struct KeyVal *);
struct String *KeyVal_GetStr(struct KeyVal *);
struct KeyVal *KeyVal_GetSubKey(struct KeyVal *);
struct KeyVal *KeyVal_GetNextKey(struct KeyVal *);
char *KeyVal_GetKeyName(struct KeyVal *);

void KeyVal_SetInt(struct KeyVal *, uint64_t);
void KeyVal_SetSubKey(struct KeyVal *, struct KeyVal *);
void KeyVal_SetNextKey(struct KeyVal *, struct KeyVal *);
void KeyVal_SetKeyName(struct KeyVal *, char *);

struct KeyVal *KeyVal_GetFirstSubKey(struct KeyVal *);
struct KeyVal *KeyVal_GetNextSubKey(struct KeyVal *);
struct KeyVal *KeyVal_FindLastSubKey(struct KeyVal *);

struct KeyVal *KeyVal_FindByKeyName(struct KeyVal *, const char *);
bool KeyVal_HasKey(struct KeyVal *, const char *);
/***************/
#endif

/************* Singly Linked List (unilist.c) *************/
struct UniListNode {
	union Value Data;
	struct UniListNode *Next;
};

struct UniListNode *UniListNode_New(void);
struct UniListNode *UniListNode_NewVal(union Value);
void UniListNode_Del(struct UniListNode *, bool(*)());
void UniListNode_Free(struct UniListNode **, bool(*)());
struct UniListNode *UniListNode_GetNextNode(const struct UniListNode *);
union Value UniListNode_GetValue(const struct UniListNode *);


struct UniLinkedList {
	struct UniListNode *Head, *Tail;
	bool (*Destructor)(/* Type **Obj */);
	size_t Len;
};

struct UniLinkedList *UniLinkedList_New(bool (*)());
void UniLinkedList_Del(struct UniLinkedList *);
void UniLinkedList_Free(struct UniLinkedList **);
void UniLinkedList_Init(struct UniLinkedList *, bool (*)());

size_t UniLinkedList_Len(const struct UniLinkedList *);
bool UniLinkedList_InsertNodeAtHead(struct UniLinkedList *, struct UniListNode *);
bool UniLinkedList_InsertNodeAtTail(struct UniLinkedList *, struct UniListNode *);
bool UniLinkedList_InsertNodeAtIndex(struct UniLinkedList *, struct UniListNode *, size_t);
bool UniLinkedList_InsertValueAtHead(struct UniLinkedList *, union Value);
bool UniLinkedList_InsertValueAtTail(struct UniLinkedList *, union Value);
bool UniLinkedList_InsertValueAtIndex(struct UniLinkedList *, union Value, size_t);

struct UniListNode *UniLinkedList_GetNode(const struct UniLinkedList *, size_t);
union Value UniLinkedList_GetValue(const struct UniLinkedList *, size_t);
void UniLinkedList_SetValue(struct UniLinkedList *, size_t, union Value);
bool UniLinkedList_DelNodeByIndex(struct UniLinkedList *, size_t);
bool UniLinkedList_DelNodeByRef(struct UniLinkedList *, struct UniListNode **);
void UniLinkedList_SetDestructor(struct UniLinkedList *, bool (*)());

struct UniListNode *UniLinkedList_GetHead(const struct UniLinkedList *);
struct UniListNode *UniLinkedList_GetTail(const struct UniLinkedList *);
/***************/


/************* Doubly Linked List (bilist.c) *************/
struct BiListNode {
	union Value Data;
	struct BiListNode *Next, *Prev;
};

struct BiListNode *BiListNode_New(void);
struct BiListNode *BiListNode_NewVal(union Value);
void BiListNode_Del(struct BiListNode *, bool(*)());
void BiListNode_Free(struct BiListNode **, bool(*)());
struct BiListNode *BiListNode_GetNextNode(const struct BiListNode *);
struct BiListNode *BiListNode_GetPrevNode(const struct BiListNode *);
union Value BiListNode_GetValue(const struct BiListNode *);


struct BiLinkedList {
	struct BiListNode *Head, *Tail;
	bool (*Destructor)(/* Type **Obj */);
	size_t Len;
};

struct BiLinkedList *BiLinkedList_New(bool (*)());
void BiLinkedList_Del(struct BiLinkedList *);
void BiLinkedList_Free(struct BiLinkedList **);
void BiLinkedList_Init(struct BiLinkedList *, bool (*)());

size_t BiLinkedList_Len(const struct BiLinkedList *);
bool BiLinkedList_InsertNodeAtHead(struct BiLinkedList *, struct BiListNode *);
bool BiLinkedList_InsertNodeAtTail(struct BiLinkedList *, struct BiListNode *);
bool BiLinkedList_InsertNodeAtIndex(struct BiLinkedList *, struct BiListNode *, size_t);
bool BiLinkedList_InsertValueAtHead(struct BiLinkedList *, union Value);
bool BiLinkedList_InsertValueAtTail(struct BiLinkedList *, union Value);
bool BiLinkedList_InsertValueAtIndex(struct BiLinkedList *, union Value, size_t);

struct BiListNode *BiLinkedList_GetNode(const struct BiLinkedList *, size_t);
union Value BiLinkedList_GetValue(const struct BiLinkedList *, size_t);
void BiLinkedList_SetValue(struct BiLinkedList *, size_t, union Value);
bool BiLinkedList_DelNodeByIndex(struct BiLinkedList *, size_t);
bool BiLinkedList_DelNodeByRef(struct BiLinkedList *, struct BiListNode **);
void BiLinkedList_SetDestructor(struct BiLinkedList *, bool (*)());

struct BiListNode *BiLinkedList_GetHead(const struct BiLinkedList *);
struct BiListNode *BiLinkedList_GetTail(const struct BiLinkedList *);
/***************/


/************* Byte Buffer (bytebuffer.c) *************/
struct ByteBuffer {
	uint8_t *Buffer;
	size_t Len, Count;
};

struct ByteBuffer *ByteBuffer_New(void);
void ByteBuffer_Init(struct ByteBuffer *);
void ByteBuffer_Del(struct ByteBuffer *);
void ByteBuffer_Free(struct ByteBuffer **);
size_t ByteBuffer_Len(const struct ByteBuffer *);
size_t ByteBuffer_Count(const struct ByteBuffer *);
uint8_t *ByteBuffer_GetBuffer(const struct ByteBuffer *);
void ByteBuffer_InsertByte(struct ByteBuffer *, uint8_t);
void ByteBuffer_InsertInt(struct ByteBuffer *, uint64_t, size_t);
void ByteBuffer_InsertFloat(struct ByteBuffer *, float);
void ByteBuffer_InsertDouble(struct ByteBuffer *, double);
void ByteBuffer_InsertString(struct ByteBuffer *, const char *, size_t);
void ByteBuffer_InsertObject(struct ByteBuffer *, const void *, size_t);
void ByteBuffer_Delete(struct ByteBuffer *, size_t);
void ByteBuffer_Resize(struct ByteBuffer *);
void ByteBuffer_DumpToFile(/* struct ByteBuffer *, FILE * */);
size_t ByteBuffer_ReadFromFile(/* struct ByteBuffer *, FILE * */);
void ByteBuffer_Append(struct ByteBuffer *, struct ByteBuffer *);
/***************/

#ifdef __cplusplus
}
#endif
