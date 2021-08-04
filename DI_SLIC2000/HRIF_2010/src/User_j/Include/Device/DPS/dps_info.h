/*
  ** @file           : dps_info.c
  ** @brief          : Get DPS Channel Information From ISL28023 Header File
  **
  ** @attention
  **
  ** Copyright (c) 2020 ChengDuHangke.
  ** All rights reserved.
  **
  ** This software is licensed by ChengDuHangke under Ultimate Liberty license
  **
*/


#ifndef DPS_INFO_H_
#define DPS_INFO_H_


/*
  ** Include
*/
#include "GlobalRef.h"
#include "isl28023.h"
#include "IIC.h"


/*
  ** Define
*/
/* 6 Channel DPS Device IIC Address */
/* DPS Chip */
#define     DPSA_8272_ADDR      (0x19)
#define     DPSB_8277_ADDR      (0x1A)
#define     DPSC_8277_ADDR      (0x1B)
#define     DPSD_8277_ADDR      (0x1C)
#define     DPSE_8277_ADDR      (0x1D)
#define     DPSF_8277_ADDR      (0x1E)
/* Dpm Chip . ISL28023 ADC Device */
#define     DPSA_28023_ADDR     (0x40)
#define     DPSB_28023_ADDR     (0x41)
#define     DPSC_28023_ADDR     (0x42)
#define     DPSD_28023_ADDR     (0x43)
#define     DPSE_28023_ADDR     (0x44)
#define     DPSF_28023_ADDR     (0x45)
/* 10 Channel DPS Bottom Board DAC Device IIC Address */
#define     DPS_AD5694_ADDR     (0x0C)
/* 10 Channel DPS Bottom Board ADC Device IIC Address */
#define     DPS1_28023_ADDR     (0x51)
#define     DPS2_28023_ADDR     (0x52)
#define     DPS3_28023_ADDR     (0x53)
#define     DPS4_28023_ADDR     (0x51)
/* DPS Board 0 */
#define     DPS_BD0             (0x00)
/* DPS Board 1 */
#define     DPS_BD1             (0x01)
/* Timer delay (ms )*/
#define    TIMER_DELAY          (1)


typedef enum {
	/* DPS Channel */
	/* DPSA CHANNEL Board 0 */
	DPSA_CHANNEL_BD0 	= 0x00,
	/* DPSB CHANNEL Board 0 */
	DPSB_CHANNEL_BD0 	= 0x01,
	/* DPSC CHANNEL Board 0 */
	DPSC_CHANNEL_BD0 	= 0x02,
	/* DPSD CHANNEL Board 0 */
	DPSD_CHANNEL_BD0 	= 0x03,
	/* DPSE CHANNEL Board 0 */
	DPSE_CHANNEL_BD0 	= 0x04,
	/* DPSF CHANNEL Board 0 */
	DPSF_CHANNEL_BD0 	= 0x05,
	/* DPS1 CHANNEL Board 0 */
	DPS1_CHANNEL_BD0 	= 0x06,
	/* DPSA CHANNEL Board 0 */
	DPS2_CHANNEL_BD0 	= 0x07,
	/* DPS3 CHANNEL Board 0 */
	DPS3_CHANNEL_BD0 	= 0x08,
	/* DPS4 CHANNEL Board 0 */
	DPS4_CHANNEL_BD0 	= 0x09,
	/* DPSA CHANNEL Board 1 */
	DPSA_CHANNEL_BD1 	= 0x0A,
	/* DPSB CHANNEL Board 1 */
	DPSB_CHANNEL_BD1 	= 0x0B,
	/* DPSC CHANNEL Board 1 */
	DPSC_CHANNEL_BD1 	= 0x0C,
	/* DPSD CHANNEL Board 1 */
	DPSD_CHANNEL_BD1 	= 0x0D,
	/* DPSE CHANNEL Board 1 */
	DPSE_CHANNEL_BD1 	= 0x0E,
	/* DPSF CHANNEL Board 1 */
	DPSF_CHANNE_BD1L 	= 0x0F,
	/* DPS1 CHANNEL Board 1 */
	DPS1_CHANNEL_BD1 	= 0x10,
	/* DPSA CHANNEL Board 1 */
	DPS2_CHANNEL_BD1 	= 0x11,
	/* DPS3 CHANNEL Board 1 */
	DPS3_CHANNEL_BD1 	= 0x12,
	/* DPS4 CHANNEL Board 1 */
	DPS4_CHANNEL_BD1 	= 0x13,
}Dps_channel;


/*
  ** Function Prototype
*/
u8 Dps_Init(u8 Bid);
u8 Read_Error_Info(Sample_Data * Sample, u8 Bid);
u8 Dps_Config(Dps_Arg * dps_cfg, Sample_Data * Sample, u8 protect, u8 Bid, u32 * PON_FLAG);
u8 Dps_Disable(Dps_Arg * dps_cfg, Sample_Data * Sample, u8 Bid, u32 * POFF_FLAG);
u8 Dps_Set_Compare_Voltagr(Dps_Arg * dps_cfg, u8 Bid);
u8 Dps_Enable_Check_Error(Dps_Arg * dps_cfg, u8 Bid, u32 * PON_FLAG);
u8 Dps_Disable_Check_Error(u8 Bid);
u8 Dps_Info_Clear(Sample_Data * Sample);
u8 Get_Max_Open_Delay(Dps_Arg * dps_cfg, u32 * Delay, u8 Bid);
u8 Get_Max_Close_Delay(Dps_Arg * dps_cfg, u32 * Delay, u8 Bid);


/* 
  ** extern
*/
extern u8 dpsa_disable;


#endif /* DPS_INFO_H_ */


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
