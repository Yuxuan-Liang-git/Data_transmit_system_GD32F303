#include "stdio.h"
#include "stdint.h"

//float转byte宏
#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

//发送函数(含2个数据)
void SendCurve(float data1, float data2)
{
    uint8_t dataBuff[4 + 4 + 4];      //缓存大小：(数据数量 + 1) * sizeof(float)
    int32_t cnt = 0;
    
    dataBuff[cnt++]=BYTE0(data1); //第一个数据，如电压
    dataBuff[cnt++]=BYTE1(data1);
    dataBuff[cnt++]=BYTE2(data1);
    dataBuff[cnt++]=BYTE3(data1);

    dataBuff[cnt++]=BYTE0(data2); //第二个数据，如电流
    dataBuff[cnt++]=BYTE1(data2);
    dataBuff[cnt++]=BYTE2(data2);
    dataBuff[cnt++]=BYTE3(data2);

    dataBuff[cnt++]=0x00;         //结束标志
    dataBuff[cnt++]=0x00;
    dataBuff[cnt++]=0x80;
    dataBuff[cnt++]=0x7F;

    // 该函数将dataBuff发送出去，可根据实际替换成你的发送函数
    for(int32_t i = 0; i < cnt; i++)
    {
        printf("%c",dataBuff[i]);
    }
}

int main()
{
    float data1 = 100.0f;
    float data2 = 200.0f;

    for (int32_t i = 0; i < 10; i++)
    {
        SendCurve(data1, data2);
        data1++;
        data2++;
    }

    return 0;
}
