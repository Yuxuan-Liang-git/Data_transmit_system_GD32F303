#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gd32f30x.h"
#include "systick.h"
#include <stdio.h>
//#include "gd25qxx.h"
#include "address_setting.h"
#include "bsp_usart.h"
#include "bsp_gpio.h"
#include "bsp_adc.h"
#include "gd25qxx.h"
#include "utility.h"

#define DELAY_US 1000

void get_clk(void);		//	ªÒ»° ±÷”


#ifdef __cplusplus
}
#endif


#endif /* MAIN_H */
