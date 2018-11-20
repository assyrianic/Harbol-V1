#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef OS_WINDOWS
	#define HARBOL_LIB
#endif
#include "harbol.h"

/* CFG Parser semantics in EBNF
	keyval = <string> [':'] (<value>|<section>) [','] ;
	section = '{' <keyval> '}' ;
	value = <string> | <number> | <color> | "true" | "false" | "null" ;
	matrix = '[' <number> [','] <number> [','] <number> [','] <number> ']' ;
	color = 'c' <matrix> ;
	vecf = 'v' <matrix> ;
	string = '"' chars '"' | "'" chars "'" ;
*/

static bool IsDecimal(const char c)
{
	return( c >= '0' && c <= '9' );
}

static bool IsOctal(const char c)
{
	return( c >= '0' && c <= '7' );
}

static bool IsHex(const char c)
{
	return( (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || IsDecimal(c) );
}

static bool IsWhiteSpace(const char c)
{
	return( c==' ' || c=='\t' || c=='\r' || c=='\v' || c=='\f' || c=='\n' );
}


static void SkipSingleLineComment(const char **strRef)
{
	if( !*strRef || !**strRef )
		return;
	
	for( ; **strRef != '\n' ; (*strRef)++ );
}

static void SkipMultiLineComment(const char **strRef)
{
	if( !*strRef || !**strRef )
		return;
	// skip past '/' && '*'
	(*strRef)++;
	do {
		(*strRef)++;
		if( !(*strRef)[1] )
			break;
	} while( **strRef!='*' && (*strRef)[1]!='/' );
}

static bool SkipWhiteSpace(const char **strRef)
{
	if( !*strRef || !**strRef )
		return false;
	
	while( **strRef && IsWhiteSpace(**strRef) )
		(*strRef)++;
	return **strRef != 0;
}

static bool SkipWSandComments(const char **strRef)
{
	if( !strRef || !*strRef || !**strRef )
		return false;
	
	if( IsWhiteSpace(**strRef) ) {
		SkipWhiteSpace(strRef);
		return SkipWSandComments(strRef);
	}
	else if( **strRef=='#' || (**strRef=='/' && (*strRef)[1]=='/') ) {
		SkipSingleLineComment(strRef);
		return SkipWSandComments(strRef);
	}
	else if( **strRef=='/' && (*strRef)[1]=='*' ) {
		SkipMultiLineComment(strRef);
		return SkipWSandComments(strRef);
	}
	else if( **strRef==':' || **strRef==',' ) {
		(*strRef)++;
		return SkipWSandComments(strRef);
	}
	else {
		return **strRef != 0;
	}
}

static bool LexString(const char **restrict strRef, struct HarbolString *const restrict str)
{
	if( !*strRef || !**strRef || !str )
		return false;
	
	else if( **strRef != '"' && **strRef != '\'' )
		return false;
	const char quote = *(*strRef)++;
	while( **strRef && **strRef != quote )
		HarbolString_AddChar(str, *(*strRef)++);
	(*strRef)++;
	return **strRef != 0;
}

static bool LexNumber(const char **restrict strRef, struct HarbolString *const restrict str, enum HarbolCfgType *typeref)
{
	if( !*strRef || !**strRef || !str || !typeref )
		return false;
	
	if( **strRef=='-' || **strRef=='+' )
		HarbolString_AddChar(str, *(*strRef)++);
	
	if( **strRef!='0' && !IsDecimal(**strRef) && **strRef!='.' )
		return false;
	
	if( **strRef=='0' ) {
		HarbolString_AddChar(str, *(*strRef)++);
		const char numtype = *(*strRef)++;
		HarbolString_AddChar(str, numtype);
		*typeref = HarbolTypeInt;
		
		switch( numtype ) {
			case 'X': case 'x': // hex
				HarbolString_AddChar(str, *(*strRef)++);
				while( IsHex(**strRef) )
					HarbolString_AddChar(str, *(*strRef)++);
				break;
			case '.': // float
				*typeref = HarbolTypeFloat;
				HarbolString_AddChar(str, *(*strRef)++);
				while( IsDecimal(**strRef) || **strRef=='e' || **strRef=='E' )
					HarbolString_AddChar(str, *(*strRef)++);
				break;
			default: // octal
				while( IsOctal(**strRef) )
					HarbolString_AddChar(str, *(*strRef)++);
		}
	}
	else if( IsDecimal(**strRef) ) { // numeric value. Check if float possibly.
		*typeref = HarbolTypeInt;
		while( IsDecimal(**strRef) )
			HarbolString_AddChar(str, *(*strRef)++);
		
		if( **strRef=='.' ) { // definitely float value.
			*typeref = HarbolTypeFloat;
			HarbolString_AddChar(str, *(*strRef)++);
			while( IsDecimal(**strRef) || **strRef=='e' || **strRef=='E' )
				HarbolString_AddChar(str, *(*strRef)++);
		}
	}
	else if( **strRef=='.' ) { // float value.
		*typeref = HarbolTypeFloat;
		HarbolString_AddChar(str, *(*strRef)++);
		while( IsDecimal(**strRef) || **strRef=='e' || **strRef=='E' )
			HarbolString_AddChar(str, *(*strRef)++);
	}
	return str->Len > 0;
}

static bool HarbolCfg_ParseSection(struct HarbolLinkMap *, const char **);
static bool HarbolCfg_ParseNumber(struct HarbolLinkMap *, const struct HarbolString *, const char **);

// keyval = <string> [':'] (<value>|<section>) [','] ;
static bool HarbolCfg_ParseKeyVal(struct HarbolLinkMap *const restrict map, const char **cfgcoderef)
{
	if( !map || !*cfgcoderef || !**cfgcoderef )
		return false;
	
	const bool safe = SkipWSandComments(cfgcoderef);
	if( !safe || (**cfgcoderef!='"' && **cfgcoderef!='\'') )
		return false;
	
	struct HarbolString keystr = {NULL, 0};
	const bool strresult = LexString(cfgcoderef, &keystr);
	if( !strresult ) {
		HarbolString_Del(&keystr);
		return false;
	}
	SkipWSandComments(cfgcoderef);
	
	bool res = false;
	// it's a section!
	if( **cfgcoderef=='{' ) {
		struct HarbolLinkMap *restrict subsection = HarbolLinkMap_New();
		res = HarbolCfg_ParseSection(subsection, cfgcoderef);
		struct HarbolVariant *var = HarbolVariant_New((union HarbolValue){.Ptr=subsection}, HarbolTypeLinkMap);
		HarbolLinkMap_Insert(map, keystr.CStr, (union HarbolValue){ .VarPtr=var });
	}
	// string value.
	else if( **cfgcoderef=='"'||**cfgcoderef=='\'' ) {
		struct HarbolString *str = HarbolString_New();
		res = LexString(cfgcoderef, str);
		if( !res )
			return false;
		
		struct HarbolVariant *var = HarbolVariant_New((union HarbolValue){.Ptr=str}, HarbolTypeString);
		HarbolLinkMap_Insert(map, keystr.CStr, (union HarbolValue){ .VarPtr=var });
	}
	// color/vector value!
	else if( **cfgcoderef=='c' || **cfgcoderef=='v' ) {
		const char valtype = *(*cfgcoderef)++;
		SkipWSandComments(cfgcoderef);
		
		if( **cfgcoderef!='[' ) {
			HarbolString_Del(&keystr);
			return false;
		}
		(*cfgcoderef)++;
		SkipWSandComments(cfgcoderef);
		
		union {
			union HarbolColor color;
			union HarbolVec4D vec4d;
		} matrix_value = {0};
		size_t iterations = 0;
		while( **cfgcoderef && **cfgcoderef != ']' ) {
			struct HarbolString numstr = {NULL, 0};
			enum HarbolCfgType type = HarbolTypeNull;
			const bool result = LexNumber(cfgcoderef, &numstr, &type);
			if( iterations<4 ) {
				if( valtype=='c' ) {
					matrix_value.color.RGBA[iterations++] = (uint8_t)strtoul(numstr.CStr, NULL, 0);
				}
				else {
					matrix_value.vec4d.XYZW[iterations++] = (float)strtof(numstr.CStr, NULL);
				}
			}
			HarbolString_Del(&numstr);
			if( !result ) {
				HarbolString_Del(&keystr);
				return false;
			}
			SkipWSandComments(cfgcoderef);
		}
		(*cfgcoderef)++;
		
		const size_t matrix_data[] = { valtype=='c' ? sizeof(union HarbolColor) : sizeof(union HarbolVec4D) };
		struct HarbolTuple *tuple = HarbolTuple_New(1, matrix_data, false);
		valtype=='c' ?
			HarbolTuple_SetField(tuple, 0, &matrix_value.color.UIntColor)
			: HarbolTuple_SetField(tuple, 0, &matrix_value.vec4d.XYZW[0]);
		
		struct HarbolVariant *var = HarbolVariant_New((union HarbolValue){.TuplePtr=tuple}, valtype=='c' ? HarbolTypeColor : HarbolTypeVec4D);
		res = HarbolLinkMap_Insert(map, keystr.CStr, (union HarbolValue){ .VarPtr=var });
	}
	// true bool value.
	else if( **cfgcoderef=='t' ) {
		if( strncmp("true", *cfgcoderef, strlen("true")) ) {
			HarbolString_Del(&keystr);
			return false;
		}
		*cfgcoderef += strlen("true");
		struct HarbolVariant *var = HarbolVariant_New((union HarbolValue){.Bool=true}, HarbolTypeBool);
		res = HarbolLinkMap_Insert(map, keystr.CStr, (union HarbolValue){ .VarPtr=var });
	}
	// false bool value.
	else if( **cfgcoderef=='f' ) {
		if( strncmp("false", *cfgcoderef, strlen("false")) ) {
			HarbolString_Del(&keystr);
			return false;
		}
		*cfgcoderef += strlen("false");
		struct HarbolVariant *var = HarbolVariant_New((union HarbolValue){.Bool=false}, HarbolTypeBool);
		res = HarbolLinkMap_Insert(map, keystr.CStr, (union HarbolValue){ .VarPtr=var });
	}
	// null value.
	else if( **cfgcoderef=='n' ) {
		if( strncmp("null", *cfgcoderef, strlen("null")) ) {
			HarbolString_Del(&keystr);
			return false;
		}
		*cfgcoderef += strlen("null");
		struct HarbolVariant *var = HarbolVariant_New((union HarbolValue){0}, HarbolTypeNull);
		res = HarbolLinkMap_Insert(map, keystr.CStr, (union HarbolValue){ .VarPtr=var });
	}
	// numeric value.
	else {
		res = HarbolCfg_ParseNumber(map, &keystr, cfgcoderef);
	}
	HarbolString_Del(&keystr);
	SkipWSandComments(cfgcoderef);
	return res;
}

static bool HarbolCfg_ParseNumber(struct HarbolLinkMap *const restrict map, const struct HarbolString *const restrict key, const char **cfgcoderef)
{
	struct HarbolString numstr = {NULL, 0};
	enum HarbolCfgType type = HarbolTypeNull;
	
	const bool result = LexNumber(cfgcoderef, &numstr, &type);
	if( !result ) {
		HarbolString_Del(&numstr);
		return result;
	}
	struct HarbolVariant *var = HarbolVariant_New(type==HarbolTypeFloat ? (union HarbolValue){.Double=strtod(numstr.CStr, NULL)} : (union HarbolValue){.Int64=strtoll(numstr.CStr, NULL, 0)}, type);
	HarbolString_Del(&numstr);
	return HarbolLinkMap_Insert(map, key->CStr, (union HarbolValue){ .VarPtr=var });
}

// section = '{' <keyval> '}' ;
static bool HarbolCfg_ParseSection(struct HarbolLinkMap *const restrict map, const char **cfgcoderef)
{
	if( **cfgcoderef!='{' )
		return false;
	
	(*cfgcoderef)++;
	SkipWSandComments(cfgcoderef);
	
	while( **cfgcoderef && **cfgcoderef != '}' ) {
		const bool res = HarbolCfg_ParseKeyVal(map, cfgcoderef);
		if( !res )
			return false;
	}
	(*cfgcoderef)++;
	return true;
}


HARBOL_EXPORT struct HarbolLinkMap *HarbolCfg_ParseFile(const char *restrict filename)
{
	if( !filename )
		return NULL;
	
	FILE *restrict cfgfile = fopen(filename, "r+");
	if( !cfgfile )
		return NULL;
	
	fseek(cfgfile, 0, SEEK_END);
	const int64_t filesize = ftell(cfgfile);
	if( filesize <= -1 ) {
		fclose(cfgfile), cfgfile=NULL;
		return NULL;
	}
	rewind(cfgfile);
	
	char *restrict cfgcode = calloc(filesize, sizeof *cfgcode);
	if( !cfgcode ) {
		fclose(cfgfile), cfgfile=NULL;
		return NULL;
	}
	const size_t val = fread(cfgcode, filesize, sizeof *cfgcode, cfgfile);
	if( val != (size_t)filesize ) {
		fclose(cfgfile), cfgfile=NULL;
		free(cfgcode), cfgcode=NULL;
		return NULL;
	}
	struct HarbolLinkMap *const restrict objs = HarbolCfg_Parse(cfgcode);
	free(cfgcode), cfgcode=NULL;
	return objs;
}


HARBOL_EXPORT struct HarbolLinkMap *HarbolCfg_Parse(const char *cfgcode)
{
	if( !cfgcode )
		return NULL;
	
	const char *iter = cfgcode;
	struct HarbolLinkMap *objs = HarbolLinkMap_New();
	if( !HarbolCfg_ParseKeyVal(objs, &iter) )
		HarbolCfg_Free(&objs);
	return objs;
}

HARBOL_EXPORT bool HarbolCfg_Free(struct HarbolLinkMap **mapref)
{
	if( !mapref || !*mapref )
		return false;
	
	const union HarbolValue *const end = HarbolLinkMap_GetIterEndCount(*mapref);
	for( union HarbolValue *iter = HarbolLinkMap_GetIter(*mapref) ; iter && iter<end ; iter++ ) {
		struct HarbolKeyValPair *n = iter->KvPairPtr;
		switch( n->Data.VarPtr->TypeTag ) {
			case HarbolTypeLinkMap: {
				struct HarbolLinkMap *innermap = HarbolVariant_GetVal(n->Data.VarPtr).LinkMapPtr;
				HarbolCfg_Free(&innermap);
				break;
			}
			case HarbolTypeString: {
				struct HarbolString *strtype = HarbolVariant_GetVal(n->Data.VarPtr).StrObjPtr;
				HarbolString_Free(&strtype);
				break;
			}
			case HarbolTypeColor:
			case HarbolTypeVec4D: {
				struct HarbolTuple *tup = HarbolVariant_GetVal(n->Data.VarPtr).TuplePtr;
				HarbolTuple_Free(&tup);
				break;
			}
			default:
				break;
		}
		HarbolVariant_Free(&n->Data.VarPtr, NULL);
	}
	HarbolLinkMap_Free(mapref, NULL);
	return *mapref==NULL;
}

/*
static const char *Harbol_GetTypeName(const enum HarbolCfgType type)
{
	switch( type ) {
		case HarbolTypeNull:        return "Null";
		case HarbolTypeLinkMap:     return "LinkMap";
		case HarbolTypeString:      return "String";
		case HarbolTypeFloat:       return "Float";
		case HarbolTypeInt:         return "Int";
		case HarbolTypeBool:        return "Boolean";
		case HarbolTypeColor:       return "Color";
		case HarbolTypeVec4D:       return "Vector";
		default: return "Unknown Type";
	}
}
*/

HARBOL_EXPORT bool HarbolCfg_ToString(const struct HarbolLinkMap *const restrict map, struct HarbolString *const str)
{
	if( !map || !str )
		return false;
	
	# define BUFFER_SIZE    512
	const union HarbolValue *const end = HarbolLinkMap_GetIterEndCount(map);
	for( union HarbolValue *iter = HarbolLinkMap_GetIter(map) ; iter && iter<end ; iter++ ) {
		const struct HarbolKeyValPair *kv = iter->KvPairPtr;
		const int32_t type = kv->Data.VarPtr->TypeTag;
		// print out key and notation.
		HarbolString_AddChar(str, '"');
		HarbolString_Add(str, &kv->KeyName);
		HarbolString_AddStr(str, "\": ");
		
		char buffer[BUFFER_SIZE] = {0};
		switch( type ) {
			case HarbolTypeNull:
				HarbolString_AddStr(str, "null\n");
				break;
			case HarbolTypeLinkMap: {
				HarbolString_AddStr(str, "{\n");
				HarbolCfg_ToString(kv->Data.VarPtr->Val.LinkMapPtr, str);
				HarbolString_AddStr(str, "}\n");
				break;
			}
			case HarbolTypeString:
				HarbolString_AddStr(str, "\"");
				HarbolString_Add(str, kv->Data.VarPtr->Val.StrObjPtr);
				HarbolString_AddStr(str, "\"\n");
				break;
			case HarbolTypeFloat:
				snprintf(buffer, BUFFER_SIZE, "%f\n", kv->Data.VarPtr->Val.Double);
				HarbolString_AddStr(str, buffer);
				break;
			case HarbolTypeInt:
				snprintf(buffer, BUFFER_SIZE, "%" PRIi64 "\n", kv->Data.VarPtr->Val.Int64);
				HarbolString_AddStr(str, buffer);
				break;
			case HarbolTypeBool:
				HarbolString_AddStr(str, kv->Data.VarPtr->Val.Bool ? "true\n" : "false\n");
				break;
			case HarbolTypeColor: {
				HarbolString_AddStr(str, "c[ ");
				struct { uint8_t r,g,b,a; } color = {0};
				HarbolTuple_ToStruct(kv->Data.VarPtr->Val.TuplePtr, &color);
				snprintf(buffer, BUFFER_SIZE, "%u, %u, %u, %u ]\n", color.r, color.g, color.b, color.a);
				HarbolString_AddStr(str, buffer);
				break;
			}
			case HarbolTypeVec4D: {
				HarbolString_AddStr(str, "v[ ");
				struct { float x,y,z,w; } vec4 = {0};
				HarbolTuple_ToStruct(kv->Data.VarPtr->Val.TuplePtr, &vec4);
				snprintf(buffer, BUFFER_SIZE, "%f, %f, %f, %f ]\n", vec4.x, vec4.y, vec4.z, vec4.w);
				HarbolString_AddStr(str, buffer);
				break;
			}
		}
	}
	return str->Len > 0;
}

static bool HarbolCfg_ParseTargetStringPath(const char *key, struct HarbolString *const restrict str)
{
	if( !key || !str )
		return false;
	
	// parse something like: "root.section1.section2.section3"
	const char *iter = key;
	
	// iterate to the null terminator and then work backwards to the last dot.
	// ughhh too many while loops lmao.
	iter += strlen(key);
	while( *iter != '.' ) 
		--iter;
	iter++;
	
	// now we save the target section and then 
	while( *iter )
		HarbolString_AddChar(str, *iter++);
	return str->Len > 0;
}

HARBOL_EXPORT struct HarbolLinkMap *HarbolCfg_GetSectionByKey(struct HarbolLinkMap *const cfgmap, const char *key)
{
	if( !cfgmap || !key )
		return NULL;
	
	struct HarbolString
		sectionstr = {NULL, 0},
		targetstr = {NULL, 0}
	;
	HarbolCfg_ParseTargetStringPath(key, &targetstr);
	const char *iter = key;
	
	// use a single linkmap ptr to iterate.
	struct HarbolLinkMap
		*itermap = cfgmap,
		*restrict retmap = NULL
	;
	
	while( itermap ) {
		while( *iter && *iter != '.' )
			HarbolString_AddChar(&sectionstr, *iter++);
		iter++;
		
		const struct HarbolVariant *const restrict var = HarbolLinkMap_Get(itermap, sectionstr.CStr).VarPtr;
		if( !var || var->TypeTag != HarbolTypeLinkMap )
			break;
		
		else if( !HarbolString_CmpStr(&sectionstr, &targetstr) ) {
			retmap = var->Val.LinkMapPtr;
			break;
		}
		else itermap = var->Val.LinkMapPtr;
		HarbolString_Del(&sectionstr);
	}
	HarbolString_Del(&sectionstr);
	HarbolString_Del(&targetstr);
	return retmap;
}

HARBOL_EXPORT struct HarbolString *HarbolCfg_GetStrByKey(struct HarbolLinkMap *const cfgmap, const char *key)
{
	if( !cfgmap || !key )
		return NULL;
	
	struct HarbolString
		sectionstr = {NULL, 0},
		targetstr = {NULL, 0}
	;
	HarbolCfg_ParseTargetStringPath(key, &targetstr);
	const char *iter = key;
	
	// use a single linkmap ptr to iterate.
	struct HarbolLinkMap *itermap = cfgmap;
	struct HarbolString *restrict retstr = NULL;
	while( itermap ) {
		while( *iter && *iter != '.' )
			HarbolString_AddChar(&sectionstr, *iter++);
		iter++;
		
		const struct HarbolVariant *const restrict var = HarbolLinkMap_Get(itermap, sectionstr.CStr).VarPtr;
		if( !var ) {
			break;
		}
		else if( var->TypeTag==HarbolTypeLinkMap )
			itermap = var->Val.LinkMapPtr;
		else if( !HarbolString_CmpStr(&sectionstr, &targetstr) && var->TypeTag==HarbolTypeString ) {
			retstr = var->Val.StrObjPtr;
			break;
		}
		HarbolString_Del(&sectionstr);
	}
	HarbolString_Del(&sectionstr);
	HarbolString_Del(&targetstr);
	return retstr;
}

HARBOL_EXPORT double HarbolCfg_GetFloatByKey(struct HarbolLinkMap *const cfgmap, const char *key)
{
	if( !cfgmap || !key )
		return 0.0;
	
	struct HarbolString
		sectionstr = {NULL, 0},
		targetstr = {NULL, 0}
	;
	HarbolCfg_ParseTargetStringPath(key, &targetstr);
	const char *iter = key;
	
	// use a single linkmap ptr to iterate.
	struct HarbolLinkMap *itermap = cfgmap;
	double retdbl = 0.0;
	while( itermap ) {
		while( *iter && *iter != '.' )
			HarbolString_AddChar(&sectionstr, *iter++);
		iter++;
		
		const struct HarbolVariant *const restrict var = HarbolLinkMap_Get(itermap, sectionstr.CStr).VarPtr;
		if( !var ) {
			break;
		}
		else if( var->TypeTag==HarbolTypeLinkMap )
			itermap = var->Val.LinkMapPtr;
		else if( !HarbolString_CmpStr(&sectionstr, &targetstr) && var->TypeTag==HarbolTypeFloat ) {
			retdbl = var->Val.Double;
			break;
		}
		HarbolString_Del(&sectionstr);
	}
	HarbolString_Del(&sectionstr);
	HarbolString_Del(&targetstr);
	return retdbl;
}

HARBOL_EXPORT int64_t HarbolCfg_GetIntByKey(struct HarbolLinkMap *const cfgmap, const char *key)
{
	if( !cfgmap || !key )
		return 0LL;
	
	struct HarbolString
		sectionstr = {NULL, 0},
		targetstr = {NULL, 0}
	;
	HarbolCfg_ParseTargetStringPath(key, &targetstr);
	const char *iter = key;
	
	// use a single linkmap ptr to iterate.
	struct HarbolLinkMap *itermap = cfgmap;
	int64_t retint = 0LL;
	while( itermap ) {
		while( *iter && *iter != '.' )
			HarbolString_AddChar(&sectionstr, *iter++);
		iter++;
		
		const struct HarbolVariant *const restrict var = HarbolLinkMap_Get(itermap, sectionstr.CStr).VarPtr;
		if( !var ) {
			break;
		}
		else if( var->TypeTag==HarbolTypeLinkMap )
			itermap = var->Val.LinkMapPtr;
		else if( !HarbolString_CmpStr(&sectionstr, &targetstr) && var->TypeTag==HarbolTypeInt ) {
			retint = var->Val.Int64;
			break;
		}
		HarbolString_Del(&sectionstr);
	}
	HarbolString_Del(&sectionstr);
	HarbolString_Del(&targetstr);
	return retint;
}

HARBOL_EXPORT bool HarbolCfg_GetBoolByKey(struct HarbolLinkMap *const cfgmap, const char *key, bool *const restrict boolref)
{
	if( !cfgmap || !key || !boolref )
		return false;
	
	struct HarbolString
		sectionstr = {NULL, 0},
		targetstr = {NULL, 0}
	;
	HarbolCfg_ParseTargetStringPath(key, &targetstr);
	const char *iter = key;
	
	// use a single linkmap ptr to iterate.
	struct HarbolLinkMap *itermap = cfgmap;
	while( itermap ) {
		while( *iter && *iter != '.' )
			HarbolString_AddChar(&sectionstr, *iter++);
		iter++;
		
		const struct HarbolVariant *const restrict var = HarbolLinkMap_Get(itermap, sectionstr.CStr).VarPtr;
		if( !var ) {
			break;
		}
		else if( var->TypeTag==HarbolTypeLinkMap )
			itermap = var->Val.LinkMapPtr;
		else if( !HarbolString_CmpStr(&sectionstr, &targetstr) && var->TypeTag==HarbolTypeBool ) {
			*boolref = var->Val.Bool;
			HarbolString_Del(&sectionstr);
			HarbolString_Del(&targetstr);
			return true;
		}
		HarbolString_Del(&sectionstr);
	}
	HarbolString_Del(&sectionstr);
	HarbolString_Del(&targetstr);
	return false;
}

HARBOL_EXPORT bool HarbolCfg_GetColorByKey(struct HarbolLinkMap *const cfgmap, const char *key, union HarbolColor *const restrict colorref)
{
	if( !cfgmap || !key || !colorref )
		return false;
	
	struct HarbolString
		sectionstr = {NULL, 0},
		targetstr = {NULL, 0}
	;
	HarbolCfg_ParseTargetStringPath(key, &targetstr);
	const char *iter = key;
	
	// use a single linkmap ptr to iterate.
	struct HarbolLinkMap *itermap = cfgmap;
	while( itermap ) {
		while( *iter && *iter != '.' )
			HarbolString_AddChar(&sectionstr, *iter++);
		iter++;
		
		const struct HarbolVariant *const restrict var = HarbolLinkMap_Get(itermap, sectionstr.CStr).VarPtr;
		if( !var ) {
			break;
		}
		else if( var->TypeTag==HarbolTypeLinkMap )
			itermap = var->Val.LinkMapPtr;
		else if( !HarbolString_CmpStr(&sectionstr, &targetstr) && var->TypeTag==HarbolTypeColor ) {
			HarbolTuple_ToStruct(var->Val.TuplePtr, colorref);
			HarbolString_Del(&sectionstr);
			HarbolString_Del(&targetstr);
			return true;
		}
		HarbolString_Del(&sectionstr);
	}
	HarbolString_Del(&sectionstr);
	HarbolString_Del(&targetstr);
	return false;
}


HARBOL_EXPORT bool HarbolCfg_GetVec4ByKey(struct HarbolLinkMap *const cfgmap, const char *key, union HarbolVec4D *const restrict vec4ref)
{
	if( !cfgmap || !key || !vec4ref )
		return false;
	
	struct HarbolString
		sectionstr = {NULL, 0},
		targetstr = {NULL, 0}
	;
	HarbolCfg_ParseTargetStringPath(key, &targetstr);
	const char *iter = key;
	
	// use a single linkmap ptr to iterate.
	struct HarbolLinkMap *itermap = cfgmap;
	while( itermap ) {
		while( *iter && *iter != '.' )
			HarbolString_AddChar(&sectionstr, *iter++);
		iter++;
		
		const struct HarbolVariant *const restrict var = HarbolLinkMap_Get(itermap, sectionstr.CStr).VarPtr;
		if( !var ) {
			break;
		}
		else if( var->TypeTag==HarbolTypeLinkMap )
			itermap = var->Val.LinkMapPtr;
		else if( !HarbolString_CmpStr(&sectionstr, &targetstr) && var->TypeTag==HarbolTypeVec4D ) {
			HarbolTuple_ToStruct(var->Val.TuplePtr, vec4ref);
			HarbolString_Del(&sectionstr);
			HarbolString_Del(&targetstr);
			return true;
		}
		HarbolString_Del(&sectionstr);
	}
	HarbolString_Del(&sectionstr);
	HarbolString_Del(&targetstr);
	return false;
}
