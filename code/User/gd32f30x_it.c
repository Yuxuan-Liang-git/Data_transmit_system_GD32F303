/*!
    \file    gd32f30x_it.c
    \brief   interrupt service routines

    \version 2017-02-10, V1.0.0, firmware for GD32F30x
    \version 2018-10-10, V1.1.0, firmware for GD32F30x
    \version 2018-12-25, V2.0.0, firmware for GD32F30x
    \version 2020-09-30, V2.1.0, firmware for GD32F30x 
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32f30x_it.h"
#include "systick.h"
#include "main.h"
#include "tcp.h"
#include "bsp_adc.h"
#include "string.h"

uint8_t count;
uint8_t cache_data[1024];
uint8_t tcp_buffer[1024];


FlagStatus send_flag;

/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SysTick_Handler(void)
{
    delay_decrement();
}

//	10us触发一次中断
void TIMER1_IRQHandler(void)
{
		if(SET == timer_interrupt_flag_get(TIMER1,TIMER_INT_UP)){

		if(adc_finish_flag == SET)
		{
			if(count<=16)	//	6.4ms发一次
			{
				memcpy(cache_data+64*count,adc_value,64);
				count++;
			} 
			else
			{
				memcpy(tcp_buffer,cache_data,1024);
				memset(cache_data,0,sizeof cache_data);
				memcpy(cache_data,adc_value,64);	
				count = 1;
				send_flag = SET;
			}
			adc_finish_flag = RESET;
		}
		adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);	
		/* clear TIMER interrupt flag */
		timer_interrupt_flag_clear(TIMER1,TIMER_INT_UP);
    }
}


////	10us触发一次中断
//void TIMER1_IRQHandler(void)
//{
//	if(SET == timer_interrupt_flag_get(TIMER1,TIMER_INT_UP)){
//		if(count<32)	//	3.2ms发一次
//		{
//			if(adc_finish_flag == SET)
//			{
//				memcpy(cache_data+64*count,adc_value,64);
//				count++;
//				adc_finish_flag = RESET;
//			}
//		} 
//		else
//		{
//			do_tcp_communicate(cache_data,2048);
//			memcpy(cache_data,0x00,2048);							//	清空缓存
//			memcpy(cache_data,adc_value,64);	
//			count = 1;
//		}
//		adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);	
//		/* clear TIMER interrupt flag */
//		timer_interrupt_flag_clear(TIMER1,TIMER_INT_UP);
//    }
//}

