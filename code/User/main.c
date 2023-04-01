#include "main.h"

//#include "udp.h"
#include "tcp.h"


uint8 data[10];

uint16_t address;
uint32_t apb1_clk,apb2_clk,ahb_clk,sys_clk;

int main(void)
{
	uint8 i;
	SystemInit();			//	初始化时钟，使用内部8M振荡电路，用PLL倍频到120M
	systick_config(); // 初始化systick计时器
	Delay_Timer_Init();		//	us级延时
	led_init();
	addr_init();
	adc_init();
	gd_usart_com_init(DEV_COM0);
	
	led_on();
	address=get_addr();
	printf("Address is:%d \n\r",address);	
	tcp_com_init(address);
	

	while(1)
	{
		led_on();
		delay_ms(1000);
		led_off();
		delay_ms(1000);
		

		
		do_tcp_client();                  /*TCP_Client 数据回环测试程序*/    

//		for(i=0;i<10;i++)
//		{
//			data[i]=i+48;
//			do_tcp_communicate(data,10);
//			delay_ms(1000);
//		}
//		for(i=0;i<10;i++)
//		{
//			data[i]=0;
//		}
//		
	}
	
	return 0;
}
void get_clk(void)
{
		apb1_clk = rcu_clock_freq_get(CK_APB1);		
		apb2_clk = rcu_clock_freq_get(CK_APB2);
		ahb_clk = rcu_clock_freq_get(CK_AHB);
		sys_clk = rcu_clock_freq_get(CK_SYS);
}
/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(DEV_COM0, (uint8_t)ch);
    while(RESET == usart_flag_get(DEV_COM0, USART_FLAG_TBE));
    return ch;
}
