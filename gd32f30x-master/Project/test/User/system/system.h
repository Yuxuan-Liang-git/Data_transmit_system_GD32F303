/*
 * @Descripttion: 
 * @Author: Jerry
 * @Date: 2021-12-01 16:39:12
 * @LastEditTime: 2021-12-09 15:09:51
 * 
 * Copyright © 2021 Jerry, All Rights Reserved
 */
#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "config.h"
#include "FreeRTOS.h"
#include "semphr.h"		//	加上这个之后就找得到xSemaphoreHandle了

extern xQueueHandle xQueue_buffer;	
extern TaskHandle_t adc_TaskHandel;
extern TaskHandle_t communicate_TaskHandel;
void system_init(void);

#endif
