/**
******************************************************************************
* @file   		udp_demo.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief   		UDP演示函数
******************************************************************************
**/
#include <stdio.h>
#include <string.h>
#include "w5500_conf.h"
#include "w5500.h"
#include "socket.h"
#include "main.h"
#include "systick.h"
#include "udp.h"

extern uint8 count_m;
extern uint8 local_ip[4];
void udp_com_init(uint8 address)
{
	local_ip[3]=address;
	remote_port=address;								/*每个板子对应的上位机端口应该不同*/
	gpio_for_w5500_config();						        /*初始化MCU相关引脚*/
	reset_w5500();											/*硬复位W5500*/
	set_w5500_mac();										/*配置MAC地址*/
	set_w5500_ip();											/*配置IP地址*/
	
	socket_buf_init(txsize, rxsize);		/*初始化8个Socket的发送接收缓存大小*/
	printf(" W5500可以和电脑的UDP端口通讯 \r\n");
	printf(" W5500的本地端口为:%d \r\n",local_port);
	printf(" 远端端口为:%d \r\n",remote_port);
	printf(" W5500可以通过UDP回传信息至PC端！ \r\n");
}



/**
*@brief		UDP测试程序
*@param		无
*@return	无
*/
void do_udp(void)
{                                                              
	uint16 len=0;
	uint8 buff[2048];                                                          /*定义一个2KB的缓存*/	
	switch(getSn_SR(SOCK_UDPS))                                                /*获取socket的状态*/
	{
		case SOCK_CLOSED:                                                        /*socket处于关闭状态*/
			socket(SOCK_UDPS,Sn_MR_UDP,local_port,0);                              /*初始化socket*/
		  break;
		
		case SOCK_UDP:                                                           /*socket初始化完成*/
			delay_1ms(10);
			if(getSn_IR(SOCK_UDPS) & Sn_IR_RECV)
			{
				setSn_IR(SOCK_UDPS, Sn_IR_RECV);                                     /*清接收中断*/
			}
			if((len=getSn_RX_RSR(SOCK_UDPS))>0)                                    /*接收到数据*/
			{
				recvfrom(SOCK_UDPS,buff, len, remote_ip,&remote_port);               /*W5500接收计算机发送来的数据*/
				buff[len-8]=0x00;                                                    /*添加字符串结束符*/
				printf("%s\r\n",buff);
				sendto(SOCK_UDPS,buff,len-8, remote_ip, remote_port);                /*W5500把接收到的数据发送给Remote*/
			}
			break;
	}
}

/**
*@brief		UDP通讯
*@param		无
*@return	无
*/
void do_udp_communicate(uint8 * data,int len)
{               
	switch(getSn_SR(SOCK_UDPS))                                                /*获取socket的状态*/
	{
		case SOCK_CLOSED:                                                        /*socket处于关闭状态*/
			socket(SOCK_UDPS,Sn_MR_UDP,local_port,0);                              /*初始化socket*/
		  break;
		
		case SOCK_UDP:                                                           /*socket初始化完成*/
			if(getSn_IR(SOCK_UDPS) & Sn_IR_RECV)
			{
				setSn_IR(SOCK_UDPS, Sn_IR_RECV);                                     /*清接收中断*/
			}
			if((getSn_RX_RSR(SOCK_UDPS))>0)                                    /*接收到数据*/
			{
				recvfrom(SOCK_UDPS,&adc_freq, 1, remote_ip,&remote_port);               /*W5500接收计算机发送来的数据*/
				adc_freq = HexToChar(adc_freq);
				timer_config(adc_freq);																						/*重新设置定时器，改变采样频率*/
				break;
			}
		
			sendto(SOCK_UDPS,data,len, remote_ip, remote_port);                /*W5500把接收到的数据发送给Remote*/
			break;
	}
}

//	以下代码作废，直接用dma+SPI发送即可
//void do_udp_communicate_2byte(uint16 * data,int len)
//{               
//	switch(getSn_SR(SOCK_UDPS))                                                /*获取socket的状态*/
//	{
//		case SOCK_CLOSED:                                                        /*socket处于关闭状态*/
//			socket(SOCK_UDPS,Sn_MR_UDP,local_port,0);                              /*初始化socket*/
//		  break;
//		
//		case SOCK_UDP:                                                           /*socket初始化完成*/
////			delay_1ms(10);
////				data[len]=0x00;                                                    /*添加字符串结束符*/
//			sendto_2byte(SOCK_UDPS,data,len, remote_ip, remote_port); 
//			break;
//	}
//}
