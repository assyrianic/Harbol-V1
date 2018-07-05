#!/bin/bash
cd "$(dirname "$0")"
gcc -Wall -Wextra -std=c99 -g -Os stringobj.c vector.c hashmap.c unilist.c bilist.c bytebuffer.c tuple.c heap.c graph.c tree.c pluginarch.c linkmap.c	test_suite.c -o TestDataStructures -ldl

gcc -Wall -Wextra -std=c99 -g -Os -fPIC -c testplugin.c
gcc -shared -o testplugin.so testplugin.o

gcc -Wall -Wextra -std=c99 -g -Os -fPIC -c testplugin2.c
gcc -shared -o testplugin2.so testplugin2.o

rm	testplugin.o testplugin2.o
