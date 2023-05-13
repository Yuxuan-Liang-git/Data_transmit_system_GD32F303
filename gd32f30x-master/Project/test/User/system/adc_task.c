#include "gd32f303e_eval.h"
#include "adc_task.h"
uint8_t adc_value[tcp_cache_size];
uint8_t *ptr; 

/*******************************************************************/
/***       			    Local Function                           ***/
/*******************************************************************/
static void adc_task(void *para)
{
	uint32_t uNotifyValue;
	uint16_t i;
	BaseType_t xReturn = pdTRUE;/* 定义一个创建信息返回值，默认为 pdPASS */ 
	while(1)
	{
		xReturn = xTaskNotifyWait(0x0,0xffffffff,&uNotifyValue,portMAX_DELAY);
		if ( pdTRUE == xReturn ) 
		{ 
			if(uNotifyValue &0x01)
			{
				for (i = 0;i<dma_cache_size;i++)
				{
						ptr = (uint8_t *)&raw_data[i];
						adc_value[4*i+3] = *(ptr+0);
						adc_value[4*i+2] = *(ptr+1);
						adc_value[4*i+1] = *(ptr+2);
						adc_value[4*i+0] = *(ptr+3);
				}
			}
			else if(uNotifyValue &0x02)
			{
				for (i = 0;i<dma_cache_size;i++)
				{
						ptr = (uint8_t *)&raw_data[i+dma_cache_size];
						adc_value[4*i+3] = *(ptr+0);
						adc_value[4*i+2] = *(ptr+1);
						adc_value[4*i+1] = *(ptr+2);
						adc_value[4*i+0] = *(ptr+3);
				}			
			}
			xQueueSendToBack(xQueue_buffer,adc_value,portMAX_DELAY);
		} 
	}
}

/******************************************************************/
/***                    Exported Functions                      ***/
/******************************************************************/
/**
 * @brief init test task.
 */
void adc_task_init(void)
{
		xTaskCreate(adc_task, "adc_task", 256, NULL, 3, &adc_TaskHandel);
}
