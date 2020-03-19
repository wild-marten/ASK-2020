Czy poniższe wyrażenia zawsze obliczą się do prawdy dla dwóch dowolnych wartości zmiennych
«x» i «y» typu «int32_t»? Jeśli nie to podaj wartości, które prowadzą do obliczenia fałszu.


• (x > 0) || (x - 1 < 0)
~~~could be false
undefined behaviour for INT_MIN

• (x & 7) != 7 || (x << 29 < 0)
~~~true 
7 in binary is 0111 shifting it multiplying it by 2^29 
gives 1 on 31th position, having in mind that
int's min value is -2^31 any number having 1 on 3rd
position will give negative number

• (x * x) >= 0
~~~could be false for x = sqrt(int32) + 1
Undefined behaviour


• x < 0 || -x <= 0
~~~true

there's more negative numbers than positive
Therefore, that'll not cause and overflow

• x > 0 || -x >= 0
~~~false for INT_MIN
Undefined behaviour


• (x | -x) >> 31 == -1
~~~false for 0

• x + y == (uint32_t)y + (uint32_t)x
~~~true
One unsigned in expression makes all other num types
unsigned. The left side is also being converted to unsigned


•x * ~y + (uint32_t)y * (uint32_t)x == -x
~~~true
All numbers get converted to unsigned so:
x * ~y + y * x = x * (y + ~y) = x * (2^32-1) mod 2^32 = -x mod 2^32 (which is exactly rvalue);


//NA ZAJĘCIACH
Treść zadania:
Czy poniższe wyrażenia zawsze obliczą się do prawdy dla dwóch dowolnych wartości zmiennych«x» i «y» typu «int32_t»? Jeśli nie to podaj wartości, które prowadzą do obliczenia fałszu

    (x > 0) | (x - 1 < 0)
    to będzie fałsz na minimalnej wartości int ponieważ wtedy x < 0 a x-1 to maksymalna wartość int zatem
    x-1 > 0

    (x & 7) != 7 | (x << 29 < 0)
    to będzie zawsze prawda ponieważ (x & 7) != 7 to będzie prawdziwe zawsze jeśli reszta z dzielenia
    liczby x przez 8 będzie różna od 7. (x << 29 < 0) to natomiast będzie prawdziwe zawsze kiedy trzeci
    bit liczby będzie jedynką bo wtedy po przesunięciu o 29 stanie się 32 bitem więc liczba będzie ujemna.
    a w przypadku kiedy reszta z dzielenia przez 8 liczby x jest równa 7 to 3 bit jest zapalony, więc
    wtedy (x << 29 < 0) będzie prawdziwe

    (x * x) >= 0
    to dla wielu przypadków nie będzie prawdziwe wystarczy aby int się przekręcił do liczby ujemnej
    przykładowo przekręci się dla x = 46341 jest to pierwiastek z maksymalnej liczby możliwej do zapisania
    w int zaokrąglony do góry

    x < 0 || -x <= 0
    to jest zawsze prawda ponieważ -0 = 0 i każda liczba dodatnia ma swoją liczbę przeciwną
    zatem -0 <= 0 to prawda oraz dla każdej liczby ujemnej prawdą będzie x < 0 a dla każdej dodatniej prawdą
    będzie -x <= 0

    x > 0 || -x >= 0
    to nie będzie prawda dla minimalnej wartości int ponieważ nie ma ona swojej liczby przeciwnej w
    zakresie inta i -x w tym przypadku będzie równa x zatem będzie < 0

    (x | -x) >> 31 == -1
    Nie zachodzi dla x = 0
    bo zarowno 0 i -0 ma pierwszy od lewej bit = 0

    x + y == (uint32_t)y + (uint32_t)x
    najpierw wykona sie dodawanie signed po lewej
    potem wykona sie dodawanie unsigned po prawej
    przy porownaniu lewa strona zostanie rzutowana na unsigned (przy rzutowaniu pozostaje ten sam “bit pattern”) a dodawanie wyglada tak samo
    przy unsigned i signed wiec sa sobie rowne zawsze

    x*~y + (uint32)y * (uint32_t)x == -x
    zawsze prawda poniewaz mozemy sobie rozpisac
    ~y jako -y - 1 i otrzymujemy
    x*(-y -1) + uyux
    czyli
    -xy -x + uyux
    -xy sie skraca z +uy*ux
    i zostaje -x
