
#include "gd32f303e_eval.h"
#include "bsp_adc.h"
#include "adc_task.h"

FlagStatus adc_task_FLAG = SET;

/*******************************************************************/
/***       			    Local Function                           ***/
/*******************************************************************/
static void adc_task(void *para)
{
		adc_init();
	
//    float f = 0.3f;
    while(1)
    {
			
			
//				adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);	
//				delay_1ms(1000);
			
        OS_MsDelay(1000);
				printf("Hello! \n");
				adc_task_FLAG = RESET;
				vTaskSuspend(adc_TaskHandel);

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
//		vTaskSuspend(adc_TaskHandel);

//	
//    OS_TaskCreate(adc_task, "adc_task", 256, NULL, OS_TASK_PRIO2, &adc_TaskHandel);
}
