#ifndef _BSP_GPIO_H__
#define _BSP_GPIO_H__

#ifdef __cplusplus
 extern "C" {
#endif


#include "gd32f30x.h"

#define LED_PIN                         GPIO_PIN_13
#define LED_GPIO_PORT                   GPIOC
#define LED_GPIO_CLK                    RCU_GPIOC

void led_init(void);
void led_on(void);
void led_off(void);

#ifdef __cplusplus
}
#endif

#endif /* _BSP_GPIO_H__ */
