#include "main.h"
#include "w5500.h"
#include "W5500_conf.h"
#include "socket.h"
#include "utility.h"

uint16_t address;
//MGAS-C3-LED

int main(void)
{
	systick_config(); // 初始化systick计时器
	Delay_Timer_Init();
	led_init();
	addr_init();
	gd_usart_com_init(DEV_COM0);
//	spi_flash_init();
	
	gpio_for_w5500_config();						        /*初始化MCU相关引脚*/
	reset_w5500();											/*硬复位W5500*/
	set_w5500_mac();										/*配置MAC地址*/
	set_w5500_ip();											/*配置IP地址*/
	
	socket_buf_init(txsize, rxsize);		/*初始化8个Socket的发送接收缓存大小*/
	printf(" W5500可以和电脑的UDP端口通讯 \r\n");
	printf(" W5500的本地端口为:%d \r\n",local_port);
	printf(" 远端端口为:%d \r\n",remote_port);
	printf(" 连接成功后，PC机发送数据给W5500，W5500将返回对应数据 \r\n");
  
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
		
		
		
		do_udp();                         /*UDP 数据回环测试*/
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
