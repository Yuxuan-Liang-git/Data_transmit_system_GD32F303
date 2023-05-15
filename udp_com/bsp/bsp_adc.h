#ifndef _BSP_ADC_H__
#define _BSP_ADC_H__

#include "gd32f30x.h"
#include "gd32f30x_adc.h"
#include "systick.h"
#include "main.h"

#define COMn                             1U

#define DEV_COM0                        USART0
#define DEV_COM0_CLK                    RCU_USART0
#define DEV_COM0_TX_PIN                 GPIO_PIN_9
#define DEV_COM0_RX_PIN                 GPIO_PIN_10
#define DEV_COM0_GPIO_PORT              GPIOA
#define DEV_COM0_GPIO_CLK               RCU_GPIOA

#define tcp_cache_size	512							//	tcp通讯16kb缓存
#define dma_cache_size	tcp_cache_size/4		//	dma内存是uint32，tcp是uint8

typedef enum {ADC_DMA_RST = 0,ADC_DMA_HF,ADC_DMA_F } ADC_DMA_FLAG;

void adc_init(void);
void adc_rcu_config(void);
void adc_gpio_config(void);
void dma_config(void);
void adc_config(void);
void timer_config(void);

extern uint32_t raw_data[dma_cache_size*2];		//	DMA双缓冲区
extern uint8_t adc_value[tcp_cache_size];
extern ADC_DMA_FLAG adc_dma_flag;
static uint32_t adc_channels[]= {
	ADC_CHANNEL_6, ADC_CHANNEL_4, ADC_CHANNEL_2,ADC_CHANNEL_0, 
	ADC_CHANNEL_7, ADC_CHANNEL_5, ADC_CHANNEL_3, ADC_CHANNEL_1, 
	ADC_CHANNEL_8, ADC_CHANNEL_14, ADC_CHANNEL_12, ADC_CHANNEL_10,
	ADC_CHANNEL_9,ADC_CHANNEL_15, ADC_CHANNEL_13, ADC_CHANNEL_11};

static uint32_t adc_gpio_port[] = {
	GPIOA,GPIOA,GPIOA,GPIOA,
	GPIOA,GPIOA,GPIOA,GPIOA,
	GPIOB,GPIOC,GPIOC,GPIOC,
	GPIOB,GPIOC,GPIOC,GPIOC
};

static uint32_t adc_gpio_pin[] = {
	GPIO_PIN_6,GPIO_PIN_4,GPIO_PIN_2,GPIO_PIN_0,
	GPIO_PIN_7,GPIO_PIN_5,GPIO_PIN_3,GPIO_PIN_1,
	GPIO_PIN_0,GPIO_PIN_4,GPIO_PIN_2,GPIO_PIN_0,
	GPIO_PIN_1,GPIO_PIN_5,GPIO_PIN_3,GPIO_PIN_1
};

#endif /* _BSP_ADC_H__ */
