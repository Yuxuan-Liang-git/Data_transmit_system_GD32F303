/**
******************************************************************************
* @file    			DHCP.c
* @author  			WIZnet Software Team 
* @version 			V1.0
* @date    			2015-02-14
* @brief   			DHCP	Client
* @attention		ʹ��DHCP����IP��ַ��Ҫ����
******************************************************************************
*/
#include <stdio.h>
#include <string.h>	

#include "w5500.h"
#include "socket.h"

#include "dhcp.h"
#include "utility.h"
#include "w5500_conf.h"

//#define DHCP_DEBUG

DHCP_Get DHCP_GET;

uint8*   SRC_MAC_ADDR   = EXTERN_DHCP_MAC;				/*����MAC��ַ*/
uint8*   GET_SN_MASK    = EXTERN_DHCP_SN;					/*��DHCP��������ȡ������������*/
uint8*   GET_GW_IP      = EXTERN_DHCP_GW;					/*��DHCP��������ȡ�������ص�ַ*/
uint8*   GET_DNS_IP     = EXTERN_DHCP_DNS;				/*��DHCP��������ȡ����DNS��������ַ*/
uint8*   DHCP_HOST_NAME = EXTERN_DHCP_NAME;   		/*������*/
uint8*   GET_SIP        = EXTERN_DHCP_SIP;				/*��DHCP��������ȡ���ı���IP*/
uint8    DHCP_SIP[4] = {0,};											/*�ѷ��ֵ�DNS��������ַ*/
uint8    DHCP_REAL_SIP[4] = {0,}; 								/*��DHCP�б���ѡ��ʹ�õ�DHCP������*/
uint8    OLD_SIP[4];        											/*�����ȡ����IP��ַ*/

uint8		dhcp_state       = STATE_DHCP_READY;       /*DHCP�ͻ���״̬*/
uint8		dhcp_retry_count = 0;                      /*DHCP���Դ���*/
uint8		DHCP_timeout     = 0;                      /*DHCP��ʱ��־λ*/
uint32  dhcp_lease_time;													 /*��Լʱ��*/
uint32	next_dhcp_time  = 0;											/*DHCP��ʱʱ��*/
uint32	dhcp_tick_cnt   = 0;                   	  
uint8		DHCP_timer;

uint8 Conflict_flag = 0;
uint32  DHCP_XID        = DEFAULT_XID;				
uint8 EXTERN_DHCPBUF[1024];
RIP_MSG*  pRIPMSG = (RIP_MSG*)EXTERN_DHCPBUF;			/*DHCP��Ϣָ��*/


