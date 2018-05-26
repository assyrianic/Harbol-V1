#!/bin/bash
cd "$(dirname "$0")"
gcc -Wall -Wextra -std=c99 -g -Os stringobj.c vector.c hashmap.c unilist.c bilist.c bytebuffer.c tuple.c heap.c graph.c tree.c pluginarch.c	test_suite.c -o TestDataStructures -ldl

gcc -Wall -Wextra -std=c99 -g -Os -fPIC -c testplugin.c
gcc -shared -o testplugin.so testplugin.o

rm	testplugin.o
