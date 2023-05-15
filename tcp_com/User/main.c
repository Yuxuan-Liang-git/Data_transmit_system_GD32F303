#include "main.h"
#include "string.h"

#include "tcp.h"

uint16_t address;
uint16_t delay_time;
uint32_t apb1_clk,apb2_clk,ahb_clk,sys_clk;
uint32_t temp;
uint32_t ADC0_0,ADC0_1;
FlagStatus finished_flag;
extern uint8_t send_data[4];

int main(void)
{
	SystemInit();			//	初始化时钟，使用内部8M振荡电路，用PLL倍频到120M
	nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
	systick_config(); // 初始化systick计时器
//	Delay_Timer_Init();		//	us级延时
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
		if(adc_dma_flag == ADC_DMA_HF)
		{
			uint16_t i;
			uint8_t *ptr; 
			for (i = 0;i<dma_cache_size;i++)
			{
					ptr = (uint8_t *)&raw_data[i];
					adc_value[4*i+3] = *(ptr+0);
					adc_value[4*i+2] = *(ptr+1);
					adc_value[4*i+1] = *(ptr+2);
					adc_value[4*i+0] = *(ptr+3);
			}
			do_tcp_communicate(adc_value,tcp_cache_size);
			adc_dma_flag = ADC_DMA_RST;
		}
		else if(adc_dma_flag == ADC_DMA_F)
		{
			uint16_t i;
			uint8_t *ptr; 
			for (i = 0;i<dma_cache_size;i++)
			{
					ptr = (uint8_t *)&raw_data[i+dma_cache_size];
					adc_value[4*i+3] = *(ptr+0);
					adc_value[4*i+2] = *(ptr+1);
					adc_value[4*i+1] = *(ptr+2);
					adc_value[4*i+0] = *(ptr+3);
			}
			do_tcp_communicate(adc_value,tcp_cache_size);
			adc_dma_flag = ADC_DMA_RST;
		}
	}
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
