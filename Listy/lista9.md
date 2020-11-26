# Ćwiczenia 9 z ASK, grupa AKr (14.05.2020)


### Zadanie 6, lista 8

:::info
**Plik wykonywalny** powstały w wyniku kompilacji poniższych plików źródłowych powinien być nie dłuższy niż 1KiB. Na podstawie **nagłówka pliku ELF** wskaż w zdeasemblowanym pierwszą instrukcję, którą wykona procesor po wejściu do programu. Na podstawie **nagłówków programu** wskaż pod jaki adres wirtualny zostanie załadowany segment z sekcją «.text».
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
Zapoznaj się ze **skryptem konsolidatora** w pliku «main.lds». Na podstawie dokumentacji wyjaśnij jak skrypt kieruje procesem konsolidacji poszczególnych sekcji i tworzeniem nagłówków programu.
:::

**plik wykonywalny** - zawiera kod i dane w formacie pozwalającym na bezpośrednie skopiowanie do pamięci i uruchomienie
**nagłówek pliku ELF** - zawiera podstawowe informacje dot. pliku takie jak: sposób zapisu danych (np. U2, little endian), typ pliku (np. relokowalny), entry point address itd.; sprawdzamy poleceniem ```readelf -h nazwa```

```readelf -h main```
```objdump -r -d main```
pierwsza instrukcja:  sub    $0x8,%rsp

**nagłówek programu** - część pliku ELF, która dla danego segmentu przechowuje informacje takie jak np. offset od początku pliku, adres wirtualny, rozmiar itd.; sprawdzamy poleceniem ```readelf -l nazwa```

```readelf -l main```
0x00000000004000e8

**Konsolidator** jest programem mającym za zadanie łączenie kilku plików obiektowych, realokacje zmiennych oraz powiązanie ze sobą referencji symboli w jednym pliku obiektowym.
**skrypt konsolidatora** - plik skryptu konsolidatora jest zwykłym plikiem tekstowy w którym znajduje się zestaw komend. Każda komenda jest słowem kluczowym po którym może nastąpić argument, lub wartość której można przypisać do zmiennej.

«main.lds»
```C=
UTPUT_FORMAT("elf64-x86-64")   //nazywa format pliku wyjsciowego
OUTPUT_ARCH(i386:x86-64)
ENTRY(_start)                  //ustawia punkt wejscia;
PHDRS                          //zarządza ustawieniem nagłówków
{
  code   PT_LOAD FLAGS(5);

  rodata PT_LOAD FLAGS(4);
  data   PT_LOAD FLAGS(6);
}
SECTIONS
{
  . = 0x400000 + SIZEOF_HEADERS; /* sets the value of the special symbol ‘.’,
  which is the location counter (licznik lokacji). If you do not specify
  the address of an output section in some other way, the address is set
  from the current value of the location counter. The location counter
  is then incremented by the size of the output section. At the start of
  the ‘SECTIONS’ command, the location counter has the value ‘0’. */

//The ‘*’ is a wildcard which matches any file name.
//The expression ‘*(.text)’ means all ‘.text’ input
//sections in all input files.

  .text :
  {
    *(.text .text.*)
  } : code                 //output

  .rodata :
  {
    *(.rodata .rodata.*)
  } : rodata

  .data : 
  {
    *(.data .data.*)
  } : data

  .bss :
  {
   *(.bss .bss.*)
   *(COMMON)      //*(COMMON) by itself refers to all uninitialized data from all input files
  } : data

  /DISCARD/ : //The special output section name ‘/DISCARD/’ may be used to discard input sections. Any input sections which are assigned to an output section named ‘/DISCARD/’ are not included in the output file.
  {
    *(.note.gnu.property)
  }
}
```


### Zadanie 7, lista 8

:::info
Na podstawie [1, §7.7.1] opowiedz jakie dane przechowują sekcje «.rel.text» i «.rel.data». Czy możliwe jest by asembler utworzył sekcję «.rel.bss»? Czym się różnią relokacje typu «R_X86_64_PC32» od «R_X86_64_32»? Na podstawie [1, §7.7.2] podręcznika zreferuj algorytm relokacji.
:::


.rel.text - wpisy relokacji dla kodu (uzupelnic kod)
.rel.data - wpisy relokacji dla danych (uzupelnienie danych)

.rel.bss nie powinno sie wygenerowac, ale istnieje mozliwosc stworzenia takiej sekcji np. po przez wstawienie kodu ktory odwoluje sie do jakiegos symbolu w sekcji bss (ta sekcja nie powinna trzymac instrukcji, od tego jest .text)
(https://stackoverflow.com/questions/37055896/what-does-an-elf-relocation-in-bss-but-relative-to-bss-mean)


«R_X86_64_PC32» - uzupelniamy adres wzgledem naszego adresu np. +5 albo -5
«R_X86_64_32» - uzupelniamy adres poprzez wstawienie bezwzglednej wartosci adresu

Algorytm relokacji:
```=c
 foreach section s {
     foreach relocation entry r {
         refptr=s+ r.offset; /* ptr to reference to be relocated */

         /* Relocate a PC-relative reference */
         if (r.type == R_X86_64_PC32) {
         refaddr = ADDR(s) + r.offset; /* ref’s run-time address */
         *refptr = (unsigned) (ADDR(r.symbol) + r.addend - refaddr);
         }

         /* Relocate an absolute reference */
         if (r.type == R_X86_64_32)
         *refptr = (unsigned) (ADDR(r.symbol) + r.addend);
     }
}
```

przechodzimy kolejno po kazdej sekcji i dla kazdego wpisu relokacji wyliczamy wartosc adresu na podstawie typu relokacji.
Powyzszy algorytm uwzglednia jedynie dwa najczestsze typy («R_X86_64_PC32» i «R_X86_64_32»)
W pierwszym przypadku wyliczany zostaje adres wzgledem adresu sekcji i miejsca w ktorym jest wpis relokacji w tej sekcji
W drugim nie obchodzi nas adres wzgledem obecnej instrukcji wiec ten krok jest pominiety

### Zadanie 8, lista 8

:::info
Na podstawie wydruku polecenia «objdump -r -d» wskaż w kodzie źródłowym z zadania 6 miejsca występowania relokacji. Następnie pokaż jak relokacje zostały wypełnione w pliku wykonywalnym. Wydrukuj tabele relokacji poszczególnych plików konsolidowalnych, a także fragment mapy konsolidacji opisujący złączoną sekcję «.text». Następnie oblicz wartości, które należy wpisać w miejsce relokacji, zgodnie z algorytmem z poprzedniego zadania.
:::


```
$ readelf -r --relocs start.o

Relocation section '.rela.text' at offset 0x150 contains 1 entry:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
00000000000a  000700000004 R_X86_64_PC32    0000000000000000 is_even - 4

```
objdump ->  a: R_X86_64_PC32	is_even-0x4
```

$ readelf -r --relocs even.o

Relocation section '.rela.text' at offset 0x150 contains 1 entry:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
000000000014  000700000004 R_X86_64_PC32    0000000000000000 is_odd - 4
```
objdump ->  14: R_X86_64_PC32	is_odd-0x4

```
$ readelf -r --relocs odd.o

Relocation section '.rela.text' at offset 0x150 contains 1 entry:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
000000000014  000700000004 R_X86_64_PC32    0000000000000000 is_even - 4

```
objdump -> 14: R_X86_64_PC32	is_even-0x4


fragment mapy konsolidacji:

```
.text           0x00000000004000e8       0x56
 *(.text .text.*)
 .text          0x00000000004000e8       0x1d even.o
                0x00000000004000e8                is_even
 .text          0x0000000000400105       0x1d odd.o
                0x0000000000400105                is_odd
 .text          0x0000000000400122       0x1c start.o
                0x0000000000400122                _start
```

I w koncu relokacje uzupelnione przez konsolidator (widac przy instrukcjach callq) :
```
00000000004000e8 <.text>:
  4000e8:	48 85 ff             	test   %rdi,%rdi
  4000eb:	75 06                	jne    0x4000f3
  4000ed:	b8 01 00 00 00       	mov    $0x1,%eax
  4000f2:	c3                   	retq   
  4000f3:	48 83 ec 08          	sub    $0x8,%rsp
  4000f7:	48 83 ef 01          	sub    $0x1,%rdi
  4000fb:	e8 05 00 00 00       	callq  0x400105
  400100:	48 83 c4 08          	add    $0x8,%rsp
  400104:	c3                   	retq   
  400105:	48 85 ff             	test   %rdi,%rdi
  400108:	75 06                	jne    0x400110
  40010a:	b8 00 00 00 00       	mov    $0x0,%eax
  40010f:	c3                   	retq   
  400110:	48 83 ec 08          	sub    $0x8,%rsp
  400114:	48 83 ef 01          	sub    $0x1,%rdi
  400118:	e8 cb ff ff ff       	callq  0x4000e8
  40011d:	48 83 c4 08          	add    $0x8,%rsp
  400121:	c3                   	retq   
  400122:	48 83 ec 08          	sub    $0x8,%rsp
  400126:	bf 2a 00 00 00       	mov    $0x2a,%edi
  40012b:	e8 b8 ff ff ff       	callq  0x4000e8
  400130:	89 c7                	mov    %eax,%edi
  400132:	b8 3c 00 00 00       	mov    $0x3c,%eax
  400137:	0f 05                	syscall 
  400139:	48 83 c4 08          	add    $0x8,%rsp
  40013d:	c3                   	retq   
```



Obliczamy odpowiednie wartosci:

ADDR(.text) = 0x4000e8
ADDR(is_even) = 0x4000e8
ADDR(is_odd) = 0x400105
ADDR(_start) = 0x400122


Dla wpisu relokacji w start.o:
refaddr = ADDR(.text) + r.offset = 0x4000e8 + a = 0x4000f2
*refptr = (unsigned) (ADDR(is_even) + (-4) - refaddr) = (unsigned) (-e)

Dla wpisu relokacji w even.o:
refaddr = ADDR(.text) + r.offset = 0x4000e8 + 0x14 = 0x4000fc
*refptr = (unsigned) (ADDR(is_odd) + (-4) - refaddr) = 0x400105 - 0x4 - 0x4000fc = 5

Dla wpisu relokacji w odd.o:
refaddr = ADDR(.text) + r.offset =0x4000e8 + 0x14 = 0x4000fc
*refptr = (unsigned) (ADDR(is_even) + (-4) - refaddr) = 0x4000e8 - 0x4 - 0x4000fc = (unsigned) (-18)

### Zadanie 1

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


*readelf -r relo3.o* daje nam następujące informacje:
```=

Relocation section '.rela.text' at offset 0x130 contains 1 entry:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
00000000000d  00020000000b R_X86_64_32S      0000000000000000 .rodata + 0

Relocation section '.rela.rodata' at offset 0x148 contains 6 entries:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
000000000000  000100000001 R_X86_64_64       0000000000000000 .text + 24
000000000008  000100000001 R_X86_64_64       0000000000000000 .text + 1f
000000000010  000100000001 R_X86_64_64       0000000000000000 .text + 1b
000000000018  000100000001 R_X86_64_64       0000000000000000 .text + 11
000000000020  000100000001 R_X86_64_64       0000000000000000 .text + 11
000000000028  000100000001 R_X86_64_64       0000000000000000 .text + 15
```
Powyższy wydruk daje nam w zasadzie wystarczające informacje do obliczenia wartości relokacji, ale dla jasności można jeszcze spojrzeć na wydruk *objdump -r -D relo3.o*:

```=
relo3.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <relo3>:
   0:	8d 57 9c             	lea    -0x64(%rdi),%edx
   3:	89 f8                	mov    %edi,%eax
   5:	83 fa 05             	cmp    $0x5,%edx
   8:	77 11                	ja     1b <relo3+0x1b>
   a:	ff 24 d5 00 00 00 00 	jmpq   *0x0(,%rdx,8)
			d: R_X86_64_32S	.rodata
  11:	83 c0 03             	add    $0x3,%eax
  14:	c3                   	retq   
  15:	b8 6e 00 00 00       	mov    $0x6e,%eax
  1a:	c3                   	retq   
  1b:	83 c0 06             	add    $0x6,%eax
  1e:	c3                   	retq   
  1f:	b8 66 00 00 00       	mov    $0x66,%eax
  24:	c3                   	retq   

Disassembly of section .rodata:

0000000000000000 <.rodata>:
	...
			0:  R_X86_64_64        .text+0x24
			8:  R_X86_64_64        .text+0x1f
			10: R_X86_64_64	.text+0x1b
			18: R_X86_64_64	.text+0x11
			20: R_X86_64_64	.text+0x11
			28: R_X86_64_64	.text+0x15

```

Tak więc relokacje znajdują się:
* 1 w sekcji .text (w linii ```a:	ff 24 d5 00 00 00 00 	jmpq   *0x0(,%rdx,8)```)
* 6 w sekcji .rodata (nasza tablica skoków)

Procedura relo3 została umieszczona pod adresem 0x1000, czyli adres sekcji .text z relo3.o też ma adres 0x1000. Adres tablicy skoków (czyli również początku sekcji .rodata z relo3.o) wynosi 0x2000.
Najpierw obliczmy adres z sekcji .text:
1. refptr = 0x1000 + 0xD; *refptr = (unsigned) (0x2000)

Teraz adresy z sekcji .rodata:

1. refptr = 0x2000 + 0x0 = 0x2000; *refptr = (un.) (0x1000 + 24 + 0) = 0x1024
2. refptr = 0x2000 + 0x8 = 0x2008; *refptr = (un.) (0x1000 + 1F + 0) = 0x101F
3.  refptr = 0x2000 + 0x10 = 0x2010; *refptr = (un.) (0x1000 + 1B + 0) = 0x101B
4.  refptr = 0x2000 + 0x18 = 0x2018; *refptr = (un.) (0x1000 + 11 + 0) = 0x1011
5.  refptr = 0x2000 + 0x20 = 0x2020; *refptr = (un.) (0x1000 + 11 + 0) = 0x1011
6.  refptr = 0x2000 + 0x28 = 0x2028; *refptr = (un.) (0x1000 + 15 + 0) = 0x1015

### Zadanie 2

:::info
Język C++ pozwala na przeciążanie funkcji (ang. function overloading), tj. dopuszcza stosowanie wielu funkcji o tej samej nazwie, ale różnej sygnaturze. Wiemy, że konsolidator nie przechowuje w tabeli symboli informacji o typach z języka programowania. Powstaje zatem problem unikatowej reprezentacji nazw używanych przez język C++. Wytłumacz na czym polega dekorowanie nazw (ang. name mangling)? Które elementy składni podlegają dekorowaniu? Przy pomocy narzędzia ``c++filt`` przekształć poniższe nazwy na sygnatury funkcji języka C++ i omów znaczenie poszczególnych fragmentów symbolu. Czy funkcja dekorująca nazwy jest różnowartościowa?
1. _Z4funcPKcRi
2. _ZN3Bar3bazEPc
3. _ ZN3BarC1ERKS_
4. _ZN3foo6strlenER6string

Wskazówka: Szczegółowe informacje na temat kodowania nazw można znaleźć w C++ ABI: External Names.
:::

Dekorowanie nazw – technika stosowana przez kompilatory współczesnych języków programowania w celu wygenerowania unikatowych (czyli funkcja ta jest różnowartościowa) nazw funkcji, struktur, klas oraz innych typów danych. 

>> c++filt _Z4funcPKcRi
func(char const*, int&)

>> c++filt _ZN3Bar3bazEPc
Bar::baz(char*)

>> c++filt _ ZN3BarC1ERKS_
Bar::Bar(Bar const&)

>> c++filt _ZN3foo6strlenER6string
foo::strlen(string&)

_Z - początek dekoracji
N ... E - <nested-name> - rekurencyjnie rozkłada obejmujący zasięg, aż do osiągnięcia globalnego zasięgu.

K - const
R - &
C1 - konstruktor
P - wskaźnik
c - char
i - int
S_ - pierwsza nazwa typu (w przypadku powyżej Bar)
cyfry stojące przed nazwami - liczba liter nazwy


### Zadanie 3

:::info
Podglądając wyjście z kompilatora języka C pokaż jak korzystając z dyrektyw asemblera opisanych w GNU as: Assembler Directives:
• zdefiniować globalną funkcję foobar,
• zdefiniować lokalną strukturę podaną niżej:
```cpp=
static const struct {
    char a[3]; 
    int b; 
    long c; 
    float pi;
} baz = { "abc", 42, -3, 1.4142 };
```
• zarezerwować miejsce dla tablicy long array[100].

Wyjaśnij znaczenie poszczególnych dyrektyw. Pamiętaj, że dla każdego zdefiniowanego symbolu należy uzupełnić odpowiednio tabelę «.symtab» o typ symbolu i rozmiar danych, do których odnosi się symbol.
:::


Spróbujemy wszystkie te czynności wykonać w pliku `task3.c`, a następnie poleceniem `gcc -S task3.c` wygenerować plik *.s i przeanalizować go używając dostępnych środków (link w treści zadania na skos).

**task3.c**

```c
void foobar () { }

static const struct {
    char a[3]; 
    int b; 
    long c; 
    float pi;
} baz = { "abc", 42, -3, 1.4142 };

long array[100];

```

**task3.s** + opisy dotyczące dyrektyw (posiłkowane [GNU as: Assembler Directives](https://sourceware.org/binutils/docs-2.26/as/Pseudo-Ops.html) i internetem, do którego już nie mam linka)
```asm
        .file	"task3.c" # informacja z jakiego pliku
	.text	# dalsze dane będą w sekcji “text” pliku wykonywalnego ELF.
	# Informuje „as”, aby zebrać następujące instrukcje na końcu podrozdziału numerowanego, który jest wyrażeniem absolutnym. Jeśli podsekcja zostanie pominięta, użyty zostanie numer zero.
	.globl	foobar # foobar ma być dostępny dla konsolidatoróœ (globalny)
	# .global makes the symbol visible to ld.
	.type	foobar, @function # ustawia typ symbolu foobar - funkcja
foobar:   # CFI - Call Frame Information
.LFB0:    # CFA - Canonical Frame Address
	.cfi_startproc # początek obszaru związanego z ramką wywołania funkcji
	# Jest używany na początku każdej funkcji, w której powinien znajdować się wpis .eh_frame. Inicjuje niektóre wewnętrzne struktury danych.
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16  # offset wzgl. rejestru wierzchu stosu
	# .cfi_offset rejestr, przesunięcia
	# Poprzednia wartość rejestru jest zapisywana z przesunięciem przesunięcia względem CFA.
	# Dyrektywa .cfi_def_cfa_offset 16 informuje, że ramka stosu jest przesunięta 16 bajtów. 
	#
	#       | Zawartość
	#-------+----------------------------
	# A     | część z danymi wywołującego
	# A+8 	| adres powrotny
	# A+16 	| dane foobar
	.cfi_offset 6, -16 # adres wierzchu stosu wywołującego będzie w rejestrze 6 
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6 # z tego co kojarzę wierzch stosu
	# cfi_def_cfa_register modyfikuje regułę obliczania CFA. Od teraz rejestr 6 będzie używany zamiast starego. Przesunięcie pozostaje takie samo.
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8 
	# .cfi_def_cfa rejestr, przesunięcia
	# .cfi_def_cfadefiniuje regułę obliczania CFA jako: pobierz adres z rejestru i dodaj do niego przesunięcie .
	ret
	.cfi_endproc # koniec obszaru
.LFE0:
	.size	foobar, .-foobar # ustawienie rozmiaru związanego z foobar
# rozmiar nie jest podany wiec zostanie obliczony później
	.section	.rodata # dyrektywa mówi, aby złożyć następujący kod w sekcję o nazwie .rodata 
	.align 16   # .align 16 przesuwa licznik lokalizacji, aż będzie wielokrotnością liczby 16. Jeśli już był, to nie trzeba nic robić.
	# adresy podzielne przez 16
    .type	baz, @object # typ baz - object | utworzony symbol
	.size	baz, 24  # rozmiar 24 bajty
baz:
	.ascii	"abc" # spodziewa się 0 lub więcej literałów po przecinku
	# Składa każdy ciąg (bez automatycznego końcowego bajtu zerowego) w kolejne adresy. 
	.zero	1 # alias dla .skip 
	# wypełnia zerem 1 bajt
	.long	42 # 32 bitowe słowo
	.quad	-3 # 64 bitowe słowo
	.long	1068827777 # 32-bitowe słowo
	.zero	4 # 4 bajty wypełnione zerami

	.comm	array,800,32 # deklaruje symbol array 0 wielkości 800 = 100 elementów o rozmiarze 8 bajtów
	# dyrektywa przyjmuje opcjonalny trzeci argument. Jest to pożądane wyrównanie symbolu określonego dla ELF jako granicy bajtów (na przykład wyrównanie 16 oznacza, że ​​najmniej znaczące 4 bity adresu powinny wynosić zero), a dla PE jako potęgi dwóch (na przykład wyrównanie 5 oznacza wyrównanie do 32-bajtowej granicy). 
	.ident	"GCC: (Ubuntu 9.3.0-10ubuntu2) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f 
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3 
3:
	.align 8 # wypełnij licznik lokalizacji (w bieżącym podsekcji) do określonej granicy pamięci.
    # .align 8 przesuwa licznik lokalizacji, aż będzie wielokrotnością liczby 8. Jeśli już był, to nie trzeba nic robić.
4:
 
```
generujemy `task3.o` poleceniem `as -o task3.o task3.s`
poprzez wywołanie `objdump -t task3.o` sprawdzamy naszą tablicę symboli.

**Symbol Table**
```
task3.o:     file format elf64-x86-64

SYMBOL TABLE:
0000000000000000 l    df *ABS*  0000000000000000 task3.c
0000000000000000 l    d  .text  0000000000000000 .text
0000000000000000 l    d  .data  0000000000000000 .data
0000000000000000 l    d  .bss   0000000000000000 .bss
0000000000000000 l    d  .rodata        0000000000000000 .rodata
0000000000000000 l     O .rodata        0000000000000018 baz
0000000000000000 l    d  .note.GNU-stack        0000000000000000 .note.GNU-stack
0000000000000000 l    d  .note.gnu.property     0000000000000000 .note.gnu.property
0000000000000000 l    d  .eh_frame      0000000000000000 .eh_frame
0000000000000000 l    d  .comment       0000000000000000 .comment
0000000000000000 g     F .text  000000000000000b foobar
0000000000000320       O *COM*  0000000000000020 array
```


### Zadanie 4

:::info
Na podstawie rozdziału §7.12 podręcznika „Computer Systems: A Programmer’s Perspective” opisz proces leniwego wiązania (ang. lazy binding) symboli i odpowiedz na następujące pytania:
• Czym charakteryzuje się kod relokowalny (ang. Position Independent Code)?
• Do czego służą sekcje «.plt» i «.got» – jakie dane są tam przechowywane?
• Czemu sekcja «.got» jest modyfikowalna, a sekcje kodu i «.plt» są tylko do odczytu?
• Co znajduje się w sekcji «.dynamic»?
Zaprezentuj leniwe wiązanie na podstawie programu «lazy». Uruchom go pod kontrolą debuggera GDB, ustaw punkty wstrzymań (ang. breakpoint) na linię 4 i 5. Po czym wykonując krokowo program (stepi) pokaż, że gdy procesor skacze do adresu procedury «puts» zapisanego w «.got» – za pierwszym wywołaniem jest tam przechowywany inny adres niż za drugim.
Wskazówka: Wykorzystaj rysunek 7.19. Dobrze jest zainstalować sobie GDB dashboard.
:::


**leniwe linkowanie** - czekanie z linkowaniem adresu procedury, aż zostanie zawołana pierwszy raz 


przy uruchamianiu programu, mamy specjalnie przygotowane PLT (Procedure linkage table) oraz GOT (Global offset table) 

PLT zawiera puste wpisy, które powodują
rozwiązanie symbolu i poprawienie wpisu w tablicy 
tak, że kolejne skoki już są pod właściwy adres

GOT zawiera adresy fragmentu kodu w PLT który poprawi dane pole GOT


Zamiast wołać funkcje bilbioteczne, wołamy odpowiedni dla nich adres w PLT, on skacze do lokalizacji podanej w tablicy GOT pod odpowiednim indexem. Początowo, ten adres to kolejna instrukcja danego PLT. Kolejna instrukcja, powoduje przekazanie odpowiedniego argumentu do dynamicznego linera, i wywołanie PLT[0] czyli fragmentu odpowiedzialnego za zawołanie dynamicznego linkera, przekazujemy mu adres tablicy relokacji (wcześniej przekazaliśmy mu index tego co chemy zrelokować) i wywołujemy go, resztą już zajmnie się dynamiczny linker.

W kolejnych wywołaniach GOT będzie już wskazywał na funkcje 

**kod relokowalny** - kod nie zależny od tego w jakim miejscu pamięci jest wykonany, adresy w nim są relatywne do PC 

**.plt** sekcja zawierająca tablice procedur relokacyjnych - opisane wyżej

**.got** globalna tablica offsetów - opisane wyżej

**.dynamic** - informacje dotyczące dynamicznego łączenia

przy pierwszym wykonaniu adres to
 
 bnd jmpq *0x2f75(%rip)        # 0x555555557fd0 <puts@got.plt>
x 0x555555557fd0 -> 0xf7e3d5a0

przy drugim wykonaaniu jest tak samo

