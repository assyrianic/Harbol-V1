
#include <stdlib.h>
#include <stdio.h>

#if OS_WINDOWS
	#include <windows.h>
#else
	#include <dlfcn.h>
#endif
#include "dsc.h"


struct PluginData *Plugin_New(void)
{
	return calloc(1, sizeof(struct PluginData));
}

void Plugin_Del(struct PluginData *const __restrict plugin)
{
	if( !plugin )
		return;
	
	if( plugin->ModulePtr )
#if OS_WINDOWS
		FreeLibrary(plugin->ModulePtr);
#else
		dlclose(plugin->ModulePtr);
#endif
	plugin->ModulePtr = NULL;
	Map_Del(&plugin->Symbols);
	String_Del(&plugin->Name);
	String_Del(&plugin->Version);
	String_Del(&plugin->Author);
	String_Del(&plugin->RunTimeName);
	String_Del(&plugin->Descr);
}

void Plugin_Free(struct PluginData **__restrict pluginref)
{
	if( !*pluginref )
		return;
	Plugin_Del(*pluginref);
	free(*pluginref), *pluginref=NULL;
}
const char *Plugin_GetName(const struct PluginData *const __restrict plugin)
{
	return plugin ? plugin->Name.CStr : NULL;
}
const char *Plugin_GetVersion(const struct PluginData *const __restrict plugin)
{
	return plugin ? plugin->Version.CStr : NULL;
}
const char *Plugin_GetAuthor(const struct PluginData *const __restrict plugin)
{
	return plugin ? plugin->Author.CStr : NULL;
}
const char *Plugin_GetRuntimeName(const struct PluginData *const __restrict plugin)
{
	return plugin ? plugin->RunTimeName.CStr : NULL;
}
const char *Plugin_GetDescription(const struct PluginData *const __restrict plugin)
{
	return plugin ? plugin->Descr.CStr : NULL;
}
void *Plugin_GetModulePtr(const struct PluginData *const __restrict plugin)
{
	return plugin ? plugin->ModulePtr : NULL;
}
void *Plugin_GetExportedSymbol(const struct PluginData *const __restrict plugin, const char *__restrict cstrName)
{
	return plugin ? Map_Get(&plugin->Symbols, cstrName).Ptr : NULL;
}
void Plugin_SetName(struct PluginData *const __restrict plugin, const char *__restrict cstrName)
{
	if( !plugin or !cstrName )
		return;
	String_CopyStr(&plugin->Name, cstrName);
}
void Plugin_SetVersion(struct PluginData *const __restrict plugin, const char *__restrict cstrVer)
{
	if( !plugin or !cstrVer )
		return;
	String_CopyStr(&plugin->Version, cstrVer);
}
void Plugin_SetAuthor(struct PluginData *const __restrict plugin, const char *__restrict cstrAuthor)
{
	if( !plugin or !cstrAuthor )
		return;
	String_CopyStr(&plugin->Author, cstrAuthor);
}
void Plugin_SetDescription(struct PluginData *const __restrict plugin, const char *__restrict cstrDesc)
{
	if( !plugin or !cstrDesc )
		return;
	String_CopyStr(&plugin->Descr, cstrDesc);
}



struct PluginManager *PluginManager_New(const char *__restrict plugindir)
{
	struct PluginManager *pm = calloc(1, sizeof *pm);
	if( pm ) {
		Map_Init(&pm->ModuleMap, (bool(*)())Plugin_Free);
		Vector_Init(&pm->ModuleVec, NULL);
		String_InitStr(&pm->PluginDir, plugindir);
	}
	return pm;
}

void PluginManager_Init(struct PluginManager *const __restrict pm, const char *__restrict plugindir)
{
	if( !pm )
		return;
	
	Map_Init(&pm->ModuleMap, (bool(*)())Plugin_Free);
	Vector_Init(&pm->ModuleVec, NULL);
	String_InitStr(&pm->PluginDir, plugindir);
}

void PluginManager_Del(struct PluginManager *const __restrict pm)
{
	if( !pm )
		return;
	for( size_t i=0 ; i<pm->ModuleVec.Count ; i++ ) {
		struct PluginData *plugin = pm->ModuleVec.Table[i].Ptr;
		if( plugin ) {
			int32_t (*OnPluginUnload)() = Map_Get(&plugin->Symbols, "OnPluginUnload").Int32Func;
			if( OnPluginUnload )
				(*OnPluginUnload)(plugin, 0, NULL);
		}
	}
	Map_Del(&pm->ModuleMap);
	Vector_Del(&pm->ModuleVec);
	String_Del(&pm->PluginDir);
}

void PluginManager_Free(struct PluginManager **__restrict pmref)
{
	if( !*pmref )
		return;
	
	PluginManager_Del(*pmref);
	free(*pmref), *pmref=NULL;
}

bool PluginManager_LoadModule(struct PluginManager *const __restrict pm, const char *__restrict modname, const size_t argcount, void *params[static argcount])
{
	if( !pm )
		return false;
	
	struct PluginData *plugin = Plugin_New();
	if( !plugin )
		return false;
	
	// set up the plugin directory.
	struct String plugindir = (struct String){0};
	String_Add(&plugindir, &pm->PluginDir);
	String_AddStr(&plugindir, "/");
	String_AddStr(&plugindir, modname);
	String_AddStr(&plugindir, ".so");
	
	// set up OS-specific dynamic loading.
#if OS_WINDOWS
	plugin->ModulePtr = LoadLibrary(plugindir.CStr);
#else
	plugin->ModulePtr = dlopen(plugindir.CStr, RTLD_LAZY);
#endif
	String_Del(&plugindir);
	if( !plugin->ModulePtr ) {
		Plugin_Free(&plugin);
		return false;
	}
	String_CopyStr(&plugin->RunTimeName, modname);
	
	// Retrieve the OnPluginLoad function so we can allow the plugins can initialize their data.
	// the function signature should look something like.
	// int32_t OnPluginLoad(struct PluginData *plugin, const size_t argcount, void *params[__restrict static argcount]);
#if OS_WINDOWS
	int32_t (*OnPluginLoad)() = (int32_t (*)()) GetProcAddress(plugin->ModulePtr, "OnPluginLoad");
#else
	int32_t (*OnPluginLoad)() = dlsym(plugin->ModulePtr, "OnPluginLoad");
#endif
	// if it exists, call it then save the pointer to our symbol map.
	if( OnPluginLoad ) {
		(*OnPluginLoad)(plugin, argcount, params);
		Map_Insert(&plugin->Symbols, "OnPluginLoad", (union Value){.Int32Func=OnPluginLoad});
	}
	
	// while we're here, let's check if the plugin has an unloading callback for data cleanup.
	// if that exists, add it to the symbol map as well.
#if OS_WINDOWS
	int32_t (*OnPluginUnload)() = (int32_t (*)()) GetProcAddress(plugin->ModulePtr, "OnPluginUnload");
#else
	int32_t (*OnPluginUnload)() = dlsym(plugin->ModulePtr, "OnPluginUnload");
#endif
	if( OnPluginUnload )
		Map_Insert(&plugin->Symbols, "OnPluginUnload", (union Value){.Int32Func=OnPluginUnload});
	
	// we're done, for now, add the plugin to our plugin manager.
	Map_Insert(&pm->ModuleMap, modname, (union Value){.Ptr=plugin});
	Vector_Insert(&pm->ModuleVec, (union Value){.Ptr=plugin});
	return true;
}

bool PluginManager_ReloadModule(struct PluginManager *const __restrict pm, const char *__restrict modname, const size_t argcount, void *params[static argcount])
{
	if( !pm )
		return false;
	
	struct PluginData *plugin = Map_Get(&pm->ModuleMap, modname).Ptr;
	if( !plugin )
		return false;
	
	int32_t (*OnPluginUnload)() = Map_Get(&plugin->Symbols, "OnPluginUnload").Int32Func;
	if( OnPluginUnload )
		(*OnPluginUnload)(plugin, argcount, params);
	
	Plugin_Del(plugin);
	
	// set up the plugin directory.
	struct String plugindir = (struct String){0};
	String_Add(&plugindir, &pm->PluginDir);
	String_AddStr(&plugindir, "/");
	String_AddStr(&plugindir, modname);
	String_AddStr(&plugindir, ".so");
	
	// set up OS-specific dynamic loading.
#if OS_WINDOWS
	plugin->ModulePtr = LoadLibrary(plugindir.CStr);
#else
	plugin->ModulePtr = dlopen(plugindir.CStr, RTLD_LAZY);
#endif
	if( !plugin->ModulePtr )
		return false;
	
	String_Del(&plugindir);
	String_CopyStr(&plugin->RunTimeName, modname);
	
	// Retrieve the OnPluginLoad function so we can allow the plugins can initialize their data.
	// the function signature should look something like.
	// int32_t OnPluginLoad(struct PluginData *plugin, const size_t argcount, void *params[__restrict static argcount]);
#if OS_WINDOWS
	int32_t (*OnPluginLoad)() = (int32_t (*)()) GetProcAddress(plugin->ModulePtr, "OnPluginLoad");
#else
	int32_t (*OnPluginLoad)() = dlsym(plugin->ModulePtr, "OnPluginLoad");
#endif
	// if it exists, call it then save the pointer to our symbol map.
	if( OnPluginLoad ) {
		(*OnPluginLoad)(plugin, argcount, params);
		Map_Insert(&plugin->Symbols, "OnPluginLoad", (union Value){.Int32Func=OnPluginLoad});
	}
	
	// while we're here, let's check if the plugin has an unloading callback for data cleanup.
	// if that exists, add it to the symbol map as well.
#if OS_WINDOWS
	OnPluginUnload = (int32_t (*)()) GetProcAddress(plugin->ModulePtr, "OnPluginUnload");
#else
	OnPluginUnload = dlsym(plugin->ModulePtr, "OnPluginUnload");
#endif
	if( OnPluginUnload )
		Map_Insert(&plugin->Symbols, "OnPluginUnload", (union Value){.Int32Func=OnPluginUnload});
	return true;
}

bool PluginManager_ReloadAllModules(struct PluginManager *const __restrict pm, const size_t argcount, void *params[static argcount])
{
	if( !pm )
		return false;
	
	for( size_t i=0 ; i<pm->ModuleVec.Count ; i++ ) {
		struct PluginData *plugin = pm->ModuleVec.Table[i].Ptr;
		if( plugin )
			PluginManager_ReloadModule(pm, plugin->RunTimeName.CStr, argcount, params);
	}
	return true;
}

bool PluginManager_UnloadModule(struct PluginManager *const __restrict pm, const char *__restrict modname, const size_t argcount, void *params[static argcount])
{
	if( !pm )
		return false;
	
	struct PluginData *plugin = Map_Get(&pm->ModuleMap, modname).Ptr;
	if( !plugin )
		return false;
	
	int32_t (*OnPluginUnload)() = Map_Get(&plugin->Symbols, "OnPluginUnload").Int32Func;
	if( OnPluginUnload )
		(*OnPluginUnload)(plugin, argcount, params);
	
	for( size_t i=0 ; i<pm->ModuleVec.Count ; i++ ) {
		struct PluginData *iter = pm->ModuleVec.Table[i].Ptr;
		if( iter==plugin ) {
			Vector_Delete(&pm->ModuleVec, i);
			break;
		}
	}
	Map_Delete(&pm->ModuleMap, modname);
	return true;
}

bool PluginManager_UnloadAllModules(struct PluginManager *const __restrict pm, const size_t argcount, void *params[static argcount])
{
	if( !pm )
		return false;
	
	// iterate all plugins and call their Unload function.
	for( size_t i=0 ; i<pm->ModuleVec.Count ; i++ ) {
		struct PluginData *plugin = pm->ModuleVec.Table[i].Ptr;
		if( plugin ) {
			int32_t (*OnPluginUnload)() = Map_Get(&plugin->Symbols, "OnPluginUnload").Int32Func;
			if( OnPluginUnload )
				(*OnPluginUnload)(plugin, argcount, params);
		}
	}
	Map_Del(&pm->ModuleMap);
	Vector_Del(&pm->ModuleVec);
	return true;
}
