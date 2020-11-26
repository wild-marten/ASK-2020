# Ćwiczenia 12 z ASK, grupa AKr (04.06.2020)

### Zadanie 5.13

:::info
Suppose we wish to write a procedure that computes the inner product of two vectors u and v. An abstract version of the function has a CPE of 14–18 with x86-64 for different types of integer and floating-point data. By doing the same sort of transformations we did to transform the abstract program combine1 into the more efficient combine4, we get the following code:
```=
/* Inner product. Accumulate in temporary */
void inner4(vec_ptr u, vec_ptr v, data_t *dest)
{
    long i;
    long length = vec_length(u);
    data_t *udata = get_vec_start(u);
    data_t *vdata = get_vec_start(v);
    data_t sum = (data_t) 0;
    for (i = 0; i < length; i++) {
        sum = sum + udata[i] * vdata[i];
    }
    *dest = sum;
}
```
Our measurements show that this function has CPEs of 1.50 for integer data and 3.00 for floating-point data. For data type double, the x86-64 assembly code for the inner loop is as follows:
```=
/*Inner loop of inner4. data_t = double, OP = *
udata in %rbp, vdata in %rax, sum in %xmm0
i in %rcx, limit in %rbx*/
.L15:                                     // loop:
    vmovsd 0(%rbp,%rcx,8), %xmm1          // Get udata[i]
    vmulsd (%rax,%rcx,8), %xmm1, %xmm1    // Multiply by vdata[i]
    vaddsd %xmm1, %xmm0, %xmm0            // Add to sum
    addq $1, %rcx                         // Increment i
    cmpq %rbx, %rcx                       // Compare i:limit
    jne .L15                              // If !=, goto loop
```
Assume that the functional units have the characteristics listed in Figure:
![](https://i.imgur.com/HCfd9TI.png)
 
1. Diagram how this instruction sequence would be decoded into operations and show how the data dependencies between them would create a critical path of operations, in the style of Figures

 ![](https://i.imgur.com/dDidEdz.png)

 ![](https://i.imgur.com/Ghb0IKU.png)

2. For data type double, what lower bound on the CPE is determined by the critical path?
3. Assuming similar instruction sequences for the integer code as well, what lower bound on the CPE is determined by the critical path for integer data?
4. Explain how the floating-point versions can have CPEs of 3.00, even though the multiplication operation requires 5 clock cycles.
:::


1. Diagram how this instruction sequence would be decoded into operations and show how the data dependencies between them would create a critical path of operations.

![](https://i.imgur.com/irRqxOV.png)

2. For data type double, what lower bound on the CPE is determined by the critical path?

Ścieżka krytyczna utworzona jest poprzez operacje dodawania na zmiennej `sum`. Zatem dolna granica CPE to równowartość kosztu dodawania na liczbach zmiennoprzecinokowych podwójnej precyzji.

3. Assuming similar instruction sequences for the integer code as well, what lower bound on the CPE is determined by the critical path for integer data?

Ponieważ ścieżka krytyczna pozostaje niezmienna to dolna granica CPE to 1.0.

4. Explain how the floating-point versions can have CPEs of 3.00, even though the multiplication operation requires 5 clock cycles.

Ponieważ operacja mnożenia nie leży na ścieżce krytycznej to może zostać przetworzona potokowo (mogła zacząć się wykonywać podczas poprzedniej iteracji).


### Zadanie 5.14

:::info
Write a version of the inner product procedure described in Problem 5.13 that uses 6 × 1 loop unrolling. For x86-64, our measurements of the unrolled version give a CPE of 1.07 for integer data but still 3.01 for both floating-point data.
1. Explain why any (scalar) version of an inner product procedure running on an Intel Core i7 Haswell processor cannot achieve a CPE less than 1.00.
2. Explain why the performance for floating-point data did not improve with loop unrolling.
:::

```c=
//////6x1
void inner4(vec_ptr u,vec_ptr v,data_t *dest)
{
    long i=0;
    long length=vec_length(u);
    //od limitu odejmujemy 5 zeby nie wyjsc poza tablice
    long limit=length-5;           
    data_t* udata=get_vec_start(u);
    data_t* vdata=get_vec_start(v);
    data_t sum=(data_t)0;
    //jedna iteracja na 6 elementow tablicy wynikowej
    for(i=0;i<limit;i+=6)
    {
        sum+=udata[i]*vdata[i];
        sum+=udata[i+1]*vdata[i+1];
        sum+=udata[i+2]*vdata[i+2];
        sum+=udata[i+3]*vdata[i+3];
        sum+=udata[i+4]*vdata[i+4];
        sum+=udata[i+5]*vdata[i+4];
    }
    for(;i<length;i++)
    {
        sum+=udata[i]*vdata[i];
    }
    *dest=sum;
}
```

 6 × 1 loop unrolling jest technika ktora pozwala zoptymalizowac branch misprediction i usprawnia potokowane operacje. Zamiast jednej operacji na iteracje robimy ich az 6, dzieki czemu nasz algorytm ma 6 razy mniej skokow warunkowych.
Niestety kazde kolejne dodawanie jakie robimy wciaz jest zalezne od wyniku poprzedniego dodawania.

:::info
1. Explain why any (scalar) version of an inner product procedure running on an Intel Core i7 Haswell processor cannot achieve a CPE less than 1.00.
:::
Dzieje sie tak poniewaz na naszej "sciezce krytycznej" wciaz kazde dodawanie jest zalezne od wyniku poprzedniego ( co widac na diagramie ), dlatego nie mozemy przekroczyc lower bound CPE dodawania (ktore dla liczb calkowitych jest rowne 1.00 a dla zmienno przecinkowych 3.00).
:::info
2. Explain why the performance for floating-point data did not improve with loop unrolling.
:::
Poprzednia wersja dla liczb zmienno przecinkowych osiagnela juz lower bound CPE, a wiec loop unrolling tego nie zoptymalizuje bardziej.

---

Na grafie zaznaczylem sciezke krytyczna na czerwono, i optymalizacje wzgledem wersji z zadania 5.14 na zielono.

![](https://i.imgur.com/zq80WhA.png)

### Zadanie 5.15

:::info
Write a version of the inner product procedure described in Problem 5.13 that uses 6 × 6 loop unrolling. Our measurements for this function with x86-64 give a CPE of 1.06 for integer data and 1.01 for floating-point data.
What factor limits the performance to a CPE of 1.00?
:::

```c=
void fun(vec_ptr u, vec_ptr v, data_t *dest){
    long i;
    long length = vec_length(u);
    data_t *udata = get_vec_start(u);
    data_t *vdata = get_vec_start(v);
    data_t sum0 = (data_t) 0;
    data_t sum1 = (data_t) 0;
    data_t sum2 = (data_t) 0;
    data_t sum3 = (data_t) 0;
    data_t sum4 = (data_t) 0;
    data_t sum5 = (data_t) 0;
    long limit = length-6;

    for (i = 0; i < limit; i+=6) {
        sum0 = sum0 + udata[i] * vdata[i];
        sum1 = sum1 + udata[i+1] * vdata[i+1];
        sum2 = sum2 + udata[i+2] * vdata[i+2];
        sum3 = sum3 + udata[i+3] * vdata[i+3];
        sum4 = sum4 + udata[i+4] * vdata[i+4];
        sum5 = sum5 + udata[i+5] * vdata[i+5];
    }
    sum0 = sum0 + sum1 + sum2 + sum3 + sum4 + sum5;
    for(; i < length; i++){
        sum0 = sum0 + udata[i] * vdata[i];
    }
    
    *dest = sum;
}
```
What factor limits the performance to a CPE of 1.00?
Throughput bound

![](https://i.imgur.com/KpZ4qxJ.png)

### Zadanie 5.16

:::info
Write a version of the inner product procedure described in Problem 5.13 that uses 6 × 1a loop unrolling to enable greater parallelism. Our measurements for this function give a CPE of 1.10 for integer data and 1.05 for floating-point data.
:::


![](https://i.imgur.com/mpPPWoq.png)


```c=

void inner6x1a(vec_ptr u, vec_ptr v, data_t *dest) {
  long i;
  long length = vec_length(u);
  data_t *udata = get_vec_start(u);
  data_t *vdata = get_vec_start(v);
  data_t sum = (data_t) 0;
  long limit = length - 5;

  for (i = 0; i < limit; i+=6) {
    sum = sum +  (((udata[i] * vdata[i]) 
                + (udata[i + 1] * vdata[i + 1]))
                + ((udata[i + 2] * vdata[i + 2])
                + (udata[i + 3] * vdata[i + 3])) 
                + ((udata[i + 4] * vdata[i + 4]) 
                + (udata[i + 5] * vdata[i + 5])));
  }
  for (; i < length; i++)
    sum += udata[i] * vdata[i];

  *dest = sum;
}

```

![](https://i.imgur.com/kP0QQqZ.png)
