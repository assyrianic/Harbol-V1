#!/bin/bash
cd "$(dirname "$0")"

gcc -Wall -Wextra -std=c99 -pedantic -s -O2 stringobj.c vector.c hashmap.c unilist.c bilist.c bytebuffer.c tuple.c mempool.c graph.c tree.c linkmap.c variant.c cfg.c test_suite.c -o harbol_testprogram

valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -v ./harbol_testprogram
