/**
************************************************************************************************
* @file   		tcp_demo.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief   		TCP 演示函数
* @attention  
************************************************************************************************
**/

#include <stdio.h>
#include <string.h>
#include "tcp.h"
#include "W5500_conf.h"
#include "w5500.h"
#include "socket.h"

uint8 buff[2048];				                              	         /*定义一个2KB的缓存*/
extern uint8 local_ip[4];
uint32_t temp;

void tcp_com_init(uint8 address)
{
		local_ip[3]=address;
		gpio_for_w5500_config();						        /*初始化MCU相关引脚*/
		reset_w5500();											/*硬复位W5500*/
		set_w5500_mac();										/*配置MAC地址*/
		set_w5500_ip();											/*配置IP地址*/

		socket_buf_init(txsize, rxsize);		/*初始化8个Socket的发送接收缓存大小*/

		printf(" 电脑作为TCP服务端,让W5500作为客户端 TCP去连接 \r\n");
		printf(" 服务器IP:%d.%d.%d.%d\r\n",remote_ip[0],remote_ip[1],remote_ip[2],remote_ip[3]);
		printf(" 监听端口:%d \r\n",remote_port);
		printf(" 连接成功后，服务器发送数据给W5500，W5500将返回对应数据 \r\n");
		printf(" 应用程序执行中……\r\n"); 
}


void do_tcp_communicate(uint8 * data,int len)
{	
	switch(getSn_SR(SOCK_TCPC))								  				         /*获取socket的状态*/
	{
		case SOCK_CLOSED:											        		         /*socket处于关闭状态*/
			socket(SOCK_TCPC,Sn_MR_TCP,local_port,Sn_MR_ND);
//      printf("SOCK_CLOSED\n");
		  break;
		
		case SOCK_INIT:													        	         /*socket处于初始化状态*/
			connect(SOCK_TCPC,remote_ip,remote_port);                /*socket连接服务器*/ 
//      printf("SOCK_INIT\n");
		  break;
		
		case SOCK_ESTABLISHED: 												             /*socket处于连接建立状态*/
			if(getSn_IR(SOCK_TCPC) & Sn_IR_CON)
			{
				setSn_IR(SOCK_TCPC, Sn_IR_CON); 							         /*清除接收中断标志位*/
			}
			if(len>0)
			{
				data[len]=0x00;  											                 /*添加字符串结束符*/
//				printf("%d\r\n",data);
				send(SOCK_TCPC,data,len);								     	         /*向Server发送数据*/
//				for(int i=0;i<32;i++)
//				{

//					temp = (data[64*i + 0] << 24) + (data[64*i + 1] << 16) + (data[64*i + 2] << 8) + data[64*i + 3];
//					printf("%d\n",temp);
////					u32 = (u8[3] << 24) + (u8[2] << 16) + (u8[1] << 8) + u8[0];
//				}
//				printf("communicate_task finished! \n");
			}	
//      printf("SOCK_ESTABLISHED\n");      
		  break;
			
		case SOCK_CLOSE_WAIT: 											    	         /*socket处于等待关闭状态*/
			close(SOCK_TCPC);
		  break;
	}
}


/**
*@brief		TCP Server回环演示函数。
*@param		无
*@return	无
*/
void do_tcp_server(void)
{	
	uint16 len=0;  
	switch(getSn_SR(SOCK_TCPS))											            	/*获取socket的状态*/
	{
		case SOCK_CLOSED:													                  /*socket处于关闭状态*/
			socket(SOCK_TCPS ,Sn_MR_TCP,local_port,Sn_MR_ND);	        /*打开socket*/
		  break;     
    
		case SOCK_INIT:														                  /*socket已初始化状态*/
			listen(SOCK_TCPS);												                /*socket建立监听*/
		  break;
		
		case SOCK_ESTABLISHED:												              /*socket处于连接建立状态*/
		
			if(getSn_IR(SOCK_TCPS) & Sn_IR_CON)
			{
				setSn_IR(SOCK_TCPS, Sn_IR_CON);								          /*清除接收中断标志位*/
			}
			len=getSn_RX_RSR(SOCK_TCPS);									            /*定义len为已接收数据的长度*/
			if(len>0)
			{
				recv(SOCK_TCPS,buff,len);								              	/*接收来自Client的数据*/
				buff[len]=0x00; 											                  /*添加字符串结束符*/
				printf("%s\r\n",buff);
				send(SOCK_TCPS,buff,len);									              /*向Client发送数据*/
		  }
		  break;
		
		case SOCK_CLOSE_WAIT:												                /*socket处于等待关闭状态*/
			close(SOCK_TCPS);
		  break;
	}
}

/**
*@brief		TCP Client回环演示函数。
*@param		无
*@return	无
*/
void do_tcp_client(void)
{	
   uint16 len=0;	

	switch(getSn_SR(SOCK_TCPC))								  				         /*获取socket的状态*/
	{
		case SOCK_CLOSED:											        		         /*socket处于关闭状态*/
			socket(SOCK_TCPC,Sn_MR_TCP,local_port++,Sn_MR_ND);
//      printf("SOCK_CLOSED\n");
		  break;
		
		case SOCK_INIT:													        	         /*socket处于初始化状态*/
			connect(SOCK_TCPC,remote_ip,remote_port);                /*socket连接服务器*/ 
//      printf("SOCK_INIT\n");
		  break;
		
		case SOCK_ESTABLISHED: 												             /*socket处于连接建立状态*/
			if(getSn_IR(SOCK_TCPC) & Sn_IR_CON)
			{
				setSn_IR(SOCK_TCPC, Sn_IR_CON); 							         /*清除接收中断标志位*/
			}
		
			len=getSn_RX_RSR(SOCK_TCPC); 								  	         /*定义len为已接收数据的长度*/
			if(len>0)
			{
				recv(SOCK_TCPC,buff,len); 							   		         /*接收来自Server的数据*/
				buff[len]=0x00;  											                 /*添加字符串结束符*/
				printf("%s\r\n",buff);
				send(SOCK_TCPC,buff,len);								     	         /*向Server发送数据*/
			}	
//      printf("SOCK_ESTABLISHED\n");      
		  break;
			
		case SOCK_CLOSE_WAIT: 											    	         /*socket处于等待关闭状态*/
			close(SOCK_TCPC);
//      printf("SOCK_CLOSE_WAIT\n");
		  break;
	}
  
}

