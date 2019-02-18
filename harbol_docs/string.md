# struct HarbolString
```c
typedef struct HarbolString {
	char *CStr;
	size_t Len;
} HarbolString;
```

### CStr
allocated array of char values used as a null-terminated string.

### Len
size_t member that stores the size of `CStr`. Is set to 0 if `CStr` is `NULL` or empty "".


# Functions/Methods

## harbol_string_new
```c
struct HarbolString *harbol_string_new(void);
```

### Description
allocates an empty string object.

### Parameters
None.

### Return Value
returns a pointer to a dynamically allocated, empty string object. `NULL` if err occurred.


## harbol_string_new_cstr
```c
struct HarbolString *harbol_string_new_cstr(const char cstr[]);
```

### Description
allocates an empty string object and initializes it with a C string.

### Parameters
* `cstr` - C string.

### Return Value
returns a pointer to a dynamically allocated string object. `NULL` if err occurred.


## harbol_string_del
```c
void harbol_string_del(struct HarbolString *str);
```

### Description
frees the memory of a string object.

### Parameters
* `str` - pointer to a string object.

### Return Value
None.


## harbol_string_free
```c
bool harbol_string_free(struct HarbolString **strref);
```

### Description
frees the memory of a string object, frees the string object itself, and sets the pointer to `NULL`.

### Parameters
* `strref` - pointer to a pointer to a string object.

### Return Value
`true` if operation was successful, `false` otherwise.


## harbol_string_init
```c
void harbol_string_init(struct HarbolString *str);
```

### Description
initializes a string object.

### Parameters
* `str` - pointer to a string object.

### Return Value
None.


## harbol_string_init_cstr
```c
void harbol_string_init_cstr(struct HarbolString *str, const char cstr[]);
```

### Description
initializes a string object with a C string.

### Parameters
* `str` - pointer to a string object.
* `cstr` - C string.

### Return Value
None.


## harbol_string_add_char
```c
void harbol_string_add_char(struct HarbolString *str, char chr);
```

### Description
concatenates a single character to a string object.

### Parameters
* `str` - pointer to a string object.
* `chr` - chararacter constant.

### Return Value
None.


## harbol_string_add_str
```c
void harbol_string_add_str(struct HarbolString *strA, const struct HarbolString *strB);
```

### Description
concatenates a string object to another.

### Parameters
* `strA` - dest pointer to a string object.
* `strB` - src pointer to a string object.

### Return Value
None.


## harbol_string_add_cstr
```c
void harbol_string_add_cstr(struct HarbolString *str, const char cstr[]);
```

### Description
concatenates a C string to a string object.

### Parameters
* `str` - pointer to a string object.
* `cstr` - C string.

### Return Value
None.


## harbol_string_get_cstr
```c
char *harbol_string_get_cstr(const struct HarbolString *str);
```

### Description
get the char data of a string object.

### Parameters
* `str` - pointer to a string object.

### Return Value
a char pointer to the allocated data of a string object.


## harbol_string_get_len
```c
size_t harbol_string_get_len(const struct HarbolString *str);
```

### Description
gets the string len of a string object.

### Parameters
* `str` - pointer to a string object.

### Return Value
`size_t` string len.


## harbol_string_copy_str
```c
void harbol_string_copy_str(struct HarbolString *strA, const struct HarbolString *strB);
```

### Description
copies a string object from another. `strA` must **NOT** alias `strB`.

### Parameters
* `strA` - dest pointer to a string object.
* `strB` - src pointer to a string object.

### Return Value
None.


## harbol_string_copy_cstr
```c
void harbol_string_copy_cstr(struct HarbolString *str, const char cstr[]);
```

### Description
copies a string object from a C string.

### Parameters
* `str` - dest pointer to a string object.
* `cstr` - src C string.

### Return Value
None.


## harbol_string_format
```c
int32_t harbol_string_format(struct HarbolString *str, const char fmt[], ...);
```

### Description
Formats data sprintf-style into a string object.

### Parameters
* `str` - pointer to a string object.
* `fmt` - C string used as format.

### Return Value
how many characters were written, as a 4-byte signed integer.


## harbol_string_cmpcstr
```c
int32_t harbol_string_cmpcstr(const struct HarbolString *str, const char cstr[]);
```

### Description
compares a string obj and a C string.

### Parameters
* `str` - pointer to a string object.
* `cstr` - C string.

### Return Value
same as `strcmp`.


## harbol_string_cmpstr
```c
int32_t harbol_string_cmpstr(const struct HarbolString *strA, const struct HarbolString *strB);
```

### Description 
compares two string objs. `strA` must **NOT** alias `strB`.

### Parameters
* `strA` - pointer to a string object.
* `strB` - pointer to a string object.

### Return Value
same as `strcmp`.


## harbol_string_ncmpcstr
```c
int32_t harbol_string_ncmpcstr(const struct HarbolString *str, const char cstr[], size_t len);
```

### Description 
compares a string obj and a C string using a size limit.

### Parameters
* `str` - pointer to a string object.
* `cstr` - C string.
* `len` - how many characters to compare.

### Return Value
same as `strncmp`.


## harbol_string_ncmpstr
```c
int32_t harbol_string_ncmpstr(const struct HarbolString *strA, const struct HarbolString *strB, size_t len);
```

### Description 
compares two string objs using a size limit. `strA` must **NOT** alias `strB`.

### Parameters
* `strA` - pointer to a string object.
* `strB` - pointer to a string object.
* `len` - how many characters to compare.

### Return Value
same as `strncmp`.


## harbol_string_is_empty
```c
bool harbol_string_is_empty(const struct HarbolString *str);
```

### Description 
checks if a string obj is empty or has an empty string.

### Parameters
* `str` - pointer to a string object.

### Return Value
`true` if it has no allocated string or the allocated string is an empty string, `false` otherwise.


## harbol_string_reserve
```c
bool harbol_string_reserve(struct HarbolString *str, size_t size);
```

### Description 
reserves a string obj with a zeroed string of `size` len.

### Parameters
* `str` - pointer to a string object.
* `size` - how much memory to reserve to the string.

### Return Value
`true` if allocation was successful, `false` otherwise.


## harbol_string_fgets
```c
char *harbol_string_fgets(struct HarbolString *str, FILE *file);
```

### Description 
uses a string obj as a buffer with `fgets`.

### Parameters
* `str` - pointer to a string object.
* `file` - pointer to a file buffer.

### Return Value
`NULL` pointer if `fgets` failed, a valid pointer otherwise.


## harbol_string_clear
```c
void harbol_string_clear(struct HarbolString *str);
```

### Description 
completely zeroes out a string obj's value, doesn't change its length.

### Parameters
* `str` - pointer to a string object.

### Return Value
None.


## harbol_string_read_file
```c
bool harbol_string_read_file(struct HarbolString *str, FILE *file);
```

### Description 
buffers an entire file into a string.

### Parameters
* `str` - pointer to a string object.
* `file` - pointer to a file buffer.

### Return Value
`true` if operation was successful, `false` otherwise.


## harbol_string_replace
```c
bool harbol_string_replace(struct HarbolString *str, char to_replace, char with);
```

### Description 
replaces a character in a string obj with a different character.

### Parameters
* `str` - pointer to a string object.
* `to_replace` - character to replace.
* `with` - character that will replace every instance of `to_replace`.

### Return Value
`true` if operation was successful, `false` otherwise.
