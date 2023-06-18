#include "stdio.h"
#include "stdint.h"

#include "modbus_crc.h"

/* �������ModbusRTU��ʽ�����ݡ�
  ��ע�⡿Ҫ��ֽ�ɻ���Modbus�������͸�ΪINT16��Modbus�ֽ����ΪLITTLE��
   param1[in]: ��ַ��
   param2[in]: ������
   param3[in]: ���������׵�ַ
   param4[in]: �������ݳ���
   param5[out]: ��������׵�ַ
   param5[out]: �������
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

// ������buff���ͳ�ȥ����ֽ�ɻ����������ʵ�ʸ���Ϊ��ķ��ͺ���
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

    // ָ��һ���׵�ַ0x01
    uint8_t addr = 0x01;
    // ָ��һ��������0x03
    uint8_t func = 0x03;
    // �ٶ�����ģ�����ݷ���һ������Ϊ2��uint16���͵�����
    int16_t data[NUM_OF_DATA] = {300, 500};
    // ����������ڴ��ת�����
    uint8_t  buff[MAX_BUFF_SIZE] = {0};
    uint8_t  buff_len = 0;

    // ת�����ݳ�ModbusRTU��ʽ�����뵽buff��
    // ��ע�⡿Ҫ��ֽ�ɻ���Modbus�������͸�ΪINT16��Modbus�ֽ����ΪLITTLE��
    if (convert_to_rtu_data(addr, func, (uint8_t*)data, NUM_OF_DATA * sizeof(int16_t), buff, &buff_len))
    {
        printf("convert data failed!\n");
        return -1;
    }

    // ������buff���ͳ�ȥ����ֽ�ɻ����������ʵ�ʸ���Ϊ��ķ��ͺ���
    send_buff(buff, buff_len);

    return 0;
}