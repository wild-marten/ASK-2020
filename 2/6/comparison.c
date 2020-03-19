#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>

uint32_t ucomp(uint32_t x, uint32_t y)
{
    return (((x >> 1) - (y >> 1) - (~x & y & 0x1)) >> 31) & 0x1;
}

int32_t scomp(int32_t x, int32_t y)
{
    return (((x >> 1) - (y >> 1) - (~x & y & 0x1)) >> 31) & 0x1;
}

int main()
{

    printf("%d\n", ucomp(233, 111));
    printf("%d\n", scomp(INT_MAX -1, INT_MAX ));
}