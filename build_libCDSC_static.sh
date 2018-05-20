#!/bin/bash
cd "$(dirname "$0")"
gcc	-Wall -Wextra -std=c99 -Os -c 	stringobj.c vector.c hashmap.c unilist.c bilist.c bytebuffer.c tuple.c
ar	cr libDataStructColl.a		stringobj.o vector.o hashmap.o unilist.o bilist.o bytebuffer.o tuple.o

rm	stringobj.o vector.o hashmap.o unilist.o bilist.o bytebuffer.o tuple.o
