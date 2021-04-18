#pragma once

#include <stdint.h>

#ifndef array_assert
    #define array_assert( x)
#endif

#define array_declare( type, length, name) \
    typedef struct                                                  \
    {                                                               \
        type m_data[ length];                                       \
    } name;                                                         \
                                                                    \
    type * name##_array_at( name * ap_data, size_t a_index)         \
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

#define array( type, length, name) \
    name##_data_type name##_data;                   \

#define array_at( name, index) name##_array_at( &name, index)

#define array_length( name)
#define array_len( name)

#define array_size( name)
#define array_loop_in_range( name, iterator_name)
