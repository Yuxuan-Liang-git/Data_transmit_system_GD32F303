#include "bsp_adc.h"
#include "gd32f30x.h"
#include <string.h>
#include "tcp.h"

//	adc采样如果有问题，调调ADC_SAMPLETIME

uint32_t raw_data[dma_cache_size*2];		//	DMA双缓冲区
uint8_t adc_value[tcp_cache_size];
ADC_DMA_FLAG adc_dma_flag;


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
		/* config ADC clock */
		rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV2);
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
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    /* TIMER0 configuration */
    timer_initpara.prescaler         = 120-1;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 100;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER0, &timer_initpara);

    /* CH0 configuration in PWM mode0 */
    timer_channel_output_struct_para_init(&timer_ocintpara);
    timer_ocintpara.ocpolarity  = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_channel_output_config(TIMER0, TIMER_CH_0, &timer_ocintpara);

    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0, 30);
    timer_channel_output_mode_config(TIMER0, TIMER_CH_0, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER0, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);

    /* TIMER0 primary output enable */
    timer_primary_output_config(TIMER0, ENABLE);
    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER0);

    /* enable TIMER0 */
    timer_enable(TIMER0);
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
    dma_data_parameter.memory_addr = (uint32_t)(raw_data);
    dma_data_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_data_parameter.periph_width = DMA_PERIPHERAL_WIDTH_32BIT;
    dma_data_parameter.memory_width = DMA_MEMORY_WIDTH_32BIT;
    dma_data_parameter.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_data_parameter.number = dma_cache_size*2;		
    dma_data_parameter.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA0, DMA_CH0, &dma_data_parameter);
  
    dma_circulation_enable(DMA0, DMA_CH0);
		
		dma_interrupt_flag_clear(DMA0,DMA_CH0,DMA_INT_FLAG_HTF);
		dma_interrupt_flag_clear(DMA0,DMA_CH0,DMA_INT_FLAG_FTF);
		//	开启DMA中断、DMA半中断
		dma_interrupt_enable(DMA0,DMA_CH0,DMA_INT_FTF);
		dma_interrupt_enable(DMA0,DMA_CH0,DMA_INT_HTF);

		nvic_irq_enable(DMA0_Channel0_IRQn,0,0);
    /* enable DMA channel */
    dma_channel_enable(DMA0, DMA_CH0);

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
    adc_mode_config(ADC_MODE_FREE);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
		/* ADC scan mode function enable */
    adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);
    adc_special_function_config(ADC1, ADC_SCAN_MODE, ENABLE);
	
		adc_resolution_config(ADC0,ADC_RESOLUTION_12B);	
		adc_resolution_config(ADC1,ADC_RESOLUTION_12B);	
    /* ADC channel length config */
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL,16);
    adc_channel_length_config(ADC1, ADC_REGULAR_CHANNEL,16);
    /* ADC regular channel config */
		//	信号变化较为迅速，需要短的采样时间SAMPLETIME 15个通道
		for (i = 0; i < 8; i++) 
		{
				// 对每个通道进行处理
				adc_regular_channel_config(ADC0, i, adc_channels[i], ADC_SAMPLETIME_28POINT5);
		}
		for (i = 8; i < 16; i++) 
		{
				// 对每个通道进行处理
				adc_regular_channel_config(ADC1, i, adc_channels[i], ADC_SAMPLETIME_28POINT5);
		}
    /* ADC external trigger enable */
		//	只需要规则组 
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);
    adc_external_trigger_config(ADC1, ADC_REGULAR_CHANNEL, ENABLE);
    /* ADC trigger config */	
		adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_EXTTRIG_REGULAR_T0_CH0);
		adc_external_trigger_source_config(ADC1, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
    /* enable ADC interface */
    adc_enable(ADC0);
    delay_1ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);
    /* enable ADC interface */
    adc_enable(ADC1);
    delay_1ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC1);
		
    /* ADC DMA function enable */
    adc_dma_mode_enable(ADC0);
}

void DMA0_Channel0_IRQHandler(void)
{
		if(dma_interrupt_flag_get(DMA0,DMA_CH0,DMA_INT_FLAG_HTF))
		{
			dma_interrupt_flag_clear(DMA0,DMA_CH0,DMA_INT_FLAG_HTF);
			if(adc_dma_flag == ADC_DMA_RST)
			{
				adc_dma_flag = ADC_DMA_HF;
			}
		}
		else if(dma_interrupt_flag_get(DMA0,DMA_CH0,DMA_INT_FLAG_FTF))
		{
//			printf("DMA_INT_FLAG_FTF \n");
			dma_interrupt_flag_clear(DMA0,DMA_CH0,DMA_INT_FLAG_FTF);
			if(adc_dma_flag == ADC_DMA_RST)
			{
				adc_dma_flag = ADC_DMA_F;
			}
		}		
}

