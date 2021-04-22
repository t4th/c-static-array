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