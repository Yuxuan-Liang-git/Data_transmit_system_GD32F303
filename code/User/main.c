#include "main.h"

#include "tcp.h"

uint16_t address;
uint16_t delay_time;
uint32_t apb1_clk,apb2_clk,ahb_clk,sys_clk;
uint32_t temp;
uint32_t ADC0_0,ADC0_1;



int main(void)
{
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

	delay_time = 5000;


	while(1)
	{
		if(send_flag == SET)
		{
			do_tcp_communicate(buffer,4096);
			send_flag = RESET;
		}
	}
	
	return 0;
}

//	回传系统时钟
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
