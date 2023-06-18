#include "stdio.h"
#include "stdint.h"

#define PRINT(title, fmt, args...) printf("{"#title":"fmt"}\n", ##args)

int main()
{
    uint8_t data1 = 100;
    int16_t data2 = 200;
    float   data3 = 300.5432f;

    for (int32_t i = 0; i < 10; i++)
    {
        PRINT(plotter, "%d, %d, %.2f", data1, data2, data3);
        PRINT(count, "hello, print count:%d", i);
        PRINT(friend, "hello, my friend", i);
        data1++;
        data2++;
        data3++;
    }

    return 0;
}