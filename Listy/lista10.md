# Ćwiczenia 10 z ASK, grupa AKr (21.05.2020)

### Zadanie 1

:::info
Rozważmy dysk o następujących parametrach: 360 obrotów na minutę, 512 bajtów na sektor, 96 sektorów na ścieżkę, 110 ścieżek na powierzchnię. Procesor czyta z dysku całe sektory. Dysk sygnalizuje dostępność danych zgłaszając przerwanie na każdy przeczytany bajt. Jaki procent czasu procesora będzie zużywała obsługa wejścia-wyjścia, jeśli wykonanie procedury przerwania zajmuje 2.5 µs? Należy zignorować czas wyszukiwania ścieżki i sektora.
Do systemu dodajemy kontroler DMA. Przerwanie będzie generowane tylko raz po wczytaniu sektora do pamięci. Jak zmieniła się zajętość procesora?
:::

Dysk posiada 360obr/min czyli w ciągu sekundy ścieżka jest czytana 6 razy. 
Mnożymy otrzymaną ilość razy ilość sektorów na ścieżkę (96 x 6) co daje nam 576 sektorów przeczytanych w 1 sekundę. Mnożymy to razy liczbę bajtów (576 x 512) i otrzymujemy 294 912B/s = 0.294912B/us, zatem aby przeczytać 1B potrzebujemy około 3.4 us, po czym zgłaszamy przerwanie, które zajmuję 2.5us.

Aby obliczyć procent zajętości procesora liczymy 2.5/3.4 * 100% = 73.5% 

W momencie gdy przerwanie będzie generowane tylko raz po wczytaniu sektora, jeżeli czytamy 576 sektorów w 1s, przeczytanie sektora zajmie nam 1740.8us (Skoro 1B czytamy 3.4us, to cały sektor czytamy w (512 x 3.4us)), czyli 

2.5/1740.8 * 100% = 0.14%


### Zadanie 2

:::info
Moduł DMA kontrolera dysku do transferu danych używa techniki podkradania cykli. 32-bitowa szyna ma przepustowość 10 milionów transferów na sekundę. Procesor RISC bez pamięci podręcznej wykonuje 32-bitowe instrukcje, z których 40% to dostępy do pamięci. O ile procent zmieni się liczba wykonywanych instrukcji w wyniku aktywności modułu DMA, jeśli transferujemy z dysku dane z prędkością 2MB/s.
:::


W zadaniu jest napisane "O ile procent zmieni się liczba wykonywanych instrukcji w wyniku aktywności modułu DMA" choć tak na prawdę chodzi o liczbę wykonanych operacji w jednostce czasu.

Szyna 32-bitowa ma przepustowość 10mln transferów na sekundę. Skoro 10mln razy na sekundę wysyła po 32 bity to jej przepustowość to 320000000b/s zatem około 38MB/s. Jeśli aktywuje się moduł DMA  i transferuje dane z prędkością 2MB/s to "zabiera" około 5% całkowitej przepustowości szyny. Przez to, że tylko 95% całkowitej przepustowości szyny jest wolne (pozostałe 5% wykorzystuje moduł DMA) to w danej jednostcze czasu dostępy pamięci które wykonuje procesor zwolnią średnio do 95% oryginalnej prędkości (oryginalnej czyli takiej w której modół DMA nic by nie robił). W zadaniu podane jest że procesor wykonuje jakieś operacje z których 40% to dostępy do pamięci. Poza tym procesor musi jeszcze wczytywać z pamięci kolejne instrukcje zatem 60% instrukcji wykona 1 dostęp do pamięci a pozostałe 40% 2 dostępy do pamięci. Zatem 60% operacji które wykonywał procesor zwolni o 2.5% a pozostałe 40% zwolni o 5% zatem całość zwolni do 60%x97.5% + 40%x95% = 96.5% czyli całość zwolni o około 3.5%.


### Zadanie 3

:::info
Nowoczesny procesor x86-64 (np. i7-6700) ma następujące czasy dostępu do poszczególnych poziomów pamięci: L1 cache: 4 cykle; L2 cache: 12 cykli; L3 cache: 40 cykli; pamięć DRAM: 200 cykli. Jaki jest średni czas dostępu do pamięci, jeśli 90% dostępów trafia w cache L1, 95% w cache L2, 98% w cache L3? Jaki jest pesymistyczny czas dostępu do pamięci?
Wskazówka: Dostęp do pamięci o poziomie i+1 zachodzi tylko wtedy, gdy chybiliśmy w pamięć na poziomie i.
:::


Średni czas dostępu:
0,9 * 4+0,1 * (0,95 * 16+0,05 * (0,98 * 56+0,02 * 256)) = 5.42

Pesymistyczny czas dostępu:
200 + 40 + 12 + 4 = 256


### Zadanie 4

:::info
Na podstawie [ 1 , § 2.1.3] i [ 1 , § 2.2] zreferuj protokół komunikacji kontrolera pamięci z modułami pamięci DRAM. Wyjaśnij kroki jakie musi podjąć kontroler by odczytać jedną lub kilka kolejnych komórek pamięci. Wyjaśnij źródło opóźnień t~CAS~ (CL), t~RCD~, t~RP~, t~RAS~ ograniczających wydajność operacji.
Wskazówka: Gdyby źródło [1] okazało się niewystarczające należy zajrzeć do [2, §11.1].
:::


![](https://i.imgur.com/cU5nBkg.png)


Kontroler rozpoczyna odczyt od zrobienia dostępu adresu rzędu na szynie adresowej i obniżeniu sygnału RAS (Row Address Selection). Wtedy czip RAMu zapamiętuje stan zaadresowanego rzędu. Następnie poprzez zrobienie dostępu na szynie adresowej i obniżeniu sygnału CAS (Column Address Selection) (po opóźnieniu $t_{RCD}$) przekazany zostaje adres kolumny.

Po zakończonym adresowaniu (i opóźnieniu $t_{CAS}$) może nastąpić transfer danych.

Ponadto, możliwe jest wielokrotne wywołanie sygnału CAS, bez uprzedniego RAS, co znacznie usprawnia odczyt.

Przed wczytaniem kolejnego rzędu, musi on najpierw zostać zdeaktywowany. Po podaniu sygnału do naładowania (tu: jednoczesne obniżenie sygnałów WE i RAS) i odczekaniu $t_{RP}$ cykli może nastąpić załadowanie następnego rzędu. 

#### Opóźnienia:

$t_{CAS} (CL)$ - minimalny czas między wykonaniem odczytu z kolumny a początkiem transferu danych.

$t_{RCD}$ - czas między wydaniem polecenia wyboru wiersza, a dostępnością danych na wyjściu z układu wzmacniającego.

$t_{RP}$ - czas na przygotowanie innego wiersza na dostęp.

$t_{RAS}$ - minimalny czas między poleceniem wyboru wiersza, a przywróceniem danych w wierszu powykonaniu operacji.

### Zadanie 5

:::info
Blok pamięci podręcznej procesorów x86-64 ma 64 bajty. Dla uproszczenia przyjmijmy, że w jednym cyklu zegarowym między pamięcią a procesorem można przesłać 64 bity danych. Ile nanosekund, w pesymistycznym przypadku, zajmie sprowadzenie bloku pamięci podręcznej z pamięci DRAM dla poniżej scharakteryzowanych modułów:
• DDR4-1600, t~CLK~= 800MHz, t~CAS~ = 10, t~RCD~ = 10, t~RP~ = 10, t~RAS~= 25,
• DDR4-2133, t~CLK~ = 1066.67MHz, t~CAS~ = 15, t~RCD~ = 15, t~RP~ = 15, t~RAS~ = 36.
Powtórz obliczenia zakładając, że pamięć działa w trybie sekwencyjnym (ang. burst mode), tj. podaje na kolejnych zboczach zegara szesnaście 64-bitowych słów bez czekania na polecenie zmiany kolumny.
:::

Pesymistyczny przypadek interpretujemy jako przymus wykonania wszystkich operacji odczytu (w optymistycznym przypadku, mogłoby się przykładowo okazać, że interesująca nas komórka znajduje się już w załadowanym wierszu).

Zatem w pierwszym trybie musimy zlokalizować i wczytać odpowiedni wiersz ($t_{RCD}$), wybrać odpowiednie komórki (za każdym razem odczekując $t_{CAS}$) i transferować kolejne słowa (przesył jednego słowa ($1B$) = 1 cykl). Pamiętając, że wszystkie te operacje nie mogą trwać krócej od $t_{RAS}$ (z fizycznych ograniczeń), ładujemy rząd z powrotem do pamięci.

$t_{s} = max(t_{RAS}, t_{RCD} + 8 \cdot t_{CAS} + 8) + t_{RP}$

Dla trybu sekwencyjnego musimy wczytać wiersz ($t_{RCD}$) oraz ustawić odpowiednią kolumnę ($t_{CAS}$). W tym trybie, nie oczekujemy na polecenie zmiany kolumny i wysyłamy kolejne słowa. Ponieważ odbywa się to na zboczach zegara, to w trakcie 4 cykli możemy wysłać aż 8 słów.

$t_{br} = max(t_{RAS}, t_{RCD} + t_{CAS} + 4) + t_{RP}$

### DDR4-1600

Jeden cykl zegara trwa: 
$$\frac{1}{8 \cdot 10^{8}} \: [\frac{1}{Hz}] = 1,25 [ns]$$

#### W normalnym trybie:

$$t_{s} = max(25, 10 + 8 \cdot 10 + 8) + 10 = 108 \: [cykli]$$

Odczytanie 64-bajtowego bloku zajmuje **$135 [ns]$**.

#### W trybie sekwencyjnym:

$$t_{br} = max(25, 10 + 10 + 4) + 10 = 35 \: [cykli]$$

Czyli **$43,75 [ns]$**.
### DDR4-2133

Jeden cykl zegara trwa: 
$$\frac{1}{1,0667 \cdot 10^{9}} \: [\frac{1}{Hz}] \approx 0,94 [ns]$$

#### W normalnym trybie:
$$t_{s} = max(36, 15 + 8 \cdot 15 + 8) + 10 = 153 \: [cykli]$$

Całkowity czas operacji: **$143,82 [ns]$**.


#### W trybie sekwencyjnym:

$$t_{br} = max(36, 15 + 15 + 4) + 15 = 51 \: [cykli]$$

Co daje **$47,94 \: [ns]$**.


### Zadanie 6

:::info
Program czyta sekwencyjnie jednowymiarową tablicę o rozmiarze 4GiB położoną pod adresem podzielnym przez 2^20^. W komputerze zainstalowano dwa moduły pamięci DDR4-2133 o parametrach: t~CAS~ = 15, t~RCD~ = 15, t~RP~ = 15, t~RAS~ = 36, maksymalny rozmiar transferu sekwencyjnego to 16 słów, długość wiersza (ang. DRAM page size) wynosi 8KiB. Ile czasu zajmie sprowadzenie danych do procesora? Należy pominąć rozważanie opóźnień wynikających z działania pamięci podręcznej i kontrolera pamięci.
Powtórz obliczenia dla systemu dysponującego pamięcią w konfiguracji dwukanałowej (ang. dual-channel).
:::

Odczyt następuje z pamięci znajdującej się pod adresem podzielnym przez 2^20^, więc zacznie się od początku nowego wiersza.

Jeden cykl zegara trwa: 
$$\frac{1}{1,0667 \cdot 10^{9}} \: [\frac{1}{Hz}] \approx 0,94 [ns]$$

Obliczamy liczbę wierszy składających się na całą tablicę: 4GiB / 8KiB = 524288

Ponieważ dokonujemy odczytu sekwencyjnie nie musimy wysyłać kolejnych żądań zmiany kolumny 15 razy po ostatnim takim sygnale, a treść pobranych komórek jest wysyłana na zboczach zegara, czyli dwukrotnie w trakcie cyklu - łącznie 16 bajtów.

W jednym wierszu o długości 8KiB mieści się 512 bajtów możliwych do wysłania w ciągu 32 cykli.

Ostatecznie możemy wyliczyć, że oprócz 524288 operacji zmian wiersza dojdzie do 32 operacji ustawienia kolumny w każdym wierszu.

Liczbę cykli potrzebnych na pobranie danych z jednego wiersza poznamy z najstępującego wzoru (32 operacje ustawienia kolumny oraz 32 cykle potrzebne na wysłanie danych):
$t_{br} = max(t_{RAS}, t_{RCD} + 32*t_{CAS} + 32) + t_{RP}$
$t_{br} = max(36, 15 + 32*15 + 32) + 15 = 542 \approx 509 [ns]$

Wynik dla całej pamięci wyniesie:
$542 * 524288 = 284164096 \approx 267114250 [ns] = 0.2671 [s]$

### Zadanie 7

:::info
Nagraj na przenośny dysk USB program memtest86 i uruchom go z poziomu wbudowanego oprogramowania UEFI. Podaj parametry systemu pamięci w swoim komputerze. Jaka jest przepustowość poszczególnych poziomów pamięci podręcznej i pamięci DRAM? Oszacuj, w taktach procesora, średni czas dostępu do pamięci podręcznej L1, L2, L3 i pamięci DRAM.
:::

|type    | size  | speed     |
| ------ | ----- | --------- |
|L1 Cache| 64k   | 88408 MB/s|
|L2 Cache| 512K  | 88408 MB/s|
|L3 Cache| 8192K |30336 MB/s |
|Memory  | 8145M |10079 MB/s |
Chipset: Unknown
Memory type: Unknown
Memory clock: 2400hz
CLK: 3094MHz
| typ | Oszacowany średni czas dostępu (128B) |
|-----|--------------------------------|
|L1   | 4 cykli |
|L2   | 4 cykli |
|L3   | 12 cykli|
|mem  | 38 cykli|

