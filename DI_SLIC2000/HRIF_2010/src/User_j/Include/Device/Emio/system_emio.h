/*
  ** @file           : emio.h
  ** @brief          : Emio Header File
  **
  ** @attention
  **
  ** Copyright (c) 2020 ChengDuHangke.
  ** All rights reserved.
  **
  ** This software is licensed by ChengDuHangke under Ultimate Liberty license
  **
*/


#ifndef __SYSTEM_EMIO_H_
#define __SYSTEM_EMIO_H_

/*
  ** Include
*/
#include "xGpiops.h"


/*
  ** Define
*/
#define     EMIO_MIN_ID         (0)
#define     EMIO_MAX_ID         (117)

#define 	SYSTEM_CPU0_LED		(54)
#define 	SYSTEM_CPU1_LED     (55)
#define 	SYSTEM_LWIP_LED     (56)

#define 	SYSTEM_BEEP         (57)
#define 	DPS0_ERR          	(58)
#define 	DPS1_ERR          	(59)
#define 	DPS0_IRQ          	(60)
#define 	DPS1_IRQ          	(61)

#define 	B0_BRD_IRQ          (62)
#define 	B1_BRD_IRQ          (63)
#define 	U0_BRD_IRQ          (64)
#define 	U1_BRD_IRQ          (65)
#define 	U2_BRD_IRQ          (66)
#define 	BURN_IRQ            (67)
#define 	DA_IRQ          	(68)

#define 	IIC_INT          	(69)
#define 	IIC_REST          	(70)
#define 	DA_TRIG          	(71)
#define 	SYS_TRIG          	(75)

#define     SPI_CS              (74)

#define     INPUT_PIN           (0)
#define     OUTPUT_PIN          (1)

#define     ENABLE_OUTPUT       (1)
#define     DISABLE_OUTPUT      (0)

#define     HIGH_LEVEL          (1)
#define     LOW_LEVEL           (0)


/*
  ** Function Prototype
*/
u8 Emio_Config(u16 DeviceId,XGpioPs *Emio);
u8 Get_Ipaddr(XGpioPs *Emio);
u8 Set_System_Lwip_Led(XGpioPs *Emio,u8 Status);
u8 Set_System_Cpu0_Led(XGpioPs *Emio,u8 Status);
u8 Set_System_Beep(XGpioPs *Emio,u8 Status);
u8 Set_Spi_Cs(XGpioPs *Emio,u8 Status);
XGpioPs * Get_Emio_Point(void);

#endif /* EMIO_H_ */


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
