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
* General Trees (*doesn't have virtual destructors can deletion API supports destructor*)

### Planned Data Structures
* Ordered Hashmaps (Linked Map)
