
#ifndef _COMMUNICATE_TASK_H_
#define _COMMUNICATE_TASK_H_

#include "osal.h"
#include "gd32f30x.h"

extern uint8_t communicate_data[2048];
void communicate_task_init(void);
static TaskHandle_t communicate_TaskHandel = NULL;

#endif
