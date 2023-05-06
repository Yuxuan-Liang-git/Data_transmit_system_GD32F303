#include "bsp_adc.h"
#include "bsp_delay.h"
#include "adc_task.h"
#include "gd32f30x.h"
#include <string.h>

xSemaphoreHandle binIRQSemaphore;		//	��ֵ�ź���
xQueueHandle xQueue_buffer;					//	��Ϣ���о��


uint8_t adc_value[64];
uint8_t cache_data[2048];


//	adc������������⣬����ADC_SAMPLETIME

void adc_init(void)
{
		adc_rcu_config();
		adc_gpio_config();
		nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
		
		binIRQSemaphore = xSemaphoreCreateBinary();		//	������ֵ�ź�����ʵ���ж��������ͬ��
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
//	�ƺ�����ƵҲ�� Ӧ�ú�ϵͳʱ��һ�� ת���ٶȻ��һЩ
//		/* config ADC clock */
//		rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV6);
}

//	ʱ��120M
/*!
    \brief      configure the timer peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void timer_config(void)
{
		timer_parameter_struct timer_initpara; //��ʱ���ṹ�� 
		rcu_periph_clock_enable(RCU_TIMER1); //������ʱ��ʱ��

//		timer_struct_para_init(&timer_initpara);//����ʱ���ṹ���ڲ������ó�Ĭ�ϲ���
		timer_deinit(TIMER1); //��λ��ʱ��

		//����TIMER1��ʱ��Ϊ120M/60/50  25us����һ��
		timer_initpara.prescaler         = 120-1;//Ԥ��Ƶ
		timer_initpara.alignedmode       = TIMER_COUNTER_EDGE; //��Ե����
		timer_initpara.counterdirection  = TIMER_COUNTER_UP; //���ϼ�����ʽ
		timer_initpara.period            = 100-1; //����ֵ
		timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
//		timer_initpara.repetitioncounter = 0; //�����ظ�������ֵ��0��ʾ���ظ�������ÿ����������������¼�
		timer_init(TIMER1,&timer_initpara);
//		timer_auto_reload_shadow_enable(TIMER1);//ʹ���Զ��ؼ���

		timer_interrupt_enable(TIMER1,TIMER_INT_UP);//ʹ������ж�

		nvic_irq_enable(TIMER1_IRQn, 1, 1);//�����ж����ȼ�
		timer_enable(TIMER1);//ʹ�ܶ�ʱ��  
	
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
//		dma_data_parameter.periph_inc = DMA_PERIPH_INCREASE_ENABLE;
    dma_data_parameter.memory_addr = (uint32_t)(&raw_data);
    dma_data_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_data_parameter.periph_width = DMA_PERIPHERAL_WIDTH_32BIT;
    dma_data_parameter.memory_width = DMA_MEMORY_WIDTH_32BIT;
    dma_data_parameter.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_data_parameter.number = 16;		//	16·ͨ����16������
    dma_data_parameter.priority = DMA_PRIORITY_HIGH;
    dma_init(DMA0, DMA_CH0, &dma_data_parameter);
  
    dma_circulation_enable(DMA0, DMA_CH0);
  
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
    adc_mode_config(ADC_DAUL_REGULAL_PARALLEL);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
		/* ADC scan mode function enable */
    adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);

    /* ADC channel length config */
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL,16);
    /* ADC regular channel config */
		//	�źű仯��ΪѸ�٣���Ҫ�̵Ĳ���ʱ��SAMPLETIME 15��ͨ��
		for (i = 0; i < 16; i++) 
		{
				// ��ÿ��ͨ�����д���
				adc_regular_channel_config(ADC0, i, adc_channels[i], ADC_SAMPLETIME_28POINT5);
		}
    /* ADC trigger config */
		adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
   
    /* ADC external trigger enable */
		//	ֻ��Ҫ������
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);
    /* enable ADC interface */
    /* ADC DMA function enable */
    adc_dma_mode_enable(ADC0);
		adc_enable(ADC0);
		delay_ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);
		nvic_irq_enable(ADC0_1_IRQn, 0,0);
		// ���ADC������ת�������жϱ�־
		adc_interrupt_flag_clear(ADC0,ADC_INT_FLAG_EOC);
		//	ʹ��ADC������ת�������ж�
		adc_interrupt_enable(ADC0,ADC_INT_EOC);
		
		adc_software_trigger_enable(ADC0,ADC_REGULAR_CHANNEL);


}
//	100us����һ���ж�
void TIMER1_IRQHandler(void)
{
		if(SET == timer_interrupt_flag_get(TIMER1,TIMER_INT_UP)){
//		adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);	
		/* clear TIMER interrupt flag */
			
		adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);		
			
		BaseType_t xHighPriorityTaskWoken = pdFALSE;
		//	�ͷŶ�ֵ�ź������ͷųɹ���	xHighPriorityTaskWoken = pdTRUE
		xSemaphoreGiveFromISR(binIRQSemaphore,&xHighPriorityTaskWoken);
		//	��鵱ǰ�Ƿ��и������ȼ���������Ҫ���У�����У��������л���������
		portYIELD_FROM_ISR(xHighPriorityTaskWoken);
			
		timer_interrupt_flag_clear(TIMER1,TIMER_INT_UP);
			
			
//		eTaskState state = eTaskGetState(adc_TaskHandel);
			
			
//		if(adc_task_FLAG == RESET)
//		{
//			adc_task_FLAG = SET;
//		}
			
//		if(adc_finish_flag == SET)
//		{
//			
//			if(count<32)	//	6.4ms��һ��
//			{
//				memcpy(cache_data+64*count,adc_value,64);
////				printf("{plotter:%d}\n", raw_data[0]);
//				count++;
//			} 
//			else
//			{
//				memcpy(tcp_buffer,cache_data,2048);
//				memset(cache_data,0,sizeof cache_data);
//				memcpy(cache_data,adc_value,64);	
//				count = 1;
//				send_flag = SET;
//			}
//			
////			send_flag = SET;
//			
//			adc_finish_flag = RESET;
//		}

    }
}

void ADC0_1_IRQHandler(void)
{
		uint8_t i;
		uint8_t * ptr; 
		adc_regular_data_read(ADC0);
		for (i = 0;i<16;i++)
		{
				ptr = (uint8_t *)&raw_data[i];
				adc_value[4*i+3] = *(ptr+0);
				adc_value[4*i+2] = *(ptr+1);
				adc_value[4*i+1] = *(ptr+2);
				adc_value[4*i+0] = *(ptr+3);
		}
		adc_finish_flag = SET;
		
//		printf("{plotter:%d}\n", raw_data[0]);
		

		adc_interrupt_flag_clear(ADC0,ADC_INT_FLAG_EOC);
}

uint32_t *get_raw_data(void)
{
	return (uint32_t *)&raw_data;
}


