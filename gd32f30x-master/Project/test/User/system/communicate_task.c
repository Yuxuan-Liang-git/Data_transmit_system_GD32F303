#include "gd32f303e_eval.h"
#include "communicate_task.h"
#include "system.h"
#include "tcp.h"

/*******************************************************************/
/***       			    Local Function                           ***/
/*******************************************************************/
static void communicate_task(void *para)
{
	uint8_t cache_data[2048];
	while(1)
	{
        OS_MsDelay(1000);		
//		//	没有收到队列信息时就堵塞在这里
//		xQueueReceive(xQueue_buffer,cache_data,portMAX_DELAY);
//		do_tcp_communicate(cache_data,2048);
//				
	}
}

/******************************************************************/
/***                    Exported Functions                      ***/
/******************************************************************/
/**
 * @brief init communicate task
 */
void communicate_task_init(void)
{
	//	FreeRTOS里优先级是数量越大优先级越高
		xTaskCreate(communicate_task, "communicate_task", 2048, NULL, OS_TASK_PRIO1, &communicate_TaskHandel);
}
