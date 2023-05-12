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
	while(1)
	{
			xTaskNotifyWait(0x00,0xffffffff,&uNotifyValue,portMAX_DELAY);	//	µÈ´ýDMAÖÐ¶Ï
			OS_MsDelay(1000);
			printf("Gotcha bitch!!!!!!! \n");
			printf("Gotcha bitch!!!!!!! \n \n");

//		uint16_t i;
//		if(uNotifyValue &0x01)
//		{
//			for (i = 0;i<dma_cache_size;i++)
//			{
//					ptr = (uint8_t *)&raw_data[i];
//					adc_value[4*i+3] = *(ptr+0);
//					adc_value[4*i+2] = *(ptr+1);
//					adc_value[4*i+1] = *(ptr+2);
//					adc_value[4*i+0] = *(ptr+3);
//			}
//		}
//		else if(uNotifyValue &0x02)
//		{
//			for (i = 0;i<dma_cache_size;i++)
//			{
//					ptr = (uint8_t *)&raw_data[i+dma_cache_size];
//					adc_value[4*i+3] = *(ptr+0);
//					adc_value[4*i+2] = *(ptr+1);
//					adc_value[4*i+1] = *(ptr+2);
//					adc_value[4*i+0] = *(ptr+3);
//			}			
//		}
				
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
		xTaskCreate(adc_task, "adc_task", 256, NULL, OS_TASK_PRIO2, &adc_TaskHandel);
}
