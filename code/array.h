#pragma once

#include <stdint.h>

#ifndef array_assert
    #define array_assert( x)
#endif

struct array_header
{
    const size_t m_length;
    const size_t m_type_size;
    const size_t m_size_in_bytes;
} array_header;

// Generic type function.
void* array_dereference( const struct array_header a_header, void * const ap_data, const size_t a_index)
{
    // Cast void* to uint8_t* for defined -byte sized- pointer arithmetic.
    const size_t byte_index = a_header.m_type_size * a_index;

    if ( byte_index < a_header.m_size_in_bytes)
    {
        const uint8_t * const p_output = ( uint8_t *) ap_data;
        return ( void*) ( &p_output[ byte_index]);
    }
    else
    {
        array_assert( !!0);
    }

    // I case of invalid index, return pointer to first element. Better than UB.
    return ap_data;
}

#define array( type, length, name)                   \
    struct array_header name##_header =              \
    {                                                \
        .m_length = length,                          \
        .m_type_size = sizeof( type),                \
        .m_size_in_bytes = { length * sizeof( type)} \
    };                                               \
    type name##_array_data[ length]                  \

#define array_length( name)     name##_header.m_length
#define array_len( name)        array_length(name)

#define array_size( name)     name##_header.m_size_in_bytes

#define array_at( type, name, index) *( type*)array_dereference( name##_header, name##_array_data, index)

#define array_loop_in_range( name, iterator_name) size_t iterator_name = 0U; for (; iterator_name < name##_header.m_length; ++iterator_name)
