#ifndef _BSP_ADC_H__
#define _BSP_ADC_H__

#include "gd32f30x.h"
#include "gd32f30x_adc.h"
#include "systick.h"


static uint32_t adc_value[16];

#define COMn                             1U

#define DEV_COM0                        USART0
#define DEV_COM0_CLK                    RCU_USART0
#define DEV_COM0_TX_PIN                 GPIO_PIN_9
#define DEV_COM0_RX_PIN                 GPIO_PIN_10
#define DEV_COM0_GPIO_PORT              GPIOA
#define DEV_COM0_GPIO_CLK               RCU_GPIOA

void adc_init(void);
void adc_rcu_config(void);
void dma_config(void);
void adc_config(void);
void timer_config(void);


static uint32_t adc_channels[]= {ADC_CHANNEL_0, ADC_CHANNEL_1, ADC_CHANNEL_2, ADC_CHANNEL_3, ADC_CHANNEL_4, ADC_CHANNEL_5, ADC_CHANNEL_6, ADC_CHANNEL_7, ADC_CHANNEL_8, ADC_CHANNEL_9, ADC_CHANNEL_10, ADC_CHANNEL_11, ADC_CHANNEL_12, ADC_CHANNEL_13, ADC_CHANNEL_14, ADC_CHANNEL_15};

#endif /* _BSP_ADC_H__ */
