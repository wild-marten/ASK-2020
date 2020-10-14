# ASK - lista 2.

:::info
Na zajęcia 2 kwietnia 2020
:::

## Zadanie 1.
:::info
Poniżej podano wartości typu «long» leżące pod wskazanymi adresami i w rejestrach:

| Adres | Wartość | Rejestr | Wartość |
| ----- | ------- | ------- | ------- |
| 0x100 | 0xFF    | %rax    | 0x100   |
| 0x108 | 0xAB    | %rcx    | 1       |
| 0x110 | 0x13    | %rdx    | 3       |
| 0x118 | 0x11    |         |         |

Oblicz wartość poniższych operandów:
:::

1. %rax - ```0x100```
2. 0x110 - ```0x13```
3. $0x108 - ```0x108```
4. (%rax) - ```[0x100] = 0xFF```
5. 8(%rax) - ```[0x100 + 8] = 0xAB```
6. 21(%rax,%rdx) - ```[0x118] = 0x11```
7. 0xFC(,%rcx,4) - ```[0xFC + 1 * 4] = [0x100] = 0xFF```
8. (%rax,%rdx,8) - ```[0x100 + 3 * 8] = [0x100 + 0x18] = 0x11```
9. 265(%rcx,%rdx,2) - ```[265 + 1 + 3 * 2] = [0x109 + 0x7] = [0x110] = 0x13```

## Zadanie 2.
:::info
Każdą z poniższych instrukcji wykonujemy w stanie maszyny opisanym tabelką z poprzedniego zadania. Wskaż miejsce, w którym zostanie umieszczony wynik działania instrukcji, oraz obliczoną wartość.
:::

1. addq %rcx,(%rax)
```adres: 0x100``` 
```wartość: 0x100```

2. subq 16(%rax),%rdx
```adres: %rdx```
```wartość: 3 - [0x100 + 0x10] = 3 - [0x110] = 3 - 0x13 = -0x10```

3. shrq $4,%rax
```adres: %rax```
```wartość: 0x100 >> 4 = 0x10```


4. incq 16(%rax)
```adres: 0x100 + 0x10 = 0x110```
```wartość: [0x110]++ = 0x14```

5. decq %rcx
```adres: %rcx```
```wartość: 1-- = 0```

6. imulq 8(%rax)
```adres: %rdx:%rax``` ??? // %rax
```wartość: 0x100 ⋅ 0xAB = 0xAB00``` ???

7. leaq 7(%rcx,%rcx,8),%rdx
```adres: %rdx```
```wartość: 7 + 1 + 1 * 8 = 0x10```

8. leaq 0xA(,%rdx,4),%rdx
```adres: %rdx```
```wartość: 0xA + 3 * 4 = 0x16```

## Zadanie 3.
:::info
W rejestrach %rdi i %rsi przechowujemy wartość zmiennych «x» i «y». Porównujemy je instrukcją «cmp %rsi,%rdi». Jakiej instrukcji należy użyć, jeśli chcemy skoczyć do etykiety «label» gdy:
(Odpowiedź uzasadnij odwołując się do semantyki bitów w rejestrze flag)
:::

cmp %rsi,%rdi -> x - y

1. «x» był wyższy lub równy «y»
``` jae / jnb (Jump if above or equal) ~CF ```

2. «y» nie był mniejszy lub równy «x»
```jl / jnge (Jump if less) SF^0F 189```

3. «x» nie był niższy lub równy «y»
```ja / jnbe (Jump if above) ~CF&~ZF```

## Zadanie 4.
:::info
Zaimplementuj w asemblerze x86-64 procedurę konwertującą liczbę typu «uint32_t» między formatem little-endian i big-endian. Argument funkcji jest przekazany w rejestrze %edi, a wynik zwracany w rejestrze %eax. Należy użyć instrukcji cyklicznego przesunięcia bitowego «ror» lub «rol».
Podaj wyrażenie w języku C, które kompilator optymalizujący przetłumaczy do instrukcji «ror» lub «rol».
:::

```
f_rol:
  rorw  $8, %di
  rorl  $16, %edi
  rorw  $8, %di
  movl  %edi, %eax
  ret
```
  
```C
uint32_t rotl32 (uint32_t value, unsigned int count) {
    return value<<count | value>>(32-count);
}
```

## Zadanie 5.
:::info
Zaimplementuj w asemblerze x86-64 funkcję liczącą wyrażenie «x + y». Argumenty i wynik funkcji są 128-bitowymi liczbami całkowitymi ze znakiem i nie mieszczą się w rejestrach maszynowych. Zatem «x» jest przekazywany przez rejestry %rdi (starsze 64 bity) i %rsi (młodsze 64 bity), analogicznie argument «y» jest przekazywany przez %rdx i %rcx, a wynik jest zwracany w rejestrach %rdx i %rax.
Wskazówka! Użyj instrukcji «adc». Rozwiązanie wzorcowe składa się z 3 instrukcji bez «ret».
:::

```
add128:
  addq %rsi %rcx
  movq %rcx %rax
  adcq %rdi %rdx
  ret
```
adc - adds the destination operand (first operand), the source operand (second operand), and the carry (CF) flag and stores the result in the destination operand. The destination operand can be a register or a memory location; the source operand can be an immediate, a register, or a memory location. (However, two memory operands cannot be used in one instruction.) The state of the CF flag represents a carry from a previous addition. When an immediate value is used as an operand, it is sign-extended to the length of the destination operand format.

## Zadanie 6.
:::info
Zaimplementuj w asemblerze x86-64 funkcję liczącą wyrażenie «x * y». Argumenty i wynik funkcji są 128-bitowymi liczbami całkowitymi bez znaku. Argumenty i wynik są przypisane do tych samych rejestrów co w poprzednim zadaniu. Instrukcja «mul» wykonuje co najwyżej mnożenie dwóch 64-bitowych liczb i zwraca 128-bitowy wynik. Wiedząc, że $n = n_{127...64} · 2^{64} + n_{63...0}$, zaprezentuj metodę obliczenia iloczynu, a dopiero potem przetłumacz algorytm na asembler.
UWAGA! Zapoznaj się z dokumentacją instrukcji «mul» ze względu na niejawne użycie rejestrów %rax i %rdx.
:::

```
mul128:

```

## Zadanie 7.
:::info
Zaimplementuj poniższą funkcję w asemblerze x86-64, przy czym wartości «x» i «y» typu «uint64_t» są przekazywane przez rejestry %rdi i %rsi, a wynik zwracany w rejestrze %rax. Najpierw rozwiąż zadanie używając instrukcji skoku warunkowego. Po napisaniu rozwiązania uprość je z użyciem instrukcji «set» albo «cmov» albo «sbb».

Wskazówka! Rozwiązanie wzorcowe składa się z 3 instrukcji bez «ret».
:::

```
addu:
    leaq (%rdi,%rsi),%rax
    cmp %rdi,%rax
    jb label
    ret
label:
    movq ULONG_MAX,%rax
    ret
----------------------------
addu:
    leaq (%rdi,%rsi),%rax
    cmp %rdi,%rax
    cmovb ULONG_MAX,%rax
    ret
   
-------------------------
addu:
    addq %rsi, %rdi
    movq %rdi, %rax
    JB label
    ret
label: movq MAX_LONG, %rax
    ret
``` 
## Zadanie 8.
:::info
W wyniku deasemblacji procedury «long decode(long x, long y)» otrzymano kod:
```
1 decode: leaq (%rdi,%rsi), %rax
2 xorq %rax, %rdi
3 xorq %rax, %rsi
4 movq %rdi, %rax
5 andq %rsi, %rax
6 shrq $63, %rax
7 ret
```
Zgodnie z System V ABI2 dla architektury x86-64, argumenty «x» i «y» są przekazywane odpowiednio przez rejestry %rdi i %rsi, a wynik zwracany w rejestrze %rax. Napisz funkcję w języku C, która będzie liczyła dokładnie to samo co powyższy kod w asemblerze. Postaraj się, aby była ona jak najbardziej zwięzła.
:::
```C
long decode(long n, long m)
{
  long x = n + m;
  x = ((n ^ x) & (m ^ x)) >> 63;
  return x; 
}
```

## Zadanie 9.
:::info
Zapisz w języku C funkcję o sygnaturze «int puzzle(long x, unsigned n)» której kod w asemblerze podano niżej. Zakładamy, że parametr «n» jest niewiększy niż 64. Przedstaw jednym zdaniem co robi ta procedura.
```
1 puzzle: testl %esi, %esi
2 je .L4
3 xorl %edx, %edx
4 xorl %eax, %eax
5 .L3: movl %edi, %ecx
6 andl $1, %ecx
7 addl %ecx, %eax
8 sarq %rdi
9 incl %edx
10 cmpl %edx, %esi
11 jne .L3
12 ret
13 .L4: movl %esi, %eax
14 ret
```
UWAGA! Instrukcje operujące na młodszej połowie 64-bitowego rejestru czyszczą jego starszą połowę (brzydota x86-64).
:::
```C
int puzzle(long x, unsigned n)
{

}
```
