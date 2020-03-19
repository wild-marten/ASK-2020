#include<stdlib.h>
#include<stdio.h>
#include<limits.h>
#include<stdint.h>

/* Kiedy x zawiera nieparzystą liczbę jedynek zwróć 1, w p.p. 0 */
int32_t odd_ones(uint32_t x)
{
    x = (x & 0x55555555) + ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x & 0x0F0F0F0F) + ((x >> 4) & 0x0F0F0F0F);
    x = (x & 0x00FF00FF) + ((x >> 8) & 0x00FF00FF);
    x = (x & 0x0000FFFF) + ((x >> 16) & 0x0000FFFF);
    return x&1;
}

int main()
{
    printf("%d", odd_ones(30));

}

//NA ZAJĘCIACH

// Do rozwiązania zadania używamy poznanej na poprzednich zajęciach metody
// dziel i zwyciężaj. Zauważamy, że wynik jest tożsamy z resztą z dzielenia
// przez 2 liczby zapalonych bitów, a to jest to samo co and z 1:

// /* Kiedy x zawiera nieparzystą liczbę jedynek zwróć 1, w p.p. 0 */
// int32_t odd_ones(uint32_t x)
// {
//     x = (x & 0x55555555) + ((x >> 1) & 0x55555555);
//     x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
//     x = (x & 0x0F0F0F0F) + ((x >> 4) & 0x0F0F0F0F);
//     x = (x & 0x00FF00FF) + ((x >> 8) & 0x00FF00FF);
//     x = (x & 0x0000FFFF) + ((x >> 16) & 0x0000FFFF);
//     return 1 & x;
// }

// x = x ^ (x>>16);

// Alternatywnie:
// x = x ^ (x>>16);
// x = x ^ (x>>8);
// x = x ^ (x>>4);
// x = x ^ (x>>2);
// x = x ^ (x>>1);
// retun x & 1;