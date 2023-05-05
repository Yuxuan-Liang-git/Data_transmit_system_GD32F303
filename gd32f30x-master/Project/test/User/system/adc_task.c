#include "gd32f303e_eval.h"
#include "bsp_adc.h"
#include "adc_task.h"

/*******************************************************************/
/***       			    Local Function                           ***/
/*******************************************************************/
static void adc_task(void *para)
{
	while(1)
	{
		//	û���յ���ֵ�ź���ʱ�Ͷ���������
		while(xSemaphoreTake(binIRQSemaphore,portMAX_DELAY)!=pdTRUE){}
		adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);		
			
//		//	���԰�raw_data��ӡ������
//		uint32_t *ptr;
//		ptr = get_raw_data();
//		printf("{plotter:%d}\n",*ptr);
			
			
			
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
