#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* Oblicz x * 3 / 4 zaokrąglając w dół. */
__int32_t threefourths(__int32_t x)
{
    __int32_t accuracy = x & 0x3;
    accuracy = ((accuracy << 1) + accuracy) >> 2;
    x = x >> 2;
    x = (x << 1) + x + accuracy;
    return x;
}

int main()
{
    __int32_t x = -28;
    printf("%d\n", threefourths(x));
    printf("%d\n", (x/4)*3);
}

//NA ZAJĘCIACH

// int32_t threefourths(int32_t x)
// {
//     //najpierw liczymy reszte z dzielenia x/4
//     //przesuwajac x w prawo o 2 bity dostajemy floor(x/4)
//     //przesuwajac x w lewo o 2 bity dostajemy 4*floor(x-4)
//     //a wiec reszta to:
//     int32_t reszta = x - ((x >> 2) << 2);
//     //mnozymy 3 razy reszte i robimy podloge z dzielenia tego przez 4
//     int32_t dodaj = (reszta + reszta + reszta) >> 2;
//     //bierzemy teraz floor(x/4)
//     x = x >> 2;
//     //zwracamy 3*floor(x/4) + 3*floor(reszta/4)
//     return x + x + x + dodaj;
// }

//alternatywnie

// x - (x<<2) - (((x>>1) | x)&1)