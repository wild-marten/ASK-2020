# Ćwiczenia 4 z ASK, grupa AKr

## Lista ćwiczeniowa 4 (02.04.2020)

### Zadanie 1

:::info
Poniżej podano wartości typu ```long``` leżące pod wskazanymi adresami i w rejestrach: 
|Adres  | Wartość| | Rejestr| Wartość|
|-------|--------|-|--------|--------|
| 0x100 | 0xFF   | | %rax   | 0x100
| 0x108 | 0xAB   | | %rcx   | 1
| 0x110 | 0x13   | | %rdx   | 3
| 0x118 | 0x11   | |
Oblicz wartość poniższych operandów:
1. %rax
2. 0x110
3. $0x108
4. (%rax)
5. 8(%rax)
6. 21(%rax,%rdx)
7. 0xFC(,%rcx,4)
8. (%rax,%rdx,8)
9. 265(%rcx,%rdx,2)
:::

1) `%rax` zwróci nam $0\text{x}100$ 
2) `0x110` zwróci nam to, co przechowuje, mianowicie $0\text{x}13$
3) `$0x108` to stała, więc wynik wynosi $0\text{x}108$
4) `(%rax)` zwraca to, co jest pod adresem równym wartości `%rax`, czyli $0\text{x}FF$
5) `8(%rax)` obliczy $0\text{x}100+8=0\text{x}108$, pod której adresem znajduje się $0$x$AB$
6) `21(%rax, %rdx)` oblicza się do $(21+0\text{x}100+0\text{x}3)=(0\text{x}15+0\text{x}100+0\text{x}3)=0\text{x}118$ pod którym kryje się $0\text{x}11$
7) `0xFC(,%rcx,4)` obliczy się do $4*0\text{x}1+0\text{xFC} = 0\text{x}100$ pod którym kryje się $0\text{xFF}$
8) `(%rax, %rdx,8)` obliczy się do $0\text{x}100 + 8*0\text{x}3 = 0\text{x}100 + 0\text{x}18 = 0\text{x}118$ pod którym kryje się $0\text{x}11$
9) `265(%rcx, %rdx,2)` obliczy się do $0\text{x}1 + 2*0\text{x}3 + 0\text{x}109 = 0\text{x}7 + 0\text{x}109 = 0\text{x}110$ pod którym kryje się $0\text{x}13$


### Zadanie 2

:::info
Każdą z poniższych instrukcji wykonujemy w stanie maszyny opisanym tabelką z poprzedniego zadania. Wskaż miejsce, w którym zostanie umieszczony wynik działania instrukcji, oraz obliczoną wartość.
1. addq %rcx,(%rax)
2. subq 16(%rax),%rdx
3. shrq $4,%rax
4. incq 16(%rax)
5. decq %rcx
6. imulq 8(%rax)
7. leaq 7(%rcx,%rcx,8),%rdx
8. leaq 0xA(,%rdx,4),%rdx
:::

1.addq %rcx, (%rax)

(%rax) = (%rax) + %rcx
0x100 = 0xFF + 0x1
0x100 = 0x100

2.subq 16(%rax), %rdx

%rdx = %rdx - 0x10(%rax)
%rdx = 0x3 - (0x100 + 0x10)
%rdx = 0x3 - (0x110)
%rdx = 0x3 - 0x13
%rdx = 0xFFFFFFF0

3.shrq $4, %rax               // shrq src,dest -> dest = dest >> src

%rax = %rax >> 0x4
%rax = 0x100 >> 0x4
%rax = 0x10

4.incq 16(%rax)

0x10(%rax) = 0x10(%rax) + 0x1
0x110 = (0x110) + 0x1
0x110 = 0x13 + 0x1
0x110 = 0x14

5.decq %rcx

%rcx = 0x1 - 0x1
%rcx = 0x0

6.imulq 8(%rax)

%rdx: %rax = 8(%rax) * %rax
%rdx: %rax = (0x100 + 0x8) * 0x100
%rdx: %rax = (0x108) * 0x100
%rdx: %rax = 0xAB * 0x100
%rdx: %rax = 0xAB00

7.leaq 7(%rcx, %rcx, 8), %rdx

%rdx = %rcx + 8*%rcx + 0x7
%rdx =  0x1 + 0x8 + 0x7
%rdx = 0x10


8.leaq 0xA(,%rdx,4),%rdx	

%rdx = 0x4 * 0x3 + 0xA
%rdx = 0xC + 0xA
%rdx = 0x16


### Zadanie 3

:::info
W rejestrach %rdi i %rsi przechowujemy wartość zmiennych ```x``` i ```y```. Porównujemy je instrukcją ```cmp %rsi,%rdi```. Jakiej instrukcji należy użyć, jeśli chcemy skoczyć do etykiety ```label``` gdy:
1. ```x``` był wyższy lub równy ```y```,
2. ```y``` nie był mniejszy lub równy ```x```,
3. ```x``` nie był niższy lub równy ```y```,


:::

```
cmp %rsi, %rdi # (%rsi trzyma y, %rdi trzyma x)
```
cmp ustawia flagi dla operacji x-y (  można na to patrzeć jak na x + (-y)  )

Flagi modyfikowane przez cmp: 
| flaga| ||
|-|-|-|
|CF|"Carry Flag"|Wystąpił Overflow dla liczb Unsigned (dodawanie z "pożyczką")|
|OF|"Overflow Flag"|Wystąpił Overflow dla liczb Signed|
|SF|"Sign Flag"|Mówi czy w wyniku operacji zapalił się bit znaku|
|ZF|"Zero Flag"|Mówi czy operacja dała wynik 0 |
|PF|"Parity Flag"|Mówi czy w wyniku operacji dostaliśmy liczbę z parzystą liczbą bitów|

#### 1. x wyższy lub równy y
(wyższy - większy w znaczeniu Unsigned)

jae - Jump if above or equal (też jnb)
wykonuje skok jeżeli CF = 0
```
jae <<label>> #~CF
```

#### 2. y nie był mniejszy lub równy x
!(y<=x) czyli x<y
a więc: x-y < 0

jl - jump if less (signed) (też jnge)
wykonuje skok gdy tylko i wyłącznie jedna z flag SF lub OF = 1
```
jl <<label>>  #SF^0F 
```

cmp zapali flagę SF bo liczba x-y < 0

#### 3. x nie był niższy lub równy y
(niższy - mniejszy w znaczeniu unsigned)
!(x <= y) <=> (x > y)

ja - Jump if above (unsigned) (też jnbe)
wykonuje skok gdy CF = 0 i SF = 0 
```
ja <<label>>  #~CF&~SF 
```


### Zadanie 4

:::info
Zaimplementuj w asemblerze x86-64 procedurę konwertującą liczbę typu ```uint32_t``` między formatem little-endian i big-endian. Argument funkcji jest przekazany w rejestrze %edi, a wynik zwracany w rejestrze %eax. Należy użyć instrukcji cyklicznego przesunięcia bitowego ```ror``` lub ```rol```.
Podaj wyrażenie w języku C, które kompilator optymalizujący przetłumaczy do instrukcji ```ror``` lub ```rol```.
:::

%edi = 0x4A 3B 2C 1D

ROLW %di, 8      ->    %edi = 0x4A 3B 1D 2C

ROLL %edi, 16    ->    %edi = 0x1D 2C 4A 3B

ROLW %di, 8      ->    %edi = 0x1D 2C 3B 4A

MOVL %edi,%eax  ->    %eax = 0x1D 2C 3D 4A

Wyrażenie w C:

x=(x<<count)|(x>>(32-count))


### Zadanie 5

:::info
Zaimplementuj w asemblerze x86-64 funkcję liczącą wyrażenie ```x + y```. Argumenty i wynik funkcji są 128-bitowymi liczbami całkowitymi ze znakiem i nie mieszczą się w rejestrach maszynowych. Zatem ```x``` jest przekazywany przez rejestry %rdi (starsze 64 bity) i %rsi (młodsze 64 bity), analogicznie argument ```y``` jest przekazywany przez %rdx i %rcx, a wynik jest zwracany w rejestrach %rdx i %rax.
Wskazówka! Użyj instrukcji ```adc```. Rozwiązanie wzorcowe składa się z 4 instrukcji bez ```ret```.

:::

| | Starsze bity | Mlodsze bity |
|-|--------------|--------------|
|x|%rdi|%rsi|
|y|%rdx|%rcx|
|ret|%rdx|%rax|

ADD:
"It evaluates the result for both signed and unsigned integer operands and sets the CF and OF flags to indicate a carry (overflow) in the signed or unsigned result, respectively. "
```
add128:
    #dodajemy mlodsze bity
    addq %rsi, %rcx    #flaga CF trzyma overflow z dodawania
    #dodajemy starsze bity + flage CF dzieki adc (DEST <- DEST + SRC + CF)
    adcq %rdi, %rdx    #rdx trzyma sume starszych bitow tak jak w poleceniu
    movq %rcx, %rax    #przenosimy sume mlodszych bitow do rax
    ret
```



### Zadanie 6

:::info
Zaimplementuj w asemblerze x86-64 funkcję liczącą wyrażenie ```x * y```. Argumenty i wynik funkcji są 128-bitowymi liczbami całkowitymi bez znaku. Argumenty i wynik są przypisane do tych samych rejestrów co w poprzednim zadaniu. Instrukcja ```mul``` wykonuje co najwyżej mnożenie dwóch 64-bitowych liczb i zwraca 128-bitowy wynik. Wiedząc, że n = n~127...64~ · 2^64^ + n~63...0~, zaprezentuj metodę obliczenia iloczynu, a dopiero potem przetłumacz algorytm na asembler.
UWAGA! Zapoznaj się z dokumentacją instrukcji ```mul``` ze względu na niejawne użycie rejestrów %rax i %rdx
:::

x - na rejestrach %rdi:%rsi
y - na rejestrach %rdx:%rcx
Wynik - na rejestrach %rdx:%rax

Wiemy, że każdą liczbę n 128-bitową można zapisać jako: $$n_{127...64} \cdot 2^{64} + n_{63...0}$$

Wobec tego $x*y = (x_{127...64}*y_{127...64}*2^{128}) + (x_{63...0}*y_{127...64}*2^{64}) + \\(y_{63...0}*x_{127...64}*2^{64}) +(x_{63...0}*y_{63...0})$ 

Wynik ma być na rejestrach %rdx:%rax (czyli 128 bitach), więc pierwszą część powyższej sumy, możemy pominąć (nie zmieści się).
Rozważamy więc jedynie 3 ostanie składniki powyższej sumy, ponieważ one mieszczą się na 128 bitach:
```
multiply:
    movq  %rdx,%r8
    movq  $0,%rdx
    movq  %rdi,%r9
    imulq %rcx,%r9
    imulq %rsi,%r8
    movq  %rsi,%rax
    mul   %rcx    # w przypadku OF wynik będzie w %rdx:%rax,
    addq  %r9,%rdx
    addq  %r8,%rdx
```

### Zadanie 7

:::info
Zaimplementuj poniższą funkcję w asemblerze x86-64, przy czym wartości «x» i «y» typu ```uint64_t``` są przekazywane przez rejestry %rdi i %rsi, a wynik zwracany w rejestrze %rax. Po napisaniu rozwiązania uprość je z użyciem instrukcji ```set``` albo ```cmov``` albo ```sbb```.

$$
    addu(x,y) = 
    \begin{cases} 
      \texttt{ULONG_MAX} & \text{dla } x + y \geq \texttt{ULONG_MAX} \\
      x + y & \text{w p.p.}
    \end{cases}
    $$
:::

Główna idea jest taka, że chcemy sprawdzić czy dodawanie x i y nie wywołało nam overflow.
Używamy warunku below (mniejszy bez znaku) odpowiednio w skoku i cmov. Jeżeli składnik okaże się większy od wyniku to mamy overflow.
Wtedy przypisujemy %rax wartość ULONG_MAX.

wersja ze skokiem warunkowym:
```
addu:
    leaq (%rdi, %rsi), %rax
    cmp %rdi, %rax
    jb label
    ret
label:
    movq $0xFFFFFFFF, %rax // ULONG_MAX = 0xFFFFFFFF
    ret
```

wersja z użyciem set, cmov lub sbb:
```
addu:
    leaq (%rdi,%rsi), %rax
    cmp %rdi,%rax
    cmovb $0xFFFFFFFF, %rax
    ret
```


### Zadanie 8

:::info
W wyniku deasemblacji procedury ```long decode(long x, long y)``` otrzymano kod:
```
decode: leaq (%rdi,%rsi), %rax
 xorq %rax, %rdi
 xorq %rax, %rsi
 movq %rdi, %rax
 andq %rsi, %rax
 shrq $63, %rax
 ret
```

Zgodnie z System V ABI dla architektury x86-64, argumenty ```x``` i ```y``` są przekazywane odpowiednio przez rejestry %rdi i %rsi, a wynik zwracany w rejestrze %rax. Napisz funkcję w języku C, która będzie liczyła dokładnie to samo co powyższy kod w asemblerze. Postaraj się, aby była ona jak najbardziej zwięzła.
:::

- long x jest w rejestrze %rdi
- long y jest w rejestrze %rsi
- long z jest w rejestrze %rax

Tutaj funkcja niezwięzła
```c=
long decode(long x, long y)
{
    long z = x + y;
    x ^= z;
    y ^= z;
    z = x;
    z &= y;
    z = ((unsigned long)z) >> 63;
    return z;
}
```

Wersja skrócona
```c=
long decode(long x, long y)
{
    long z = x + y;
    return (((unsigned long)((x^z) & (y^z))) >> 63);
}
```
Funkcja sprawdza czy podczas dodawania liczb ze znakiem doszło do przepełnienia.

### Zadanie 9

:::info
Zapisz w języku C funkcję o sygnaturze ```int puzzle(long x, unsigned n)``` której kod w asemblerze podano niżej. Zakładamy, że parametr ```n``` jest niewiększy niż 64. Przedstaw jednym zdaniem
co robi ta procedura.
```
 puzzle: testl %esi, %esi
   je .L4
   xorl %edx, %edx
   xorl %eax, %eax
   .L3: movl %edi, %ecx
   andl $1, %ecx
   addl %ecx, %eax
   sarq %rdi
   incl %edx
   cmpl %edx, %esi
   jne .L3
   ret
   .L4: movl %esi, %eax
   ret
```

:::

- %rdi - x
- %edi - n
- %eax - nasz wynik (cnt)
- %edx - zmienna pomocnicza do petli (y)
```c=
int puzzle(long x , unsigned int n)
{
    int cnt=0; 
    for(int y=0;y!=n;y++)
    {
        cnt+=(x&1);
        x>>=1;
    }
    return cnt; 
}
```

Dana procedura liczy ile jest zapalonych bitów wśród n najmniej znaczących bitów liczby x.

