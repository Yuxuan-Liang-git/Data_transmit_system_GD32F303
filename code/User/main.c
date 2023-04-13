#include "main.h"

//#include "udp.h"
#include "tcp.h"

uint8_t data[4];

uint16_t address;
uint32_t apb1_clk,apb2_clk,ahb_clk,sys_clk;
uint32_t temp;
uint32_t ADC0_0,ADC0_1;

int main(void)
{
	uint8 i,j;
	SystemInit();			//	��ʼ��ʱ�ӣ�ʹ���ڲ�8M�񵴵�·����PLL��Ƶ��120M
	systick_config(); // ��ʼ��systick��ʱ��
	Delay_Timer_Init();		//	us����ʱ
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
//		printf("{adc_value:");
//	
//		for(i=0;i<15;i++)
//		{
//			printf("%d,",adc_value[i]);
//		} 
//		printf("}\n");
		
		
		
//		printf("{plotter:%d}\n", adc_value[0]);
		
//		printf("{plotter:%d,%d,%d}\n", adc_value[0],adc_value[1],adc_value[2]);

		data[0] = adc_value[0] >> 24;
		data[1] = (adc_value[0] >> 16)&0xff;
		data[2] =( adc_value[0] >> 8)&0xff;
		data[3] = adc_value[0] &0xff ;
//		for(i=0;i<4;i++)
//		{
//			data[i]=adc_value[0] >> 8*i;
//		}
			do_tcp_communicate(data,4);


	
//		sprintf(data, "%lu", adc_value[0]);

//		do_tcp_communicate(data,4);
//		delay_ms(100);

//		for(i=0;i<10;i++)
//		{
//			data[i]=0;
//		}
		
	}
	
	return 0;
}

//	�ش�ϵͳʱ��
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
