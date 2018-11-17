# Harbol

## Introduction

**Harbol** is a data structure collection, similar to C++ Boost and GLib, that is meant to be a smaller and more lightweight collection of data structures, code systems, and convenience software.


### Features

* Object-oriented style Data Structures.
* Tagged union (variant type) to support any type of values and their tags.
* Convertible Data structures (data can be converted between applicable types).
* C++ style automated strings.
* Vector / Dynamic Array.
* Hashmap.
* Singly Linked List.
* Doubly Linked List.
* Byte Buffer.
* Tuple type.
* Memory Pool.
* "general purpose" Graph. (edges support weight data)
* General Tree.
* Linked Hashmap (preserves insertion order).
* Variant Type.
* JSON-like Key-Value Configuration File Parser (allows retrieving data from keys through python-style pathing).


## Usage

```c
#include "harbol.h"

int main(const int argc, char *argv[])
{
	HarbolString str; HarbolString_InitStr(&str, "my initial string!");
	HarbolString_AddStr(&str, "and another string concatenated!");
	HarbolString_Del(&str);
	
	HarbolVector vec; HarbolVector_Init(&vec);
	HarbolVector_Insert(&vec, (HarbolValue){.Float=2.f});
	HarbolVector_Insert(&vec, (HarbolValue){.Float=3.f});
	HarbolVector_Insert(&vec, (HarbolValue){.Float=4.f});
	const float f = HarbolVector_Get(&vec, 1).Float;
	HarbolVector_Del(&vec, NULL);
	
	HarbolHashmap *ptrmap = HarbolMap_New();
	HarbolMap_Insert(ptrmap, "style", (HarbolValue){.Float=2.3553f});
	HarbolMap_Insert(ptrmap, "border", (HarbolValue){.Double=12.995});
	HarbolMap_Free(&ptrmap, NULL);
}
```

## Contributing

To submit a patch, first file an issue and/or present a pull request.

## Help

If you need help or have any question, make an issue on the github repository.
Simply drop a message or your question and you'll be reached in no time!

## Installation

### Requirements

C99 compiler and libc implementation with stdlib.h, stdio.h, and stddef.h.

### Installation

To build the library, simply run `make harbol` which will make the static library version of libharbol.
for a shared library version, run `make harbol_shared`.

### Testing

For testing code changes or additions, simply run `make test` with `test_suite.c` in the repository which will build an executable called `harbol_testprogram`.


## Credits

* Khanno Hanna - main developer of libharbol.


## Contact

I can be contacted at edyonan@yahoo.com


## License

This project is licensed under MIT License.
