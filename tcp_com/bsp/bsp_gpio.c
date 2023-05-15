
#include "bsp_gpio.h"
#include "gd32f30x.h"

void led_init(void)
{
		rcu_periph_clock_enable(LED_GPIO_CLK);
		rcu_periph_clock_enable(RCU_AF);	
	
		gpio_init(LED_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_PIN);
}

void led_on(void)
{
		gpio_bit_reset(LED_GPIO_PORT, LED_PIN); //��PC13�õ͵�ƽ������LED
}

void led_off(void)
{
		gpio_bit_set(LED_GPIO_PORT, LED_PIN); //��PC13�øߵ�ƽ��Ϩ��LED
}