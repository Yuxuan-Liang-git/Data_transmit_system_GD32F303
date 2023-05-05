#include "gd32f303e_eval.h"
#include "adc_task.h"

/*******************************************************************/
/***       			    Local Function                           ***/
/*******************************************************************/
static void adc_task(void *para)
{
	uint8_t count = 0;
	while(1)
	{
		//	û���յ���ֵ�ź���ʱ�Ͷ���������
		while(xSemaphoreTake(binIRQSemaphore,portMAX_DELAY)!=pdTRUE){}
			
//		//	���԰�raw_data��ӡ������
//		uint32_t *ptr;
//		ptr = get_raw_data();
//		printf("{plotter:%d}\n",*ptr);
			
		if(count<32)	
		{
			memcpy(cache_data+64*count,adc_value,64);
			count++;
		} 
		else
		{
		
			memset(cache_data,0,sizeof cache_data);
			memcpy(cache_data,adc_value,64);	
			count = 1;
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
		xTaskCreate(adc_task, "adc_task", 256, NULL, OS_TASK_PRIO2, &adc_TaskHandel);
}
