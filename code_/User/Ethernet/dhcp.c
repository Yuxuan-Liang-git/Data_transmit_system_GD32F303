/**
******************************************************************************
* @file    			DHCP.c
* @author  			WIZnet Software Team 
* @version 			V1.0
* @date    			2015-02-14
* @brief   			DHCP	Client
* @attention		使用DHCP配置IP地址需要定义
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

uint8*   SRC_MAC_ADDR   = EXTERN_DHCP_MAC;				/*本地MAC地址*/
uint8*   GET_SN_MASK    = EXTERN_DHCP_SN;					/*从DHCP服务器获取到的子网掩码*/
uint8*   GET_GW_IP      = EXTERN_DHCP_GW;					/*从DHCP服务器获取到的网关地址*/
uint8*   GET_DNS_IP     = EXTERN_DHCP_DNS;				/*从DHCP服务器获取到的DNS服务器地址*/
uint8*   DHCP_HOST_NAME = EXTERN_DHCP_NAME;   		/*主机名*/
uint8*   GET_SIP        = EXTERN_DHCP_SIP;				/*从DHCP服务器获取到的本机IP*/
uint8    DHCP_SIP[4] = {0,};											/*已发现的DNS服务器地址*/
uint8    DHCP_REAL_SIP[4] = {0,}; 								/*从DHCP列表中选择使用的DHCP服务器*/
uint8    OLD_SIP[4];        											/*最初获取到的IP地址*/

uint8		dhcp_state       = STATE_DHCP_READY;       /*DHCP客户端状态*/
uint8		dhcp_retry_count = 0;                      /*DHCP重试次数*/
uint8		DHCP_timeout     = 0;                      /*DHCP超时标志位*/
uint32  dhcp_lease_time;													 /*租约时间*/
uint32	next_dhcp_time  = 0;											/*DHCP超时时间*/
uint32	dhcp_tick_cnt   = 0;                   	  
uint8		DHCP_timer;

uint8 Conflict_flag = 0;
uint32  DHCP_XID        = DEFAULT_XID;				
uint8 EXTERN_DHCPBUF[1024];
RIP_MSG*  pRIPMSG = (RIP_MSG*)EXTERN_DHCPBUF;			/*DHCP消息指针*/


