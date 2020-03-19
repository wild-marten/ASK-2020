#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int32_t sgn(int32_t x)
{
    //when the shift is not arithmetic
//    return x >> 31 | -x >> 31;

    return x >> 31 | (-x >> 31) & 0x1;
}

int main()
{
    printf("%d", sgn(0));
}


//NA ZAJĘCIACH 

// Chcemy wydobyć znak liczby x. W ty celu stosujemy x>>31.
// Dla liczb ujemnych wynikiem jest -1. jednak dla liczb dodatnich wynikiem jest 0.
// A zatem musimu użyć -x>>31, aby przekonać się, czy nie jest to przypadkiem liczba dodatnia (wynikiem będzie wtedy -1).
// Stosując OR mamy możliwość sprawdzenia, czy x jest liczbą niezerową, a wynikiem będzie przeciwny znak do pożądanego ze względu na minusy, a zatem trzeba odwrócić znak (minus przed wyrażeniem).


// int sign(int32_t x){
//     return -(-(x>>31)|(-x>>31));
// }

// int u_sign(uint32_t x){
//     // używając uint32_t możemy wykonać tylko następujacą operację
//     return (-x>>31);
// }