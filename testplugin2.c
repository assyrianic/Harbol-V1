
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>


int32_t OnPluginLoad(void *plugin, const size_t argcount, void *params[static argcount])
{
	(void)plugin;
	(void)argcount;
	(void)params;
	puts("loading another plugin.");
	return 0;
}

int32_t OnPluginUnload(void *plugin, const size_t argcount, void *params[static argcount])
{
	(void)plugin;
	(void)argcount;
	(void)params;
	puts("unloading another plugin.");
	return 0;
}
