/**
******************************************************************************
* @file   		udp_demo.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief   		UDP��ʾ����
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
	gpio_for_w5500_config();						        /*��ʼ��MCU�������*/
	reset_w5500();											/*Ӳ��λW5500*/
	set_w5500_mac();										/*����MAC��ַ*/
	set_w5500_ip();											/*����IP��ַ*/
	
	socket_buf_init(txsize, rxsize);		/*��ʼ��8��Socket�ķ��ͽ��ջ����С*/
	printf(" W5500���Ժ͵��Ե�UDP�˿�ͨѶ \r\n");
	printf(" W5500�ı��ض˿�Ϊ:%d \r\n",local_port);
	printf(" Զ�˶˿�Ϊ:%d \r\n",remote_port);
	printf(" ���ӳɹ���PC���������ݸ�W5500��W5500�����ض�Ӧ���� \r\n");
}

/**
*@brief		UDP���Գ���
*@param		��
*@return	��
*/
	void do_udp(void)
{                                                              
	uint16 len=0;
	uint8 buff[2048];                                                          /*����һ��2KB�Ļ���*/	
	switch(getSn_SR(SOCK_UDPS))                                                /*��ȡsocket��״̬*/
	{
		case SOCK_CLOSED:                                                        /*socket���ڹر�״̬*/
			socket(SOCK_UDPS,Sn_MR_UDP,local_port,0);                              /*��ʼ��socket*/
		  break;
		
		case SOCK_UDP:                                                           /*socket��ʼ�����*/
			delay_ms(10);
			if(getSn_IR(SOCK_UDPS) & Sn_IR_RECV)
			{
				setSn_IR(SOCK_UDPS, Sn_IR_RECV);                                     /*������ж�*/
			}
			if((len=getSn_RX_RSR(SOCK_UDPS))>0)                                    /*���յ�����*/
			{
				recvfrom(SOCK_UDPS,buff, len, remote_ip,&remote_port);               /*W5500���ռ����������������*/
				buff[len-8]=0x00;                                                    /*����ַ���������*/
				printf("%s\r\n",buff);                                               /*��ӡ���ջ���*/ 
				sendto(SOCK_UDPS,buff,len-8, remote_ip, remote_port);                /*W5500�ѽ��յ������ݷ��͸�Remote*/
			}
			break;
	}

}

