# POC c99 compatible array wrapper for run-time boundary checks.

Pseudo C++ std::array version for C99 to check run-time out-of-bounds memory access for static arrays for security/safety stuff.

Definietely not to be used in hot paths.

Current implemention call user defined function when out of boundary access occurs and function return pointer to first element since it is always available.

It is user job to handle this error however he seems fit. Invalidating the data seems to be the most obvious.

This is just a concept.

## how
The 'obvious' way is to create array 'hidde' descriptor:

For each instance of an array this macro is creating instance of an array_header and type definition of array_type.
Information from this header is later used to validate c-style array access.

```c
struct array_header
{
    const size_t m_length;
    const size_t m_type_size;
    const size_t m_size_in_bytes;
} array_header;

#define array( type, length, name)                   \
    typedef struct { type m_data; } name##_type;     \
    const struct array_header name##_header =        \
    {                                                \
        .m_length = length,                          \
        .m_type_size = sizeof( type),                \
        .m_size_in_bytes = (length * sizeof( type))  \
    };                                               \
    name##_type name##_array_data[ length]           \
```

Downfall is that nesting such array in other structures is not possible.

Like usual in this case, accessing array_header or array_data directly result in undefined behaviour.

### usage

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

    float read0 = array_at( my_array, 2);

    array_at( my_array, 5) = 5.23f;

    float read1 = array_at( my_array, 5);

    array_loop_in_range( my_array, i)
    {
        printf( "%zu = %lf\n", i, array_at( my_array, i));
    }

    // out of bound read
    float read2 = array_at( my_array, 10);
    
    // out of bound write
    array_at( my_array, 15) = 56.123f;

    return 0;
}
```

The other way to achieve this is to define type strong array type and create functions to use this type.
Implemented on branch https://github.com/t4th/c-static-array/tree/generate_functions

```c
#define array_declare( type, length, type_name) \
    typedef struct                                                   \
    {                                                                \
        type m_data[ length];                                        \
    } type_name;                                                     \
                                                                     \
    type * type_name##_array_at( type_name * ap_data, size_t a_index)\
    {                                                                \
        if ( a_index < length)                                       \
        {                                                            \
            return &ap_data->m_data[ a_index];                       \
        }                                                            \
        else                                                         \
        {                                                            \
            array_assert( !!0);                                      \
            return &ap_data->m_data[ 0];                             \
        }                                                            \
    }                                                                \
    size_t type_name##_array_length(void) { return length; }         \
    size_t type_name##_array_size(void) { return sizeof(type_name); }\
```

It is way better due to not using macros, being truly typestrong and allowing nesting in structs.

Downfall is that each array type must be defined outside function scope and each array has different function names.

```c
#include <stdio.h>

void out_of_bound_action( int expr);

#define array_assert( x) out_of_bound_action( x)

#include "../code/array.h"

void out_of_bound_action( int expr)
{
    printf( "out of bound access occurred\n");
}

// declare array type and all associated functions
array_declare( float, 10, my_array_t);

array_declare( int, 3, int_t);
array_declare( char, 10, char_t);

typedef struct
{
    array( int_t, int_arr);
    array( char_t, char_arr);
} both_t;

int main(void)
{
    array( my_array_t, my_array_name) = { 1.123f, 2.3f};
     
    array_at( my_array_t, my_array_name, 1) = 4.23f;

    float read = array_at( my_array_t, my_array_name, 11);

    both_t both = {{1,2,3}, {'a', 'b', 'c'}};

    int read1 = array_at( int_t, both.int_arr, 0);

    return 0;
}
```

## conclusion
This fun little prove of concept shows that achieving good defensive programming in C is not only hard, but not practical at all.

It also shows the reason behind initial C++ template implemention, which achieved best of both solutions with nice code syntax and not so nice linker errors ;).