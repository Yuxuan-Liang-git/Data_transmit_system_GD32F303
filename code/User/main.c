#include "main.h"
#include "w5500.h"
#include "W5500_conf.h"
#include "socket.h"
#include "utility.h"

uint16_t address;
//MGAS-C3-LED

int main(void)
{
	systick_config(); // ��ʼ��systick��ʱ��
	Delay_Timer_Init();
	led_init();
	addr_init();
	gd_usart_com_init(DEV_COM0);
//	spi_flash_init();
	
	gpio_for_w5500_config();						        /*��ʼ��MCU�������*/
	reset_w5500();											/*Ӳ��λW5500*/
	set_w5500_mac();										/*����MAC��ַ*/
	set_w5500_ip();											/*����IP��ַ*/
	
	socket_buf_init(txsize, rxsize);		/*��ʼ��8��Socket�ķ��ͽ��ջ����С*/
	printf(" W5500���Ժ͵��Ե�UDP�˿�ͨѶ \r\n");
	printf(" W5500�ı��ض˿�Ϊ:%d \r\n",local_port);
	printf(" Զ�˶˿�Ϊ:%d \r\n",remote_port);
	printf(" ���ӳɹ���PC���������ݸ�W5500��W5500�����ض�Ӧ���� \r\n");
  
	led_on();
	while(1)
	{
		led_on();
		delay_1ms(1000);
		led_off();
		delay_1ms(100);
		
		address=get_addr();
		printf("Address is:%d \n\r",address);
//		
		
		
		
		do_udp();                         /*UDP ���ݻػ�����*/
	}
	
    return 0;
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(DEV_COM0, (uint8_t)ch);
    while(RESET == usart_flag_get(DEV_COM0, USART_FLAG_TBE));
    return ch;
}
