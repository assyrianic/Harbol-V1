#ifdef OS_WINDOWS
#	define HARBOL_LIB
#	include <windows.h>
#	include <direct.h>
#else
#	include <dlfcn.h>
#	include <unistd.h>
#endif

#include "tinydir.h"
#include "harbol.h"


#if OS_WINDOWS
#	ifndef MODULE_GET_FUNC
#		define MODULE_GET_FUNC GetProcAddress
#	endif
#	ifndef MODULE_CLOSE
#		define MODULE_CLOSE FreeLibrary
#	endif
#else
#	ifndef MODULE_GET_FUNC
#		define MODULE_GET_FUNC dlsym
#	endif
#	ifndef MODULE_CLOSE
#		define MODULE_CLOSE dlclose
#	endif
#endif

#if OS_WINDOWS
#	ifndef DIRECTORY_SEP
#		define DIRECTORY_SEP "\\"
#	endif
#	ifndef LIB_EXT
#		define LIB_EXT "dll"
#	endif
#elif OS_LINUX_UNIX
#	ifndef DIRECTORY_SEP
#		define DIRECTORY_SEP "/"
#	endif
#	ifndef LIB_EXT
#		define LIB_EXT "so"
#	endif
#elif OS_MAC
#	ifndef DIRECTORY_SEP
#		define DIRECTORY_SEP "/"
#	endif
#	ifndef LIB_EXT
#		define LIB_EXT "dylib"
#	endif
#endif


struct HarbolPlugin *harbol_plugin_new(HarbolModule module)
{
	struct HarbolPlugin *plugin = calloc(1, sizeof *plugin);
	if( plugin )
		plugin->SharedLib = module;
	return plugin;
}

bool harbol_plugin_free(struct HarbolPlugin **const pluginref)
{
	if( !*pluginref )
		return false;
	struct HarbolPlugin *const plugin = *pluginref;
	if( plugin->SharedLib ) {
		MODULE_CLOSE(plugin->SharedLib), plugin->SharedLib=NULL;
	}
	harbol_string_del(&plugin->LibPath);
	memset(plugin, 0, sizeof *plugin);
	free(*pluginref), *pluginref=NULL;
	return true;
}

HARBOL_EXPORT HarbolModule harbol_plugin_get_module(const struct HarbolPlugin *const plugin)
{
	return !plugin ? NULL : plugin->SharedLib;
}

HARBOL_EXPORT const char *harbol_plugin_get_name(const struct HarbolPlugin *const plugin)
{
	return !plugin || !plugin->SharedLib ? NULL : plugin->Name->CStr; 
}

HARBOL_EXPORT const char *harbol_plugin_get_path(const struct HarbolPlugin *plugin)
{
	return !plugin || !plugin->SharedLib ? NULL : plugin->LibPath.CStr; 
}

HARBOL_EXPORT void *harbol_plugin_get_sym(const struct HarbolPlugin *const restrict plugin, const char sym_name[restrict])
{
	return ( !plugin || !plugin->SharedLib || !sym_name ) ? NULL : (void *)(uintptr_t)MODULE_GET_FUNC(plugin->SharedLib, sym_name);
}

HARBOL_EXPORT bool harbol_plugin_reload(struct HarbolPlugin *const plugin)
{
	if( !plugin || !plugin->LibPath.CStr )
		return false;
	else {
		if( plugin->SharedLib ) {
			MODULE_CLOSE(plugin->SharedLib), plugin->SharedLib=NULL;
		}
		plugin->SharedLib = 
#if OS_WINDOWS
			LoadLibrary(plugin->LibPath.CStr);
#else
			dlopen(plugin->LibPath.CStr, RTLD_NOW | RTLD_GLOBAL);
#endif
		return plugin->SharedLib != NULL;
	}
}

/************************************************************************************/

HARBOL_EXPORT struct HarbolPluginManager *harbol_plugin_manager_new(const char directory[restrict], const bool load_plugins, fnHarbolPluginEvent *const load_cb)
{
	if( !directory )
		return NULL;
	struct HarbolPluginManager *restrict manager = calloc(1, sizeof *manager);
	harbol_plugin_manager_init(manager, directory, load_plugins, load_cb);
	return manager;
}

static void _load_plugin(struct HarbolPluginManager *const manager, tinydir_file *const f, fnHarbolPluginEvent *const load_cb)
{
	HarbolModule module = 
#if OS_WINDOWS
			LoadLibrary(f->path);
#else
			dlopen(f->path,  RTLD_NOW | RTLD_GLOBAL);
#endif
	if( !module ) {
	#ifdef OS_WINDOWS
		fprintf(stderr, "Harbol Plugin Manager Error: **** Unable to load module: '%s' ****\n", f->name);
	#else
		fprintf(stderr, "Harbol Plugin Manager Error: **** %s ****\n", dlerror());
	#endif
	} else {
		struct HarbolPlugin *plugin = harbol_plugin_new(module);
		if( !plugin ) {
			fprintf(stderr, "Harbol Plugin Manager Error: **** Unable to allocate plugin for module: '%s' ****\n", f->name);
			MODULE_CLOSE(module);
		} else {
			harbol_string_init_cstr(&plugin->LibPath, f->path);
			char *ext_dot = strchr(f->name, '.');
			if( ext_dot )
				*ext_dot = 0;
			
			const bool res = harbol_linkmap_insert(&manager->Plugins, f->name, (union HarbolValue){ .Ptr=plugin });
			if( res ) {
				struct HarbolKeyValPair *const kv = harbol_linkmap_get_node_by_key(&manager->Plugins, f->name);
				plugin->Name = &kv->KeyName;
				if( load_cb )
					(*load_cb)(manager, &plugin);
			} else {
				harbol_plugin_free(&plugin);
			}
		}
	}
}

static bool _recursive_scan_plugin_directory(struct HarbolPluginManager *const manager, tinydir_dir *const dir, fnHarbolPluginEvent *const load_cb)
{
	while( dir->has_next ) {
		tinydir_file file;
		if( tinydir_readfile(dir, &file)<0 )
			continue;
		else if( file.is_dir ) {
			if( file.name[0]=='.' ) {
				/* jumping to tinydir_next at end of loop so we can advance the dir iterator. */
				goto dir_iter_loop;
			}
			tinydir_dir sub_dir;
			if( tinydir_open(&sub_dir, file.path)<0 ) {
				/* jumping to tinydir_next at end of loop so we can advance the dir iterator. */
				goto dir_iter_loop;
			}
			else _recursive_scan_plugin_directory(manager, &sub_dir, load_cb);
		} else if( !strcmp(file.extension, LIB_EXT) ) {
			_load_plugin(manager, &file, load_cb);
		}
dir_iter_loop:;
		if( tinydir_next(dir)<0 )
			break;
	}
	tinydir_close(dir);
	return manager->Plugins.Order.Count > 0;
}

static bool _recursive_scan_by_name(struct HarbolPluginManager *const restrict manager, tinydir_dir *const restrict dir, const char plugin_name[restrict], fnHarbolPluginEvent *const load_cb)
{
	while( dir->has_next ) {
		tinydir_file file;
		if( tinydir_readfile(dir, &file)<0 )
			continue;
		else if( file.is_dir ) {
			if( file.name[0]=='.' ) {
				/* jumping to tinydir_next at end of loop so we can advance the dir iterator. */
				goto dir_iter_loop;
			}
			tinydir_dir sub_dir;
			if( tinydir_open(&sub_dir, file.path)<0 ) {
				/* jumping to tinydir_next at end of loop so we can advance the dir iterator. */
				goto dir_iter_loop;
			}
			else _recursive_scan_by_name(manager, &sub_dir, plugin_name, load_cb);
		} else if( !strcmp(file.extension, LIB_EXT) ) {
			char *ext_dot = strchr(file.name, '.');
			if( ext_dot )
				*ext_dot = 0;
			if( !strcmp(plugin_name, file.name) )
				_load_plugin(manager, &file, load_cb);
		}
dir_iter_loop:;
		if( tinydir_next(dir)<0 )
			break;
	}
	tinydir_close(dir);
	return manager->Plugins.Order.Count > 0;
}

HARBOL_EXPORT bool harbol_plugin_manager_init(struct HarbolPluginManager *const restrict manager, const char directory[restrict], const bool load_plugins, fnHarbolPluginEvent *const load_cb)
{
	if( !manager || !directory )
		return false;
	else {
		memset(manager, 0, sizeof *manager);
		// FILENAME_MAX is defined in <stdio.h>
		char currdir[FILENAME_MAX] = {0};
#if OS_WINDOWS
		if( GetCurrentDirectory(sizeof currdir, currdir) )
#else
		if( getcwd(currdir, sizeof currdir) )
#endif
		{
			harbol_string_format(&manager->Directory, "%s%s%s", currdir, DIRECTORY_SEP, directory);
			if( load_plugins )
				return harbol_plugin_manager_load_plugins(manager, load_cb);
			else return true;
		}
		else return false;
	}
}

HARBOL_EXPORT bool harbol_plugin_manager_del(struct HarbolPluginManager *const manager, fnHarbolPluginEvent *const unload_cb)
{
	if( !manager )
		return false;
	
	harbol_plugin_manager_unload_plugins(manager, unload_cb);
	harbol_string_del(&manager->Directory);
	return true;
}

HARBOL_EXPORT bool harbol_plugin_manager_free(struct HarbolPluginManager **const managerref, fnHarbolPluginEvent *const unload_cb)
{
	if( !managerref || !*managerref )
		return false;
	else {
		harbol_plugin_manager_del(*managerref, unload_cb);
		free(*managerref), *managerref=NULL;
		return true;
	}
}

HARBOL_EXPORT struct HarbolPlugin *harbol_plugin_manager_get_plugin_by_name(const struct HarbolPluginManager *const restrict manager, const char plugin_name[restrict])
{
	return !manager ? NULL : harbol_linkmap_get(&manager->Plugins, plugin_name).Ptr;
}

HARBOL_EXPORT struct HarbolPlugin *harbol_plugin_manager_get_plugin_by_index(const struct HarbolPluginManager *const manager, const size_t index)
{
	return !manager || index>manager->Plugins.Order.Count ? NULL : manager->Plugins.Order.Table[index].KvPairPtr->Data.Ptr;
}

HARBOL_EXPORT const char *harbol_plugin_manager_get_plugin_dir(const struct HarbolPluginManager *manager)
{
	return !manager ? NULL : manager->Directory.CStr;
}

HARBOL_EXPORT size_t harbol_plugin_manager_get_plugin_count(const struct HarbolPluginManager *manager)
{
	return !manager ? 0 : manager->Plugins.Order.Count;
}

HARBOL_EXPORT bool harbol_plugin_manager_load_plugin_by_name(struct HarbolPluginManager *const restrict manager, const char plugin_name[restrict], fnHarbolPluginEvent *const load_cb)
{
	if( !manager )
		return false;
	else {
		tinydir_dir dir;
		if( tinydir_open(&dir, manager->Directory.CStr)<0 ) {
			fprintf(stderr, "Harbol Plugin Manager Error: **** Unable to Open Directory: '%s' ****\n", manager->Directory.CStr);
			harbol_string_del(&manager->Directory);
			tinydir_close(&dir);
			return false;
		} else {
			return _recursive_scan_by_name(manager, &dir, plugin_name, load_cb);
		}
	}
}

HARBOL_EXPORT bool harbol_plugin_manager_delete_plugin_by_name(struct HarbolPluginManager *const restrict manager, const char plugin_name[restrict], fnHarbolPluginEvent *const unload_cb)
{
	if( !manager || !plugin_name )
		return false;
	else {
		struct HarbolPlugin *plugin = harbol_plugin_manager_get_plugin_by_name(manager, plugin_name);
		if( unload_cb )
			(*unload_cb)(manager, &plugin);
		harbol_linkmap_delete(&manager->Plugins, plugin_name, (fnDestructor *)(uintptr_t)harbol_plugin_free);
		return true;
	}
}

HARBOL_EXPORT bool harbol_plugin_manager_delete_plugin_by_index(struct HarbolPluginManager *const manager, const size_t index, fnHarbolPluginEvent *const unload_cb)
{
	if( !manager )
		return false;
	else {
		struct HarbolPlugin *plugin = harbol_plugin_manager_get_plugin_by_index(manager, index);
		if( unload_cb )
			(*unload_cb)(manager, &plugin);
		harbol_linkmap_delete_by_index(&manager->Plugins, index, (fnDestructor *)(uintptr_t)harbol_plugin_free);
		return true;
	}
}

HARBOL_EXPORT bool harbol_plugin_manager_load_plugins(struct HarbolPluginManager *const manager, fnHarbolPluginEvent *const load_cb)
{
	if( !manager )
		return false;
	else {
		tinydir_dir dir;
		if( tinydir_open(&dir, manager->Directory.CStr)<0 ) {
			fprintf(stderr, "Harbol Plugin Manager Error: **** Unable to Open Directory: '%s' ****\n", manager->Directory.CStr);
			harbol_string_del(&manager->Directory);
			tinydir_close(&dir);
			return false;
		} else {
			return _recursive_scan_plugin_directory(manager, &dir, load_cb);
		}
	}
}

HARBOL_EXPORT bool harbol_plugin_manager_unload_plugins(struct HarbolPluginManager *const manager, fnHarbolPluginEvent *const unload_cb)
{
	if( !manager )
		return false;
	else {
		for( size_t i=0; i<manager->Plugins.Order.Count; i++ ) {
			struct HarbolPlugin *plugin = harbol_plugin_manager_get_plugin_by_index(manager, i);
			if( unload_cb )
				(*unload_cb)(manager, &plugin);
		}
		harbol_linkmap_del(&manager->Plugins, (fnDestructor *)(uintptr_t)harbol_plugin_free);
		return true;
	}
}

HARBOL_EXPORT bool harbol_plugin_manager_reload_plugins(struct HarbolPluginManager *const manager, fnHarbolPluginEvent *const prereload_cb, fnHarbolPluginEvent *const postreload_cb)
{
	if( !manager )
		return false;
	else {
		const union HarbolValue *const end = harbol_linkmap_get_iter_end_count(&manager->Plugins);
		for( const union HarbolValue *iter = harbol_linkmap_get_iter(&manager->Plugins); iter && iter<end; iter++ ) {
			struct HarbolPlugin *plugin = iter->KvPairPtr->Data.Ptr;
			if( prereload_cb )
				(*prereload_cb)(manager, &plugin);
			harbol_plugin_reload(plugin);
			if( postreload_cb )
				(*postreload_cb)(manager, &plugin);
		}
		return true;
	}
}
