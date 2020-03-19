#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int32_t sgn(int32_t x)
{
    return x >> 31 | (-x >> 31) & 0x1;
}

int main()
{
    printf("%d", sgn(0));
}


