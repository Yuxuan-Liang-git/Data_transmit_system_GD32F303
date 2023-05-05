/*
 * @Descripttion: test task
 * @Author: Jerry
 * @Date: 2022-01-04 15:12:26
 * @LastEditTime: 2022-01-04 15:13:45
 * 
 * Copyright © 2021 Jerry, All Rights Reserved
 */

#ifndef _ADC_TASK_H_
#define _ADC_TASK_H_

#include "osal.h"
#include "gd32f30x.h"


void adc_task_init(void);
static TaskHandle_t adc_TaskHandel = NULL;
extern FlagStatus adc_task_FLAG;
#endif
