/*
  ** @file           : system_emio.c
  ** @brief          : System Emio Source File
  **
  ** @attention
  **
  ** Copyright (c) 2020 ChengDuHangke.
  ** All rights reserved.
  **
  ** This software is licensed by ChengDuHangke under Ultimate Liberty license
  **
*/


/*
  ** Include
*/
#include "system_emio.h"
#include "debug_config.h"
#include "return_val.h"
#include "Driver.h"
#include "eeprom.h"
#include "shell_port.h"



/*
  ** Static Function Prototype
*/
static u8 System_Emio_Init(u16 DeviceId,XGpioPs *Emio);
static u8 System_Emio_Config(XGpioPs *Emio,u8 Pin,u8 Direction,u8 Enable);
static u8 System_Emio_Set(XGpioPs *Emio,u8 Pin,u8 Status);
static u8 System_Emio_Get(XGpioPs *Emio,u8 Pin,u8 *Status);
XGpioPs Emio;


/*
  ** Function Name        : Emio_Config
  ** Function Description : Emio Config
  ** Function Parameter   : DeviceId            : Gpio Device Id
  **                        Emio				: Point To XGpioPs Pointer
  ** Function Return      : CONFIG_SUCCESS 		: Config Success
  **                        CONFIG_FAILED	    : Config Fail
  ** Notes				  : None
*/
u8 Emio_Config(u16 DeviceId,XGpioPs *Emio)
{
	/* System Emio_Init */
	if (INIT_SUCCESS != System_Emio_Init(DeviceId,Emio)) {
		debug("System_Emio_Init Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		return CONFIG_FAILED;
	}

	/* Set SYSTEM_CPU0_LED Pin Output */
	if (CONFIG_SUCCESS != System_Emio_Config(Emio,SYSTEM_CPU0_LED,OUTPUT_PIN,ENABLE_OUTPUT)) {
		debug("SYSTEM_CPU0_LED Config Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		return CONFIG_FAILED;
	}

	/* Set SYSTEM_CPU0_LED Pin Low Level */
	if (SET_SUCCESS != System_Emio_Set(Emio,SYSTEM_CPU0_LED,LOW_LEVEL)) {
		debug("SYSTEM_CPU0_LED Set Low Level Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		return CONFIG_FAILED;
	}

	/* Set SYSTEM_LWIP_LED Pin Output */
	if (CONFIG_SUCCESS != System_Emio_Config(Emio,SYSTEM_LWIP_LED,OUTPUT_PIN,ENABLE_OUTPUT)) {
		debug("SYSTEM_LWIP_LED Config Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		return CONFIG_FAILED;
	}

	/* Set SYSTEM_LWIP_LED Pin Low Level */
	if (SET_SUCCESS != System_Emio_Set(Emio,SYSTEM_LWIP_LED,LOW_LEVEL)) {
		debug("SYSTEM_LWIP_LED Set Low Level Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		return CONFIG_FAILED;
	}

	/* Set SYSTEM_LWIP_LED Pin Output */
	if (CONFIG_SUCCESS != System_Emio_Config(Emio,SYSTEM_BEEP,OUTPUT_PIN,ENABLE_OUTPUT)) {
		debug("SYSTEM_LWIP_LED Config Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		return CONFIG_FAILED;
	}

	/* Set SYSTEM_LWIP_LED Pin Low Level */
	if (SET_SUCCESS != System_Emio_Set(Emio,SYSTEM_BEEP,LOW_LEVEL)) {
		debug("SYSTEM_LWIP_LED Set Low Level Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		return CONFIG_FAILED;
	}

	if (CONFIG_SUCCESS != System_Emio_Config(Emio,SYSTEM_CPU1_LED, OUTPUT_PIN, ENABLE_OUTPUT)) {
		debug("SYSTEM_IP4 Config Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		return CONFIG_FAILED;
	}

	if (SET_SUCCESS != System_Emio_Set(Emio,SYSTEM_CPU1_LED,LOW_LEVEL)) {
		debug("SYSTEM_LWIP_LED Set Low Level Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		return CONFIG_FAILED;
	}

	/* trigger */
	if (CONFIG_SUCCESS != System_Emio_Config(Emio,SYS_TRIG, OUTPUT_PIN, ENABLE_OUTPUT)) {
		debug("SYSTEM_IP4 Config Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		return CONFIG_FAILED;
	}

	if (SET_SUCCESS != System_Emio_Set(Emio,SYS_TRIG,LOW_LEVEL)) {
		debug("SYSTEM_LWIP_LED Set Low Level Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		return CONFIG_FAILED;
	}

	/* IIC reset */
	if (CONFIG_SUCCESS != System_Emio_Config(Emio,IIC_REST, OUTPUT_PIN, ENABLE_OUTPUT)) {
		debug("SYSTEM_IP4 Config Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		return CONFIG_FAILED;
	}


	if (SET_SUCCESS != System_Emio_Set(Emio,IIC_REST, HIGH_LEVEL)) {
		debug("SYSTEM_LWIP_LED Set Low Level Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		return CONFIG_FAILED;
	}

	/* Spi Cs */
	if (CONFIG_SUCCESS != System_Emio_Config(Emio,SPI_CS, OUTPUT_PIN, ENABLE_OUTPUT)) {
		debug("SYSTEM_IP4 Config Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		return CONFIG_FAILED;
	}

	if (SET_SUCCESS != System_Emio_Set(Emio,SPI_CS,HIGH_LEVEL)) {
		debug("SYSTEM_LWIP_LED Set Low Level Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		return CONFIG_FAILED;
	}

	return CONFIG_SUCCESS;
}


/*
  ** Function Name        : Set_System_Lwip_Led
  ** Function Description : Set SYSTEM_CPU0_LED Status
  ** Function Parameter   : Emio				: Point To XGpioPs Pointer
  **                        Status              : HIGH_LEVEL or LOW_LEVEL
  ** Function Return      : SET_SUCCESS 		: Set Success
  **                        SET_FAILED			: Set Fail
  ** Notes				  : None
*/
u8 Set_System_Cpu0_Led(XGpioPs *Emio,u8 Status)
{
	return System_Emio_Set(Emio,SYSTEM_CPU0_LED,Status);
}


/*
  ** Function Name        : Set_System_Lwip_Led
  ** Function Description : Set SYSTEM_LWIP_LED Status
  ** Function Parameter   : Emio				: Point To XGpioPs Pointer
  **                        Status              : HIGH_LEVEL or LOW_LEVEL
  ** Function Return      : SET_SUCCESS 		: Set Success
  **                        SET_FAILED			: Set Fail
  ** Notes				  : None
*/
u8 Set_System_Lwip_Led(XGpioPs *Emio,u8 Status)
{
	return System_Emio_Set(Emio,SYSTEM_LWIP_LED,Status);
}


/*
  ** Function Name        : Set_Spi_Cs
  ** Function Description : Set Spi Cs Low Or High Level
  ** Function Parameter   : Emio				: Point To XGpioPs Pointer
  **                        Status              : HIGH_LEVEL or LOW_LEVEL
  ** Function Return      : SET_SUCCESS 		: Set Success
  **                        SET_FAILED			: Set Fail
  ** Notes				  : None
*/
u8 Set_Spi_Cs(XGpioPs *Emio,u8 Status)
{
	return System_Emio_Set(Emio,SPI_CS,Status);
}


/*
  ** Function Name        : Set_System_Lwip_Led
  ** Function Description : Set SYSTEM_LWIP_LED Status
  ** Function Parameter   : Emio				: Point To XGpioPs Pointer
  **                        Status              : HIGH_LEVEL or LOW_LEVEL
  ** Function Return      : SET_SUCCESS 		: Set Success
  **                        SET_FAILED			: Set Fail
  ** Notes				  : None
*/
u8 Set_System_Beep(XGpioPs *Emio,u8 Status)
{
	return System_Emio_Set(Emio,SYSTEM_BEEP,Status);
}


/*
  ** Function Name        : Get_Ipaddr
  ** Function Description : Get Hardware Ip Addr
  ** Function Parameter   : Emio				: Point To XGpioPs Pointer
  ** Function Return      : ip_addr
  ** Notes				  : None
*/
u8 Get_Ipaddr(XGpioPs *Emio)
{
	u8 ip_addr  = 98;

	ip_addr =  Read_IPAddr(mbd_info.EEPROM_SlaveAddr, mbd_info.HWChan);
	if(ip_addr < 0)
	{
		ip_addr = 127;
		shellprintln("   Use default ip...");
	}

	return ip_addr;
}


/*
  ** Function Name        : System_Emio_Init
  ** Function Description : System Emio Pins Init
  ** Function Parameter   : DeviceId            : Device Id
  **                        Emio				: Point To XGpioPs Pointer
  ** Function Return      : INIT_SUCCESS 		: Init Success
  **                        INIT_FAILED			: Init Fail
  ** Notes				  : Static
*/
static u8 System_Emio_Init(u16 DeviceId,XGpioPs *Emio)
{
	u8 Status;
	XGpioPs_Config *ConfigPtr;

	/* Initialize The GPIO Driver , Xilinx_API */
	ConfigPtr = XGpioPs_LookupConfig(DeviceId);
	Status = XGpioPs_CfgInitialize(Emio, ConfigPtr,
					ConfigPtr->BaseAddr);
	if (XST_SUCCESS != Status) {
		return INIT_FAILED;
	}

	return INIT_SUCCESS;
}


/*
  ** Function Name        : System_Emio_Config
  ** Function Description : System Emio Pin Config
  ** Function Parameter   : Emio				: Point To XGpioPs Pointer
  **                        Pin                 : Which Pin
  **                        Direction           : OUTPUT_PIN or INPUT_PIN
  **                        Enable              : ENABLE_OUTPUT or DISABLE_OUTPUT
  ** Function Return      : CONFIG_SUCCESS 		: Config Success
  **                        CONFIG_FAILED	    : Config Fail
  ** Notes				  : Static
*/
static u8 System_Emio_Config(XGpioPs *Emio,u8 Pin,u8 Direction,u8 Enable)
{
	/* Assert Emio Parameter */
	if (NULL == Emio) {
		return CONFIG_FAILED;
	}

	/* Assert Pin Parameter */
	if ((EMIO_MIN_ID > Pin) || (EMIO_MAX_ID < Pin)) {
		return CONFIG_FAILED;
	}

	/* Assert Direction Parameter */
	if ((OUTPUT_PIN != Direction) && (INPUT_PIN != Direction)) {
		return CONFIG_FAILED;
	}

	/* Assert Enable Parameter */
	if ((ENABLE_OUTPUT != Enable) && (DISABLE_OUTPUT != Enable)) {
		return CONFIG_FAILED;
	}

	/* Pin Direction Is Output */
	if (OUTPUT_PIN == Direction) {
		/* Set Pin Direction Is Output , Xilinx_API */
		XGpioPs_SetDirectionPin(Emio,Pin,OUTPUT_PIN);
		/* Enable or Disable Output Pin, Xilinx_API */
		XGpioPs_SetOutputEnablePin(Emio,Pin, Enable);
	}
	/* Pin Direction Is Input */
	else if (INPUT_PIN == Direction) {
		/* Set Pin Direction Is Input , Xilinx_API */
		XGpioPs_SetDirectionPin(Emio,Pin,INPUT_PIN);
	}
	/* Others */
	else {

	}

	return CONFIG_SUCCESS;
}


/*
  ** Function Name        : System_Emio_Set
  ** Function Description : System Emio Pin Set status
  ** Function Parameter   : Emio				: Point To XGpioPs Pointer
  **                        Pin                 : Which Pin
  **                        Status              : HIGH_LEVEL or LOW_LEVEL
  ** Function Return      : SET_SUCCESS 		: Set Success
  **                        SET_FAILED			: Set Fail
  ** Notes				  : Static
*/
static u8 System_Emio_Set(XGpioPs *Emio,u8 Pin,u8 Status)
{
	/* Assert Emio Parameter */
	if (NULL == Emio) {
		return SET_FAILED;
	}

	/* Assert Pin Parameter */
	if ((EMIO_MIN_ID > Pin) || (EMIO_MAX_ID < Pin)) {
		return SET_FAILED;
	}

	/* Assert Status Parameter */
	if ((HIGH_LEVEL != Status) && (LOW_LEVEL != Status)) {
		return SET_FAILED;
	}

	/* Set Pin Level Status , Xilinx_API */
	XGpioPs_WritePin(Emio, Pin, Status);

	return SET_SUCCESS;
}


/*
  ** Function Name        : System_Emio_Get
  ** Function Description : System Emio Pin Get status
  ** Function Parameter   : Emio				: Point To XGpioPs Pointer
  **                        Pin                 : Which Pin
  **                        Status              : HIGH_LEVEL or LOW_LEVEL
  ** Function Return      : GET_SUCCESS 		: Get Success
  **                        GET_FAILED			: Get Fail
  ** Notes				  : Static
*/
static u8 System_Emio_Get(XGpioPs *Emio,u8 Pin,u8 *Status)
{
	/* Assert Emio Parameter */
	if (NULL == Emio) {
		return GET_FAILED;
	}

	/* Assert Pin Parameter */
	if ((EMIO_MIN_ID > Pin) || (EMIO_MAX_ID < Pin)) {
		return GET_FAILED;
	}

	/* Get Pin Level Status , Xilinx_API */
	*Status = (u8)XGpioPs_ReadPin(Emio, Pin);

	return GET_SUCCESS;
}


/*
  ** Function Name        : Get_Emio_Point
  ** Function Description : Get Point Emio Pointer
  ** Function Parameter   : Void
  ** Function Return      : Queue * 			: The Point Emio Pointer
  ** Notes				  : None
*/
XGpioPs * Get_Emio_Point(void)
{
	return &Emio;
}


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
