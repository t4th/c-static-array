# POC c99 compatible array wrapper for run-time boundary checks.

Pseudo C++ std::array version for C99 to check run-time out-of-bounds memory access for static arrays for security/safety stuff.

Definietely not to be used in hot paths.

Current implemention call user defined function when out of boundary access occurs and function return pointer to first element since it is always available.

It is user job to handle this error however he seems fit. Invalidating the data seems to be the most obvious.

Since I have not yet found way to pass/keep type in C, it must be passed as argument when accessing data.

## todo:

### decide on passing out-of-bound callback: each array has its own/file scope/system scope

### decide what to when out-of-bound occurs: currently just return pointer to 1st element since its always available

## how
For each instance of an array this macro is creating instance of an array_header.
Information from this header is later used to validate c-style array access.

```c
struct array_header
{
    const size_t m_length;
    const size_t m_type_size;
    const size_t m_size_in_bytes;
} array_header;

#define array( type, length, name)                   \
    struct array_header name##_header =              \
    {                                                \
        .m_length = length,                          \
        .m_type_size = sizeof( type),                \
        .m_size_in_bytes = { length * sizeof( type)} \
    };                                               \
    type name##_array_data[ length]                  \
```

Like usual in this case, accessing array_header or array_data directly result in undefined behaviour.

## api

```c
array_length( name)  // return legnth, ie. number of elements in an array
array_len( name)     // alias to array_length

array_size( name)   // return size in bytes or an array

array_at( type, name, index) // access element of an array. can be used to read or write.

array_loop_in_range( name, iterator_name) // jsut wrapped for-loop :P
```

## usage

```c
#include <stdio.h>

void out_of_bound_action( int expr);

#define array_assert( x) out_of_bound_action( x)

#include "array.h"

void out_of_bound_action( int expr)
{
    printf( "out of bound access occurred\n");
}

int main(void)
{
    array( float, 10, my_array) = { [2] = 1.23f};

    printf( "array size = %zu bytes\n", array_size( my_array));

    float read0 = array_at( float, my_array, 2);

    array_at( float, my_array, 5) = 5.23f;

    float read1 = array_at( float, my_array, 5);

    array_loop_in_range( my_array, i)
    {
        printf( "%zu = %lf\n", i, array_at( float, my_array, i));
    }

    // out of bound read
    float read2 = array_at( float, my_array, 10);
    
    // out of bound write
    array_at( float, my_array, 15) = 56.123f;

    return 0;
}
```