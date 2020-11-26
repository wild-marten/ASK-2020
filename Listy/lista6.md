# Ćwiczenia 6 z ASK, grupa AKr

## Lista ćwiczeniowa 6 (16.04.2020)

### Zadanie 1

:::info
Poniższy wydruk otrzymano w wyniku deasemblacji rekurencyjnej procedury zadeklarowanej następująco: «long puzzle(long n, long *p)». Zapisz w języku C kod odpowiadający tej procedurze. Następnie opisz zawartość jej rekordu aktywacji (ang. stack frame). Wskaż rejestry zapisane przez funkcję wołaną (ang. callee-saved registers), zmienne lokalne i adres powrotu.
```=
puzzle:
    push %rbp
    xorl %eax, %eax
    mov %rsi, %rbp
    push %rbx
    mov %rdi, %rbx
    sub $24, %rsp
    test %rdi, %rdi
    jle .L1
    lea 8(%rsp), %rsi
    lea (%rdi,%rdi), %rdi
    call puzzle
    add 8(%rsp), %rax
    add %rax, %rbx
.L1: 
    mov %rbx, (%rbp)
    add $24, %rsp
    pop %rbx
    pop %rbp
    ret
```
Uwaga! Wskaźnik wierzchołka stosu w momencie wołania procedury musi być wyrównany do adresu podzielnego przez 16.

:::


```c=
long puzzle(long n,long* p)
{
    long res=0;//%rax
    if(n>0)
    {
        res=puzzle(n*2,p);
        res+=*p;
        n+=res;
    }
    *p=n;
    return res;
}
```
|rekord aktywacji|
|----------------|
|%rbp            |
|%rbx            |
|        -        |
|        na to miejsce wskazuje %rsi gdy wywolujemy funkcje       |
|         -       |
|call puzzle     |

rejestry zapisane przez funkcje wołaną: %rbp, %rbx

zmienne lokalne = res -> %rax

adres powrotu jest w %rbp przy wywołaniu funkcji odkładamy go na stos,
przy powrocie jest zdejmowany (instrukcje push i pop)


### Zadanie 2


:::info
 Skompiluj poniższy kod źródłowy kompilatorem gcc z opcjami «-Og -fomit-frame-pointer» i wykonaj deasemblację jednostki translacji przy użyciu programu «objdump». Wytłumacz co robi procedura alloca(3), a następnie wskaż w kodzie maszynowym instrukcje realizujące przydział i zwalnianie pamięci.
```cpp=
#include <alloca.h>

long aframe(long n, long idx, long *q) {
   long i;
   long **p = alloca(n * sizeof(long *));
   p[n-1] = &i;
   for (i = 0; i < n; i++)
      p[i] = q;
   return *p[idx];
}
```
Wskazówka: Przeczytaj również co robi instrukcja «leave».

:::


```c=
000000000000066a <aframe>:
 66a:   55                      push   %rbp
 66b:   48 89 e5                mov    %rsp,%rbp
 //przydział pamięci na stosie - początek
 66e:   48 83 ec 10             sub    $0x10,%rsp
 672:   64 48 8b 04 25 28 00    mov    %fs:0x28,%rax
 679:   00 00
 67b:   48 89 45 f8             mov    %rax,-0x8(%rbp)
 //w rax obliczamy ile pamięci potrzebujemy na n*long
 67f:   31 c0                   xor    %eax,%eax
 681:   4c 8d 0c fd 00 00 00    lea    0x0(,%rdi,8),%r9
 688:   00
 689:   49 8d 41 1e             lea    0x1e(%r9),%rax
 68d:   48 83 e0 f0             and    $0xfffffffffffffff0,%rax
 691:   48 29 c4                sub    %rax,%rsp
 //przydział pamięci na stosie - koniec
 694:   4c 8d 44 24 0f          lea    0xf(%rsp),%r8
 699:   49 83 e0 f0             and    $0xfffffffffffffff0,%r8
 69d:   4c 89 c1                mov    %r8,%rcx
 6a0:   48 8d 45 f0             lea    -0x10(%rbp),%rax
 6a4:   4b 89 44 08 f8          mov    %rax,-0x8(%r8,%r9,1)
 6a9:   48 c7 45 f0 00 00 00    movq   $0x0,-0x10(%rbp)
 6b0:   00
 6b1:   eb 09                   jmp    6bc <aframe+0x52>
 6b3:   48 89 14 c1             mov    %rdx,(%rcx,%rax,8)
 6b7:   48 83 45 f0 01          addq   $0x1,-0x10(%rbp)
 6bc:   48 8b 45 f0             mov    -0x10(%rbp),%rax
 6c0:   48 39 f8                cmp    %rdi,%rax
 6c3:   7c ee                   jl     6b3 <aframe+0x49>
 6c5:   49 8b 04 f0             mov    (%r8,%rsi,8),%rax
 6c9:   48 8b 00                mov    (%rax),%rax
 6cc:   48 8b 75 f8             mov    -0x8(%rbp),%rsi
 6d0:   64 48 33 34 25 28 00    xor    %fs:0x28,%rsi
 6d7:   00 00
 6d9:   75 02                   jne    6dd <aframe+0x73>
 //zwolnienie pamięci na stosie
 6db:   c9                      leaveq
 6dc:   c3                      retq
 6dd:   e8 5e fe ff ff          callq  540 <__stack_chk_fail@plt>
```
alloca - funkcja alokująca pamięć, która jest automatycznie zwalniana, kiedy funkcja, która wywołała alloca wraca do swojego callera(wywoływacza?).

leave zamyka ramkę. Jest równoważne skopiowaniu %rbp do %rsp, a następnie przywróceniu starej wartości %rbp ze stosu.

### Zadanie 3


:::info
Poniżej widnieje kod procedury o sygnaturze «long puzzle5(void)». Podaj rozmiar i składowe rekordu aktywacji procedury «puzzle5». Procedura «readlong», która wczytuje ze standardowego wejścia liczbę całkowitą, została zdefiniowana w innej jednostce translacji. Jaka jest jej sygnatura? Przetłumacz procedurę «puzzle5» na język C i wytłumacz jednym zdaniem co ona robi.
```=
puzzle5:
    subq $24, %rsp
    movq %rsp, %rdi
    call readlong
    leaq 8(%rsp), %rdi
    call readlong
    movq (%rsp), %rax
    cqto
    idivq 8(%rsp)
    xorl %eax, %eax
    testq %rdx, %rdx
    sete %al
    addq $24, %rsp
    ret
```
:::


```c=

void readlong(long* a){
    scanf("%l", &a);
}

int puzzle5() {
    long a;
    readlong(&a);
    long b;
    readlong(&b);
    if(a%b == 0)
        return 0;
    return 1;
}

```

procedura puzzle5 mówi nam czy występuje reszta z dzielenia dwóch long'ów wczytanych w trakcie wykonania procedury (~ (b|a) )

rozmiar rekordu aktywacji - 32 bajty

trzymamy w nim zmienne a i b



### Zadanie 4


:::info
Procedurę ze zmienną liczbą parametrów używającą pliku nagłówkowego ```stdarg.h``` skompilowano z opcjami «-Og -mno-sse». Po jej deasemblacji otrzymano następujący wydruk. Co robi ta procedura i jaka jest jej sygnatura? Jakie dane są przechowywane w rekordzie aktywacji tej procedury? Prezentację zacznij od przedstawienia definicji struktury «va_list».
```=
puzzle7:
    movq %rsi, -40(%rsp)
    movq %rdx, -32(%rsp)
    movq %rcx, -24(%rsp)
    movq %r8, -16(%rsp)
    movq %r9, -8(%rsp)
    movl $8, -72(%rsp)
    leaq 8(%rsp), %rax
    movq %rax, -64(%rsp)
    leaq -48(%rsp), %rax
    movq %rax, -56(%rsp)
    movl $0, %eax
    jmp .L2
 .L3: 
    movq -64(%rsp), %rdx
    leaq 8(%rdx), %rcx
    movq %rcx, -64(%rsp)
 .L4: 
    addq (%rdx), %rax
 .L2: 
    subq $1, %rdi
    js .L6
    cmpl $47, -72(%rsp)
    ja .L3
    movl -72(%rsp), %edx
    addq -56(%rsp), %rdx
    addl $8, -72(%rsp)
    jmp .L4
 .L6: 
    ret
```
Wskazówka: Przeczytaj rozdział §3.5.7 dokumentu opisującego ABI dostępnego na stronie przedmiotu.

:::

Definicja struktury va_list:
```C=
typedef struct{
    unsigned int gp_offset; /* "odległość" od reg_save_area 
    do kolejnego argumentu przekazanego przez zwykły rejestr*/
    unsigned int fp_offset; /* "odległość" od reg_save_area 
    do kolejnego rejestru typu %xmm0...15 
    (rejestry dla liczb zmiennoprzecinkowych )*/
    void *overflow_arg_area;/* używany do pobierania argumentów 
    przekazanych przez stos*/
    void *reg_save_area; /* początek miejsca, gdzie mamy argumenty 
    przekazane przez rejestry */
}
```
W naszej procedurze:
- gp_offset:  -72(%rsp)
- overflow_arg_area: 8(%rsp)
- reg_save_area: -48(%rsp)
- fp_offset: kompilowaliśmy z flagą -mno-sse, dzięki czemu
wszystkie argumenty przekazane zostały przez rejestry ogólnego przeznaczenia albo stos. W tej procedurze fp_offset nie jest wyodrębniony.

--------------------------

```=
puzzle7:
    movq %rsi, -40(%rsp) 
    movq %rdx, -32(%rsp)
    movq %rcx, -24(%rsp)
    movq %r8, -16(%rsp)
    movq %r9, -8(%rsp)
    movl $8, -72(%rsp)
    leaq 8(%rsp), %rax
    movq %rax, -64(%rsp)
    leaq -48(%rsp), %rax
    movq %rax, -56(%rsp) # Koniec przygotowania stosu
    movl $0, %eax
    jmp .L2    
 .L3: 
    movq -64(%rsp), %rdx # Czytamy kolejny argument przekazany
    leaq 8(%rdx), %rcx   #                          przez stos
    movq %rcx, -64(%rsp)
 .L4: 
    addq (%rdx), %rax # w %rdx mamy obliczony adres kolejnego  
 .L2:                 # argumentu funkcji. Dodajemy go do %rax
    subq $1, %rdi
    js .L6
    cmpl $47, -72(%rsp) # Sprawdzamy, czy jeszcze możemy
    ja .L3              # korzystać z argumentów z rejestrów
    movl -72(%rsp), %edx # Czytamy kolejny argument przekazany przez rejestr
    addq -56(%rsp), %rdx
    addl $8, -72(%rsp)
    jmp .L4
 .L6: 
    ret
```

Stan stosu po wykonaniu początkowych instrukcji (lin. 11):
| ADRES | ZAWARTOŚĆ |
| -------- | -------- | 
|   32(%rsp)  | ...     | 
|   24(%rsp)  | arg3     | 
|   16(%rsp)  | arg2     | 
|   8(%rsp)  |    arg1  |
|   %rsp  |    | 
|   -8(%rsp)  | r9     | 
|   -16(%rsp)  | r8     | 
|   -24(%rsp)  | rcx     | 
|   -32(%rsp)  | rdx     | 
|   -40(%rsp)  | rsi     | 
|   -48(%rsp)  |     | 
|   -56(%rsp)  | reg_save_area, początkowo:   -48(%rsp)    | 
|   -64(%rsp)  | overflow_arg_area, pocz:    8(%rsp)     | 
|   -72(%rsp)  | gp_offset, pocz:    8     | 
|   -80(%rsp)  | ...     | 

- rsi,rdx...r9 - argumenty funkcji przekazane przez rejestry
- arg1,arg2... - argumenty funkcji przekazane przez stos

----------------------
W naszej procedurze czytamy kolejne argumenty funkcji (przekazane przez rejestr albo stos) i dodajemy do %rax, przy czym pierwszy argument funkcji (%rdi) to liczba składników (kolejnych argumentów funkcji). Nasza funkcja ma więc sygnaturę:

```C=
long add_args(long n, ...)
```
