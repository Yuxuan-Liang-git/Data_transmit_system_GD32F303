/*
 * @Descripttion: test task
 * @Author: Jerry
 * @Date: 2022-01-04 15:12:40
 * @LastEditTime: 2022-02-09 10:31:21
 * 
 * Copyright © 2021 Jerry, All Rights Reserved
 */
#include "stdio.h"
#include "osal.h"
#include "bsp_delay.h"

#include "test_task.h"

/*******************************************************************/
/***       			    Local Function                           ***/
/*******************************************************************/
static void test_task(void *para)
{
    while(1)
    {
				printf("task: %s, freestack: %d, proi: %d\n", OS_TaskGetName(NULL), OS_TaskGetFreeStackSpace(NULL), OS_TaskGetPriority(NULL));

        OS_SysInfo();
        OS_MsDelay(1000);			
    }
}

/******************************************************************/
/***                    Exported Functions                      ***/
/******************************************************************/
/**
 * @brief init test task.
 */
void test_task_init(void)
{
    OS_TaskCreate(test_task, "test_task", 256, NULL, OS_TASK_PRIO1, NULL);
}
