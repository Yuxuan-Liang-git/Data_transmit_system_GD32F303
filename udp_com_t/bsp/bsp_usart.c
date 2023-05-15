#include "bsp_usart.h"
#include "gd32f30x.h"
#include <string.h>

/* private variables */
static rcu_periph_enum COM_CLK[COMn] = {DEV_COM0_CLK};
static uint32_t COM_TX_PIN[COMn] = {DEV_COM0_TX_PIN};
static uint32_t COM_RX_PIN[COMn] = {DEV_COM0_RX_PIN};
static uint32_t COM_GPIO_PORT[COMn] = {DEV_COM0_GPIO_PORT};
static rcu_periph_enum COM_GPIO_CLK[COMn] = {DEV_COM0_GPIO_CLK};

/*!
    \brief      configure COM port
    \param[in]  com: COM on the board
      \arg        DEV_COM0: COM0 on the board
    \param[out] none
    \retval     none
*/
void gd_usart_com_init(uint32_t com)
{
    uint32_t com_id = 0U;
    if(DEV_COM0 == com){
        com_id = 0U;
    }else{
        while(1);
    }
    
    /* enable GPIO clock */
    rcu_periph_clock_enable(COM_GPIO_CLK[com_id]);

    /* enable USART clock */
    rcu_periph_clock_enable(COM_CLK[com_id]);

    /* connect port to USARTx_Tx */
    gpio_init(COM_GPIO_PORT[com_id], GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, COM_TX_PIN[com_id]);

    /* connect port to USARTx_Rx */
    gpio_init(COM_GPIO_PORT[com_id], GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, COM_RX_PIN[com_id]);

    /* USART configure */
    usart_deinit(com);
		
    usart_baudrate_set(com, 115200U);
    usart_receive_config(com, USART_RECEIVE_ENABLE);
    usart_transmit_config(com, USART_TRANSMIT_ENABLE);
    usart_enable(com);
}

