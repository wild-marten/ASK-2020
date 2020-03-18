#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

struct A{
    char a;
    char b;
    char c;
    char d;
    char e;
};

void secret(uint8_t *to, uint8_t *from, size_t count)
{
    size_t n = (count + 7) / 8;

    switch (count % 8)
    {
    case 0:
        do
        {
            *to++ = *from++;
        case 7:
            *to++ = *from++;
        case 6:
            *to++ = *from++;
        case 5:
            *to++ = *from++;
        case 4:
            *to++ = *from++;
        case 3:
            *to++ = *from++;
        case 2:
            *to++ = *from++;
        case 1:
            *to++ = *from++;
        } while (--n > 0);
    }
}

void aa(uint8_t *to, uint8_t *from, size_t count)
{
    size_t n = (count + 7) / 8;

    void* jump_arr[8] = {&&a, &&b, &&c, &&d, &&e, &&f, &&g, &&h};

    size_t idx;

    if(count!= 0)
        idx = (count-1)%8;
    else 
        idx = 0;

    goto *jump_arr[idx];

    h:
    do
    {
    *to++ = *from++;
    g:
        *to++ = *from++;
        f:
            *to++ = *from++;
            e:
                *to++ = *from++;
                d:
                    *to++ = *from++;
                    c:
                        *to++ = *from++;
                        b:
                            *to++ = *from++;
                            a:
                                *to++ = *from++;
    } while (--n > 0);
}



int main()
{
     uint8_t a[2] ={0, 0}, b[2]={1, 1};
    
    printf("%lu\n", sizeof(struct A));

    // aa(&a, &b, 3);

    // // printf("%d %d\n", a, *aaa);
    // printf("%p %p\n", a, b);

    // secret(b, a, 3);

    // for(int i=0; i<2; i++)
    //     printf("%d ", b[i]);

    //   for(int i=0; i<2; i++)
    //     printf("%d ", a[i]);


    // printf("%p %p\n", a, b);
    return 0;
}