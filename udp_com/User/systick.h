#ifndef SYS_TICK_H
#define SYS_TICK_H
 
#include "gd32f30x_timer.h"
#define DELAY_TIMER				TIMER3
#define RCU_DELAY_TIMER			RCU_TIMER3
#define DELAY_TIMER_IRQHandler	TIMER3_IRQHandler
#define DELAY_TIMER_IRQn		TIMER3_IRQn
 
//��ʱ������Ƶ
#define DELAY_TIMER_PRESCALER	0	//��Ƶֵ0,Ƶ�ʺ�ϵͳʱ��һ��
//1us�Ķ�ʱ������
#define DELAY_TIMER_PERIOD		(SystemCoreClock / 1000 /1000)-1	//71	//SystemCoreClock / 1000 /1000
 
#define TMR_ENABLE()	TIMER_CTL0(DELAY_TIMER)|=(uint32_t)TIMER_CTL0_CEN	//������ʱ��
#define TMR_DISABEL()	TIMER_CTL0(DELAY_TIMER) &= ~(uint32_t)TIMER_CTL0_CEN //�رն�ʱ��
#define TMR_CLR_FLAG()	TIMER_INTF(DELAY_TIMER) = (~(uint32_t)TIMER_INT_FLAG_UP)	//����жϱ�־λ
#define TMR_SET_CNT(X)	TIMER_CNT(DELAY_TIMER) = (uint32_t)(X)	//���ü�����
 
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
