# Ćwiczenia 8 z ASK, grupa AKr (30.04.2020)

### Zadanie 1

:::info
Poniżej podano zawartość pliku «swap.c»:
```cpp=
extern int buf[];

int *bufp0 = &buf[0];
static int *bufp1;

static void incr() {
    static int count = 0;
    count++;
}

void swap() {
    int temp;
    incr();
    bufp1 = &buf[1];
    temp = *bufp0;
    *bufp0 = *bufp1;
    *bufp1 = temp;
}
```
Wygeneruj plik relokowalny «swap.o», po czym na podstawie wydruku polecenia «readelf -t -s» dla każdego elementu tablicy symboli podaj:
• adres symbolu względem początku sekcji,
• typ symbolu – tj. «local», «global», «extern»,
• rozmiar danych, na które wskazuje symbol,
• numer i nazwę sekcji – tj. «.text», «.data», «.bss» – do której odnosi się symbol.
Co przechowują sekcje «.strtab» i «.shstrtab»?
:::


```
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND
     1: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS swap.c
     2: 0000000000000000     0 SECTION LOCAL  DEFAULT    1
     3: 0000000000000000     0 SECTION LOCAL  DEFAULT    3
     4: 0000000000000000     0 SECTION LOCAL  DEFAULT    4
     5: 0000000000000000     0 SECTION LOCAL  DEFAULT    5
     6: 0000000000000000     8 OBJECT  LOCAL  DEFAULT    4 _ZL5bufp1
     7: 0000000000000008     4 OBJECT  LOCAL  DEFAULT    4 _ZZL4incrvE5count
     8: 0000000000000000    22 FUNC    LOCAL  DEFAULT    1 _ZL4incrv
     9: 0000000000000000     0 SECTION LOCAL  DEFAULT    8
    10: 0000000000000000     0 SECTION LOCAL  DEFAULT    9
    11: 0000000000000000     0 SECTION LOCAL  DEFAULT    7
    12: 0000000000000000     8 OBJECT  GLOBAL DEFAULT    5 bufp0
    13: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND buf
    14: 0000000000000016    72 FUNC    GLOBAL DEFAULT    1 _Z4swapv
```
```Value``` - adres
```Type``` - typ
```Size``` - rozmiar
```Ndx``` - number sekcji
|numer|nazwa|
|-----|-----|
|UND|undefined|
|ABS|nie podlega żadnej sekcji|
|1 | ```.text```|
|3 | ```.data```           |
|4|```.bss```|
|5|```.data.rel```|
|7|```.comment```|
| 8| ```.note.GNU-stack```|
|9|```.eh_frame```|


```.strtab``` - String Table
String Table jest trzyma stringi z nazwami symboli.


```.shstrtab``` - Section Header String Table
Section Header String Table trzyma stringi z nazwami sekcji.



### Zadanie 2

:::info
Rozważmy program składający się z dwóch plików źródłowych:
```cpp=
/* mismatch-a.c */
void p2(void);

int main(void) {
    p2();
    return 0;
}
```
```cpp=
/* mismatch-b.c */
#include <stdio.h>

char main;

void p2(void) {
    printf("0x%x\n", main);
}
```
Po uruchomieniu program drukuje pewien ciąg znaków i kończy działanie bez zgłoszenia błędu. Czemu tak się dzieje? Skąd pochodzi wydrukowana wartość? Zauważ, że zmienna «main» w pliku «mismatch-a.c» jest niezainicjowana. Co by się stało, gdybyśmy w funkcji «p2» przypisali wartość pod zmienną «main»? Co by się zmieniło gdybyśmy w pliku «mismatch-b.c» zainicjowali zmienną «main» w miejscu jej definicji?
:::


Dzieje się tak ponieważ ```char main``` jest symbolem słabym (ponieważ jest niezainicjowany), natomiast 
```int main()``` jest symbolem silnym. Konsolidator preferuje symbole silne zatem output będzie pochodził z
```int main()```. Po wykonaniu ```objdump -d mismatch-a.o``` dostajemy output
```
0000000000000000 <main>:
   0:	f3 0f 1e fa          	endbr64 
   4:	50                   	push   %rax
   5:	e8 00 00 00 00       	callq  a <main+0xa>
   a:	31 c0                	xor    %eax,%eax
   c:	5a                   	pop    %rdx
   d:	c3                   	retq
```
natomiast output samego programu to ```0xfffffff3```. Można zauważyć że jest to pierwszy bajt zapisu funkcji ```int main()``` rozszerzony do 4 bajtów.

jeśli spróbujemy zainicjalizować ```char main``` to stanie się to symbol silny zatem będziemy mieli konflikt silnych symboli i spowoduje to linker error.

natomiast jeśli spróbujemy zmienić wartość ```char main``` to dostaniemy błąd podczas wykonania programu ponieważ spróbujemy zmienić zawartość kodu funkcji ```int main()``` co nie jest dozwolone. 

### Zadanie 3

:::info
Zapoznaj się z narzędziami do analizy plików relokowalnych w formacie ELF i bibliotek statycznych, tj. «objdump», «readelf» i «ar»; a następnie odpowiedz na następujące pytania:

1. Ile plików zawierają biblioteki «libc.a» i «libm.a» (katalog «/usr/lib/x86_64-linux-gnu»)?
2. Czy wywołanie kompilatora z opcją «-Og» generuje inny kod wykonywalny niż «-Og -g»?
3. Z jakich bibliotek współdzielonych korzysta interpreter języka «Python» (plik «/usr/bin/python»)?

Zaprezentuj w jaki sposób można dojść do odpowiedzi korzystając z podanych poleceń.
:::

Podpunkt 1.:
1. Wejdź do katalogu «/usr/lib/x86_64-linux-gnu».
2. Wpisz ```ar t libc.a | wc -l```. Wynik: ```1690```
3. Wpisz ```cat libm.a```, aby wyświetlić ścieżki (u mnie ~/libm-2.27.a, ~/libmvec.a).
Następnie ```ar t libm-2.27.a | wc -l```; wynik: ```794```.
Potem ```ar t libmvec.a | wc -l```; wynik: ```129```.
Razem: 794 + 129 = 923.

Podpunkt 2.:
Flaga -Og wyłącza pewne optymalizacje.
Flaga -g oznacza włączenie standardowych informacji dla debuggera (można to sprawdzić szukając sekcji .debug_info).


Podpunkt 3.:
Wejdź do /usr/bin. Następnie:
```readelf -d python2.7```
```readelf -d python2.7 | grep "NEEDED"```
![](https://i.imgur.com/1rZggZl.png)



### Zadanie 4

:::info
Rozważmy program składający się z dwóch plików źródłowych:
```cpp=
/* str-a.c */
#include <stdio.h>

char *somestr(void);

int main(void) {
    char *s = somestr();
    s[5] = ’\0’;
    puts(s);
    return 0;
}
```
```cpp=
/* str-b.c */
char *somestr(void) {
    return "Hello, world!";
}
```
Po uruchomieniu program kończy się z błędem dostępu do pamięci. Dlaczego? Gdzie została umieszczona stała znakowa "Hello, world!"? Popraw ten program tak, by się poprawnie zakończył. Gdzie został umieszczony ciąg znaków po poprawce? Nie wolno modyfikować sygnatury procedury «somestr» i pliku «str-a.c», ani korzystać z dodatkowych procedur.
:::

Po uruchomieniu program kończy się z błędem dostępu do pamięci. Dlaczego?

Dzieje się tak ponieważ próbujemy wprowadzić zmiany do read-only
W standardzie C jest napisane,że ciągi znaków które nie są wrzucone na stos lub sterte trafiają zawsze do read-only data.

Co możemy zrobić, żeby temu zapobiec? 

1. Możemy wrzucić "Hello, world!" na stos.
```c=
char str[] = "Hello, world!";
```
tylko wtedy pojawi się problem, ponieważ str będzie lokalną wartością
więc po zakończeniu funkcji zostanie zdjęta ze stosu i nie będzie istnieć poza funkcją. - czyli to zmieni jeden problem na inny

2. Możemy użyć malloc żeby wrzucić "Hello, world!" na sterte
```c=
char *str = (char*)malloc(sizeof(char)*14);
```
i po kolei wrzucać literkę po literce, albo skopiować używajac strcpy.
Problem z tym rozwiązaniem jest taki, że wymaga użycia
stdlib.h 

3. Możemy zadeklarować "Hello, world!" jako static

```c=
static char str[] = "Hello, world!";
```
to rozwiązanie spowoduje, że ciąg znaków "Hello, world!" zostanie umieszczony w sekcji .data dzięki czemu zwrócony wskaźnik będzie wskazywał na ciąg znaków istniejący cały czas w trakcie działania naszego programu
(tak samo jak domyślnie przy return "Hello, world!")
i będzie można zmieniać ten ciąg znaków bo nie będzie już read-only.

Jedyne co może być kłopotliwe w tym rozwiązaniu, to fakt, że przy każdym wywołaniu funkcji dostaniemy wskaźnik do tego samego ciągu znaków w .data
a więc jeśli go zmienimy gdzieś poza funkcją somestr(void) to przy kolejnym wywołaniu tej funkcji dostaniemy wskaźnik na ten sam ciąg, który został zmieniony.

```
int main(void) {
    char *s = somestr();
    s[5] = ’\0’;
    puts(s);
    s = somestr();
    puts(s); 
    return 0;
}
```
a więc ten kod wyprodukuje w konsoli:
Hello
Hello


### Zadanie 5

:::info
Posiłkując się narzędziem «objdump» podaj rozmiary sekcji «.data» i «.bss» plików «bar.o» i «foo.o». Wskaż rozmiar i pozycje symboli względem początków odpowiednich sekcji. Wyjaśnij znaczenie opcji «-fno-common» przekazywanej do kompilatora.
```cpp=
/* bar.c */
int bar = 42;
short dead[15];
```
```cpp=
/* foo.c */
long foo = 19;
char code[17];
```
Na czym polega częściowa konsolidacja z użyciem opcji «-r» do polecenia «ld»? Czym różni się sposób wygenerowania plików «merge-1.o» i «merge-2.o»? Na podstawie mapy konsolidacji porównaj pozycje symboli i rozmiary sekcji w plikach wynikowych. Z czego wynikają różnice skoro konsolidator nie dysponuje informacjami o typach języka C?
:::

bar.o
```
 [Nr] Name
       Type              Address          Offset            Link
       Size              EntSize          Info              Align
       Flags

  [ 2] .data
       PROGBITS         0000000000000000  0000000000000040  0
       0000000000000004 0000000000000000  0                 4
       [0000000000000003]: WRITE, ALLOC
```
O rozmiarze sekcji informuje nas parametr Size.

|Plik |Rozmiar «.data»(B)|Rozmiar «.bss»(B)|
|:---:|:----------------:|:---------------:|
|bar.o|       0x4        |      0x1e       |
|foo.o|       0x8        |      0x11       |

symtab dla bar.o
```
Symbol table '.symtab' contains 7 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 0000000000000000     0 SECTION LOCAL  DEFAULT    1 
     2: 0000000000000000     0 SECTION LOCAL  DEFAULT    2 
     3: 0000000000000000     0 SECTION LOCAL  DEFAULT    3 
     4: 0000000000000000     0 SECTION LOCAL  DEFAULT    4 
     5: 0000000000000000    30 OBJECT  GLOBAL DEFAULT    3 dead
     6: 0000000000000000     4 OBJECT  GLOBAL DEFAULT    2 bar
```
|Symbol| Offset| Sekcja |
|:----:|:-----:|:------:|
| dead |  0x0  | «.bss» |
|  bar |  0x0  |«.data» |

symtab dla foo.o
```
Symbol table '.symtab' contains 7 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 0000000000000000     0 SECTION LOCAL  DEFAULT    1 
     2: 0000000000000000     0 SECTION LOCAL  DEFAULT    2 
     3: 0000000000000000     0 SECTION LOCAL  DEFAULT    3 
     4: 0000000000000000     0 SECTION LOCAL  DEFAULT    4 
     5: 0000000000000000    17 OBJECT  GLOBAL DEFAULT    3 code
     6: 0000000000000000     8 OBJECT  GLOBAL DEFAULT    2 foo
```
|Symbol| Offset| Sekcja |
|:----:|:-----:|:------:|
| dead |  0x0  | «.bss» |
|  bar |  0x0  |«.data» |


Opcja [-fno-common](https://gcc.gnu.org/onlinedocs/gcc/Code-Gen-Options.html#Code-Gen-Options) (domyślna) umieszcza niezainicjalizowane zmienne globalne w sekcji «.bss».

Opcja [-r](https://linux.die.net/man/1/ld) dla ld oznacza, że konsolidator ma wygenerować plik relokowalny. 

:::info
Czym różni się sposób wygenerowania plików «merge-1.o» i «merge-2.o»?
:::

merge-1.o generujemy konsolidując częściowo foo.o z bar.o, a merge-2.o bar.o z foo.o.

|   Plik  |Rozmiar «.data»(B)|Rozmiar «.bss»(B)|
|:-------:|:----------------:|:---------------:|
|merge-1.o|       0xc        |      0x3e       |
|merge-2.o|      0x10        |      0x31       |

Na podstawie map:
merge-1.map:
```
.data           0x0000000000000000        0xc
 *(.data)
 .data          0x0000000000000000        0x8 foo.o
                0x0000000000000000                foo
 .data          0x0000000000000008        0x4 bar.o
                0x0000000000000008                bar

.data1
 *(.data1)

.bss            0x0000000000000000       0x3e
 *(.bss)
 .bss           0x0000000000000000       0x11 foo.o
                0x0000000000000000                code
 *fill*         0x0000000000000011        0xf 
 .bss           0x0000000000000020       0x1e bar.o
                0x0000000000000020                dead
```
merge-2.map
```
.data           0x0000000000000000       0x10
 *(.data)
 .data          0x0000000000000000        0x4 bar.o
                0x0000000000000000                bar
 *fill*         0x0000000000000004        0x4 
 .data          0x0000000000000008        0x8 foo.o
                0x0000000000000008                foo

.data1
 *(.data1)

.bss            0x0000000000000000       0x31
 *(.bss)
 .bss           0x0000000000000000       0x1e bar.o
                0x0000000000000000                dead
 *fill*         0x000000000000001e       0x2 
 .bss           0x0000000000000020       0x11 foo.o
                0x0000000000000020                code
```

Wynika to z tego, że sekcja może wymagać wyrównania.
Z użyciem ```readelf -t merge-1.o``` lub ```readelf -t merge-2.o``` możemy dla każej sekcji wyczytać jej wartość dla parametru Align (dostosowany jest do przechowywanych zmiennych).
W tym przypadku dla ```«.data»``` jest to 8, a dla ```«.bss»``` to 16. O dodanym paddingu informuje nas ```*fill*```.



### Zadanie 6

:::info
Plik wykonywalny powstały w wyniku kompilacji poniższych plików źródłowych powinien być nie dłuższy niż 1KiB. Na podstawie nagłówka pliku ELF wskaż w zdeasemblowanym pierwszą instrukcję, którą wykona procesor po wejściu do programu. Na podstawie nagłówków programu wskaż pod jaki adres wirtualny zostanie załadowany segment z sekcją «.text».
```cpp=
/* start.c */
int is_even(long);

void _start(void) {
    asm volatile(
        "syscall"
            : /* no output */
            : "a" (0x3c /* exit */),
        "D" (is_even(42)));
}
```
```cpp=
/* even.c */
int is_odd(long n);

int is_even(long n) 
    if (n == 0)
        return 1;
    else
        return is_odd(n - 1);
}
```
```cpp=
/* odd.c */
int is_even(long n);

int is_odd(long n) {
    if (n == 0)
        return 0;
    else
        return is_even(n - 1);
}
```
Wskaż w kodzie źródłowym miejsca występowania relokacji. Zidentyfikuj je w wydruku polecenia «objdump -r -d», po czym pokaż jak zostały wypełnione w pliku wykonywalnym. Na podstawie rozdziału §7.7 podręcznika „Computer Systems: A Programmer’s Perspective” zreferuj algorytm relokacji. Wydrukuj tabele relokacji plików relokowalnych, fragment mapy konsolidacji opisujący złączoną sekcję «.text», po czym oblicz ręcznie wartości, które należy wpisać w miejsce relokacji.
:::


**Plik wykonywalny** - zawiera kod i dane w formie pozwalającej na bezpośrednie skopiowanie do pamięci i uruchomienie
**Nagłowek pliku ELF** - zawiera podstawowe informacje dot. pliku takie jak: sposób zapisu danych (np. U2, little endian), typ pliku (np. relokowalny), entry point address itd. 
(polecenie: readelf -h nazwa_pliku)
Przykładowo w nagłówku pliku *main* mamy entry point address = 0x40009B, czyli pierwszą wykonaną instrukcją będzie sub $0x8,%rsp
**Segment** - część pliku zawierająca kod wykonywalny.
**Nagłówek programu** - część pliku ELF, która dla danego segmentu przechowuje informacje takie jak np. offset od początku pliku, adres wirtualny, rozmiar itd.
Przykładowo dla pliku *main* mamy adres wirtualny = 0x400000 dla segmentu z sekcją .text
(polecenie: readelf -l nazwa_pliku)
**Relokacja** - scalanie kilku plików relokowalnych w jeden; następuje przy tym scalanie ze sobą odpowiednich sekcji oraz odpowiednie dopasowywanie adresów pamięci (np. przy instrukcjach call)
W naszym kodzie źródłowym obliczanie odpowiednich adresów nastąpi gdy wywołujemy: 
1. w funkcji _start - funkcję is_even
2. w funkcji is_even - funkcję is_odd
3. w funkcji is_odd - funkcję is_even

W zdeasemblowanym pliku *main* (objdump -d -r main) wygląda to tak:
```=

main:     file format elf64-x86-64

Disassembly of section .text:

0000000000400078 <is_even>:
  400078:	48 85 ff             	test   %rdi,%rdi
  40007b:	74 08                	je     400085 <is_even+0xd>
  40007d:	48 ff cf             	dec    %rdi
  400080:	e9 06 00 00 00       	jmpq   40008b <is_odd> # relokacja
  400085:	b8 01 00 00 00       	mov    $0x1,%eax
  40008a:	c3                   	retq   

000000000040008b <is_odd>:
  40008b:	48 85 ff             	test   %rdi,%rdi
  40008e:	74 08                	je     400098 <is_odd+0xd>
  400090:	48 ff cf             	dec    %rdi
  400093:	e9 e0 ff ff ff       	jmpq   400078 <is_even> # relokacja
  400098:	31 c0                	xor    %eax,%eax
  40009a:	c3                   	retq   

000000000040009b <_start>:
  40009b:	48 83 ec 08          	sub    $0x8,%rsp
  40009f:	bf 2a 00 00 00       	mov    $0x2a,%edi
  4000a4:	e8 cf ff ff ff       	callq  400078 <is_even> # relokacja
  4000a9:	89 c7                	mov    %eax,%edi
  4000ab:	b8 3c 00 00 00       	mov    $0x3c,%eax
  4000b0:	0f 05                	syscall 
  4000b2:	58                   	pop    %rax
  4000b3:	c3                   	retq   
  ```
W plikach *odd.o*, *even.o*, *start.o* mieliśmy odwołania do funkcji, których adresu jeszcze nie znaliśmy. W takiej sytuacji zostały utworzone relocation entries, na przykład:
```=
even.o:     file format elf64-x86-64

Disassembly of section .text:

0000000000000000 <is_even>:
   0:   48 85 ff                test   %rdi,%rdi
   3:   74 08                   je     d <is_even+0xd>
   5:   48 ff cf                dec    %rdi
   8:   e9 00 00 00 00          jmpq   d <is_even+0xd>
                        9: R_X86_64_PC32        is_odd-0x4
   d:   b8 01 00 00 00          mov    $0x1,%eax
  12:   c3                      retq
```
W 8 bajcie funkcji is_even mamy instrukcję e9 00 00 00 00 00, przy czym te 8 zer to adres który w przyszłości (w pliku main) zostanie odpowiednio zmieniony. Relocation entry jest tu postaci 9: R_X86_64_PC32 is_odd-0x4, gdzie:
* 9 - offset od początku funkcji is_even
* R_X86_64_PC32 - typ relokacji
* is_odd-0x4 - symbol do którego relocation entry się odnosi 

Gdy w pliku *main* obliczamy odpowiednie adresy, postępujemy wg algorytmu (zależy on od typu relokacji, tutaj dla każdego relocation entry mamy R_X86_64_PC32, więc poniżej znajduje się algorytm dla tego przypadku):

----------
w każdej sekcji s dla każdego relocation entry r:
    1. refptr = s + r.offset (wskaźnik na adres który powinniśmy zmienić)
    2. refadr = ADRES(s) + r.offset
    3. *refptr = (unsigned) (ADRES(r.symbol) + *refptr - refadr)
    
(gdy jednak typ relocation entry to R_X86_32):
    1. refptr = s + r.offset
    2. *refptr = (unsigned) (ADRES(r.symbol) - *refptr)

----------
W naszym przypadku do dyspozycji mamy następujące dane:
* relocation entries (polecenie readelf -r file_name.o):
```=
is_even.o
Relocation section '.rela.text' at offset 0xf8 contains 1 entry:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
000000000009  000500000002 R_X86_64_PC32     0000000000000000 is_odd - 4
------------------------------------
is_odd.o:

Relocation section '.rela.text' at offset 0xf0 contains 1 entry:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
000000000009  000500000002 R_X86_64_PC32     0000000000000000 is_even - 4
------------------------------------
start.o:

Relocation section '.rela.text' at offset 0x100 contains 1 entry:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
00000000000a  000500000002 R_X86_64_PC32     0000000000000000 is_even - 4
```
* fragment z pliku main.map (mamy tutaj adresy poszczególnych sekcji):
```=
.text           0x0000000000400078       0x3c
 *(.text.unlikely .text.*_unlikely .text.unlikely.*)
 *(.text.exit .text.exit.*)
 *(.text.startup .text.startup.*)
 *(.text.hot .text.hot.*)
 *(.text .stub .text.* .gnu.linkonce.t.*)
 .text          0x0000000000400078       0x13 even.o
                0x0000000000400078                is_even
 .text          0x000000000040008b       0x10 odd.o
                0x000000000040008b                is_odd
 .text          0x000000000040009b       0x19 start.o
                0x000000000040009b                _start
```
Mamy więc do obliczenia 3 adresy, jakie powiny zostać zmienione po relokacji:
1. w funkcji is_even, wywołanie is_odd:
r.offset = 0x9
r.symbol = is_odd - 0x4
ADRES(s) = 0x400078
ADRES(r.symbol) = 0x40008B
i teraz zgodnie z algorytmem:
refptr = 0x400078 + 0x9
refadr = 0x400078 + 0x9
*refptr = 0x40008B - 0x4 + 0 - (0x400078 + 0x9) = **0x6**

2. w funkcji is_odd, wywołanie is_even:
r.offset = 0x9
r.symbol = is_even - 0x4
ADRES(s) = 0x40008B
ADRES(r.symbol) = 0x400078
refptr = 0x40008B + 0x9
refadr = 0x40008B + 0x9
*refptr = 0x400078 - 0x4 + 0 - (0x40008B + 0x9) = **0xFFFFFFE0**
3. w funkcji _start, wywołanie is_even:
r.offset = 0xA
r.symbol = is_even - 0x4
ADRES(s) = 0x40009B
ADRES(r.symbol) = 0x400078
refptr = 0x40009B + 0xA 
refadr = 0x40008B + 0xA
*refptr = 0x400078 - 0x4 + 0 - (0x40009B + 0xA) = **0xFFFFFFCF** 


Możemy sprawdzić obliczenia patrząc na zdeasemblowany plik *main* (pamiętając, że tutaj adresy są zapisane w Little Endian):
```=

main:     file format elf64-x86-64

Disassembly of section .text:

0000000000400078 <is_even>:
  400078:	48 85 ff             	test   %rdi,%rdi
  40007b:	74 08                	je     400085 <is_even+0xd>
  40007d:	48 ff cf             	dec    %rdi
  400080:	e9 06 00 00 00       	jmpq   40008b <is_odd>
  400085:	b8 01 00 00 00       	mov    $0x1,%eax
  40008a:	c3                   	retq   

000000000040008b <is_odd>:
  40008b:	48 85 ff             	test   %rdi,%rdi
  40008e:	74 08                	je     400098 <is_odd+0xd>
  400090:	48 ff cf             	dec    %rdi
  400093:	e9 e0 ff ff ff       	jmpq   400078 <is_even>
  400098:	31 c0                	xor    %eax,%eax
  40009a:	c3                   	retq   

000000000040009b <_start>:
  40009b:	48 83 ec 08          	sub    $0x8,%rsp
  40009f:	bf 2a 00 00 00       	mov    $0x2a,%edi
  4000a4:	e8 cf ff ff ff       	callq  400078 <is_even>
  4000a9:	89 c7                	mov    %eax,%edi
  4000ab:	b8 3c 00 00 00       	mov    $0x3c,%eax
  4000b0:	0f 05                	syscall 
  4000b2:	58                   	pop    %rax
  4000b3:	c3                   	retq 
```


### Zadanie 7

:::info
W trakcie tłumaczenia poniższego kodu na asembler kompilator umieścił tablicę skoków dla instrukcji przełączania w sekcji «.rodata».
```cpp=
int relo3(int val) {
    switch (val) {
        case 100:
            return val;
        case 101:
            return val + 1;
        case 103:
        case 104:
            return val + 3;
        case 105:
            return val + 5;
        default:
            return val + 6;
    }
}
```
```
0000000000000000 <relo3>:
0:  8d 47 9c                 lea -0x64(%rdi),%eax
3:  83 f8 05                 cmp $0x5,%eax
6:  77 15                    ja 1d <relo3+0x1d>
8:  89 c0                    mov %eax,%eax
a:  ff 24 c5 00 00 00 00     jmpq *0x0(,%rax,8)
11: 8d 47 01                 lea 0x1(%rdi),%eax
14: c3                       retq
15: 8d 47 03                 lea 0x3(%rdi),%eax
18: c3                       retq
19: 8d 47 05                 lea 0x5(%rdi),%eax
1c: c3                       retq
1d: 8d 47 06                 lea 0x6(%rdi),%eax
20: c3                       retq
21: 89 f8                    mov %edi,%eax
23: c3                       retq
```
Dla sekcji «.text» i «.rodata» określ pod jakimi miejscami znajdują się relokacje, a następnie podaj zawartość tablicy relokacji «.rela.text» i «.rela.rodata», tj. listę rekordów składających się z:
• przesunięcia relokacji względem początku sekcji,
• typu relokacji,
• nazwy symbolu i przesunięcia względem początku symbolu.
W wyniku konsolidacji pliku wykonywalnego zawierającego procedurę «relo3», została ona umieszczona pod adresem 0x1000, a tablica skoków pod 0x2000. Oblicz wartości, które należy wstawić w miejsca relokacji.
:::

### Zadanie 8

:::info
Na podstawie rozdziału §7.12 podręcznika „Computer Systems: A Programmer’s Perspective” opisz proces leniwego wiązania (ang. lazy binding) symboli i odpowiedz na następujące pytania:
• Czym charakteryzuje się kod relokowalny (ang. Position Independent Code)?
• Do czego służą sekcje «.plt» i «.got» – jakie dane są tam przechowywane?
• Czemu sekcja «.got» jest modyfikowalna, a sekcje kodu i «.plt» są tylko do odczytu?
• Co znajduje się w sekcji «.dynamic»?
Zaprezentuj leniwe wiązanie na podstawie programu «lazy». Uruchom go pod kontrolą debuggera GDB, ustaw punkty wstrzymań (ang. breakpoint) na linię 4 i 5. Po czym wykonując krokowo program (stepi) pokaż, że gdy procesor skacze do adresu procedury «puts» zapisanego w «.got» – za pierwszym wywołaniem jest tam przechowywany inny adres niż za drugim.
Wskazówka: Wykorzystaj rysunek 7.19. Dobrze jest zainstalować sobie GDB dashboard.
:::

