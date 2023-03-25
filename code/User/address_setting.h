#ifndef _ADDRESS_SETTING_H__
#define _ADDRESS_SETTING_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

#define ADDR_GPIO_CLK		RCU_GPIOD
#define ADDR_GPIO_Port 	GPIOD

static uint32_t ADDR_PIN[8]	= {GPIO_PIN_0,GPIO_PIN_1,GPIO_PIN_2,GPIO_PIN_3,
																	 GPIO_PIN_4,GPIO_PIN_5,GPIO_PIN_6,GPIO_PIN_7};


void addr_init(void);
uint16_t get_addr(void);
																	 
																	 
#ifdef __cplusplus
}
#endif

#endif /* _ADDRESS_SETTING_H__ */
																	 