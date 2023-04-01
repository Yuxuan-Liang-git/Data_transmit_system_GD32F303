#include "address_setting.h"

uint8 i;
void addr_init(void)
{
		rcu_periph_clock_enable(RCU_AF);	
		rcu_periph_clock_enable(ADDR_GPIO_CLK);

		for(i=0;i<8;i++)
		{		
			gpio_init(ADDR_GPIO_Port, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, ADDR_PIN[i]);
		}

}

uint8 get_addr(void)
{
	uint8	addr;
	FlagStatus ADDR_state[8];
	for(i=0;i<8;i++)
	{
		ADDR_state[i]=gpio_input_bit_get(ADDR_GPIO_Port,ADDR_PIN[i]);
	}
	addr=ADDR_state[0]|ADDR_state[1]<<1|ADDR_state[2]<<2|ADDR_state[3]<<3|
		ADDR_state[4]<<4|ADDR_state[5]<<5|ADDR_state[6]<<6|ADDR_state[7]<<7;
	return addr;
}
