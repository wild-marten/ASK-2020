#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int32_t abc(int32_t x)
{
    return (x^(x>>31)) - (x>>31);
}

int main()
{
    printf("%d", abc(-1231));
}