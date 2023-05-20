/*
 * @Descripttion: 
 * @Author: Jerry
 * @Date: 2021-12-01 16:39:12
 * @LastEditTime: 2022-01-04 15:31:29
 * 
 * Copyright © 2021 Jerry, All Rights Reserved
 */
#include "system.h"
#include "bsp_usart.h"
#include "osal.h"
#include "gd32f303e_eval.h"
#include "util.h"
#include "test_task.h"
#include "adc_task.h"
#include "communicate_task.h"

TaskHandle_t adc_TaskHandel = NULL;
TaskHandle_t communicate_TaskHandel = NULL;

void system_init(void)
{
    printf("Software Version: %s,%s\n\n", GET_SOFTWARE_VER_STR(), get_complie_time());
		
		xQueue_buffer = xQueueCreate(1,tcp_cache_size);					//	创建队列
		taskENTER_CRITICAL();				//	进临界段 关中断
    test_task_init();
//		adc_task_init();
//		communicate_task_init();
		taskEXIT_CRITICAL();

}
