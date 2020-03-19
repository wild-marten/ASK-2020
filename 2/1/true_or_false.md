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
