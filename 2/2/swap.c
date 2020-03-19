#include <stdio.h>

int swap(int *x, int *y)
{
    *x = *x + *y;
    *y = *x - *y;
    *x = *x - *y;
}

int main()
{
    int x = 10, y = 2000;
    swap(&x, &y);

    printf("%d %d \n", x, y);
}