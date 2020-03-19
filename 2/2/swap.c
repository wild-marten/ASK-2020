#include <stdio.h>

int swap(int *x, int *y)
{
    *x = *x ^ *y;
    *y = *x ^ *y;
    *x = *x ^ *y;
}

int main()
{
    int x = 10, y = 2000;
    swap(&x, &y);

    printf("%d %d \n", x, y);
}


//NA ZAJĘCIACH:
// Zadanie polega na tym, że musimy zamienić wartości 2 zmiennych bez używania dodatkowych zmiennych.
// Jako że możemy wykorzystywać operatory bitowe to można to zrobić za pomocą XORów tak jak poniżej.

// x = x ^ y;
// y = x ^ y; // (x^y)^y
// x = x ^ y; // (x^y)^x

// Pierwszy XOR znajduje nam miejsca na których liczby x i y się różnią.
// Gdy użyjemy XORa drugi raz, to bity liczby y różniące się względem liczby x podmienią swoje wartości i w zmiennej y będzie zapisana wartość x.
// Przy trzecim użyciu analogicznie tylko w zmiennej x zapisujemy wartość y.