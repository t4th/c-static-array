#include <stdio.h>

void out_of_bound_action( int expr);

#define array_assert( x) out_of_bound_action( x)

#include "../code/array.h"

void out_of_bound_action( int expr)
{
    printf( "out of bound access occurred\n");
}

array_declare( float, 10, my_array_t);

int main(void)
{
    my_array_t my_array = { .m_data = { [0] = 0.1f }};

    *my_array_t_array_at( &my_array, 1) = 3.2f;

    return 0;
}