#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int32_t abs(int32_t x)
{
    return x - (x << 1 & x >> 31);
}

int main()
{
    printf("%d", abs(1));
}