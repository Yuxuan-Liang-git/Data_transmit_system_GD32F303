#ifndef __UDP_DEMO_H
#define __UDP_DEMO_H

#include "gd32f30x.h"
#include "Types.h"
extern uint16 udp_port;/*定义UDP的一个端口并初始化*/

void udp_com_init(uint8 address);
void do_udp(void);
void do_udp_communicate(uint8 * data,int len);
#endif 


