# Ćwiczenia 3 z ASK, grupa AKr

## Lista ćwiczeniowa 3 (26.03.2020)

### Zadanie 1

:::info
Zastąp instrukcję dzielenia całkowitoliczbowego zmiennej $n$ typu ```int32_t``` przez stałą 3 przy pomocy operacji mnożenia liczb typu ```int64_t```. Skorzystaj z faktu, że $\frac{x}{k} \equiv x \cdot   \frac{1}{k}$. Przedstaw dowód poprawności swojego rozwiązania. Instrukcja dzielenia działa zgodnie z wzorem podanym na wykładzie, tj.:
$$
  \texttt{div3}(n) = 
  \begin{cases} 
    \lfloor \frac{n}{3} \rfloor & \text{dla } n \geq 0 \\
    \lceil \frac{n}{3} \rceil & \text{dla } n < 0
  \end{cases}
$$

x = 0x55555556
y = ((x * n) >> 32)
y = y + ((n>>31) & 1); 

Dowód:


Zauważmy, że x = (2^(32)+2)/3
1. dla n >= 0
 y = x * n >> 32 czyli $\frac{x*n}{2^{32}}$
 rozipisując y = $$ \lfloor \frac{2^{32} * n + 2 * n}{3 * 2^{32}}\rfloor$$ = $\lfloor \frac{n}{3}$  + $\frac{2*n}{3 * 2^{32}}\rfloor$
 
 2. dla n < 0
     y = x * n >> 32 + 1, czyli $\frac{x*n}{2^{32}}$ + 1
 rozipisując y = $$ \lfloor \frac{2^{32} * n + 2 * n + 3 * 2^{32}}{3 * 2^{32}}\rfloor$$ = $$ \lceil \frac{2^{32} * n + 2 * n + 1}{3 * 2^{32}}\rceil$$ = $\lceil \frac{n}{3}$  + $\frac{2*n + 1}{3 * 2^{32}}\rceil$
 
 Przejście z podłogi do sufitu: $$ \lfloor \frac{n + d - 1}{d} \rfloor =  \lceil \frac{n}{d} \rceil $$ 
 
 W obu przypadkach "reszta", która nam została jest bardzo mała i nie zmienia wyniku w przypadku, gdyby jej nie było.
 
> [name=Artur Kraska] Intuicyjnie: ```0x55555555>>32``` reprezentuje nam liczbę 0,01010101..010101~(2)~ = 0,333..33~(10)~

### Zadanie 2

:::info
Standard IEEE 754-2008 definiuje liczby zmiennopozycyjne o szerokości 16-bitów. Zapisz ciąg bitów reprezentujący liczbę $1.5625 \cdot 10^{-1}$. Porównaj zakres liczbowy i dokładność w stosunku do liczb zmiennopozycyjnych pojedynczej precyzji (```float```).
:::

Odpowiedz:
0.15625 = 0b0 01100 0100000000

halfy sa postaci: 1 bit znaku, 5 bitow cechy, 10 bitow mantysy
najpierw zapisujemy 0.15625 jako fixed precision:
0.00101 przesuwamy . o 3 miejsca w prawo (mnozymy przez 2^-3)
otrzymujemy 1.01
bias dla half'ów to 15 wiec chcemy zakodowac 15 - 3 = 12
s = 0
exp = 01100 = 12
frac = 0100000000
czyli 0.15625 = 0b0 01100 0100000000

zakres liczbowy single precision (float):
[-3.4 * 10^38, 3.4 * 10^38]

dokladnosc single precision (float):
24/log2(10) co daje 7 znaczacych cyfr po przecinku 

zakres liczbowy half precision:
[-65504,65504]

* największa liczba - 2^15 * (2-1/2^10^)
* najmniejsza dodatnia wartość znormalizowana - 2^-14^ * 1
* najmniejsza wartość dodatnia - 2^-14^ *1/2^10^ = 2^-24^

dokladnosc half precision:
11/log2(10) co daje 3 znaczace cyfry po przecinku 

### Zadanie 3

:::info
Oblicz ręcznie $3.984375 \cdot 10^{-1} + 3.4375 \cdot 10^{-1} + 1.771 \cdot 10^{3}$ używając liczb w formacie z poprzedniego zadania. Zapisz wynik binarnie i dziesiętnie. Czy wynik się zmieni jeśli najpierw wykonamy drugie dodawanie?
:::

Najpierw dostosujmy liczby do naszej reprezentacji:
$3.984375 \cdot 10^{-1} = 0.0110011_{2} = 1.10011_{2} \cdot 2^{-2}$
$3.4375 \cdot 10^{-1} = 0.01011_{2} = 1.011_{2} \cdot 2^{-2}$
$1.771 \cdot 10^{3} = 11011101011 = 1.1011101011 \cdot 2^{10}$

Przystępujemy do obliczania pamiętając o wyrównaniu, by uzyskać takie same wykładniki(dodając) oraz o zasadach zaokrąglania (round to even): 
$1.10011_{2} \cdot 2^{-2} + 1.011 \cdot 2^{-2} + 1.1011101011_{2} \cdot 2^{10}=$
$10.0011_{2} \cdot 2^{-2} + 1.1011101011_{2} \cdot 2^{10}=$
$1.011111_{2} \cdot 2^{-1} + 1.1011101011_{2} \cdot 2^{10}=$
$(0.00000000001011111_{2} + 1.1011101011_{2}) \cdot 2^{10}=$
$1.101110101110_{2} \cdot 2^{10} = 1.10111011_{2} \cdot 2^{10} = 1772_{10}$

Zamieniając kolejność:
$1.011 \cdot 2^{-2} + 1.1011101011_{2} \cdot 2^{10} + 1.10011_{2} \cdot 2^{-2}=$
$(0.000000000001011_{2} + 1.1011101011_{2}) \cdot 2^{10} + 1.10011_{2} \cdot 2^{-2}=$
$1.1011101011_{2} \cdot 2^{10} + 1.10011_{2} \cdot 2^{-2} =$
$1.1011101011_{2} \cdot 2^{10} = 1771_{10}$

Wyniki różnią się w zależności od tego, w jakiej kolejności wykonywaliśmy dodawanie. Powodem jest to, że po każdym dodawaniu musimy zaokrąglić liczbę tak, aby zmieściła się w naszym typie, stąd dodając bardzo małą liczbę do dużej możemy ją 'zgubić', jeśli nie jest wystarczająco znacząca (tak stało się przy zamienieniu kolejności działania).
### Zadanie 4

:::info
Mamy zmienne $x$, $y$ i $z$ typu ```int32_t``` ustawione na wybrane przez nas wartości. Konwertujemy je do liczb typu  ```double``` zapisanych w zmiennych $dx$, $dy$ i $dz$. Rozważmy poniższe wyrażenia z języka ```C```. Wskaż, które z nich mogą obliczyć się do fałszu. Podaj kontrprzykład – konkretne wartości naszych zmiennych całkowitoliczbowych.
* ```(float)x == (float)dx```
* ```dx - dy == (double)(x - y)```
* ```(dx + dy) + dz == dx + (dy + dz)```
* ```(dx * dy) * dz == dx * (dy * dz)```
* ```dx / dx == dz / dz```

:::


* ```(float)x == (float)dx``` - Zawsze true
    
    Zmiana z int na double jest bezstratna i po zmianie na float w równianiu
    otrzymujemy to samo.  
    
* ```dx - dy == (double)(x - y)``` 

    Kontrprzykład: x = INT_MIN  i y = 1
    
    Po lewej stronie wykona się normalnie odejmowanie,
    a natomiast po prawej będziemy mieli underflow i dopiero
    po tym zmiane na double.
* ```(dx + dy) + dz == dx + (dy + dz)``` - Zawsze true

    Dodawanie nigdy nie wyjdzie poza zakres nawet jak każda
    z liczb będzie równa INT_MAX, bo 3*INT_MAX < DBL_MAX
    Tak samo jeśli chodzi o underflow.
    Z racji, że operujemy na liczbach rzędu 2^33^, spokojnie mieścimy się w zakresie double.
* ```(dx * dy) * dz == dx * (dy * dz)``` 
    
    Wybierając duże, przypadkowe x,y,z może zdarzyć sie, że
    nastąpi zaokrąglenie które sprawi, że otrzymamy fałsz.
    Z racji, że operujemy na liczbach rzędu 2^96^, czasami potrzebne jest zaokrąglanie, które może sprawić, że dostaniemy różne wyniki.
* ```dx / dx == dz / dz```

    Kontrprzykład: x = 1 y = 0
    
    Po lewej stronie otrzymamy 1 natomiast po prawej będziemy
    mieli dzielenie 0/0 i dostaniemy jakąś przpadkową liczbę.

### Zadanie 5

:::info
Reprezentacje binarne liczb zmiennoprzecinkowych $f$ i $g$ typu ```float``` zostały załadowane odpowiednio do zmiennych ```x``` i ```y``` typu ```uint32_t```. Podaj wyrażenie, które:

* zmieni znak liczby ```x```,
* obliczy wartość $\lfloor log_2 |\texttt{f}| \rfloor$ typu ```int``` dla $f$ w postaci znormalizowanej,
* zwróci wartość logiczną operacji ```x == y```,
* zwróci wartość logiczną operacji ```x < y```.
  
Pamiętaj, że dla liczb zmiennopozycyjnych w standardzie IEEE 754 zachodzi $-0  \equiv +0$. Można pominąć rozważanie wartości ```NaN```.
:::

1. Należy zmienić bit znaku na przeciwny: ```x ^= (1 << 31);```
2. Korzystamy z informacji zapisanych w EXP: ```((x >> 23) & 255) - 127;```
3. Specjalny przypadek $0 = -0$: ```(x == y) | (x | y == (1 << 31))```
4. Porównujemy, znowu $0 = -0$ jest problemem:
```(x>=0 && x<y) || (x<0 && x>y)```
```(((~(x | y) & (x - y)) | (x & y & (y - x)) | (x & ~y)) & (1ll << 31)) != 0 & ((x | y) != (1ll << 31))```
> Zauważmy, że jeśli liczby są tego samego znaku, to najpierw powinniśmy porównać ich wykładniki, a jeśli są równe, to porównać mantysy. Bity liczby są akurat tak ustawione, że tak naprawdę wystarczy porównać ich wartości bitowe. Do tego wystarczy orównać znak ich różnicy. Zatem aby wykonać zadanie musimy:
> * jeśli obie są dodatnie, to sprawdzić znak x-y,
> * jeśli obie są ujemne, to sprawdzić znak y-x,
> * jeśli x jest ujemne a y dodatnie to zwracamy prawdę,
> * na końcu sprawdzić czy nie są zerami.
> 

$$-Nan < -\infty < -Norm < -Denorm \leq +Denorm < Norm < +\infty < +Nan$$


### Zadanie 6

:::info
Reprezentacja binarna liczby zmiennoprzecinkowej $f$ typu   ```float``` została załadowana do zmiennej ```x``` typu   ```uint32_t```. Podaj algorytm obliczający $f \cdot 2^i$ wykonujący  obliczenia na zmiennej ```x``` używając wyłącznie operacji na liczbach  całkowitych. Osobno rozważ $i \ge 0$ i $i < 0$.  Zakładamy, że liczba $f$~jest  znormalizowana, ale wynik operacji może dać wartość $\pm \infty$, $\pm 0$ lub  liczbę zdenormalizowaną. Możesz założyć, że wynik zaokrąglamy w kierunku zera.
:::

#### Rozwiązanie:
```c=
int e = ((x & 0x7f800000) >> 23) - 127;
e = e + i;

if (e>127)
    x=0x7FF0000000000000 + x^(1<<31);
else if (-150 <= e && e <= -127)
    x = (x & 0x80000000) + ((x&0x007fffff) >> -(e + 127)); 
else if (e < -150)
     x = 0; 
else
    x = (x&807fffff) + (127 + e)<<23;

```
do wyliczenia korzystamy ze wzoru:
$(-1)^S \cdot M \cdot 2^{i + E}$ 
gdzie M to Mantysa, E to cecha, S to bit znaku
###### obliczamy i + exponent 
```c=
int e = ((x & 0x7f800000) >> 23) - 127;
e = e + i;
```

###### Jeżeli jest za duże to ustawiamy x = INF
```c=
if (e>127)
    x=0x7FF0000000000000 + x^(1<<31);
```

###### na tym przedziale wynik jest zdenormalizowany
```c=
else if (-150 <= e && e <= -127)
    x = (x & 0x80000000) + ((x&0x007fffff) >> -(e + 127));
```
###### Jeżeli za mała liczba - ustwiamy x = 0 (pomijamy znak 0)
```c=
else if (e < -150)
     x = 0; 
```

###### w każdym innym wypadku wynik jest znormalizowany
```c=
else
    x = (x&807fffff) + (127 + e)<<23;
```

### Zadanie 7

:::info
Masz liczbę ```double``` jako ```uint64_t```. Wyprodukuj ```float```  z zaokręgleniem do najbliższej parzystej. Podaj definicje bitów guard, round, sticky.
:::

>Zanim przystąpimy do rozwiązania należy zwrócić uwagę na format double i float:
>
>$double$ 
>|sign  |exponent| fraction|
>|------|--------|---------|
>|1 bit |11 bits | 52 bits |
>$float$ 
>|sign  |exponent| fraction|
>|------|--------|---------|
>|1 bit | 8 bits | 23 bits |
>
> Double zapewnia nam większą dokładność, dlatego musimy zaokrąglić mantysę zgodnie z założeniami round to even.
> 
>
> Ponieważ na zakodowanie wykładnika float mamy 8 bitów to:
> $E_{minn} = -126$ - dla liczb w postaci znormalizowanej
> $E_{minz} = -149$ - dla liczb w postaci zdenormalizowanej
> $E_{max} = 127$
> 
> Konwertując musimy zatem rozważyć następujące przypadki brzegowe:
> * Jeśli wyliczone $E_{double} > 127$ zwracamy +&#x221e;,
> * Gdy $E_{double} < -149$ zwracamy 0,
> * W przypadku $-149 ≤ E_{double} ≤ -126$ zwracamy liczbę zdenormalizowaną.


```cpp
uint32_t double_to_float(uint64_t x)
{
    // obliczamy każdą część naszej liczby double
    int32_t sign = (x >> 63) & 1;

    /*dekodujemy wykładnik i kodujemy go na wykładnik float:
    (x >> 52) & ((1 << 11) - 1) <- wyodrębnienie EXPd
    1023 <- Bias double
    127  <- Bias float */
    int32_t e = (x >> 52) & ((1 << 11) - 1) - 1023 + 127;
    int64_t frac = x & ((1 << 52) - 1);

    //zaczynamy obliczać części do floata

    /* wyodrębniamy pierwsze 23 bity pierwotnej mantysy*/
    int32_t new_frac = (frac >> 29);

    //wartości do zaokrąglania
    /* 
        guard - najmniej znaczący bit wyniku
        round - najbardziej znaczący usuwany bit
        sticky - OR pozostałych usuwanych bitów
    */
    int32_t guard = (frac >> 29) & 1;
    int32_t round = (frac >> 28) & 1;
    int32_t sticky = (frac & 0xFFFFFFF) != 0; 
    //0xFFFFFFF wyodrębnia 28 dolnych bitów

    // zaokrąglenie
    /*
     1.gdy round = sticky = 1 mamy przypadek, w którym jesteśmy w 
     'ponad połowie' 
     do następnej liczby i zaokrąglamy

     2.gdy round = guard = 1 mamy przypadki 
     * jeśli sticky = 0 to jesteśmy w połowie i zaokrąglamy 
       do najbliższej parzystej (w górę)
     * jeśli sticky = 1 to przypadek 1.
     */
    if (round == 1 && (sticky == 1 || guard == 1))
        new_frac++;

    //sprawdzamy czy zaokrąglając mantysę nie przekroczymy 2
    if (new_frac >= (1 << 23))
    {
        e++;
        new_frac ^= (1 << 23);
    }

    // jeśli mamy zbyt duży wykładnik, zwracamy inf
    if (e > 253)
        return (sign << 31) | 0x7F800000;

    // jeśli mamy zbyt mały wykładnik, zwracamy 0
    if (e < -23)
        return 0;
    
    // jeśli wykładnik wpada w przedział zdenormalizowanych
    if (e >= -23 && e <= 0)
    {
        //przesuwamy mantysę
        new_frac = new_frac >> -e;
        e = 0;
    }

    return (sign << 31) | (e << 23) | new_frac;
}
```

### Zadanie 8

:::info
Uzupełnij ciało funkcji zadeklarowanej następująco:
```cpp
  /* Skonwertuj reprezentację liczby float do wartości int32_t. */ 
  int32_t float2int(int32_t f);
```
  Zaokrąglij liczbę w kierunku zera. Jeśli konwersja spowoduje nadmiar lub  $f$ ma wartość ```NaN```, zwróć ```0x80000000```. Dla czytelności napisz najpierw rozwiązanie z instrukcjami warunkowymi. Potem przepisz je, by zachować  zgodność z wytycznymi z nagłówka listy.
  
Uwaga: prowadzący zezwala na użycie instrukcji ```if``` i porównań.
:::

```c=
int float2int(int f){
    //0x7FFFFF =   0b00000000011111111111111111111111
    //0x7F800000 = 0b01111111100000000000000000000000
//1.
    /*
    Wyciągamy wszystkie części float'a
    i dosuwamy je do prawej strony.
    Każdy bit sign jest równy bitowi znaku.
    */
    int sign = f>>31;//znak
    int mant = 0x7FFFFF&f;//mantysa
    int exp = (0x7F800000&f)>>23;//cecha
    
//2.
    /*
    (mant | 1<<23) - dopisujemy do mantysy
    ukrytą jedynkę sprzed przecinka.
    (exp-127) - pozbywamy się uprzedzenia cechy
    Przemnażamy mantysę przez 2^cecha - uzyskujemy
    wartość bezwzględną wyniku.
    +-23 - bierzemy pod uwagę, że mantysa, to tak naprawdę liczby po przecinku
    */
    /*
    sign jest całe wypełnione tym samym bitem,
    dlatego and'owanie z sign działa.
    Jeśli f jest ujemna, to odwracamy res,
    inaczej zostawiamy bez zmian.
    */
    int res;
    if(sign){
        res = (mant | 1<<23)>>(exp-127+23);
        res = -res;
    }
    else{
        (mant | 1<<23)<<(exp-127-23);
    }
    
    //bitowo:
    /*
    int res = sign & (mant | 1<<23)>>(exp-127+23) + ~sign & (mant | 1<<23)<<(exp-127-23);
    res = sign & -res + ~sign & res;
    */
    
//3.
    /*
    Sprawdzamy NaN i infinity(oba mają mantysę równą 0b11111111).
    (b<<31)>>31 propaguje nam wartość b[0] na każdy
    bit (czego potrzebujemy, by nasz bitowy "if" działał).
    */
    
    if(exp==0b11111111){
        res = 0x80000000;
    }
    //bitowo:
    int mask = ((exp==0b11111111)<<31)>>31
    res = mask & 0x80000000 + ~mask & res;
    
//4.
    /*
    Sprawdzamy nadmiar.
    Nasz wynik zajmuje 24 najmłodsze bity res. 
    Zatem największe exp nie powodujące nadmiaru to 7 
    (bo potem wejdziemy z ukrytym bitem sprzed przecinka
    (dopisanym w 2.) na bit znaku).
    */
    
    //można to zapisać bitowo, ale dla czytelności zostawmy tak
    if(exp>7){
        res = 0x80000000;
    }
    
    return res;
    
}
```

