# Ćwiczenia 7 z ASK, grupa AKr (23.04.2020)


### Zadanie 1

:::info
 Przeczytaj poniższy kod w języku C i odpowiadający mu kod w asemblerze, a następnie wywnioskuj jakie są wartości stałych «A» i «B».
```cpp=
typedef struct {
    int x[A][B];
    long y;
} str1;

typedef struct {
    char array[B];
    int t;
    short s[A];
    long u;
} str2;

void set_val(str1 *p, str2 *q) {
    long v1 = q->t;
    long v2 = q->u;
    p->y = v1 + v2;
}
```
```=
set_val:
    movslq 8(%rsi),%rax
    addq 32(%rsi),%rax
    movq %rax,184(%rdi)
    ret
```
:::

1. long v1=q->t      ==     *q+8
2. long v2=q->u      ==     *q+32
3. p->y            ==    rdi+184

:::info
```cpp=
typedef struct {
    char array[B]; // 8     //
    int t;                 // 32
    short s[A];            //
    long u;
} str2;
```
Więc aby przejść do int t musimy się przesunąc o 8 bajtów,czyli char array[B] wynosi maksymalnie 8 bajtów.
Minimalny rozmiar char array[B] to 5,gdyż inaczej int mogłby ustawić się na wcześniejszej pozycji równej 4. 
Char jest 1 bajtowy, więc 5<=B<=8.

Aby przejść do long u to przesuwamy się o 32 bajty.
32- 8(char array)- 4 (int t)=20
Więc short s[A] ma maksymalnie 20 bajtów, zaś minimalnie 13, gdyż 32-8 (tyle ile long mogłby się przesunąć) +1 (aby przesuniecie nie było możliwe)- 8 -4=13
Short ma 2 bity, więc 7<=A<=10

Maksymalna wielkość int x[A][B] wynosi 184 bajtów.
Minimalna to 184-8(wielkość longa)+1=177.
Ilość komórek to 177/4=45(zaokrąglone w góre) < x <184/4=46 
A * B=45 lub A * B=46
5<=B<=8        7<=A<=10
Z obliczeń wynika,że A=5 a B=9.

:::



### Zadanie 2

:::info
Przeczytaj poniższy kod w języku C i odpowiadający mu kod w asemblerze, a następnie wywnioskuj jakie są wartości stałych «R», «S» i «T».
```cpp=
long A[R][S][T];
long store_elem(long i, long j, long k, long *dest)
{
    *dest = A[i][j][k];
    return sizeof(A);
}
```
```=
store_elem:
    leaq (%rsi,%rsi,2),%rax //rax = 3j
    leaq (%rsi,%rax,4),%rax //rax = 13j
    movq %rdi,%rsi          //rsi = i
    salq $6,%rsi            //rsi = i<< 6 = 64i
    addq %rsi,%rdi          //rdi = i+ 64i = 65i
    addq %rax,%rdi          //rdi = 13j + 65i
    addq %rdi,%rdx          //rdx = 13j + 65i + k  
    movq A(,%rdx,8),%rax    //rax = A + 8(65i + 13j + k)
    movq %rax,(%rcx)        //*dest = A[i][j][k]
    movq $3640,%rax         //sizeof(A) = 8 * R * S * T = 3640
    ret
```
:::


Szukamy wartości liczb R, S T. Widzimy, że sizeof(A) = 3640 oraz jest to tablica typu long, zatem mozemy wyliczyć, że 
R * S * T = 3640/8 = 455.
Z wykładu wiemy, że chcać uzyskać element tablicy A[R][S][T] oznaczony A[i][j][k] musimy przesunąć wskaźnik A na miejsce  
A + 8 * i * S * T + 8 * j * T + 8 * k = A + 8(i * S * T + j * T + k).

W kodzie chcemy odnieść się do pozycji A + 8(65i+ 13j + k), zatem widzimy, że T = 13,
a S = 65/13 = 5. Z naszego iloczynu R * S * T możemy wyliczyć, że R = 455/65 = 7.

Zatem:
T = 13
S = 5
R = 7


### Zadanie 3

:::info
Przeczytaj poniższy kod w języku C i odpowiadający mu kod w asemblerze, a następnie wywnioskuj jaka jest wartość stałej «CNT» i jak wygląda definicja struktury «a_struct».
```cpp=
typedef struct {
    int first;
    a_struct a[CNT];
    int last;
} b_struct;

void test (long i, b_struct *bp) {
    int n = bp->first + bp->last;
    a_struct *ap = &bp->a[i];
    ap->x[ap->idx] = n;
}
```
```=
test:
    movl 0x120(%rsi),%ecx  //0x120 = 288B, przesunięcie o tyle wskaźnika
    addl (%rsi),%ecx
    leaq (%rdi,%rdi,4),%rax
    leaq (%rsi,%rax,8),%rax //obliczamy rozmiar struktury - 8*5 = 40B
    movq 0x8(%rax),%rdx
    movslq %ecx,%rcx
    movq %rcx,0x10(%rax,%rdx,8)
    retq
```
:::


```cpp=
typedef struct A
{
    long idx;
    long x[4];
}A;
```
CNT = 7

Przesuwamy wskaźnik o 288B, 8B rezerwujemy na int, więc zosteje nam 280B na struktury. Następnie chcąc dostać się do struktury musimy pomnożyć liczbę przez 40(obliczona przy pomocy poleceń leaq), stąd A posiada 40B, posiada też liczbę i tablicę - co wnioskujemy z funkcji void test w C, z których liczba jest na początku. Tablica jest tablicą longów, bo nasze n rozszerzamy na 64 bit. Long ma 64 bit(operujemy na rejestrach 64 bit), wieć jeśli chcemy mieć ich tablice będzie miała rozmiar 5 (40/8), jednak potrzbujemy miejsce na idx, więc ma rozmiar 4. 



### Zadanie 4

:::info
Przeczytaj definicję unii «elem» i wyznacz jej rozmiar w bajtach. Następnie przepisz procedurę «proc» na kod w języku C.
```cpp=
union elem {
    struct {
        long *p;
        long y;
    } e1;
    struct {
        long x;
        union elem *next;
    } e2;
};
```
```=
proc:
    movq 8(%rdi),%rax
    movq (%rax),%rdx
    movq (%rdx),%rdx
    subq 8(%rax),%rdx
    movq %rdx,(%rdi)
    ret
```
:::


Unia w C ma zajmuje tyle pamięci co jej największy element.
Tutaj elementami są 2 structy. Każdy z tych structów zajmuje 16 bajtów zatem cała unia
również tyle zajmuje.

najpierw zapiszę wszystkie operacje bez interpretowania których structów dotyczą.

```cpp=
elem* proc(elem* now)
{
    *now = *(*(*(now+8))) - *(*(now+8)+8);
    return now->next;
}
```

teraz kod już po zinterpretowaniu

```cpp=
elem* proc(elem* now)
{
    now->e2.x = *(now->e2.next->e1.p) - now->e2.next->e1.y;
    return now->next;
}
```
wyjaśnienie:
zacznijmy od interpretacji tej części ```*(*(*(now+8)))```
```
(*(now+8))
```
to może być ```now->e2.next``` lub ```now->e1.y``` jednak w następnym kroku 
chcę wykonać na tym elemencie dereferencje więc musi to być wskaźnik, zatem jest to ```now->e2.next```
```
(*(*(now+8))
```
jest to dereferencja wskaźnika na elem a wskaźnik na union elem jest jednocześnie wskaźnikiem na 1 element 
tego uniona zatem może być to ```now->e2.next->e1.p``` lub ```now->e2.next->e2.x```. Znowu kolejnym
krokiem będzie dereferencja zatem znowu musi być to wskaźnik zatem mamy pewność że jest to
```now->e2.next->e1.p```

```
(*(*(*(now+8)))
```

dokonujemy dereferencji na wskaźniku na long zatem jest to ```*(now->e2.next->e1.p)```.
dane wyrażenie ma typ long. Można w takim razie wywnioskować już na tym etapie że to co chcemy
odjąć powinno mieć również typ long a także to do czego chcemy przypisać wartość różnicy również
powinno mieć typ long. Z tego widzimy od razy że ```*now``` ma być typu long zatem będzie to ```now->e2.x```

w takim razie intepretacja tej części ```*(*(now+8)+8)```

tak jak we wcześniejszym przykładzie na ```*(now+8)``` będę chciał dokonać w następnym kroku dereferencje 
zatem musi być to wskaźnik więc będzie to ```now->e2.next```

wiemy z poprzedniej części rozwiązania że ```*(*(now+8)+8)``` powinno być longiem zatem będzie to 
```now->e2.next->e1.y```

### Zadanie 5

:::info
Przeczytaj definicje struktur «SA» i «SB», po czym przeanalizuj kod procedur o sygnaturach «SB eval(SA s)» i «long wrap(long x, long y, long z)». Nastepnie zapisz w języku C kod odpowiadający procedurom «eval» i «wrap». Narysuj diagram przedstawiający zawartość rekordu aktywacji procedury «wrap» w momencie wywołania funkcji «eval».
```cpp=
typedef struct A {
    long u[2];
    long *v;
} SA;
typedef struct B {
    long p[2];
    long q;
} SB;
```
```=
eval:
    movq %rdi, %rax
    movq 16(%rsp), %rcx // bierzemy y ze stosu
    movq 24(%rsp), %rdx // bierzemy &z ze stosu
    movq (%rdx), %rsi   // dokonujemy dereferencji
    movq %rcx, %rdx
    imulq %rsi, %rdx    // y * z
    movq %rdx, (%rdi)   // kładziemy wynik na stosie
    movq 8(%rsp), %rdx  // bierzemy x ze stosu
    movq %rdx, %rdi     
    subq %rsi, %rdi     // x - z
    movq %rdi, 8(%rax)  // kładziemy wynik na stosie
    subq %rcx, %rdx     // x - y
    movq %rdx, 16(%rax) // wynik kładziemy na stosie
    ret

wrap:
    subq $72, %rsp  // szykujemy miejsce na stosie
    movq %rdx, (%rsp) // kładziemy 'z' na stos
    movq %rsp, %rdx
    leaq 8(%rsp), %rax
    pushq %rdx  // kładziemy '&z' na stos
    pushq %rsi  // kładziemy 'y' na stos
    pushq %rdi  // kładziemy 'x' na stos
    movq %rax, %rdi // jako argument dajemy adres "pod" zmiennymi na stosie
    call eval
    movq 40(%rsp), %rax  // bierzemy (x - z)
    addq 32(%rsp), %rax  // dodajemy do niego (y * z)   
    imulq 48(%rsp), %rax // wynik mnożymy przez (x - y)
    addq $96, %rsp       // zwalniamy miejsce w stosie
    ret
```
:::

         W momencie wywołania eval  
        |------------------------|
        |  96    |               |
        |  88    |               |
        |  80    |               |
        |  72    |               |
        |  64    |               |
        |  56    |               |
        |  48    |               |
        |  40    |               |%rax, %rdi
        |  32    | z             |
        |  24    | &z            |v
        |  16    | y             |u[1]
        |  8     | x             |u[0]
        |  0     |returnAdress   |%rsp
    
    
    
            Po zakończeniu eval  
        |------------------------|
        |  96    |               |
        |  88    |               |
        |  80    |               |
        |  72    |               |
        |  64    |               |
        |  56    | x - y         |q
        |  48    | x - z         |p[1]
        |  40    | y * z         |p[0]
        |  32    | z             |
        |  24    | &z            |
        |  16    | y             |
        |  8     | x             |%rsp



```c=
SB eval(SA sa)
{
    SB sb;
    sb.p[0] = sa.u[1] * (*sa.v);
    sb.p[1] = sa.u[0] - (*sa.v);
    sb.q    = sa.u[0] - sa.u[1];
    return sb;
}


long wrap(long x,long y,long z)
{
    SA sa;
    sa.v = &z;
    sa.u[0] = x;
    sa.u[1] = y;
    SB sb = eval(sa);

    return (sb.p[1] + sb.p[0]) * sb.q;
}
```




### Zadanie 6

:::info
Poniżej widniej kod procedury o sygnaturze «float puzzle6(struct P *, float)». Wyznacz definicję typu «struct P». Przetłumacz tę procedurę na język C i wyjaśnij jednym zdaniem co robi.
```=
puzzle6:
    movq (%rdi), %rdx
    leaq 8(%rdi), %rcx
    xorl %eax, %eax
    vxorps %xmm1, %xmm1, %xmm1
    vmovss .LC1(%rip), %xmm2
.L2: 
    cmpq %rdx, %rax
    jge .L5
    vfmadd231ss (%rcx,%rax,4), %xmm2, %xmm1
    incq %rax
    vmulss %xmm0, %xmm2, %xmm2
    jmp .L2
.L5: 
    vmovaps %xmm1, %xmm0
    ret

.LC1: .long 0x3f800000
```
:::


```c=
struct P{
    long n;
    float a[];
}

float puzzle6(struct P * p , float q)
{
    int n = p->n;//2
    float *a = p.a;//3
    
    float sum = 0;//5
    float pow = 1.0;//6 (0x3f800000)
    for(long i = 0; i<=n; i++)//4, 7-8, 10, 12
    {
        
        sum += a[i] * pow;//9
        pow*= q;//11
    }
    return sum;
    
}

```
```c=
vfmadd231ss a, b, c
powieksz c przez pomnożone a przez b  
c+=a*b;

vmulss a, b, c
c=a*b;
```

Wynikiem jest:
$$\sum_{i=0}^{n} a_i*q^i$$
