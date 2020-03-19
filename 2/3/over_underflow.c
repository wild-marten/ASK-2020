#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

int32_t over(int32_t x, int32_t y)
{
    //check if s = x + y is under/overflow (which can be only overflow btw)
    
    // Step 1. Are x and y same sign? 
    // Step 2. Are s and (any of two) same sign?
    int32_t s = x + y;
    return (~(x^y) >> 31) & ((s^x) >> 31) & 0x1;
}

int main()
{
    printf("%d\n", over(0, 0));
}


//NA ZAJĘCIACH
//Niedomiar lub nadmiar zajdzie gdy wynikiem sumowania dwóch liczb o tym samym znaku będzie liczba o przeciwnym znaku. 
// Dodatkowe &1 jest spowodowane tym, by po przesunięciu o 31 zostawało 0 lub 1 zamiast liczby ujemnej. 
// Pierwsza część (przed alternatywą bitową) szuka niedomiaru, a druga - nadmiaru. W wypadku ich wystąpienia powinna zwrócić 1, w pp. 0.

// (((x&y) & ~(x+y) | ~(x|y) & (x+y))>>31) & 1