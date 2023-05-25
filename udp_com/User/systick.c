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


#include "gd32f30x.h"
#include "systick.h"

volatile static float count_1us = 0;
volatile static float count_1ms = 0;

/*!
    \brief      configure systick
    \param[in]  none
    \param[out] none
    \retval     none
*/
void systick_config(void)
{
    systick_clksource_set(SYSTICK_CLKSOURCE_HCLK);
    count_1us = (float)SystemCoreClock/120000000;
    count_1ms = (float)count_1us * 1000;
}

/*!
    \brief      delay a time in microseconds in polling mode
    \param[in]  count: count in microseconds
    \param[out] none
    \retval     none
*/
void delay_1us(uint32_t count)
{
    uint32_t ctl;
    
    /* reload the count value */
    SysTick->LOAD = (uint32_t)(count * count_1us);
    /* clear the current count value */
    SysTick->VAL = 0x0000U;
    /* enable the systick timer */
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
    /* wait for the COUNTFLAG flag set */
    do{
        ctl = SysTick->CTRL;
    }while((ctl&SysTick_CTRL_ENABLE_Msk)&&!(ctl & SysTick_CTRL_COUNTFLAG_Msk));
    /* disable the systick timer */
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    /* clear the current count value */
    SysTick->VAL = 0x0000U;
}

/*!
    \brief      delay a time in milliseconds in polling mode
    \param[in]  count: count in milliseconds
    \param[out] none
    \retval     none
*/
void delay_1ms(uint32_t count)
{
    uint32_t ctl;
    
    /* reload the count value */
    SysTick->LOAD = (uint32_t)(count * count_1ms);
    /* clear the current count value */
    SysTick->VAL = 0x0000U;
    /* enable the systick timer */
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
    /* wait for the COUNTFLAG flag set */
    do{
        ctl = SysTick->CTRL;
    }while((ctl&SysTick_CTRL_ENABLE_Msk)&&!(ctl & SysTick_CTRL_COUNTFLAG_Msk));
    /* disable the systick timer */
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    /* clear the current count value */
    SysTick->VAL = 0x0000U;
}


