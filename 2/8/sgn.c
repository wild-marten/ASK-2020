#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int32_t sgn(int32_t x)
{
    return (x >> 31 | ((-x >> 1) & 0x40000000) >> 30);
}

int main()
{
    printf("%d", sgn(-21313));
}


