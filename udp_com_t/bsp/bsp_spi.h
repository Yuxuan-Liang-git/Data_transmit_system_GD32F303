#ifndef _BSP_SPI_H__
#define _BSP_SPI_H__

#include "gd32f30x.h"

#define  SPI_W550_CS_LOW()            gpio_bit_reset(GPIOB, GPIO_PIN_6)
#define  SPI_W550_CS_HIGH()           gpio_bit_set(GPIOB, GPIO_PIN_6)

#define COMn                             1U

#define DEV_COM0                        USART0
#define DEV_COM0_CLK                    RCU_USART0
#define DEV_COM0_TX_PIN                 GPIO_PIN_9
#define DEV_COM0_RX_PIN                 GPIO_PIN_10
#define DEV_COM0_GPIO_PORT              GPIOA
#define DEV_COM0_GPIO_CLK               RCU_GPIOA

void w5500_spi_init(void);
void gd_usart_com_init(uint32_t com);
#endif /* _BSP_SPI_H__ */
