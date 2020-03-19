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

//NA ZAJĘCIACH
// x - liczba ujemna, której wartość bezwzględną chcemy obliczyć
// y = x>>31 - same 1 lub 0 w zależności czy liczba dodatnia czy ujemna
// z = (x^y) - y

// XOR - 1 1 lub 0 0 -> 0 / 0 1 lub 1 0 -> 1
// x^y (XOR) powoduje że w przypadku ujemnej liczby x, otrzymujemy w wyniku liczbę abs(x) - 1,
// po czym odejmujemy od niej y, czyli uprzednio stworzone -1 (same 1) - dodajemy 1
// W przypadku dodatniej liczby w y, zapisywane są same 0, czyli liczba 0, natomiast po zastosowaniu XOR,
// dostaniemy dokładnie tą samą liczbę co na początku
// Przykładowo dla -5:
// -5 to 1011, y = 1111 x^y -> 0100 = 4, 4 - (-1) = 5;

// Alternatywnie
// int mask = x>>31;
// (mask & (-x)) + (~mask & x)