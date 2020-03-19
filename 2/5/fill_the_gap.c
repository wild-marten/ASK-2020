#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* Oblicz x * 3 / 4 zaokrąglając w dół. */
__int32_t threefourths(__int32_t x)
{
    __int32_t accuracy = (x & 0x3) >> 3;
    accuracy += accuracy << 1;
    x = x >> 2;
    x += x << 1 + accuracy;
    return x;
}

int main()
{
    printf("%d", threefourths(16));
}