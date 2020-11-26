# Ćwiczenia 11 z ASK, grupa AKr (28.05.2020)

### Zadanie 1

:::info
Rozważmy pamięć podręczną z mapowaniem bezpośrednim adresowaną bajtowo. Używamy adresów 32-bitowych w następującym formacie: (tag, index, offset) = (addr31...10, addr9...5, addr4...0).
• Jaki jest rozmiar bloku w 32-bitowych słowach?
• Ile wierszy ma nasza pamięć podręczna?
• Jaki jest stosunek liczby bitów składujących dane do liczby bitów składujących metadane?
:::


**pamięć podręczna z mapowaniem bezpośrednim** - pamięć podręczna, w której na jeden zbiór przypada tylko jeden wiersz. 

**adresowanie bajtowe** - adresowanie, za pomocą którego można uzyskać dostęp do poszczególnych bajtów

• Jaki jest rozmiar bloku w 32-bitowych słowach?
Na offset mamy przeznaczone 5 bitów w adresie, zatem rozmiar bloku w wierszu to
$2^5 = 32$ bajty. 

1 bajt = 8 bitów czyli 4 bajty = 32 bity = 1 słowo 32bit, więc po przeliczeniu rozmiar bloku to 
8 słów 32bit.

• Ile wierszy ma nasza pamięć podręczna?
Na wybór zbioru przeznaczono w adresie 5 bitów. Wiemy, że w każdym zbiorze jest 1 wiersz, więc liczba wierszy to: $2^5 = 32$.

• Jaki jest stosunek liczby bitów składujących dane do liczby bitów składujących metadane?

Wystarczy że policzymy stosunek dla jednego wiersza.
Na metadane skłądają się bity zawierające informacje o znaczniku i bit valid. Pozostałe (bloki w wierszach) to dane.

valid - 1 bit
znacznik - 22 bity (tyle przeznaczono w adresie na zapis znacznika)
blok - 32 Bajty = $8 * 32 = 256$ bitów.

Stosunek danych do metadanych: 256/23

### Zadanie 2

:::info
Mamy system z pamięcią operacyjną adresowaną bajtowo. Szerokość szyny adresowej wynosi 12. Pamięć podręczna ma organizację sekcyjno-skojarzeniową o dwuelementowych zbiorach, a blok ma 4 bajty. Dla podanego niżej stanu pamięci podręcznej wyznacz, które bity adresu wyznaczają: offset, indeks, znacznik. Wszystkie wartości numeryczne podano w systemie szesnastkowym.
| Indeks | | Znacznik | Valid |  B0 | B1 | B2 | B3 | | Znacznik | Valid |  B0 | B1 | B2 | B3 |
|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
| 0 | | 00 | 1 | 40 | 41 | 42 | 43 | | 83 | 1 | FE | 97 | CC | D0 |
| 1 | | 00 | 1 | 44 | 45 | 46 | 47 | | 83 | 0 | – | – | – | – |
| 2 | | 00 | 1 | 48 | 49 | 4A | 4B | | 40 | 0 | – | – | – | – |
| 3 | | FF | 1 | 9A | C0 | 03 | FF | | 00 | 0 | – | – | – | – |
Określ, które z poniższych operacji odczytu wygenerują trafienie albo chybienie i ew. jakie wartości wczytają:
Adres | Trafienie? | Wartość
-|-|-
832 | . . . | . . .
835 | . . . | . . .
FFD | . . . | . . .
:::

W celu pobrania rozkazu lub danej następuje sprawdzenie czy potrzebna informacja jest zapisana w pamięci podręcznej. Jeśli - tak, to mamy do czynienia z trafieniem, jeśli nie - chybieniem.

Blok ma 4 bajty, więc aby zapisać informację do którego bajtu się odwołać potrzebujemy 2 bity. Podobnie index: potrzebujemy 2 bity, ponieważ mamy 4 indexy. Zatem na znacznik pozostaje nam 12 bitów - 2 bity (blok) - 2 bity (index) = 8 bitów.

Adres | Offset | Indeks | Znacznik
-|-|-|-
832   | 0x2    | 0x0    | 0x83
835   | 0x1    | 0x1    | 0x83
FFD   | 0x1    | 0x3    | 0xFF
  
Adres | Trafienie? | Wartość
-|-|-
832   | TAK        | CC
835   | NIE        | BRAK
FFD   | TAK        | C0


### Zadanie 3

:::info
Rozważmy pamięć podręczną z poprzedniego zadania. Mamy następującą sekwencję odwołań do czterobajtowych słów pamięci o adresach zadanych liczbami w systemie szesnastkowym:
```
0 4 10 84 E8 A0 400 1C 8C C1C B4 884
```
Załóż, że na początku pamięć podręczna jest pusta. Jak wiele bloków zostało zastąpionych? Jaka jest efektywność pamięci podręcznej (liczba trafień procentowo)? Podaj zawartość pamięci podręcznej po wykonaniu powyższych odwołań – każdy ważny wpis wypisz jako krotkę (tag, index, . . . ). Dla każdego chybienia wskaż, czy jest ono przymusowe (ang. compulsory miss), czy wynika z kolizji na danym adresie (ang. conflict miss) czy ograniczonej pojemności (ang. capacity miss).

:::

0 - tag = 00 s = 0 b = 0 cold miss  

4 -  tag = 00 s = 1 b = 0 cold miss

10 - tag = 01 s = 0 b = 0 cold miss 

84 - tag = 08 s = 1 b = 0  cold miss

E8 - tag = 0E s = 2 b = 0 cold miss

A0 - tag = 0A s = 0 b = 0 conflict miss 

400 - tag = 40 s = 0 b = 0 - conflict miss

1C - tag = 01 s = 3 b = 0 cold miss

8C - tag = 08 s = 3 b = 0 cold miss

C1C - tag = C1 s = 3 b = 0 cold miss 

B4 - tag = 0B s = 1 b = 0 conflict miss 

884 - tag = 88 s = 1 b = 0 conflict miss
 
przyjmujac ze poswiecamy dane ktore sa w pamieci najdluzej nasza pamiec moze wygladac w ten sposob

  S    |       TT       |
|-|-|
  0    |       0A       |
  0    |       40       |
  1    |       0B       |
  1    |       88       |
  2    |       E8       |
  2    |       --       |
  3    |       08       |
  3    |       C1       |



efektywnosc = 0%


### Zadanie 4

:::info
Powtórz poprzednie zadanie dla poniższych organizacji pamięci podręcznej. Zakładamy, że bloki są długości dwóch słów pamięci. Ile dodatkowych bitów na linię pamięci podręcznej potrzeba na implementację określonej polityki wymiany (ang. replacement policy ).
• sekcyjno-skojarzeniowa 2-drożna, 16 bloków, polityka NRU (ang. Not Recently Used)
• w pełni asocjacyjna (ang. fully associative), 8 bloków, polityka LRU (ang. Least Recently Used).
:::


Polityka wymiany - algorytm za pomocą którego zarządzamy pamięcią, m. in. stwierdzamy, które wartości chcemy utrzymywać (i zastępować) w cache'u.

• Sekcyjno-skojarzeniowa 2-drożna, 16 bloków, polityka NRU

Mamy 8 zbiorów po 2 linie. Potrzebujemy dodatkowy jeden bit na zbiór, aby reprezentować co usuwamy (pół bitu na linię).

Używamy adresów 12-bitowych w następującym formacie: $(tag, index, offset) = (addr_{11}\dots_4, addr_3\dots_1, addr_0\dots_0)$

| Adres |  Adres (rozpisany)  |  Miss (co zastępujemy)  |
|-------|----------------|--------------------|
|  000  | 00000000 000 0 | compulsory miss    |
|  004  | 00000000 010 0 | compulsory miss    |
|  010  | 00000001 000 0 | compulsory miss    |
|  084  | 00001000 010 0 | compulsory miss    |
|  0E8  | 00001110 100 0 | compulsory miss    |
|  0A0  | 00001010 000 0 | conflict miss (000)|
|  400  | 01000000 000 0 | conflict miss (010)|
|  01C  | 00000001 110 0 | compulsory miss    |
|  08C  | 00001000 110 0 | compulsory miss    |
|  C1C  | 11000001 110 0 | conflict miss (01C)|
|  0B4  | 00001011 010 0 | conflict miss (004)|
|  884  | 10001000 010 0 | conflict miss (084)|

• W pełni asocjacyjna (ang. fully associative), 8 bloków, polityka LRU

Mamy 1 zbiór z 8 liniami. Potrzebujemy dodatkowe $\lceil log_2(8!)\rceil$ na zbiór (więcej w zad. 8), czyli w sumie 16 bitów (po 2 bity na linię).

Używamy adresów 12-bitowych w następującym formacie: $(tag, offset) = (addr_{11}\dots_1, addr_0\dots_0)$

| Adres |  Adres (rozpisany)   |   Miss (co zastępujemy)  |
|-------|---------------|---------------------|
|  000  | 00000000000 0 | compulsory miss     |
|  004  | 00000000010 0 | compulsory miss     |
|  010  | 00000001000 0 | compulsory miss     |
|  084  | 00001000010 0 | compulsory miss     |
|  0E8  | 00001110100 0 | compulsory miss     |
|  0A0  | 00001010000 0 | compulsory miss     |
|  400  | 01000000000 0 | compulsory miss     |
|  01C  | 00000001110 0 | compulsory miss     |
|  08C  | 00001000110 0 | capacity miss (000) |
|  C1C  | 11000001110 0 | capacity miss (004) |
|  0B4  | 00001011010 0 | capacity miss (010) |
|  884  | 10001000010 0 | capacity miss (084) |

### Zadanie 5

:::info
Odpowiedz na następujące pytania dotyczące organizacji pamięci podręcznej:
1. Do wyboru zbioru pamięci podręcznej używamy bitów znajdujących się w środku adresu, zaraz przed offsetem bloku. Czemu jest to lepszy pomysł niż używanie najbardziej znaczących bitów adresu?
2. Zdecydowana większość procesorów posiada odrębną pamięć podręczną pierwszego poziomu dla danych i dla instrukcji. Jakie korzyści to przynosi?

:::


1. Programy cechuje lokalność - odwołują się do danych bliskich siebie. Jeśli używalisbyśmy najbardziej znaczących bitów, to dane blisko siebie trafiałyby do tych samych zbiorów, nadpisując się. Zwiększyłoby to liczbę cache miss.
2. Instrukcji (zazwyczaj) się nie nadpisuje, a jedynie je czyta. Pamięć podręczna dla instrukcji może więc być bardziej wyspecjalizowana w czytaniu. Ponadto nowoczesne procesory potrafią czytać z obu tych cachy jednocześnie. Wiele procesorów implementuje też dekodowanie instrukcji i trzymanie ich już zdekodowanych w pamięci podręcznej, co dodatkowo zwiększa wydajność.


### Zadanie 6

:::info
Rozważamy system z dwupoziomową pamięcią podręczną z polityką zapisu write-back z writeallocate. Dodatkowo zakładamy, że blok o określonym adresie może znajdować się tylko na jednym poziomie pamięci podręcznej (ang. exclusive caches). Przy pomocy schematu blokowego przedstaw algorytm obsługi zapisu słowa maszynowego do pamięci. Pierwszym elementem diagramu ma być predykat „Czy słowo jest w L1?”. Pamiętaj o bicie dirty i o tym, że pamięć podręczna może być całkowicie wypełniona! Zakładamy, że pamięć podręczna pierwszego poziomu nie może komunikować się bezpośrednio z pamięcią operacyjną.

:::


Polityka zapisu mowi nam jakie przyjmujemy strategie przy zapisach danych
w cache ( WRITE-HIT i WRITE-MISS )

( STRATEGIE PRZY WRITE-HIT )

WRITE-THROUGH -> od razu update'ujemy pamiec,
                 w rezultacie CACHE trzyma taka sama wartosc jak pamiec

WRITE-BACK    -> Dane poczatkowo zostaja zupdate'owane tylko w cache, dopiero
                 w momencie gdy pamiec ma byc nadpisana - zapisujemy zmieniona wartosc
                 w cache nizszego stopnia.
                 (Przy Write-back kazdy blok musi miec tzw. "dirty bit"
                  mowiacy o tym czy dane w nim sa odpowiednio:
                  Dirty -> Nadpisane wzgledem cache nizszego poziomu
                  Clean -> Nie nadpisane)

( STRATEGIE PRZY WRITE-MISS )

WRITE-ALLOCATE -> Wyciagamy dane z pamieci nizszego stopnia i update'ujemy
                  tylko w cache do ktorego sciagnelismy ten blok.

WRITE AROUND   -> Bezposrednio zapisujemy w pamieci bez wciaganie do cache.

W tym zadaniu przyjmujemy, ze system dziala z polityka WRITE-BACK i WRITE-ALLOCATE ( jest to najczesciej spotykane polaczenie ). Pamiec podreczna jest
dwupoziomowa wiec mamy L1 i L2, a polityka zawierania sie Cache 
( Cache Inclusion Policy ) to "Exclusive Cache" czyli nie mozemy trzymac
tego samego bloku na roznych poziomach pamieci podrecznej.

Algorytm obslugi zapisu slowa w schemacie blokowym:


https://drive.google.com/file/d/1uA2OdcU5FNxUuMegIEyCAST4HGWIeMNM/view?usp=sharing






### Zadanie 7

:::info
Załóżmy, że dostęp do pamięci głównej trwa 70ns, a dostępy do pamięci stanowią 36% wszystkich instrukcji. Rozważmy system z pamięcią podręczną o następującej strukturze: L1 – 2 KiB, współczynnik chybień 8.0%, czas dostępu 0.66ns (1 cykl procesora); L2 – 1 MiB, współczynnik chybień 0.5%, czas dostępu 5.62ns. Procesor charakteryzuje się współczynnikiem CPI (ang. clocks per instruction) równym 1.0, jeśli pominiemy instrukcje robiące dostępy do pamięci danych. Odpowiedz na poniższe pytania:
• Jaki jest średni czas dostepu do pamięci dla procesora: tylko z pamięcią podręczną L1, z L1 i L2?
• Procesor wykonuje wszystkie instrukcje, łącznie z dostępami do pamięci danych. Oblicz jego CPI kiedy posiada: tylko pamięć podręczną L1, z L1 i L2.
Uwaga: Zakładamy, że wszystkie instrukcje wykonywane przez program są w pamięci podręcznej L1i.

:::

Mamy:

dostęp do pamięci głównej --- $70ns$
dostępy do pamięci --- $36\%$ wszystkich instrukcji
0.66ns --- 1 cykl procesora

$L1$ : 

2 KiB, 
współczynnik chybień 8.0% (czyli 92% trafień),
czas dostępu 0.66ns (1 cykl procesora);

$L2$ : 

1 MiB, 
współczynnik chybień 0.5% (czyli 99,5% trafień), 
czas dostępu 5.62ns.

Aby znaleźć średni czas dostępu do pamięci, mamy wzór:
:::warning
$T_{avg} = T_c + h * M$

&nbsp;

$h \quad -$ współczynnik chybień
$T_c \quad -$ czas dostępu do informacji z pamięci podręcznej
$M \quad -$ czas dostępu do pamięci głównej
:::

---

stąd:

* Jaki jest średni czas dostepu do pamięci dla procesora: 

    * tylko z pamięcią podręczną $L1$
    
        $h \space = 0.08$
        $T_c \space = 0.66ns$
        $M = 70ns$
   
        $T_{avg} = 0.66 + 0.08 * 70 = 6.26\space ns \approx 9,485 \space cykli$
       
    * z $L1$ i $L2$?

        $h_1 \space = 0.08$
        $h_2 \space = 0.005$
        $T_{c_1} \space = 0.66ns$
        $T_{c_2} \space = 5.62ns$
        $M = 70ns$
       :::warning
        $T_{avg} = T_{c_1} + h_1 * (T_{c_2} + h_2 * M)$
        :::
        
        $T_{avg} = 0.66 + 0.08 * (5.62 + 0.005 * 70) = 1,1376 \space ns \approx 1,72 \space cykli$
        
---

$CPI$ -- clocks per instructions -- ilość cykli na instrukcję 

![](https://i.imgur.com/e6YSrc5.png)

Gdzie $IC_{i}$ to liczba instrukcji dla danego typu instrukcji $i$, $CC_{i}$ to cykle zegara dla tego typu instrukcji i $IC = \sum_{i} (IC_ {i})$ jest całkowitą liczbą instrukcji. 


&nbsp;

:::warning
współczynnik $CPI$ równa się $1.0$, a zatem:

$CPI = 1 + T_{avg} * procent\_dostępów\_do\_pamięci$
:::

* Oblicz jego $CPI$ kiedy posiada: 

    * tylko pamięć podręczną $L1$, 
    
        $CPI = 1 + 9,485 * 0.36 = 4,4146$
    
    * z $L1$ i $L2$.

        $CPI = 1 + 1,72 * 0.36 = 1,6192$

---

### Zadanie 8

:::info
Dla czterodrożnej sekcyjno-skojarzeniowej pamięci podręcznej implementujemy politykę zastępowania LRU. Masz do dyspozycji dodatkowe $\lceil log_2(4!)\rceil$ bitów na zbiór. Nie można modyfikować zawartości linii w zbiorze, ani zamieniać elementów kolejnością. Jak wyznaczyć kandydata do usunięcia ze zbioru? Jak aktualizować informacje zawarte w dodatkowych bitach przy wykonywaniu dostępów do elementów zbioru?
:::


Z zadania wiemy, że mamy do dyspozycji 5 dodatkowych bitów.
Możemy wykorzystać je w ten sposób, że podzielimy nasze 4 linie pamięci,
na dwie grupy i 1 z tych 5 bitów będzie określał, z której grupy ostatnio korzystaliśmy.
Pozostałe będą nam oznaczać do której linii ostatnio się odnieśliśmy.

W tabelkach
1 - oznacza, że ostatnio korzystaliśmy z tej linii
0 - przeciwny przypadek


Przykład:

- Przykładowy stan

    Grupa 0:
    | Dane                | A | B |
    | ------------------- |:-:|:-:|  
    | Ostatnio użyta linia| 1 | 0 |

    Grupa 1:
    | Dane                | C | D |
    | ------------------- |:-:|:-:|  
    | Ostatnio użyta linia| 1 | 0 |

    Ostatnio użyta grupa: 0

-----------------


- Dostęp do B

    Grupa 0:
    | Dane                | A | B |
    | ------------------- |:-:|:-:|  
    | Ostatnio użyta linia| 0 | 1 |

    Grupa 1:
    | Dane                | C | D |
    | ------------------- |:-:|:-:|  
    | Ostatnio użyta linia| 1 | 0 |

    Ostatnio użyta grupa: 0

-----------------

- Dostęp do D

    Grupa 0:
    | Dane                | A | B |
    | ------------------- |:-:|:-:|  
    | Ostatnio użyta linia| 0 | 1 |

    Grupa 1:
    | Dane                | C | D |
    | ------------------- |:-:|:-:|  
    | Ostatnio użyta linia| 0 | 1 |

    Ostatnio użyta grupa: 1

------------------

- Dostęp do X - ostatnio użyliśmy grupy 1, więc przechodzimy do 0,
		a tam ostatni użyliśmy B, więc ofiarą staje się A

    Grupa 0:
    | Dane                | X | B |
    | ------------------- |:-:|:-:|  
    | Ostatnio użyta linia| 1 | 0 |

    Grupa 1:
    | Dane                | C | D |
    | ------------------- |:-:|:-:|  
    | Ostatnio użyta linia| 0 | 1 |

    Ostatnio użyta grupa: 0

