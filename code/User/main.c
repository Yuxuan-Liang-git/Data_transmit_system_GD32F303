#include "main.h"
//#include "w5500.h"
//#include "W5500_conf.h"
//#include "socket.h"
//#include "utility.h"

#include "udp.h"

uint16_t address;
uint32_t apb1_clk,apb2_clk,ahb_clk,sys_clk;

int main(void)
{
	SystemInit();			//	��ʼ��ʱ�ӣ�ʹ���ڲ�8M�񵴵�·����PLL��Ƶ��120M
	systick_config(); // ��ʼ��systick��ʱ��
	Delay_Timer_Init();		//	us����ʱ
	led_init();
	addr_init();
	adc_init();
	gd_usart_com_init(DEV_COM0);
	
	udp_com_init();
	
	led_on();
	while(1)
	{
		led_on();
		delay_ms(1000);
		led_off();
		delay_ms(1000);
		
		address=get_addr();
		printf("Address is:%d \n\r",address);	
		
		do_udp();                         /*UDP ���ݻػ�����*/
		
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
