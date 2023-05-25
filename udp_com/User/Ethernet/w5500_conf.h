#ifndef _W5500_CONF_H_
#define _W5500_CONF_H_

#include "gd32f30x.h"
#include "stdio.h"
#include "types.h"
#define __GNUC__



typedef  void (*pFunction)(void);

extern uint8    remote_ip[4];                            	  /* Զ��IP��ַ                   */
extern uint16   remote_port;                            	  /* Զ�˶˿ں�                   */
extern uint16   local_port;                             	  /* ���屾�ض˿�                 */
extern uint8    use_dhcp;                              	    /* �Ƿ�ʹ��DHCP��ȡIP           */
extern uint8    use_eeprom;                             	  /* �Ƿ�ʹ��EEPROM�е�IP������Ϣ */


/*����SPI0��ΪW5500��Ӳ���ӿ�*/
#define WIZ_SPIx_GPIO_PORT      GPIOB									  	  /* GPIO�˿�                     */
#define WIZ_SPIx_GPIO_CLK       RCU_GPIOB	  	  						/* GPIO�˿�ʱ��*/
#define WIZ_SPIx                SPI0                   	    /* ����W5500���õ�SPI�ӿ�       */
#define WIZ_SPIx_CLK            RCU_SPI0    	    					/* ����W5500���õ�SPI�ӿ�ʱ��   */
#define WIZ_SPIx_SCLK           GPIO_PIN_3						   	  /* ����W5500��ʱ�ӹܽ�          */
#define WIZ_SPIx_MISO           GPIO_PIN_4						   	  /* ����W5500��MISO�ܽ�          */
#define WIZ_SPIx_MOSI           GPIO_PIN_5						   	  /* ����W5500��MOSI�ܽ�          */
#define WIZ_SPIx_SCS          	GPIO_PIN_6						 	    /* ����W5500��Ƭѡ�ܽ�          */
#define WIZ_SPIx_SCS_PORT     	GPIOB									 	    /* GPIO�˿�                     */
#define WIZ_SPIx_SCS_CLK      	RCU_GPIOB	 	   							 /* GPIO�˿�ʱ��                */

#define WIZ_RESET            	 	GPIO_PIN_8									/* ����W5500��RESET�ܽ�         */
#define WIZ_SPIx_RESET_PORT   	GPIOB									  	  /* GPIO�˿�                     */
#define WIZ_SPIx_RESET_CLK    	RCU_GPIOB	 	    						/* GPIO�˿�ʱ��    				      */

#define WIZ_INT               	GPIO_PIN_7									/* ����W5500��INT�ܽ�           */
#define WIZ_SPIx_INT_PORT     	GPIOB									      /* GPIO�˿�                     */
#define WIZ_SPIx_INT_CLK      	RCU_GPIOB	    						  /* GPIO�˿�ʱ��                 */


#define FW_VER_HIGH  						1               
#define FW_VER_LOW    					0
#define ON	                 		1
#define OFF	                 		0
#define HIGH	           	 			1
#define LOW		             			0

#define MAX_BUF_SIZE		 				1460       			            /*����ÿ�����ݰ��Ĵ�С*/
#define KEEP_ALIVE_TIME	     		30	// 30sec
#define TX_RX_MAX_BUF_SIZE      2048							 
#define EEPROM_MSG_LEN        	sizeof(EEPROM_MSG)

#define IP_FROM_DEFINE	        0       			              /*ʹ�ó�ʼ�����IP��Ϣ*/

extern uint8	ip_from;											                /*ѡ��IP��Ϣ����Դ*/
#pragma pack(1)
/*�˽ṹ�嶨����W5500�ɹ����õ���Ҫ����*/
typedef struct _CONFIG_MSG											
{
  uint8 mac[6];																							/*MAC��ַ*/
  uint8 lip[4];																							/*local IP����IP��ַ*/
  uint8 sub[4];																							/*��������*/
  uint8 gw[4];																							/*����*/	
  uint8 dns[4];																							/*DNS��������ַ*/
  uint8 rip[4];																							/*remote IPԶ��IP��ַ*/
	uint8 sw_ver[2];																					/*����汾��*/

}CONFIG_MSG;
#pragma pack()

#pragma pack(1)
/*�˽ṹ�嶨����eepromд��ļ����������ɰ����޸�*/
typedef struct _EEPROM_MSG	                    
{
	uint8 mac[6];																							/*MAC��ַ*/
  uint8 lip[4];																							/*local IP����IP��ַ*/
  uint8 sub[4];																							/*��������*/
  uint8 gw[4];																							/*����*/
}EEPROM_MSG_STR;
#pragma pack()

extern CONFIG_MSG  	ConfigMsg;


/*MCU������غ���*/
void gpio_for_w5500_config(void);														/*SPI�ӿ�reset ���ж�����*/


/*W5500SPI��غ���*/
void IINCHIP_WRITE( uint32 addrbsb,  uint8 data);						/*д��һ��8λ���ݵ�W5500*/
uint8 IINCHIP_READ(uint32 addrbsb);													/*��W5500����һ��8λ����*/
uint16 wiz_write_buf(uint32 addrbsb,uint8* buf,uint16 len);	/*��W5500д��len�ֽ�����*/
uint16 dma_wiz_write_buf(uint32 addrbsb,uint8* buf,uint16 len);		//	ֱ����DMA��w5500д������
uint16 wiz_write_buf_2byte(uint32 addrbsb,uint16 * buf,uint16 len);	/*��W5500д��len�ֽ�����*/
uint16 wiz_read_buf(uint32 addrbsb, uint8* buf,uint16 len);	/*��W5500����len�ֽ�����*/
void spi_dma_send(uint8* buf,uint16 len);		//	ֱ����dma����
/*W5500����������غ���*/
void reset_w5500(void);																			/*Ӳ��λW5500*/
void set_w5500_mac(void);																		/*����W5500��MAC��ַ*/
void set_w5500_ip(void);																		/*����W5500��IP��ַ*/


#endif
