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
#include "utility.h"
#include "udp.h"

void udp_com_init(void)
{
	gpio_for_w5500_config();						        /*初始化MCU相关引脚*/
	reset_w5500();											/*硬复位W5500*/
	set_w5500_mac();										/*配置MAC地址*/
	set_w5500_ip();											/*配置IP地址*/
	
	socket_buf_init(txsize, rxsize);		/*初始化8个Socket的发送接收缓存大小*/
	printf(" W5500可以和电脑的UDP端口通讯 \r\n");
	printf(" W5500的本地端口为:%d \r\n",local_port);
	printf(" 远端端口为:%d \r\n",remote_port);
	printf(" 连接成功后，PC机发送数据给W5500，W5500将返回对应数据 \r\n");
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
			delay_ms(10);
			if(getSn_IR(SOCK_UDPS) & Sn_IR_RECV)
			{
				setSn_IR(SOCK_UDPS, Sn_IR_RECV);                                     /*清接收中断*/
			}
			if((len=getSn_RX_RSR(SOCK_UDPS))>0)                                    /*接收到数据*/
			{
				recvfrom(SOCK_UDPS,buff, len, remote_ip,&remote_port);               /*W5500接收计算机发送来的数据*/
				buff[len-8]=0x00;                                                    /*添加字符串结束符*/
				printf("%s\r\n",buff);                                               /*打印接收缓存*/ 
				sendto(SOCK_UDPS,buff,len-8, remote_ip, remote_port);                /*W5500把接收到的数据发送给Remote*/
			}
			break;
	}

}

