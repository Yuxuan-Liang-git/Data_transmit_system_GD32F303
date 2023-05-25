#ifndef SYS_TICK_H
#define SYS_TICK_H
 
#include "gd32f30x_timer.h"
#define DELAY_TIMER				TIMER3
#define RCU_DELAY_TIMER			RCU_TIMER3
#define DELAY_TIMER_IRQHandler	TIMER3_IRQHandler
#define DELAY_TIMER_IRQn		TIMER3_IRQn
 
//定时器不分频
#define DELAY_TIMER_PRESCALER	0	//分频值0,频率和系统时钟一样
//1us的定时计数是
#define DELAY_TIMER_PERIOD		(SystemCoreClock / 1000 /1000)-1	//71	//SystemCoreClock / 1000 /1000
 
#define TMR_ENABLE()	TIMER_CTL0(DELAY_TIMER)|=(uint32_t)TIMER_CTL0_CEN	//开启定时器
#define TMR_DISABEL()	TIMER_CTL0(DELAY_TIMER) &= ~(uint32_t)TIMER_CTL0_CEN //关闭定时器
#define TMR_CLR_FLAG()	TIMER_INTF(DELAY_TIMER) = (~(uint32_t)TIMER_INT_FLAG_UP)	//清除中断标志位
#define TMR_SET_CNT(X)	TIMER_CNT(DELAY_TIMER) = (uint32_t)(X)	//配置计数器
 
extern uint16_t us_count;
extern uint16_t dur;
 
void Delay_Timer_Init(void);
void delay_1us(uint16_t count);
void delay_1ms(uint16_t count);

void start_count(void);
uint16_t get_dur(void);
 
#endif



//#ifndef SYS_TICK_H
//#define SYS_TICK_H

//#include <stdint.h>

///* configure systick */
//void systick_config(void);
///* delay a time in milliseconds */
//void delay_1ms(uint32_t count);

//void delay_1us(uint32_t count);

//void start_count(void);
//uint32_t get_dur(void);

//extern uint32_t dur;

/////* delay decrement */
////void delay_decrement(void);

//#endif /* SYS_TICK_H */
