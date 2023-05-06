/**
************************************************************************************************
* @file   		tcp_demo.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief   		TCP ��ʾ����
* @attention  
************************************************************************************************
**/

#include <stdio.h>
#include <string.h>
#include "tcp.h"
#include "W5500_conf.h"
#include "w5500.h"
#include "socket.h"

uint8 buff[2048];				                              	         /*����һ��2KB�Ļ���*/
extern uint8 local_ip[4];
uint32_t temp;

void tcp_com_init(uint8 address)
{
		local_ip[3]=address;
		gpio_for_w5500_config();						        /*��ʼ��MCU�������*/
		reset_w5500();											/*Ӳ��λW5500*/
		set_w5500_mac();										/*����MAC��ַ*/
		set_w5500_ip();											/*����IP��ַ*/

		socket_buf_init(txsize, rxsize);		/*��ʼ��8��Socket�ķ��ͽ��ջ����С*/

		printf(" ������ΪTCP�����,��W5500��Ϊ�ͻ��� TCPȥ���� \r\n");
		printf(" ������IP:%d.%d.%d.%d\r\n",remote_ip[0],remote_ip[1],remote_ip[2],remote_ip[3]);
		printf(" �����˿�:%d \r\n",remote_port);
		printf(" ���ӳɹ��󣬷������������ݸ�W5500��W5500�����ض�Ӧ���� \r\n");
		printf(" Ӧ�ó���ִ���С���\r\n"); 
}


void do_tcp_communicate(uint8 * data,int len)
{	
	switch(getSn_SR(SOCK_TCPC))								  				         /*��ȡsocket��״̬*/
	{
		case SOCK_CLOSED:											        		         /*socket���ڹر�״̬*/
			socket(SOCK_TCPC,Sn_MR_TCP,local_port,Sn_MR_ND);
//      printf("SOCK_CLOSED\n");
		  break;
		
		case SOCK_INIT:													        	         /*socket���ڳ�ʼ��״̬*/
			connect(SOCK_TCPC,remote_ip,remote_port);                /*socket���ӷ�����*/ 
//      printf("SOCK_INIT\n");
		  break;
		
		case SOCK_ESTABLISHED: 												             /*socket�������ӽ���״̬*/
			if(getSn_IR(SOCK_TCPC) & Sn_IR_CON)
			{
				setSn_IR(SOCK_TCPC, Sn_IR_CON); 							         /*��������жϱ�־λ*/
			}
			if(len>0)
			{
				data[len]=0x00;  											                 /*����ַ���������*/
//				printf("%d\r\n",data);
				send(SOCK_TCPC,data,len);								     	         /*��Server��������*/
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
			
		case SOCK_CLOSE_WAIT: 											    	         /*socket���ڵȴ��ر�״̬*/
			close(SOCK_TCPC);
		  break;
	}
}


/**
*@brief		TCP Server�ػ���ʾ������
*@param		��
*@return	��
*/
void do_tcp_server(void)
{	
	uint16 len=0;  
	switch(getSn_SR(SOCK_TCPS))											            	/*��ȡsocket��״̬*/
	{
		case SOCK_CLOSED:													                  /*socket���ڹر�״̬*/
			socket(SOCK_TCPS ,Sn_MR_TCP,local_port,Sn_MR_ND);	        /*��socket*/
		  break;     
    
		case SOCK_INIT:														                  /*socket�ѳ�ʼ��״̬*/
			listen(SOCK_TCPS);												                /*socket��������*/
		  break;
		
		case SOCK_ESTABLISHED:												              /*socket�������ӽ���״̬*/
		
			if(getSn_IR(SOCK_TCPS) & Sn_IR_CON)
			{
				setSn_IR(SOCK_TCPS, Sn_IR_CON);								          /*��������жϱ�־λ*/
			}
			len=getSn_RX_RSR(SOCK_TCPS);									            /*����lenΪ�ѽ������ݵĳ���*/
			if(len>0)
			{
				recv(SOCK_TCPS,buff,len);								              	/*��������Client������*/
				buff[len]=0x00; 											                  /*����ַ���������*/
				printf("%s\r\n",buff);
				send(SOCK_TCPS,buff,len);									              /*��Client��������*/
		  }
		  break;
		
		case SOCK_CLOSE_WAIT:												                /*socket���ڵȴ��ر�״̬*/
			close(SOCK_TCPS);
		  break;
	}
}

/**
*@brief		TCP Client�ػ���ʾ������
*@param		��
*@return	��
*/
void do_tcp_client(void)
{	
   uint16 len=0;	

	switch(getSn_SR(SOCK_TCPC))								  				         /*��ȡsocket��״̬*/
	{
		case SOCK_CLOSED:											        		         /*socket���ڹر�״̬*/
			socket(SOCK_TCPC,Sn_MR_TCP,local_port++,Sn_MR_ND);
//      printf("SOCK_CLOSED\n");
		  break;
		
		case SOCK_INIT:													        	         /*socket���ڳ�ʼ��״̬*/
			connect(SOCK_TCPC,remote_ip,remote_port);                /*socket���ӷ�����*/ 
//      printf("SOCK_INIT\n");
		  break;
		
		case SOCK_ESTABLISHED: 												             /*socket�������ӽ���״̬*/
			if(getSn_IR(SOCK_TCPC) & Sn_IR_CON)
			{
				setSn_IR(SOCK_TCPC, Sn_IR_CON); 							         /*��������жϱ�־λ*/
			}
		
			len=getSn_RX_RSR(SOCK_TCPC); 								  	         /*����lenΪ�ѽ������ݵĳ���*/
			if(len>0)
			{
				recv(SOCK_TCPC,buff,len); 							   		         /*��������Server������*/
				buff[len]=0x00;  											                 /*����ַ���������*/
				printf("%s\r\n",buff);
				send(SOCK_TCPC,buff,len);								     	         /*��Server��������*/
			}	
//      printf("SOCK_ESTABLISHED\n");      
		  break;
			
		case SOCK_CLOSE_WAIT: 											    	         /*socket���ڵȴ��ر�״̬*/
			close(SOCK_TCPC);
//      printf("SOCK_CLOSE_WAIT\n");
		  break;
	}
  
}

