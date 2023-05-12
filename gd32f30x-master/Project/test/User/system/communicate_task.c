#include "gd32f303e_eval.h"
#include "communicate_task.h"
#include "system.h"
#include "tcp.h"
#include "bsp_adc.h"
uint8_t tcp_cache[tcp_cache_size];
/*******************************************************************/
/***       			    Local Function                           ***/
/*******************************************************************/
static void communicate_task(void *para)
{
	while(1)
	{
		//	没有收到队列信息时就堵塞在这里
		xQueueReceive(xQueue_buffer,tcp_cache,portMAX_DELAY);
		do_tcp_communicate(tcp_cache,tcp_cache_size);
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
