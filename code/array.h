#pragma once

#include <stdint.h>

#ifndef array_assert
    #define array_assert( x)
#endif

#define array_declare( type, length, type_name) \
    typedef struct                                                  \
    {                                                               \
        type m_data[ length];                                       \
    } type_name;                                                         \
                                                                    \
    type * type_name##_array_at( type_name * ap_data, size_t a_index)         \
    {                                                               \
        if ( a_index < length)                                      \
        {                                                           \
            return &ap_data->m_data[ a_index];                      \
        }                                                           \
        else                                                        \
        {                                                           \
            array_assert( !!0);                                     \
            return &ap_data->m_data[ 0];                            \
        }                                                           \
    }                                                               \
    size_t type_name##_array_length(void) { return length; }\
    size_t type_name##_array_size(void) { return sizeof(type_name); }\

#define array( type_name, name) type_name name

#define array_at( type_name, name, index) *type_name##_array_at( &name, index)

#define array_length( type_name, name) type_name##_array_length()
#define array_len( type_name, name) array_length(name)

#define array_size( type_name, name) type_name##_array_size()
