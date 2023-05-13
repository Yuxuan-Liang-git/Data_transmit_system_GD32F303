/*
**************************************************************************************************
* @file    		w5500_conf.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief  		配置MCU，移植W5500程序需要修改的文件，配置W5500的MAC和IP地址
**************************************************************************************************
*/
#include <stdio.h> 
#include <string.h>

#include "w5500_conf.h"
//#include "bsp_i2c_ee.h"
//#include "utility.h"
#include "systick.h"
#include "w5500.h"
#include "dhcp.h"
#include "bsp_TiMbase.h"

CONFIG_MSG  ConfigMsg;																	/*配置结构体*/
EEPROM_MSG_STR EEPROM_MSG;															/*EEPROM存储信息结构体*/

/*定义MAC地址,如果多块W5500网络适配板在同一现场工作，请使用不同的MAC地址*/
uint8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};

/*定义默认IP信息*/
uint8 local_ip[4]  ={192,168,1,88};											/*定义W5500默认IP地址*/
uint8 subnet[4]    ={255,255,255,0};										/*定义W5500默认子网掩码*/
uint8 gateway[4]   ={192,168,1,1};											/*定义W5500默认网关*/
uint8 dns_server[4]={114,114,114,114};									/*定义W5500默认DNS*/

uint16 local_port=5000;	                       					/*定义本地端口*/

/*定义远端IP信息*/
uint8  remote_ip[4]={192,168,1,102};											/*远端IP地址*/
uint16 remote_port=5000;																/*远端端口号*/

/*IP配置方法选择，请自行选择*/
uint8	ip_from=IP_FROM_DEFINE;				

uint8   dhcp_ok   = 0;													   			/*dhcp成功获取IP*/
uint32	ms        = 0;															  	/*毫秒计数*/
uint32	dhcp_time = 0;															  	/*DHCP运行计数*/


/**
*@brief		配置W5500的IP地址
*@param		无
*@return	无
*/
void set_w5500_ip(void)
{	
		
   /*复制定义的配置信息到配置结构体*/
	memcpy(ConfigMsg.mac, mac, 6);
	memcpy(ConfigMsg.lip,local_ip,4);
	memcpy(ConfigMsg.sub,subnet,4);
	memcpy(ConfigMsg.gw,gateway,4);
	memcpy(ConfigMsg.dns,dns_server,4);
	if(ip_from==IP_FROM_DEFINE)	
		printf(" 使用定义的IP信息配置W5500\r\n");
	
		
	/*以下配置信息，根据需要选用*/	
	ConfigMsg.sw_ver[0]=FW_VER_HIGH;
	ConfigMsg.sw_ver[1]=FW_VER_LOW;	

	/*将IP配置信息写入W5500相应寄存器*/	
	setSUBR(ConfigMsg.sub);
	setGAR(ConfigMsg.gw);
	setSIPR(ConfigMsg.lip);
	
	getSIPR (local_ip);			
	printf(" W5500 IP地址   : %d.%d.%d.%d\r\n", local_ip[0],local_ip[1],local_ip[2],local_ip[3]);
	getSUBR(subnet);
	printf(" W5500 子网掩码 : %d.%d.%d.%d\r\n", subnet[0],subnet[1],subnet[2],subnet[3]);
	getGAR(gateway);
	printf(" W5500 网关     : %d.%d.%d.%d\r\n", gateway[0],gateway[1],gateway[2],gateway[3]);
}

/**
*@brief		配置W5500的MAC地址
*@param		无
*@return	无
*/
void set_w5500_mac(void)
{
	memcpy(ConfigMsg.mac, mac, 6);
	setSHAR(ConfigMsg.mac);	/**/
	memcpy(DHCP_GET.mac, mac, 6);
}

/**
*@brief		配置W5500的GPIO接口
*@param		无
*@return	无
*/
void gpio_for_w5500_config(void)
{
   spi_parameter_struct spi_init_struct;

    rcu_periph_clock_enable(WIZ_SPIx_RESET_CLK);
		rcu_periph_clock_enable(WIZ_SPIx_INT_CLK);
		rcu_periph_clock_enable(WIZ_SPIx_GPIO_CLK);
		rcu_periph_clock_enable(WIZ_SPIx_SCS_CLK);	
    rcu_periph_clock_enable(WIZ_SPIx_CLK);
		rcu_periph_clock_enable(RCU_AF);
		//	开启端口复用 好像是gd32需要开的东西
		gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);/*!< JTAG-DP disabled and SW-DP enabled */
		gpio_pin_remap_config(GPIO_SPI0_REMAP, ENABLE);//REMAP SPI0
	
    /* SPI0_SCK(PB3), SPI0_MISO(PB4) and SPI0_MOSI(PB5) GPIO pin configuration */
    gpio_init(WIZ_SPIx_GPIO_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, WIZ_SPIx_SCLK | WIZ_SPIx_MISO | WIZ_SPIx_MOSI);
    /* SPI0_CS(PB6) GPIO pin configuration */
    gpio_init(WIZ_SPIx_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, WIZ_SPIx_SCS);

    /* SPI0 parameter configuration */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_8;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(WIZ_SPIx, &spi_init_struct);

    /* enable SPI0 */
    spi_enable(WIZ_SPIx);
		
		/*定义RESET引脚PB8*/
		gpio_init(WIZ_SPIx_RESET_PORT,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,WIZ_RESET);
		/*定义INT引脚PB7*/	
		gpio_init(WIZ_SPIx_RESET_PORT,GPIO_MODE_IPU,GPIO_OSPEED_50MHZ,WIZ_RESET);
}


/**
*@brief		W5500片选信号设置函数
*@param		val: 为“0”表示片选端口为低，为“1”表示片选端口为高
*@return	无
*/
void wiz_cs(uint8_t val)
{
	if (val == LOW) 
	{
		gpio_bit_reset(WIZ_SPIx_SCS_PORT,WIZ_SPIx_SCS);
	}
	else if (val == HIGH)
	{
		gpio_bit_set(WIZ_SPIx_SCS_PORT,WIZ_SPIx_SCS);
	}
}

/**
*@brief		设置W5500的片选端口SCSn为低
*@param		无
*@return	无
*/
void iinchip_csoff(void)
{
	wiz_cs(LOW);
}

/**
*@brief		设置W5500的片选端口SCSn为高
*@param		无
*@return	无
*/
void iinchip_cson(void)
{	
   wiz_cs(HIGH);
}

/**
*@brief		W5500复位设置函数
*@param		无
*@return	无
*/
void reset_w5500(void)
{
	gpio_bit_reset(WIZ_SPIx_RESET_PORT,WIZ_RESET);
	delay_1us(100);  
//	delay_1ms(1);
	gpio_bit_set(WIZ_SPIx_RESET_PORT,WIZ_RESET);
	delay_1ms(1);
}

uint8_t SPI_SendByte(uint8_t byte)
{
  /* Loop while DR register in not emplty */
	while(RESET == spi_i2s_flag_get(WIZ_SPIx, SPI_FLAG_TBE));

  /* Send byte through the SPI0 peripheral */
	spi_i2s_data_transmit(WIZ_SPIx, byte);

  /* Wait to receive a byte */
	while(RESET == spi_i2s_flag_get(WIZ_SPIx, SPI_FLAG_RBNE));

  /* Return the byte read from the SPI bus */
	return spi_i2s_data_receive(WIZ_SPIx);
}

/**
*@brief		STM32 SPI1读写8位数据
*@param		dat：写入的8位数据
*@return	无
*/
uint8  IINCHIP_SpiSendData(uint8 dat)
{
   return(SPI_SendByte(dat));
}

/**
*@brief		写入一个8位数据到W5500
*@param		addrbsb: 写入数据的地址
*@param   data：写入的8位数据
*@return	无
*/
void IINCHIP_WRITE( uint32 addrbsb,  uint8 data)
{
   iinchip_csoff();                              		
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);	
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4);  
   IINCHIP_SpiSendData(data);                   
   iinchip_cson();                            
}

/**
*@brief		从W5500读出一个8位数据
*@param		addrbsb: 写入数据的地址
*@param   data：从写入的地址处读取到的8位数据
*@return	无
*/
uint8 IINCHIP_READ(uint32 addrbsb)
{
   uint8 data = 0;
   iinchip_csoff();                            
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8))    ;
   data = IINCHIP_SpiSendData(0x00);            
   iinchip_cson();                               
   return data;    
}

/**
*@brief		向W5500写入len字节数据
*@param		addrbsb: 写入数据的地址
*@param   buf：写入字符串
*@param   len：字符串长度
*@return	len：返回字符串长度
*/
uint16 wiz_write_buf(uint32 addrbsb,uint8* buf,uint16 len)
{
   uint16 idx = 0;
   if(len == 0) printf("Unexpected2 length 0\r\n");
   iinchip_csoff();                               
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4); 
   for(idx = 0; idx < len; idx++)
   {
     IINCHIP_SpiSendData(buf[idx]);
   }
   iinchip_cson();                           
   return len;  
}

/**
*@brief		从W5500读出len字节数据
*@param		addrbsb: 读取数据的地址
*@param 	buf：存放读取数据
*@param		len：字符串长度
*@return	len：返回字符串长度
*/
uint16 wiz_read_buf(uint32 addrbsb, uint8* buf,uint16 len)
{
  uint16 idx = 0;
  if(len == 0)
  {
    printf("Unexpected2 length 0\r\n");
  }
  iinchip_csoff();                                
  IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
  IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
  IINCHIP_SpiSendData( (addrbsb & 0x000000F8));    
  for(idx = 0; idx < len; idx++)                   
  {
    buf[idx] = IINCHIP_SpiSendData(0x00);
  }
  iinchip_cson();                                  
  return len;
}



