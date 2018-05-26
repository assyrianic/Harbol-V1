# C Data Structure Collection (DSC)
self-contained all-in-one STL-style data structure collection in C.

### Features:
* Object-oriented style Data Structure methods including virtual destructors.
* Tagged union (variant type) to support any values.
* Polymorphic destructors for easy clean up of data.
* Convert container types between one another!

### Current Data Structures Available:
* C++ style string object
* Vector / Dynamic Array (*can apply virtual destructor*)
	* can be converted to a hashmap, singly linked list, doubly linked list, tuple, and graph.
* Hashmap (*can apply virtual destructor*)
	* can be converted to a vector, singly linked list, doubly linked list, tuple, and graph.
* Singly Linked List (*can apply virtual destructor*)
	* can be converted to a vector, hashmap, doubly linked list, tuple, and graph.
* Doubly Linked List (*can apply virtual destructor*)
	* can be converted to a vector, hashmap, singly linked list, tuple, and graph.
* Byte Buffer
* Variant Type (using a fat union)
* Tuple
	* can be converted to a vector, hashmap, singly linked list, doubly linked list, and graph.
* Heap Memory Pool
* Graph (*can apply virtual destructors for edge weight data and vertex data*)
	* can be converted to a vector, hashmap, singly linked list, doubly linked list, and tuple.
* General Trees (*doesn't have virtual destructors as the deletion API takes external destructors*)
* Basic Plugin Architecture Manager.

### How to install/use
Run the `build_libCDSC_static.sh` script to build a static library that you can then link into your C (or C++ if u don't want to use STL) program.
