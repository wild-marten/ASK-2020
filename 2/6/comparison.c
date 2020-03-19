#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>



uint32_t ucomp(uint32_t x, uint32_t y)
{
    //when the shift is not arithmetic
    //return ((x >> 1) - (y >> 1) - (~x & y & 0x1)) >> 31;
    
    return (((x >> 1) - (y >> 1) - (~x & y & 0x1)) >> 31) & 0x1;
}

int32_t scomp(int32_t x, int32_t y)
{
    //when the shift is not arithmetic
    //return ((x >> 1) - (y >> 1) - (~x & y & 0x1)) >> 31;
    
    return (((x >> 1) - (y >> 1) - (~x & y & 0x1)) >> 31) & 0x1;
}

int main()
{

    printf("%d\n", ucomp(233, 111));
    printf("%d\n", scomp(INT_MAX -1, INT_MAX ));
}

//NA ZAJĘCIACH
   // x,y - unsigned int
    // (~x&y) |((~x|y) & (x-y))

    // Dzięki temu wyrażeniu,na najstarszym bicie uzyskamy odpowiedź
    // (pisząc np (x-y)31 mam na myśli najstarszy bit liczby (x-y))
    // 1: (~x&y) - sprawdza przypadek gdy y31=1 i x31=0 (czyli oczywiscie x<y)
    // 2: (~x|y) - niespełnione jedynie gdy x31=1 i y31=0 (czyli oczywiście x>y)
    // 3: (x-y) - tutaj trzeba rozważyć więcej możliwości:
    // GDY X<Y:
    // gdy y31=1 i x31=1 to x-y ma wartosc co najmniej 0x80000001 czyli (x-y)31=1,
    // gdy y31=0 i x31=0 wtedy (x-y) równe co najmniej 0x80000001 czyli (x-y)31=1,
    // gdy y31=0 i x31=1, to sprzeczne z X<Y
    // gdy y31=1 i x31=0, to już sprawdzone w 1
    // GDY X = Y
    // (x-y)31 =0 czyli ok
    // GDY X>Y
    // gdy x31=1 i y31=0, to już rozważone w 2
    // gdy x31=1 i y31=1, to w najgorszym przypadku gdy odejmujemy 0xFFFFFFFF - 0x80000001, to (x-y)31=0,czyli ok
    // gdy x31=0 i y31=0, to analogicznie jak powyżej
    // gdy x31=0 i y31=1 to sprzeczne z X>Y

    // x,y - signed int
    // (x&~y) | ((x|~y) & (x-y))
    // postępujemy analogicznie jak w a) rozważając przypadki



 