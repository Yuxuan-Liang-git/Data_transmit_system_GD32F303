#ifndef _BSP_USART_H__
#define _BSP_USART_H__

#include "gd32f30x.h"

#define COMn                             1U

#define DEV_COM0                        USART0
#define DEV_COM0_CLK                    RCU_USART0
#define DEV_COM0_TX_PIN                 GPIO_PIN_9
#define DEV_COM0_RX_PIN                 GPIO_PIN_10
#define DEV_COM0_GPIO_PORT              GPIOA
#define DEV_COM0_GPIO_CLK               RCU_GPIOA

void gd_usart_com_init(uint32_t com);
#endif /* _BSP_USART_H__ */
