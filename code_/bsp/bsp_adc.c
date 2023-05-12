#include "bsp_adc.h"
#include "gd32f30x.h"
#include <string.h>
#include "tcp.h"

//	adc采样如果有问题，调调ADC_SAMPLETIME

uint32_t raw_data[dma_cache_size*2];		//	DMA双缓冲区
uint8_t adc_value[tcp_cache_size];
ADC_DMA_FLAG adc_dma_flag;

uint32_t temp_data[dma_cache_size*2/16];
void adc_init(void)
{
		adc_rcu_config();
		adc_gpio_config();
//		nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    /* TIMER configuration */
    timer_config();
    /* DMA configuration */
    dma_config();
    /* ADC configuration */
    adc_config();
}

void adc_gpio_config(void)
{
	uint8_t i;
	for(i = 0 ;i<16;i++)
	{
		gpio_init(adc_gpio_port[i], GPIO_MODE_AIN, GPIO_OSPEED_MAX, adc_gpio_pin[i]);		
	}
}

void adc_rcu_config(void)
{
		rcu_periph_clock_enable(RCU_GPIOA);
		/* enable DMA clock */
		rcu_periph_clock_enable(RCU_DMA0);
		/* enable TIMER0 clock */
		rcu_periph_clock_enable(RCU_TIMER0);
		/* enable ADC0 clock */
		rcu_periph_clock_enable(RCU_ADC0);
//	似乎不分频也行 应该和系统时钟一致 转换速度会快一些
//		/* config ADC clock */
//		rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV6);
}

//	时钟120M
/*!
    \brief      configure the timer peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void timer_config(void)
{
		timer_parameter_struct timer_initpara; //定时器结构体 
		rcu_periph_clock_enable(RCU_TIMER1); //开启定时器时钟

//		timer_struct_para_init(&timer_initpara);//将定时器结构体内参数配置成默认参数
		timer_deinit(TIMER1); //复位定时器

		//配置TIMER1，时钟为120M/60/50  25us触发一次
		timer_initpara.prescaler         = 120-1;//预分频
		timer_initpara.alignedmode       = TIMER_COUNTER_EDGE; //边缘对齐
		timer_initpara.counterdirection  = TIMER_COUNTER_UP; //向上计数方式
		timer_initpara.period            = 100-1; //计数值
		timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
//		timer_initpara.repetitioncounter = 0; //设置重复计数器值，0表示不重复计数，每次溢出都产生更新事件
		timer_init(TIMER1,&timer_initpara);
//		timer_auto_reload_shadow_enable(TIMER1);//使能自动重加载

		timer_interrupt_enable(TIMER1,TIMER_INT_UP);//使能溢出中断

		nvic_irq_enable(TIMER1_IRQn, 1, 0);//配置中断优先级
		timer_enable(TIMER1);//使能定时器  
	
}

/*!
    \brief      configure the DMA peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dma_config(void)
{
    /* ADC_DMA_channel configuration */
    dma_parameter_struct dma_data_parameter;
    
    /* ADC_DMA_channel deinit */
    dma_deinit(DMA0, DMA_CH0);
    
    /* initialize DMA single data mode */
    dma_data_parameter.periph_addr = (uint32_t)(&ADC_RDATA(ADC0));
    dma_data_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_data_parameter.memory_addr = (uint32_t)(&raw_data);
    dma_data_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_data_parameter.periph_width = DMA_PERIPHERAL_WIDTH_32BIT;
    dma_data_parameter.memory_width = DMA_MEMORY_WIDTH_32BIT;
    dma_data_parameter.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_data_parameter.number = dma_cache_size*2;		
    dma_data_parameter.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA0, DMA_CH0, &dma_data_parameter);
  
    dma_circulation_enable(DMA0, DMA_CH0);
		dma_memory_to_memory_disable(DMA0,DMA_CH0);
		
		//	开启DMA中断、DMA半中断
		dma_interrupt_enable(DMA0,DMA_CH0,DMA_INT_FTF);
		dma_interrupt_enable(DMA0,DMA_CH0,DMA_INT_HTF);
  
    /* enable DMA channel */
    dma_channel_enable(DMA0, DMA_CH0);
		
		nvic_irq_enable(DMA0_Channel0_IRQn,2,0);
}

//	PA0~7: CH0 CH1 CH2 CH3 CH4 CH5 CH6 CH7
//	PB0~1: CH8 CH9
//	PC0~5: CH10 CH11 CH12 CH13 CH14 CH15
/*!
    \brief      configure the ADC peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_config(void)
{
		uint8_t i;
    /* configure the ADC sync mode */
    adc_mode_config(ADC_DAUL_REGULAL_PARALLEL);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
		/* ADC scan mode function enable */
    adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);
    /* ADC channel length config */
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL,16);
    /* ADC regular channel config */
		//	信号变化较为迅速，需要短的采样时间SAMPLETIME 15个通道
		for (i = 0; i < 16; i++) 
		{
				// 对每个通道进行处理
				adc_regular_channel_config(ADC0, i, adc_channels[i], ADC_SAMPLETIME_28POINT5);
		}
    /* ADC trigger config */	//	用定时器手动触发ADC采样
		adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
    /* ADC external trigger enable */
		//	只需要规则组 
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);
    /* enable ADC interface */
    /* ADC DMA function enable */
    adc_dma_mode_enable(ADC0);
		delay_1ms(1);
		adc_enable(ADC0);
		delay_1ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);
//		adc_software_trigger_enable(ADC0,ADC_REGULAR_CHANNEL);
}

void DMA0_Channel0_IRQHandler(void)
{
		uint16_t j;
		if(dma_interrupt_flag_get(DMA0,DMA_CH0,DMA_INT_FLAG_HTF))
		{
			dma_interrupt_flag_clear(DMA0,DMA_CH0,DMA_INT_FLAG_HTF);
			if(adc_dma_flag == ADC_DMA_RST)
			{
				adc_dma_flag = ADC_DMA_HF;
			}
	//		for(j=0;j<dma_cache_size*2/16;j++)
	//		{
	//			temp_data[j]=raw_data[j*16];
	//		}
		}
		else if(dma_interrupt_flag_get(DMA0,DMA_CH0,DMA_INT_FLAG_FTF))
		{
			dma_interrupt_flag_clear(DMA0,DMA_CH0,DMA_INT_FLAG_FTF);
			if(adc_dma_flag == ADC_DMA_RST)
			{
				adc_dma_flag = ADC_DMA_F;
			}
	//		for(j=0;j<dma_cache_size*2/16;j++)
	//		{
	//			temp_data[j]=raw_data[j*16];
	//		}
		}		
	

}

