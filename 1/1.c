#include <stdio.h>
#include <stdlib.h>

int pop(unsigned x)
{
    x = x - ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x + (x >> 4)) & 0x0F0F0F0F;
    x = x + (x >> 8);
    x = x + (x >> 16);
    return x & 0x0000003F;
}

struct A
{
    __int8_t a;
    void *b;
    __int8_t c;
    __int16_t d;

};

struct B
{
    __uint16_t a;
    double b;
    void *c;
};

int main()
{
    struct A* u = malloc(sizeof(struct A)*2);
    u->a = 0;
    u->b = &u->a;
    u->c = 3;
    u->d = 4;

    __int8_t *b = &(u->c) + sizeof(struct A);
    *b = 42;
    


    
    printf("%p\n", u->b);
    printf("%d\n", u->c);
    printf("%d\n", u->d);
    printf("%d\n", u[1].c);


    __int8_t c = 1;
    int a[] = {1, 3, 4, 5};

    // volatile int i=0;
    // a[i] = a[i++] - i;

    // //printf("%d\n", i);

    // for(int j=0; j<4; j++)
    //     printf("%d\n", a[j]);

    // __uint32_t i = 0, k = 2, x = 24;
    // //1
    // // __uint32_t bit = (x & (1 << k)) >> k;
    // // __uint32_t y = x | (bit << i);

    // // printf("%d %d\n", x, y);

    // //2
    // printf("%d\n", pop(8));

    // printf("int8: %ld\n", sizeof(__int8_t));
    // printf("void *: %ld\n", sizeof(void *));
    // printf("int16: %ld\n", sizeof(__int16_t));
    // printf("uint16: %ld\n", sizeof(__uint16_t));
    // printf("double: %ld\n", sizeof(double));

    // printf("A: %ld\n", sizeof(A));
    // printf("B: %ld\n", sizeof(B));

    return 0;
}