/*
  ** @file           : fpga_ctrl.h
  ** @brief          : Through FPGA Control DPS Enable Header File
  **
  ** @attention
  **
  ** Copyright (c) 2020 ChengDuHangke.
  ** All rights reserved.
  **
  ** This software is licensed by ChengDuHangke under Ultimate Liberty license
  **
*/


#ifndef FPGA_CTRL_H_
#define FPGA_CTRL_H_


/*
  ** Include
*/
#include "xil_types.h"
#include "dps_info.h"


/*
  ** Typedef
*/
/*
  ** Fpga control registers
*/
typedef enum {
	/* FPGA firmware version */
	VER_REG             = 0x00,
	/* All channel error information register */
	ERROR_NUM_REG 		= 0x01,
	/* Reset FPGA control register */
	RESET_REG           = 0x02,
	/* Huwei power module enable regsiter */
	HW_ENABLE_REG       = 0x03,
	/* Error start check register */
	ERR_START_REG       = 0x04,
	/* Level 1 enable register */
	ENABLE_LV1_REG 		= 0x05,
	/* Level 2 enable register */
	ENABLE_LV2_REG 		= 0x06,
	/* Level 3 enable register */
	ENABLE_LV3_REG		= 0x07,
	/* Force switch power on */
	POWER_ON_REG 		= 0x08,

	/* ChannelA Level 2 enable delay register . unit : ms */
	CHA_ON_DELAY_REG 	= 0x09,
	/* ChannelB Level 2 enable delay register . unit : ms */
	CHB_ON_DELAY_REG 	= 0x0A,
	/* ChannelC Level 2 enable delay register . unit : ms */
	CHC_ON_DELAY_REG 	= 0x0B,
	/* ChannelD Level 2 enable delay register . unit : ms */
	CHD_ON_DELAY_REG 	= 0x0C,
	/* ChannelE Level 2 enable delay register . unit : ms */
	CHE_ON_DELAY_REG 	= 0x0D,
	/* ChannelF Level 2 enable delay register . unit : ms */
	CHF_ON_DELAY_REG 	= 0x0E,
	/* Channel1 Level 2 enable delay register . unit : ms */
	CH1_ON_DELAY_REG 	= 0x0F,
	/* Channel2 Level 2 enable delay register . unit : ms */
	CH2_ON_DELAY_REG 	= 0x10,
	/* Channel3 Level 2 enable delay register . unit : ms */
	CH3_ON_DELAY_REG 	= 0x11,
	/* Channel4 Level 2 enable delay register . unit : ms */
	CH4_ON_DELAY_REG 	= 0x12,

	/* ChannelA Level 2 disable delay register . unit : ms */
	CHA_OFF_DELAY_REG 	= 0x13,
	/* ChannelB Level 2 disable delay register . unit : ms */
	CHB_OFF_DELAY_REG 	= 0x14,
	/* ChannelC Level 2 disable delay register . unit : ms */
	CHC_OFF_DELAY_REG 	= 0x15,
	/* ChannelD Level 2 disable delay register . unit : ms */
	CHD_OFF_DELAY_REG 	= 0x16,
	/* ChannelE Level 2 disable delay register . unit : ms */
	CHE_OFF_DELAY_REG 	= 0x17,
	/* ChannelF Level 2 disable delay register . unit : ms */
	CHF_OFF_DELAY_REG 	= 0x18,
	/* Channel1 Level 2 disable delay register . unit : ms */
	CH1_OFF_DELAY_REG 	= 0x19,
	/* Channel2 Level 2 disable delay register . unit : ms */
	CH2_OFF_DELAY_REG 	= 0x1A,
	/* Channel3 Level 2 disable delay register . unit : ms */
	CH3_OFF_DELAY_REG 	= 0x1B,
	/* Channel4 Level 2 disable delay register . unit : ms */
	CH4_OFF_DELAY_REG 	= 0x1C,

	/* ChannelA error debounce time register . unit : us*/
	CHA_ERROR_DELAY_REG = 0x1D,
	/* ChannelB error debounce time register . unit : us*/
	CHB_ERROR_DELAY_REG = 0x1E,
	/* ChannelC error debounce time register . unit : us*/
	CHC_ERROR_DELAY_REG = 0x1F,
	/* ChannelD error debounce time register . unit : us*/
	CHD_ERROR_DELAY_REG = 0x20,
	/* ChannelE error debounce time register . unit : us*/
	CHE_ERROR_DELAY_REG = 0x21,
	/* ChannelF error debounce time register . unit : us*/
	CHF_ERROR_DELAY_REG = 0x22,
	/* Channel1 error debounce time register . unit : us*/
	CH1_ERROR_DELAY_REG = 0x23,
	/* Channel2 error debounce time register . unit : us*/
	CH2_ERROR_DELAY_REG = 0x24,
	/* Channel3 error debounce time register . unit : us*/
	CH3_ERROR_DELAY_REG = 0x25,
	/* Channel4 error debounce time register . unit : us*/
	CH4_ERROR_DELAY_REG = 0x26,
}Fpga_dps_reg;


typedef enum {
	DPS_DISABLE = 0x00,
	DPS_ENABLE = 0x01,
}Enable_status;


typedef enum {
	READ_BITS = 0x00,
	WRITE_BITS = 0x01,
}Read_Write_sts;


typedef enum {
	/* All Channel Disable */
	ALL_DISABLE = 0x0000,
	/* All Channel Enable level 1 */
	L1_ALL_ENABLE = 0x003F,
	/* All Channel Enable level 2 */	
	L2_ALL_ENABLE = 0x03FF,
	/* Channel A ~ F Disable level 2 */
	L2_CHA_F_DISABLE = 0x03C0,
	/* All Channel Enable level 3 */	
	L3_ALL_ENABLE = 0x003F,	
	/* All Channel Enable Error Check */	
	ALL_CHECK_ERR = 0x00FF,	
}All_Enable_Status;


/*
  ** Define
*/
#define  CPLD_SPI			(SPI0)


/*
  ** Function Prototype
*/
s32 Fpga_Dps_Transfer(u8 * t_data, u8 * r_data,u16 length, u8 bid);
u8 Config_Dps_Fpga_Reg(u8 reg, u16 reg_data, u8 bid);
u8 Write_Dps_Fpga_Reg(u8 reg, u16 reg_data, u8 bid);
u8 Read_Dps_Fpga_Reg(u8 reg, u16 * reg_data, u8 bid);
u8 Read_Dps_Fpga_Version(u8 * reg_data, u8 bid);
u8 Dps_Fpga_Reset(u8 bid);
u8 Set_All_Dps_Check_Error(All_Enable_Status en, u8 bid);
u8 Set_Dps_Channel_L1_Enable(Dps_channel ch, Enable_status en, u8 bid);
u8 Set_All_Dps_L1_Enable(All_Enable_Status en, u8 bid);
u8 Set_Dps_Channel_L2_Enable(Dps_channel ch, Enable_status en, u8 bid);
u8 Set_All_Dps_L2_Enable(All_Enable_Status en, u8 bid);
u8 Set_Dps_Channel_L3_Enable(Dps_channel ch, Enable_status en, u8 bid);
u8 Set_All_Dps_L3_Enable(All_Enable_Status en, u8 bid);


#endif /* FPGA_CTRL_H_ */


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
