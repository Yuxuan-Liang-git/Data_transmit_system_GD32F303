#include "stdio.h"
#include "stdint.h"

#include "modbus_crc.h"

/* 构造符合ModbusRTU格式的数据。
  【注意】要将纸飞机中Modbus数据类型改为INT16，Modbus字节序改为LITTLE！
   param1[in]: 地址码
   param2[in]: 功能码
   param3[in]: 输入数据首地址
   param4[in]: 输入数据长度
   param5[out]: 输出数据首地址
   param5[out]: 输出数据
 */
int convert_to_rtu_data(uint8_t addr_code, uint8_t func_code, 
                        uint8_t* in_buff, uint8_t in_size, 
                        uint8_t* out_buff, uint8_t* out_size)
{
    uint16_t crc = 0;

    if (!in_buff)
        return -1;
    if (!out_buff)
        return -1;
    if (!out_size)
        return -1;
    
    out_buff[0] = addr_code;
    out_buff[1] = func_code;
    out_buff[2] = in_size;
    memcpy(&out_buff[3], in_buff, in_size);
    crc = crc_modbus(out_buff, in_size + 3);
    out_buff[in_size + 3] = (crc & 0x00FF) >> 0;
    out_buff[in_size + 4] = (crc & 0xFF00) >> 8;
    *out_size = in_size + 5;

    return 0;
}

// 将缓存buff发送出去（给纸飞机），请根据实际更换为你的发送函数
void send_buff(uint8_t* buff, uint8_t len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%c", buff[i]);
    }
}

int main()
{
    #define NUM_OF_DATA     2
    #define MAX_BUFF_SIZE   256

    // 指定一个首地址0x01
    uint8_t addr = 0x01;
    // 指定一个功能码0x03
    uint8_t func = 0x03;
    // 假定两个模拟数据放在一个长度为2的uint16类型的数组
    int16_t data[NUM_OF_DATA] = {300, 500};
    // 输出缓存用于存放转换结果
    uint8_t  buff[MAX_BUFF_SIZE] = {0};
    uint8_t  buff_len = 0;

    // 转换数据成ModbusRTU格式并放入到buff中
    // 【注意】要将纸飞机中Modbus数据类型改为INT16，Modbus字节序改为LITTLE！
    if (convert_to_rtu_data(addr, func, (uint8_t*)data, NUM_OF_DATA * sizeof(int16_t), buff, &buff_len))
    {
        printf("convert data failed!\n");
        return -1;
    }

    // 将缓存buff发送出去（给纸飞机），请根据实际更换为你的发送函数
    send_buff(buff, buff_len);

    return 0;
}