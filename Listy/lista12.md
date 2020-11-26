# Ćwiczenia 13 z ASK, grupa AKr (25.06.2020)


### Zadanie 1

:::info
Sprzętowa **translacja adresów** umożliwiła systemom operacyjnym wydajną implementację **izolacji procesów**, **stronnicowania na żądanie** (ang. demand paging), **wymiany do pamięci drugorzędnej** (ang. swapping) i **współdzielenia pamięci**. Jakie korzyści przynosi stosowanie powyższych mechanizmów? Przekonaj prowadzącego i słuchaczy posługując się przykładami. Należy użyć pojęć: **zbiór rezydentny** (ang. resident set) i **zbiór roboczy** (ang. working set).
:::

**translacja adresów** - proces w ktorym adres wirtualny jest tlumaczony na adres fizyczny.

**izolacja procesow** - metody zapewniajace, ze dany fragment pamieci nalezy do konkretnego procesu i zabraniajace innym procesom robienia do nich dostepow. Jest to bardzo wazny aspekt dla cyberbezpieczenstwa

**stronicowanie na żądanie** - strony sa wciagniete do pamieci dopiero w momencie kiedy sa 'dotkniete'. To znaczy, ze w momencie alokacji pamieci nie wciagamy jeszcze swiezo zaalokowanych stron do pamieci, czekamy z tym do momentu zrobienia dostepu. Jest to bardzo wazne poniewaz pozwala na ograniczenie danych wciagnietych przez proces do pamieci do zbioru roboczego bedacego malym podzbiorem calej pamieci przydzielonej dla programu.

**wymiana do pamieci drugorzednej** - fragmenty pamieci ktore nam sie juz nie przydadza zostaja przeniesione do pamieci drugorzednej, tym samym zwalniajac miejsce dla innych stron.
**wspoldzielenie pamieci** - korzystanie z tego samego bloku pamieci przez wiele roznych programow. Dzieki temu kazdy proces korzystajacy np. z libc nie musi wprowadzac do pamieci swojej instancji tego kodu. Kazdy proces moze korzystac z jednej i tej samej. 


**zbiór rezydentny** - Pamiec przydzielona dla programu.
**zbiór roboczy** - Pamiec z ktorej program faktycznie korzysta. ( podzbior zbioru rezydentnego )



### Zadanie 2

:::info
Zakładamy taki sam model podsystemu pamięci jak na slajdach do wykładu „Virtual Memory: Systems” (strony 9–16). Powtórz proces translacji adresów i adresowania pamięci podręcznej dla adresów: 0x027c, 0x03a9 i 0x0040 zakładając poniższy stan **TLB**, pamięci podręcznej i **tablicy stron**.
Zawartość TLB:
Zbiór | | Tag | PPN | V | | Tag | PPN | V | | Tag | PPN | V | | Tag | PPN | V |
-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
0 | | - | - | 0 | | 09 | 0D | 1 | | - | - | 0 | | 07 | 02 | 1 |
1 | | 03 | 2D | 1 | | - | - | 0 | | 02 | 17 | 1 | | - | - | 0  |
2 | | - | - | 0 | | - | - | 0 | | - | - | 0 | | - | - | 0 |
3 | | - | - | 1 | | 03 | 0D | 1 | | 0A | 34 | 1 | | - | - | 0 |

Zawartość pamięci podręcznej:
Idx | | Tag | V | | B0 | B1 | B2 | B3
-|-|-|-|-|-|-|-|-| 
0 | | 19 | 1 | | 99 | 11 | 23 | 11
1 | | - | 0 | | - | - | - | -
2 | | 1B | 1 | | 00 | 02 | 04 | 08
3 | | - | 0 | | - | - | - | -
4 | | 32 | 1 | | 43 | 6D | 8F | 09
5 | | 0D | 1 | | 36 | 72 | F0 | 1D
6 | | - | 0 | | - | - | - | -
7 | | 16 | 1 | | 11 | C2 | DF | 03 |
8 | | 24 | 1 | | 3A | 00 | 51 | 89
9 | | - | 0 | | - | - | - | -
A | | 2D | 1 | | 93 | 15 | DA | 3B
B | | - | 0 | | - | - | - | -
C | | - | 0 | | - | - | - | - |
D | | 16 | 1 | | 04 | 96 | 34 | 15
E | | 13 | 1 | | 83 | 77 | 1B | D3
F | | 09 | 1 | | DE | 01 | C4 | 32

Zawartość tablicy stron:
VPN | PPN | V
-|-|-
00 | 28 | 1
01 | - | 0
02 | 33 | 1
03 | 02 | 1
04 | - | 0
05 | 16 | 1
06 | - | 0
07  | - | 0
08 | 13 | 1
09 | 17 | 1
0A | 09 | 1
0B | - | 0
0C | - | 0
0D | 2D | 1
0E | 11 | 1
0F | 0D | 1



:::


![](https://i.imgur.com/i44mEtH.png)
* 0x027c
0x027c = $00001001111100_2$
VPN - $00001001_2$ = 0x09
VPO - $111100_2$ = 0x3C
TLBI - $01_2$ = 0x1  (set)
TLBT - $000010_2$ = 0x02 (tag)
TLB Hit? Y
PPN - 0x17
Physical adress - $10111111100_2$
CO - $00_2$ = 0x0 (bajt)
CI - $1111_2$ = 0xF (indeks)
CT - $10111_2$ = 0x17 (tag)
MISS


* 0x03a9
0x03a9 = $00001110101001_2$
VPN - $00001110_2$ = 0x0E
VPO -  $101001_2$ = 0x29
TLBI - $10_2$ = 0x2
TLBT - $000011_2$ = 0x03
TLB Hit? N
Page Fault? N
PPN - 0x11
Physical adress - $10001101001_2$
CO - $01_2$ = 0x1 
CI - $1010_2$ = 0xA
CT - $10001_2$ = 0x11
MISS

* 0x0040
0x0040 = $00000001000000_2$
VPN - $000000001_2$ = 0x01
VPO - $000000_2$ = 0x00
TLBI - $01_2$ = 0x1
TLBT - $0000000_2$ = 0x00
TLB Hit? N
Page Fault? Y


### Zadanie 3

:::info
W tym zadaniu będziemy analizowali w jaki sposób system operacyjny musi aktualizować **tablicę stron** wraz z kolejnymi dostępami do pamięci głównej. Załóż, że strony są wielkości 4KiB, TLB jest **w pełni asocjacyjne** z zastępowaniem LRU. Najwyższa wartość pola LRU koduje najlepszego kandydata na **ofiarę** (ang. victim). Jeśli potrzebujesz **wtoczyć** (ang. swap-in) stronę z dysku użyj następnego numeru **ramki** (ang. page frame) większego od największego istniejącego w tablicy stron. 
Dla poniższych danych podaj ostateczny stan TLB i tablicy stron po wykonaniu wszystkich dostępów do pamięci. Dla każdej operacji dostępu do pamięci wskaż czy było to trafienie w TLB, trafienie w tablicę stron, czy też **błąd strony**.

Początkowa zawartość tablicy stron:
VPN | Valid | PPN
-|-|-
00 | 1 | 05
01 | 0 | dysk
02 | 0 | dysk
03 | 1 | 06
04 | 1 | 09
05 | 1 | 0B
06 | 0 | dysk
07 | 1 | 04
08 | 0 | dysk
09 | 0 | dysk
0A | 1 | 03
0B | 1 | 0C
0C | 0 | brak

Początkowa zawartość TLB:
Valid | Tag | LRU | PPN
-|-|-|-
1 | 0B | 0 | 0C
1 | 07 | 1 | 04
1 | 03 | 2 | 06
0 | 04 | 3 | 09

Ciąg dostępów do pamięci:
Adres |
-|
123d |
08b3 |
365c |
871b |
bee6 |
3140 |
c049 |

:::

**tablica stron** - struktura danych używana przez mechanizmy wirtualizacji pamięci do przechowywania sposobu odwzorowania adresów pamięci logicznej (wirtualnej) w adresy pamięci fizycznej.
**TLB w pełni asocjacyjne** - TLB w którym cache każdego adresu możemy zapisać w dowolnym miejscu
**ofiara** (victim) - wpis w tablicy który usuwamy 
**wtoczenie** (swap-in) - proces zapisywania strony do pamięci podręcznej po błędzie strony
**ramka**(page frame) - fizyczny blok pamięci trzymający strony
**błąd strony** - próba odczytania strony która nie jest w ramie 

Skoro strony mają 4KiB to mamy 4 bity VPN i $log_2(4KiB)=12$ bitów offsetu. 
![](https://i.imgur.com/A6v8IaR.png)


###  Dostępy

| Adres | VPN | TLB? |         PPN         |
|:-----:|:---:|:----:|:-------------------:|
| 123d  | 01  | nie  | page-fault dysk->0D |
| 08b3  | 00  | nie  |         05          |
| 365c  | 03  | nie  |         06          |
| 871b  | 08  | nie  | page-fault dysk->0E |
| bee6  | 0B  | nie  |         0C          |
| 3140  | 03  | tak  |         06          |
| c049  | 0C  | nie  |        błąd         |

### Końcowe wartości

#### Tablica stron

| VPN | Valid | PPN  |
|:---:|:-----:|:----:|
| 00  |   1   |  05  |
| 01  |   1   |  0D  |
| 02  |   0   | dysk |
| 03  |   1   |  06  |
| 04  |   1   |  09  |
| 05  |   1   |  0B  |
| 06  |   0   | dysk |
| 07  |   1   |  04  |
| 08  |   1   |  0E  |
| 09  |   0   | dysk |
| 0A  |   1   |  03  |
| 0B  |   1   |  0C  |
| 0C  |   0   | brak |

#### TLB:

| Valid | Tag | LRU | PPN |
|:-----:|:---:|:---:|:---:|
|   1   | 0B  |  1  | 0C  |
|   1   | 03  |  0  | 06  |
|   1   | 00  |  2  | 05  |
|   1   | 01  |  3  | 0D  |

### Zadanie 4

:::info
Niech system posługuje się 32-bitowymi adresami wirtualnymi, rozmiar strony ma 4KiB, a rozmiar wpisu tablicy stron zajmuje 4 bajty. Dla procesu, który łącznie używa 1GiB swojej przestrzeni adresowej podaj rozmiar tablicy stron: (a) jednopoziomowej, (b) dwupioziomowej, gdzie katalog tablicy stron ma 1024 wpisy. Dla drugiego przypadku – jaki jest maksymalny i minimalny rozmiar tablicy stron?
:::


#### Jednopoziomowa

Po prostu odcinamy od adresu bity potrzebne na offset i mnożymy ilość wpisów przez ich rozmiar.
$2^{32}/2^{12}*2^{2}=2^{22} = 4MiB$

#### Dwupoziomowa
Ilość wpisów w sumie: $2^{32}/2^{12}=2^{20}$
Ilość wpisów dla jednego poziomu: $2^{10}$
Wielkość wpisów w 1GiB przestrzeni adresowwej: $2^{18}$

##### Maksymalny
Mamy 1024 wpisy w katalogu tablic, czyli $2^{10}$
Zakładając, że układamy wpisy w kolejnych tablicach stron w katalogu, sumujemy rozmiar katalogu z rozmiarem zapełnionych katalogów, czyli (2^{10} + 2^{10}*2^{10}) razy rozmiar jednego wpisu.
Finalnie otrzymujemy działanie $(2^{10} + 2^{10}*2^{10}) * 4B = 4KiB + 4MiB$ 

##### Minimalny
Bierzemy ponownie rozmiar samych wpisów katalogu: $2^{10} = 4KiB$
W tym przypadku zakładamy, że są wypełnione w najoptymalniejszy spobób. Więc wszystkie zmieszczą się w $2^8$ wpisach w katalogu.
Otrzymujemy wzór $4KiB + 2^{8}*4KiB = 4Kib + 1MiB$



### Zadanie 5

:::info
Wiemy, że pamięć podręczna TLB jest niezbędna do przeprowadzania szybkiej translacji adresów. Czemu, w najogólniejszym przypadku, należy wyczyścić zawartość TLB przy przełączaniu przestrzeni adresowych? Jak można uniknąć tej kosztownej operacji?
Wskazówka: Rozważ wprowadzenie identyfikatorów przestrzeni adresowych (ASID), tak jak w architekturze MIPS.
:::

Zawartość TLB jest czyszczona, ponieważ część dostępów do TLB staje się nieważna. 

Na przykład w mikroprocesorze Alpha 21264 korzysta się z numeru przestrzeni adresowej (ASN) i tylko dostępy z ASN pasującym do aktualnego procesu są uznawane za ważne. Natomiast w Intel Pentium Pro flagi PGE (page global enable) w rejestrze kontrolnym CR4 i "global (G) flag of a page-directory or page-table entry" (ciężko mi było przetłumaczyć) pomagają nam ustalić które strony są często używane i uchronić je przed usunięciem. 

Źródło:
https://en.wikipedia.org/wiki/Translation_lookaside_buffer#Address-space_switch



### Zadanie 6

:::info
Wyznacz maksymalny rozmiar zbioru roboczego procesu, dla którego nie będzie on generował nowych chybień w TLB (ang. TLB reach)? Rozważ wariant pesymistyczny i optymistyczny dla czterodrożnego TLB o 64 wpisach. Jak zmieni się oszacowanie, jeśli zezwolimy na używanie dużych stron (ang. huge pages) o wielkości 4MiB?

:::

***Zbiór roboczy*** to ta część przydzielonej pamięci adresowej procesu która jest faktycznie wykorzystywana.
![](https://i.imgur.com/xBkByzq.png)
skoro TLB jest 4-drożne to w każdym zbiorze znajdują się 4 wpisy tablicy stron, a skoro wszystkich wpisów jest 64 to zbiorów jest $64/4 = 16$. 
Strony domyślnie mają po 4KiB.


Optymistyczny warniant to taki, w którym nie mamy żadnych konfliktów i każdy wpis jest używany. Skoro używalibyśmy wszystkich 64 wpisów, a każda strona ma po 4KiB to mamy łącznie $64*4KiB = 256KiB$
Pesymistyczny wariant to taki, w którym wszystko trafiło do jednego zbioru wtedy mamy tylko 4 wpisy, czyli $4*4KiB = 16KiB$

Dla dużych stron strony zamiast 4KiB mają 4MiB 
W przypadku optymistycznym mamy
$64*4MiB = 256MiB$
w Pesymistycznym
$4*4MiB = 16MiB$



### Zadanie 7    

:::info
Opisz dokładnie pola deskryptorów stron (ang. page table entry ) i deskryptorów katalogów stron (ang. page directory entry ) dla architektury x86-64. Które z bitów pomocniczych:
• opisują politykę zarządzania pamięcią podręczną dla zawartości strony,
• wspomagają algorytmy zastępowania stron pamięci wirtualnej, 
• określają uprawnienia dostępu do zawartości strony (w tym tryb pracy procesora). 
Wskazówka: Przeczytaj §9.7.1 z podręcznika. Szczegóły można znaleźć w §4.5 wolumenu 3 dokumentacji procesorów Intel.

:::

Page directory entry:
![](https://i.imgur.com/oFxsK8n.png)
![](https://i.imgur.com/KME6PYt.png)

Page table entry:
![](https://i.imgur.com/hu6VCR9.png)
![](https://i.imgur.com/yv6ox8v.png)

* Bit opisujący politykę zarządzania pamięcią podręczną: WT
* Bit wspomagający algorytm zastępowania stron: A
* Bity określające uprawnienie dostępu do strony: R/W, U/S, D, XD



### Zadanie 8

:::info
Na wykładzie przyjęliśmy, że translacja adresów jest wykonywana przed dostępem do pamięci podręcznej. Taki schemat określa się mianem pamięci podręcznej indeksowanej i znakowanej adresami fizycznymi (ang. physically-indexed, physically-tagged). Wyjaśnij jak wykonywać równolegle dostęp do TLB i pamięci podręcznej, stosując schemat pamięci indeksowanej wirtualnie i znakowanej fizycznie.
Wskazówka: Posłuż się slajdem 25 do wykładu „Virtual Memory: Systems”, ale wytłumacz to szczegółowo!

:::


W schemacie rozważanym na wykładzie dostęp do pamięci podręcznej L1 wykonywany jest dopiero po translacji adresów. Oznacza to, że płacimy sume czasów tych operacji. Przyspieszymy to, wykonując te operacje jednocześnie.
Aby móc dokonać dostępu do cache bez uprzedniego tłumaczenia adresów, posłużymy się schematem “Virtually indexed, physically tagged”. Użyjemy adresu fizycznego dla tagu. Konkretnie, weźmiemy bity z VPO i użyjemy ich jako offset i index dla pamięci podręcznej. Aby było to możliwe offset i index pamięci cache muszą mieć łącznie tyle samo bitów co PPO, a bity VPO muszą być takie same co bity PPO. Wtedy, kiedy TLB będzie zajmowało się translacją adresów, jednocześnie, bez tłumaczenia, możemy uzyskać dostęp do pamięci podręcznej, co istotnie przyspiesza cały proces.![](https://i.imgur.com/cbikVvL.png)


