#!/bin/bash
cd "$(dirname "$0")"
#clang-3.5	-Wall -std=c99 -Os lexer.c main.c -o Lexer_Clang
gcc -Wall -Wextra -std=c99 -g -Os stringobj.c vector.c hashmap.c unilist.c bilist.c bytebuffer.c tuple.c heap.c	test_suite.c -o TestDataStructures
