/*
  ** @file           : isl8272.h
  ** @brief          : The Device Of ISL8272(IIC PWR Device) Header File
  **
  ** @attention
  **
  ** Copyright (c) 2020 ChengDuHangke.
  ** All rights reserved.
  **
  ** This software is licensed by ChengDuHangke under Ultimate Liberty license
  **
*/


#ifndef ISL8272_H_
#define ISL8272_H_


/*
  ** Include
*/
#include "xil_types.h"
#include "dps_info.h"


/*
  ** Define
*/


/*
  ** Typedef Enum
*/
/* ISL8272 Register */
typedef enum {
	OPERATION 					= 0x01,
	ON_OFF_CONFIG 				= 0x02,
	CLEAR_FAULTS  				= 0x03,
	STORE_USER_ALL				= 0x15,
	RESTORE_USER_ALL			= 0x16,
	VOUT_MODE					= 0x20,
	VOUT_COMMAND				= 0x21,
	VOUT_CAL_OFFSET				= 0x23,
	VOUT_MAX					= 0x24,
	VOUT_MARGIN_HIGH			= 0x25,
	VOUT_MARGIN_LOW				= 0x26,
	VOUT_TRANSITION_RATE		= 0x27,
	VOUT_DROOP					= 0x28,
	FREQUENCY_SWITCH			= 0x33,
	INTERLEAVE					= 0x37,
	IOUT_CAL_GAIN				= 0x38,
	IOUT_CAL_OFFSET				= 0x39,
	VOUT_OV_FAULT_LIMIT			= 0x40,
	VOUT_OV_FAULT_RESPONSE		= 0x41,
	VOUT_OV_WARN_LIMIT			= 0x42,
	VOUT_UV_WARN_LIMIT			= 0x43,
	VOUT_UV_FAULT_LIMIT			= 0x44,
	VOUT_UV_FAULT_RESPONSE		= 0x45,
	IOUT_OC_FAULT_LIMIT			= 0x46,
	IOUT_UC_FAULT_LIMIT			= 0x4B,
	OT_FAULT_LIMIT				= 0x4F,
	OT_FAULT_RESPONSE			= 0x50,
	OT_WARN_LIMIT				= 0x51,
	UT_WARN_LIMIT				= 0x52,
	UT_FAULT_LIMIT				= 0x53,
	UT_FAULT_RESPONSE			= 0x54,
	VIN_OV_FAULT_LIMIT			= 0x55,
	VIN_OV_FAULT_RESPONSE		= 0x56,
	VIN_OV_WARN_LIMIT			= 0x57,
	VIN_UV_WARN_LIMIT			= 0x58,
	VIN_UV_FAULT_LIMIT			= 0x59,
	VIN_UV_FAULT_RESPONSE		= 0x5A,
	POWER_GOOD_ON				= 0x5E,
	TON_DELAY					= 0x60,
	TON_RISE					= 0x61,
	TOFF_DELAY					= 0x64,
	TOFF_FALL					= 0x65,
	STATUS_BYTE					= 0x78,
	STATUS_WORD					= 0x79,
	STATUS_VOUT					= 0x7A,
	STATUS_IOUT					= 0x7B,
	STATUS_INPUT				= 0x7C,
	STATUS_TEMPERATURE			= 0x7D,
	STATUS_CML					= 0x7E,
	STATUS_MFR_SPECIFIC			= 0x80,
	READ_VIN					= 0x88,
	READ_VOUT					= 0x8B,
	READ_IOUT					= 0x8C,
	READ_INTERNAL_TEMP			= 0x8D,
	READ_DUTY_CYCLE				= 0x94,
	READ_FREQUENCY				= 0x95,
	READ_IOUT_0					= 0x96,
	READ_IOUT_1					= 0x97,
	MFR_ID						= 0x99,
	MFR_MODEL					= 0x9A,
	MFR_REVISION				= 0x9B,
	MFR_LOCATION				= 0x9C,
	MFR_DATE					= 0x9D,
	MFR_SERIAL					= 0x9E,
	LEGACY_FAULT_GROUP			= 0xA8,
	USER_DATA_00				= 0xB0,
	ISENSE_CONFIG				= 0xD0,
	USER_CONFIG					= 0xD1,
	DDC_CONFIG					= 0xD3,
	POWER_GOOD_DELAY			= 0xD4,
	ASCR_CONFIG					= 0xDF,
	SEQUENCE					= 0xE0,
	DDC_GROUP					= 0xE2,
	DEVICE_ID					= 0xE4,
	MFR_IOUT_OC_FAULT_RESPONSE	= 0xE5,
	MFR_IOUT_UC_FAULT_RESPONSE	= 0xE6,
	SYNC_CONFIG					= 0xE9,
	SNAPSHOT					= 0xEA,
	BLANK_PARAMS				= 0xEB,
	SNAPSHOT_CONTROL			= 0xF3,
	RESTORE_FACTORY				= 0xF4,
	MFR_VMON_OV_FAULT_LIMIT		= 0xF5,
	MFR_VMON_UV_FAULT_LIMIT		= 0xF6,
	MFR_READ_VMON				= 0xF7,
	VMON_OV_FAULT_RESPONSE		= 0xF8,
	VMON_UV_FAULT_RESPONSE		= 0xF9,
}Dps_Reg;

/* Power mode */
typedef enum {
	LINE_MODE	= 0x0,
	LADDER_MODE	= 0x1,
}DpsMode;


/*
  ** Function Prototype
*/
u8 Isl8272_Config(Dps_channel Ch, Dps_Arg * Dps_cfg, u8 Protect, u8 Bid);
u8 Config_Isl8272_Reg(u8 Reg_Addr, u8 * Reg_data, u8 Data_Len ,u8 Slaveaddr, IIC_Chan Chan);
u8 Write_Isl8272_Reg(u8 Reg_Addr, u8 * Reg_data, u8 Data_Len ,u8 Slaveaddr, IIC_Chan Chan);
u8 Read_Isl8272_Reg(u8 Reg_Addr, u8 * Reg_data, u8 Data_Len ,u8 Slaveaddr, IIC_Chan Chan);
u8 Set_Isl8272_Voltage(Dps_channel Ch, float Vo);
u16 CoverToL11(u32 Data);


#endif /* ISL8272_H_ */


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
