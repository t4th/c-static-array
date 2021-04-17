#include <stdio.h>

void out_of_bound_action( int expr);

#define array_assert( x) out_of_bound_action( x)

#include "../code/array.h"

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