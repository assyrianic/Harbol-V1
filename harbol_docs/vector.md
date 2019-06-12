# struct HarbolVector
```c
typedef struct HarbolVector {
	union HarbolValue *Table;
	size_t Len, Count;
} HarbolVector;
```

### Table
allocated array of `union HarbolValue` values.

### Len
size_t member that stores the size of `Table`. Is set to 0 if `Table` is `NULL`.

### Count
size_t member that stores how many items `Table` holds.


# Functions/Methods

## harbol_vector_new
```c
struct HarbolVector *harbol_vector_new(void);
```

### Description
allocates an empty dynamic array object.

### Parameters
None.

### Return Value
returns a pointer to a dynamically allocated, empty dynamic array. `NULL` if err occurred.


## harbol_vector_init
```c
void harbol_vector_init(struct HarbolVector *vec);
```

### Description
initializes a dynamic array.

### Parameters
* `vec` - pointer to a dynamic array.

### Return Value
None.


## harbol_vector_del
```c
void harbol_vector_del(struct HarbolVector *vec, fnDestructor *dtor);
```

### Description
cleans up the dynamic array's data. Calls a destructor, if valid, during deallocation.

### Parameters
* `vec` - pointer to a dynamic array.
* `dtor` - pointer to a custom destructor function.

### Return Value
None.


## harbol_vector_free
```c
void harbol_vector_free(struct HarbolVector **vecref, fnDestructor *dtor);
```

### Description
cleans up the dynamic array's data, frees the pointer and sets it to NULL. Calls a destructor, if valid, during deallocation.

### Parameters
* `vecref` - pointer to a pointer to a dynamic array.
* `dtor` - pointer to a custom destructor function.

### Return Value
None.


## harbol_vector_get_len
```c
size_t harbol_vector_get_len(const struct HarbolVector *vec);
```

### Description
gets the length of the dynamic array.

### Parameters
* `vec` - pointer to a constant dynamic array.

### Return Value
`size_t` value that represents how large the dynamic array's current buffer is.


## harbol_vector_get_count
```c
size_t harbol_vector_get_count(const struct HarbolVector *vec);
```

### Description
gets the count of items in the dynamic array.

### Parameters
* `vec` - pointer to a constant dynamic array.

### Return Value
`size_t` value that represents how many items have been insert into the dynamic array.


## harbol_vector_get_iter
```c
union HarbolValue *harbol_vector_get_iter(const struct HarbolVector *vec);
```

### Description
gets an iterator-pointer from the dynamic array buffer.

### Parameters
* `vec` - pointer to a constant dynamic array.

### Return Value
a pointer of type `union HarbolValue` which points to the base of the dynamic array buffer.


## harbol_vector_get_iter_end_len
```c
union HarbolValue *harbol_vector_get_iter_end_len(const struct HarbolVector *vec);
```

### Description
gets an iterator-pointer from the dynamic array buffer.

### Parameters
* `vec` - pointer to a constant dynamic array.

### Return Value
a pointer of type `union HarbolValue` which points to the end of the dynamic array buffer's length.


## harbol_vector_get_iter_end_count
```c
union HarbolValue *harbol_vector_get_iter_end_count(const struct HarbolVector *vec);
```

### Description
gets an iterator-pointer from the dynamic array buffer.

### Parameters
* `vec` - pointer to a constant dynamic array.

### Return Value
a pointer of type `union HarbolValue *` which points to the end of the dynamic array buffer's item count.


## harbol_vector_resize
```c
void harbol_vector_resize(struct HarbolVector *vec);
```

### Description
increases the size of the vector's buffer.

### Parameters
* `vec` - pointer to a dynamic array.

### Return Value
None.


## harbol_vector_truncate
```c
void harbol_vector_truncate(struct HarbolVector *vec);
```

### Description
decreases the size of the vector's buffer.

### Parameters
* `vec` - pointer to a dynamic array.

### Return Value
None.


## harbol_vector_insert
```c
bool harbol_vector_insert(struct HarbolVector *vec, union HarbolValue val);
```

### Description
inserts an items to the end of the vector buffer, thus appending the item.

### Parameters
* `vec` - pointer to a dynamic array.
* `val` - value of type `union HarbolValue`.

### Return Value
true if operation was successful, false otherwise.


## harbol_vector_pop
```c
union HarbolValue harbol_vector_pop(struct HarbolVector *vec);
```

### Description
removes the last item in the vector's buffer and returns it.

### Parameters
* `vec` - pointer to a dynamic array.

### Return Value
value of `union HarbolValue`.


## harbol_vector_get
```c
union HarbolValue harbol_vector_get(const struct HarbolVector *vec, size_t index);
```

### Description
gets an item from the vector via index.

### Parameters
* `vec` - pointer to a dynamic array.
* `index` - `size_t` integer location of the item to retrieve.

### Return Value
value of `union HarbolValue`.


## harbol_vector_set
```c
void harbol_vector_set(struct HarbolVector *vec, size_t index, union HarbolValue val);
```

### Description
gets an item from the vector via index.

### Parameters
* `vec` - pointer to a dynamic array.
* `index` - `size_t` integer location of the item to replace.
* `val` - value of `union HarbolValue` to replace the item, at the index, with.

### Return Value
None.


## harbol_vector_delete
```c
void harbol_vector_delete(struct HarbolVector *vec, size_t index, fnDestructor *dtor);
```

### Description
deletes an item from the vector.

### Parameters
* `vec` - pointer to a dynamic array.
* `index` - `size_t` integer location of the item to delete.
* `dtor` - optional function pointer to destroy/deallocate item.

### Return Value
None.


## harbol_vector_add
```c
void harbol_vector_add(struct HarbolVector *vecA, const struct HarbolVector *vecB);
```

### Description
gets an item from the vector via index.

### Parameters
* `vecA` - pointer to destination dynamic array.
* `vecB` - pointer to source dynamic array.

### Return Value
None.


## harbol_vector_copy
```c
void harbol_vector_copy(struct HarbolVector *vecA, const struct HarbolVector *vecB);
```

### Description
gets an item from the vector via index.

### Parameters
* `vecA` - pointer to destination dynamic array.
* `vecB` - pointer to source dynamic array.

### Return Value
None.


## harbol vector converters

```c
void harbol_vector_from_unilist(struct HarbolVector *vec, const struct HarbolUniList *list);
void harbol_vector_from_bilist(struct HarbolVector *vec, const struct HarbolBiList *list);
void harbol_vector_from_hashmap(struct HarbolVector *vec, const struct HarbolHashmap *map);
void harbol_vector_from_graph(struct HarbolVector *vec, const struct HarbolGraph *graph);
void harbol_vector_from_linkmap(struct HarbolVector *vec, const struct HarbolLinkMap *map);

struct HarbolVector *harbol_vector_new_from_unilist(const struct HarbolUniList *list);
struct HarbolVector *harbol_vector_new_from_bilist(const struct HarbolBiList *list);
struct HarbolVector *harbol_vector_new_from_hashmap(const struct HarbolHashmap *map);
struct HarbolVector *harbol_vector_new_from_graph(const struct HarbolGraph *graph);
struct HarbolVector *harbol_vector_new_from_linkmap(const struct HarbolLinkMap *map);
```

### Description
converts the vector to a different data structure.


## harbol_generic_vector_resizer
```c
void harbol_generic_vector_resizer(void *const table_n_len, const size_t len, const size_t element_size)
```

### Description
generic vector resizer.

### Parameters
* `table_n_len` - untyped pointer to a struct that contains a pointer and a `size_t` as the first two members.
* `len` - new size of the vector buffer.
* `element_size` - size of the elements to be contained in the vector buffer.

### Return Value
None.
