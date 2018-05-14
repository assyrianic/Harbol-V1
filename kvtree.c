
#include <stdlib.h>
#include <stdio.h>
#include "dsc.h"


struct KeyVal *KeyVal_New(void)
{
	struct KeyVal *node = calloc(1, sizeof *node);
	if( node ) {
		node->KeyName = String_New();
		if( !node->KeyName )
			free(node), node=NULL;
	}
	return node;
}

struct KeyVal *KeyVal_NewS(char *__restrict strKeyName)
{
	struct KeyVal *node = calloc(1, sizeof *node);
	if( node ) {
		node->KeyName = String_New();
		if( !node->KeyName ) {
			free(node), node=NULL;
			return node;
		}
		String_CopyStr(node->KeyName, strKeyName);
	}
	return node;
}

void KeyVal_Free(struct KeyVal **__restrict kv)
{
	if( !*kv )
		return;
	
	struct KeyVal *iter = *kv;
	String_Del(&iter->KeyName);
	if( iter->Data.Type==TypeKeyval )
		KeyVal_Free(&iter->Data.Val.Keyval);
	else if( iter->Data.Type==TypeStr )
		String_Free(&iter->Data.Val.Str);
	
	if( iter->NextKey )
		KeyVal_Free(&iter->NextKey);
	free(*kv); *kv=NULL;
	iter=NULL;
}

void KeyVal_Init(struct KeyVal *const __restrict kv)
{
	if( !kv )
		return;
	
	memset(kv, 0, sizeof *kv);
	kv->KeyName = String_New();
}


void KeyVal_Print(struct KeyVal *const __restrict kv)
{
	if( !kv )
		return;
	
	printf("KeyVal_Print :: Key Name: %s\n", kv->KeyName->CStr);
	if( kv->Data.Type==TypeKeyval ) {
		puts("Printing SubKey");
		KeyVal_Print(kv->Data.Val.Keyval);
	}
	else if( kv->Data.Type==TypeStr )
		printf("KeyVal_Print :: String Value: %s\n", kv->Data.Val.Str->CStr);
	else printf("KeyVal_Print :: Int Value: %llu\n", kv->Data.Val.UInt64);
	
	if( kv->NextKey ) {
		puts("Printing NextKey");
		KeyVal_Print(kv->NextKey);
	}
}

uint64_t KeyVal_GetInt(struct KeyVal *const __restrict kv)
{
	return !kv or kv->Data.Type != TypeUInt64 ? 0 : kv->Data.Val.UInt64;
}

struct String *KeyVal_GetStr(struct KeyVal *const __restrict kv)
{
	return !kv or kv->Data.Type != TypeStr ? NULL : kv->Data.Val.Str;
}

struct KeyVal *KeyVal_GetSubKey(struct KeyVal *const __restrict kv)
{
	return !kv or kv->Data.Type != TypeKeyval ? NULL : kv->Data.Val.Keyval;
}

struct KeyVal *KeyVal_GetNextKey(struct KeyVal *const __restrict kv)
{
	return !kv ? NULL : kv->NextKey;
}

char *KeyVal_GetKeyName(struct KeyVal *const __restrict kv)
{
	return !kv or !kv->KeyName ? NULL : kv->KeyName->CStr;
}


void KeyVal_SetInt(struct KeyVal *const __restrict kv, const uint64_t val)
{
	if( !kv )
		return;
	
	kv->Data.Val.UInt64 = val;
	kv->Data.Type = TypeUInt64;
}

void KeyVal_SetSubKey(struct KeyVal *const __restrict kv, struct KeyVal *const __restrict subkey)
{
	if( !kv or !subkey )
		return;
	
	kv->Data.Val.Keyval = subkey;
	kv->Data.Type = TypeKeyval;
}

void KeyVal_SetNextKey(struct KeyVal *const __restrict kv, struct KeyVal *const __restrict nextkey)
{
	if( !kv or !nextkey )
		return;
	
	kv->NextKey = nextkey;
}

void KeyVal_SetKeyName(struct KeyVal *const __restrict kv, char *__restrict cstr)
{
	if( !kv or !cstr )
		return;
	
	String_CopyStr(kv->KeyName, cstr);
}

struct KeyVal *KeyVal_GetFirstSubKey(struct KeyVal *const __restrict kv)
{
	if( !kv )
		return NULL;
	
	struct KeyVal *Ret = kv->Data.Val.Keyval;
	while( Ret and Ret->Data.Type != TypeKeyval )
		Ret = Ret->NextKey;
	
	return Ret;
}

struct KeyVal *KeyVal_GetNextSubKey(struct KeyVal *const __restrict kv)
{
	if( !kv )
		return NULL;
	
	struct KeyVal *Ret = kv->NextKey;
	while( Ret and Ret->Data.Type != TypeKeyval )
		Ret = Ret->NextKey;
	
	return Ret;
}

struct KeyVal *KeyVal_FindLastSubKey(struct KeyVal *const __restrict kv)
{
	if( !kv )
		return NULL;
	else if( kv->Data.Type==TypeKeyval and !kv->Data.Val.Keyval )
		return NULL;
	
	// Scan for the last one
	struct KeyVal *lastKid = kv->Data.Val.Keyval;
	while( lastKid->NextKey )
		lastKid = lastKid->NextKey;
	return lastKid;
}

struct KeyVal *KeyVal_FindByKeyName(struct KeyVal *const __restrict kv, char *__restrict cstr)
{
	if( !kv or !cstr )
		return NULL;
	
	struct KeyVal *k=NULL;
	// iterate through our linked keys first.
	for( k=kv->Data.Val.Keyval ; k ; k=k->NextKey ) {
		if( !String_StrCmpCStr(k->KeyName, cstr) )
			break;
	}
	return k;
}


bool KeyVal_HasKey(struct KeyVal *const __restrict kv, char *__restrict cstr)
{
	if( !kv or !cstr )
		return false;
	
	struct KeyVal *k=NULL;
	// iterate through our linked keys first.
	for( k=kv->Data.Val.Keyval ; k ; k=k->NextKey ) {
		if( !String_StrCmpCStr(k->KeyName, cstr) )
			break;
	}
	return k != NULL;
}


// https://www.youtube.com/watch?v=oY2KnW19Tls
