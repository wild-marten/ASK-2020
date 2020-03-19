0x7F7F7F7F -> 01111111011111110111111101111111
0x80808080 -> 10000000100000001000000010000000

W obu przypadkach najpierw maskujemy najbardziej znaczący bit i wykonujemy zwykłe operacje na reszcie. 
Następnie osobno wyznaczamy najbardziej znaczący bit i go wstawiamy.

    Dodawanie:

s = (x & 0x7F7F7F7F) + (y & 0x7F7F7F7F)
s = ((x ^ y) | 0x80808080) ^ s

    Odejmowanie:

s = (x | 0x80808080) - (y & 0x7F7F7F7F)
s = ~( ((x ^ y) | 0x7F7F7F7F) ^ y)
