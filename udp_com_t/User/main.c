#include "main.h"
#include "string.h"

#include "udp.h"

uint16_t address;
uint16_t delay_time;
uint32_t apb1_clk,apb2_clk,ahb_clk,sys_clk;

uint32_t ADC0_0,ADC0_1;
FlagStatus finished_flag;

uint32_t temp_cache[dma_cache_size*64];
uint8_t temp_value[tcp_cache_size];
uint8_t count;

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
	udp_com_init();
//	tcp_com_init(address);
	while(1)
	{
		do_udp();
//		if(adc_dma_flag == ADC_DMA_HF )
//		{
//			memcpy(&temp_cache[dma_cache_size*2*count],&raw_data[0],sizeof(raw_data)/2);
//			adc_dma_flag = ADC_DMA_RST;
//		}
//		else if(adc_dma_flag == ADC_DMA_F)
//		{
//			memcpy(&temp_cache[dma_cache_size*(2*count+1)],&raw_data[dma_cache_size],sizeof(raw_data)/2);
//			adc_dma_flag = ADC_DMA_RST;
//			count ++ ;
//		}
//		if(count == 4)
//		{
//			uint16_t i,j;
//			uint8_t *ptr; 
////			adc_disable(ADC0);
////			adc_disable(ADC1);
//			for(j=0;j<count;j++)
//			{
//				memset(temp_value,0,sizeof(temp_value));
//				for (i = 0;i<dma_cache_size;i++)
//				{
//						ptr = (uint8_t *)&temp_cache[i+j*dma_cache_size];
//						temp_value[4*i+3] = *(ptr+0);
//						temp_value[4*i+2] = *(ptr+1);
//						temp_value[4*i+1] = *(ptr+2);
//						temp_value[4*i+0] = *(ptr+3);
//				}
//				do_udp_communicate(temp_value,tcp_cache_size);
//			}			
//			count = 0;
////			break;
//		}

//			memcpy(&printf_cache[temp*dma_cache_size*2],raw_data,sizeof(raw_data));	
		
//		if(adc_dma_flag == ADC_DMA_HF)
//		{
//			adc_dma_flag = ADC_DMA_RST;
//		}
//		else if(adc_dma_flag == ADC_DMA_F)
//		{
//			uint16_t i;	
//			for (i = 0;i<2*dma_cache_size;i++)
//			{
//					printf("{plotter:%d}\n", printf_cache[16*i]);
//			}
//			adc_dma_flag = ADC_DMA_RST;
//			break;
//		}	
		
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
