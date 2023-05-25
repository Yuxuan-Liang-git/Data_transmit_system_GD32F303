#include "systick.h"
#include "gd32f30x.h"
 
uint16_t us_count;
uint16_t dur;
/*----------------------------------------------------------------------------------
Function   :Delay_Timer_Init
Description:初始化延时函数定时器
Input      :无
Retrun     :无
----------------------------------------------------------------------------------*/
void Delay_Timer_Init(void)
{
	timer_parameter_struct tmr;
 
	rcu_periph_clock_enable(RCU_DELAY_TIMER);
	timer_deinit(DELAY_TIMER);

 /* TIMERx configuration */
	tmr.prescaler         = DELAY_TIMER_PRESCALER;
	tmr.alignedmode       = TIMER_COUNTER_EDGE;
	tmr.counterdirection  = TIMER_COUNTER_UP;
	tmr.period            = DELAY_TIMER_PERIOD;
	tmr.clockdivision     = TIMER_CKDIV_DIV1;
	tmr.repetitioncounter = 0;
	timer_init(DELAY_TIMER,&tmr);

	timer_interrupt_enable(DELAY_TIMER,TIMER_INT_UP); //使能更新中断
	nvic_irq_enable(DELAY_TIMER_IRQn,2,0); //使能中断线
	TMR_DISABEL();
}
 
 
/*----------------------------------------------------------------------------------
Function   :DELAY_TIMER_IRQHandler
Description:定时器中断
Input      :无
Retrun     :无
----------------------------------------------------------------------------------*/
void DELAY_TIMER_IRQHandler(void)
{
    //timer_interrupt_flag_clear(DELAY_TIMER, TIMER_INT_FLAG_UP); //清除中断标志位
	TMR_CLR_FLAG();
    us_count++;
}
 
 
/*----------------------------------------------------------------------------------
Function   :Delay_1us
Description:延时us
Input      :count:微秒数
Retrun     :无
----------------------------------------------------------------------------------*/
void delay_1us(uint16_t count)
{
	TMR_SET_CNT(0);	//timer_counter_value_config(DELAY_TIMER,0);	
	TMR_ENABLE();	//timer_enable
	us_count = 0;		
	while (us_count < count);
	TMR_DISABEL();	//timer_disable
}
 
void delay_1ms(uint16_t count)
{
   while (count--)
   {
	   delay_1us(1000);	//为了避开中断
   }          
} 	

void start_count(void)
{
	TMR_SET_CNT(0);	//timer_counter_value_config(DELAY_TIMER,0);	
	TMR_ENABLE();	//timer_enable
	us_count = 0;
}

uint16_t get_dur(void)
{
		uint16_t duration = us_count;
		TMR_DISABEL();
		return duration;
}


//#include "gd32f30x.h"
//#include "systick.h"
//volatile static uint32_t delay;
//uint32_t dur;

///*!
//    \brief      configure systick
//    \param[in]  none
//    \param[out] none
//    \retval     none
//*/
//void systick_config(void)
//{
//    /* setup systick timer for 1000000Hz interrupts */
//    if (SysTick_Config(SystemCoreClock / 1000000U)){
//        /* capture error */
//        while(1){
//        }
//    }
//    /* configure the systick handler priority */
//    NVIC_SetPriority(SysTick_IRQn, 0x00U);
//}

///*!
//    \brief      delay a time in milliseconds
//    \param[in]  count: count in milliseconds
//    \param[out] none
//    \retval     none
//*/
//void delay_1ms(uint32_t count)
//{
//    delay = count*1000;

//    while(0U != delay){
//    }
//}

//void delay_1us(uint32_t count)
//{
//    delay = count;

//    while(0U != delay){
//    }
//}

///*!
//    \brief      delay decrement
//    \param[in]  none
//    \param[out] none
//    \retval     none
//*/
//void delay_decrement(void)
//{
//    if(0U != delay){
//        delay--;
//    }
//}

//void start_count(void)
//{
//    delay = 10000;
//}

//uint32_t get_dur(void)
//{
//		uint32_t duration = 10000-delay;
//		delay = 0;
//		return duration;
//}









//#include "gd32f30x.h"
//#include "systick.h"

//volatile static float count_1us = 0;
//volatile static float count_1ms = 0;

///*!
//    \brief      configure systick
//    \param[in]  none
//    \param[out] none
//    \retval     none
//*/
//void systick_config(void)
//{
//    systick_clksource_set(SYSTICK_CLKSOURCE_HCLK);
//    count_1us = (float)SystemCoreClock/120000000;
//    count_1ms = (float)count_1us * 1000;
//}

///*!
//    \brief      delay a time in microseconds in polling mode
//    \param[in]  count: count in microseconds
//    \param[out] none
//    \retval     none
//*/
//void delay_1us(uint32_t count)
//{
//    uint32_t ctl;
//    
//    /* reload the count value */
//    SysTick->LOAD = (uint32_t)(count * count_1us);
//    /* clear the current count value */
//    SysTick->VAL = 0x0000U;
//    /* enable the systick timer */
//    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
//    /* wait for the COUNTFLAG flag set */
//    do{
//        ctl = SysTick->CTRL;
//    }while((ctl&SysTick_CTRL_ENABLE_Msk)&&!(ctl & SysTick_CTRL_COUNTFLAG_Msk));
//    /* disable the systick timer */
//    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
//    /* clear the current count value */
//    SysTick->VAL = 0x0000U;
//}

///*!
//    \brief      delay a time in milliseconds in polling mode
//    \param[in]  count: count in milliseconds
//    \param[out] none
//    \retval     none
//*/
//void delay_1ms(uint32_t count)
//{
//    uint32_t ctl;
//    
//    /* reload the count value */
//    SysTick->LOAD = (uint32_t)(count * count_1ms);
//    /* clear the current count value */
//    SysTick->VAL = 0x0000U;
//    /* enable the systick timer */
//    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
//    /* wait for the COUNTFLAG flag set */
//    do{
//        ctl = SysTick->CTRL;
//    }while((ctl&SysTick_CTRL_ENABLE_Msk)&&!(ctl & SysTick_CTRL_COUNTFLAG_Msk));
//    /* disable the systick timer */
//    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
//    /* clear the current count value */
//    SysTick->VAL = 0x0000U;
//}


