#ifndef _W5500_CONF_H_
#define _W5500_CONF_H_

#include "gd32f30x.h"
#include "stdio.h"
#include "types.h"
#define __GNUC__



typedef  void (*pFunction)(void);

extern uint8    remote_ip[4];                            	  /* 远端IP地址                   */
extern uint16   remote_port;                            	  /* 远端端口号                   */
extern uint16   local_port;                             	  /* 定义本地端口                 */
extern uint8    use_dhcp;                              	    /* 是否使用DHCP获取IP           */
extern uint8    use_eeprom;                             	  /* 是否使用EEPROM中的IP配置信息 */


/*定义SPI0作为W5500的硬件接口*/
#define WIZ_SPIx_GPIO_PORT      GPIOB									  	  /* GPIO端口                     */
#define WIZ_SPIx_GPIO_CLK       RCU_GPIOB	  	  						/* GPIO端口时钟*/
#define WIZ_SPIx                SPI0                   	    /* 定义W5500所用的SPI接口       */
#define WIZ_SPIx_CLK            RCU_SPI0    	    					/* 定义W5500所用的SPI接口时钟   */
#define WIZ_SPIx_SCLK           GPIO_PIN_3						   	  /* 定义W5500的时钟管脚          */
#define WIZ_SPIx_MISO           GPIO_PIN_4						   	  /* 定义W5500的MISO管脚          */
#define WIZ_SPIx_MOSI           GPIO_PIN_5						   	  /* 定义W5500的MOSI管脚          */
#define WIZ_SPIx_SCS          	GPIO_PIN_6						 	    /* 定义W5500的片选管脚          */
#define WIZ_SPIx_SCS_PORT     	GPIOB									 	    /* GPIO端口                     */
#define WIZ_SPIx_SCS_CLK      	RCU_GPIOB	 	   							 /* GPIO端口时钟                */

#define WIZ_RESET            	 	GPIO_PIN_8									/* 定义W5500的RESET管脚         */
#define WIZ_SPIx_RESET_PORT   	GPIOB									  	  /* GPIO端口                     */
#define WIZ_SPIx_RESET_CLK    	RCU_GPIOB	 	    						/* GPIO端口时钟    				      */

#define WIZ_INT               	GPIO_PIN_7									/* 定义W5500的INT管脚           */
#define WIZ_SPIx_INT_PORT     	GPIOB									      /* GPIO端口                     */
#define WIZ_SPIx_INT_CLK      	RCU_GPIOB	    						  /* GPIO端口时钟                 */


#define FW_VER_HIGH  						1               
#define FW_VER_LOW    					0
#define ON	                 		1
#define OFF	                 		0
#define HIGH	           	 			1
#define LOW		             			0

#define MAX_BUF_SIZE		 				1460       			            /*定义每个数据包的大小*/
#define KEEP_ALIVE_TIME	     		30	// 30sec
#define TX_RX_MAX_BUF_SIZE      2048							 
#define EEPROM_MSG_LEN        	sizeof(EEPROM_MSG)

#define IP_FROM_DEFINE	        0       			              /*使用初始定义的IP信息*/

extern uint8	ip_from;											                /*选择IP信息配置源*/
#pragma pack(1)
/*此结构体定义了W5500可供配置的主要参数*/
typedef struct _CONFIG_MSG											
{
  uint8 mac[6];																							/*MAC地址*/
  uint8 lip[4];																							/*local IP本地IP地址*/
  uint8 sub[4];																							/*子网掩码*/
  uint8 gw[4];																							/*网关*/	
  uint8 dns[4];																							/*DNS服务器地址*/
  uint8 rip[4];																							/*remote IP远程IP地址*/
	uint8 sw_ver[2];																					/*软件版本号*/

}CONFIG_MSG;
#pragma pack()

#pragma pack(1)
/*此结构体定义了eeprom写入的几个变量，可按需修改*/
typedef struct _EEPROM_MSG	                    
{
	uint8 mac[6];																							/*MAC地址*/
  uint8 lip[4];																							/*local IP本地IP地址*/
  uint8 sub[4];																							/*子网掩码*/
  uint8 gw[4];																							/*网关*/
}EEPROM_MSG_STR;
#pragma pack()

extern CONFIG_MSG  	ConfigMsg;


/*MCU配置相关函数*/
void gpio_for_w5500_config(void);														/*SPI接口reset 及中断引脚*/


/*W5500SPI相关函数*/
void IINCHIP_WRITE( uint32 addrbsb,  uint8 data);						/*写入一个8位数据到W5500*/
uint8 IINCHIP_READ(uint32 addrbsb);													/*从W5500读出一个8位数据*/
uint16 wiz_write_buf(uint32 addrbsb,uint8* buf,uint16 len);	/*向W5500写入len字节数据*/
uint16 wiz_write_buf_2byte(uint32 addrbsb,uint16 * buf,uint16 len);	/*向W5500写入len字节数据*/
uint16 wiz_read_buf(uint32 addrbsb, uint8* buf,uint16 len);	/*从W5500读出len字节数据*/

/*W5500基本配置相关函数*/
void reset_w5500(void);																			/*硬复位W5500*/
void set_w5500_mac(void);																		/*配置W5500的MAC地址*/
void set_w5500_ip(void);																		/*配置W5500的IP地址*/


#endif
