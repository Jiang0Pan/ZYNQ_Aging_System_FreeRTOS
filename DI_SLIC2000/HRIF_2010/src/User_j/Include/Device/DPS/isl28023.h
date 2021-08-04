/*
  ** @file           : isl28023.h
  ** @brief          : ISL28023 Information Header File
  ** 				   (DPM . ADC . IIC Control Device)
  **
  ** @attention
  **
  ** Copyright (c) 2020 ChengDuHangke.
  ** All rights reserved.
  **
  ** This software is licensed by ChengDuHangke under Ultimate Liberty license
  **
*/

#ifndef ISL28023_H_
#define ISL28023_H_


/*
  ** include
*/
#include "IIC.h"


/*
  ** Define
*/
#define		DPM_VSHUNT_FS		(0.08)				//V
#define		DPM_ADC_RES			(1<<15)				//65535
#define		DPM_SCALE			  (0.00512)			//
#define		DPM_VBUSLSB_12	(0.00025)			//V
#define		DPM_VBUSLSB_60	(0.001)				//V
#define		DPM_VSHUNTLSB		(0.0000025)			//V
#define		DPM_CURRENTLSB	(0.00000244140625)	//(mA/full) CurrentLSBm
#define		DPM_MAX_VBUS		(0x0)			  	//12V


/*
  ** Typedef enum
*/
/* ISL28023 Register */
typedef enum {

	//IC Device Details
	DPM_CAPABILITY				= 0x19,
	DPM_VOUT_MODE				= 0x20,
	DPM_PMBUS_REV				= 0x99,
	DPM_IC_DEVICE_ID			= 0xAD,
	DPM_IC_DEVICE_REV			= 0xAE,

	//Global IC Controls
	DPM_RESTORE_DEFAULT_LL		= 0x12,
	DPM_OPERATION				= 0x01,

	// channel control
	DPM_SET_DPM_MODE			= 0xD2,
	DPM_DPM_CONV_STATUS			= 0xD3,
	DPM_CONFIG_ICHANNEL			= 0xD4,
	DPM_IOUT_CAL_GAIN			= 0x38,
	DPM_CONFIG_VCHANNEL			= 0xD5,
	DPM_CONFIG_PEAK_DET			= 0xD7,
	DPM_CONFIG_EXCITATION		= 0xE2,

	//Measurement Registers
	DPM_READ_VSHUNT_OUT			= 0xD6,
	DPM_READ_VOUT				= 0x8B,
	DPM_READ_IOUT				= 0x8C,
	DPM_READ_PEAK_MIN_IOUT		= 0xD8,
	DPM_READ_PEAK_MAX_IOUT		= 0xD9,
	DPM_READ_POUT				= 0x96,
	DPM_READ_VSHUNT_OUT_AUX		= 0xE0,
	DPM_READ_VOUT_AUX			= 0xE1,
	DPM_READ_TEMPERATURE_1		= 0x8D,

	//Threshold Detectors
	DPM_VOUT_OV_THRESHOLD_SET	= 0xDA,
	DPM_VOUT_UV_THRESHOLD_SET	= 0xDB,
	DPM_IOUT_OC_THRESHOLD_SET	= 0xDC,

	//SMB Alert
	DPM_CONFIG_INTR				= 0xDD,
	DPM_FORCE_FEEDTHR_ALERT		= 0xDE,
	DPM_SMBALERT_MASK			= 0x1B,
	DPM_SMBALERT2_MASK			= 0xDF,
	DPM_CLEAR_FAULTS			= 0x03,
	DPM_STATUS_VOUT				= 0x7A,
	DPM_STATUS_IOUT				= 0x7B,
	DPM_STATUS_TEMPERATURE		= 0x7D,
	DPM_STATUS_CML				= 0x7E,
	DPM_STATUS_BYTE				= 0x78,
	DPM_STATUS_WORD				= 0x79,

	//Voltage Margin
	DPM_CONFIG_VOL_MARGIN		= 0xE4,
	DPM_SET_VOL_MARGIN			= 0xE3,

	//External Clock Control
	DPM_CONFIG_EXT_CLK			= 0xE5,
}Dpm_Reg;


/*
  ** Function Prototype
*/
u8 Dpm_Init(u8 Bid);
u16 Dpm_Gain_Count(void);
int Dpm_Reg_Val(u16 Regval, u8 EQ_N);
float Dpm_VBus(Dpm_Info *dpm, IIC_Chan Chan);
float Dpm_VShunt(Dpm_Info *dpm, IIC_Chan Chan);
float Dpm_IOUT(Dpm_Info *dpm, IIC_Chan Chan);
float Dpm_POUT(Dpm_Info *dpm, IIC_Chan Chan);
float Dpm_TEMP(Dpm_Info *dpm, IIC_Chan Chan);
float Dpm_Max_IOUT(Dpm_Info *dpm, IIC_Chan Chan);
float Dpm_Min_IOUT(Dpm_Info *dpm, IIC_Chan Chan);
u8 Config_Isl28023_Reg(u8 Reg, u16 S_buf, u16 S_byte,u8 Slave_addr,IIC_Chan Chan);
s32 Dpm_Config(Dpm_Info *dpm, IIC_Chan Chan);
void Build_Dps_Info(short DpsVI[], short DpsVO[], short DpsIO[], short DpsTemp[], u16 DpsStat[]);
u8 Set_dac_out(float voltage, u8 SlaveAddr, IIC_Chan Chan);


#endif /* ISL28023_H_ */


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
