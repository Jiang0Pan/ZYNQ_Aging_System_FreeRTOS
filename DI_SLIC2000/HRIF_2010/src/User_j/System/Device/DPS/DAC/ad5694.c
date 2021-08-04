/*
  ** @file           : ad5694.c
  ** @brief          : AD5694 Device (ADC Device) Source File
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
#include "sleep.h"
#include "return_val.h"
#include "debug_config.h"
#include "GlobalRef.h"
#include "SMBus.h"
#include "dps_info.h"
#include "system_state.h"
#include "ad5694.h"


/*
  ** Define
*/
/* AD5694 Device IIC Address */
#define 	AD5694_ONCHIP_ADDRESS           (0x0C)
/* AS5694 Commands */
/* No Operation Command */
#define 	AD5694_CMD_NO_OPERATION         (0x0)
/*
  ** Write Input Register Command
  ** If LADC is low write input register auto load DAC register
  ** If LADC is high write input register need to updata DAC register
*/
#define 	AD5694_CMD_WR_REG_NEED_LDAC     (0x1)
/*
  ** Update DAC Register Command
  ** Update DAC Register n with contents of Input Register n
*/
#define 	AD5694_CMD_UPDATE_REG           (0x2)
/*
  ** Write DAC Register Command
  ** Write to and update DAC Channel n
*/
#define 	AD5694_CMD_WR_UPDATE_REG        (0x3)
/*
  ** Power Down/Up DAC Command
  ** Power down/power up DAC
*/
#define 	AD5694_CMD_PWR_ON_OFF           (0x4)
/* Hardware LDAC mask register */
#define 	AD5694_CMD_HW_DAC               (0x5)
/* Software reset (power-on reset) */
#define 	AD5694_CMD_SOFT_RST             (0x6)
/* AD5694(DAC Device) reference voltage */
#define 	AD5694_VREF						(5.0f)
/* AD5694 Bit Width */
#define 	AD5694_BIT_WIDTH                (12)
/* AD5694 Command Bit */
#define     AD5694_CMD_BIT                  (4)
/* AD5694 Invalid Resgiter Bit */
#define     AD5694_INV_REG_VIT              (4)

/* All Disselected DAC A Channel */
#define     DAC_NO_CHAN                 	(0x0)
/* Selected DAC A Channel */
#define 	DAC_A_CHAN						(0x1)
/* Selected DAC B Channel */
#define     DAC_B_CHAN						(0x2)
/* Selected DAC A & B Channel */
#define     DAC_A_B_CHAN					(0x3)
/* Selected DAC C Channel */
#define     DAC_C_CHAN						(0x4)
/* Selected DAC A & C Channel */
#define     DAC_A_C_CHAN					(0x5)
/* Selected DAC B & A Channel */
#define     DAC_B_C_CHAN					(0x6)
/* Selected DAC A & B & C Channel */
#define     DAC_A_B_C_CHAN					(0x7)
/* Selected DAC D Channel */
#define     DAC_D_CHAN         				(0x8)
/* Selected DAC A & D Channel */
#define     DAC_A_D_CHAN         			(0x9)
/* Selected DAC B & D Channel */
#define     DAC_B_D_CHAN         			(0xA)
/* Selected DAC A & B & D Channel */
#define     DAC_A_B_D_CHAN         			(0xB)
/* Selected DAC C & D Channel */
#define     DAC_C_D_CHAN         			(0xC)
/* Selected DAC A & C & D Channel */
#define     DAC_A_C_D_CHAN         			(0xD)
/* Selected DAC B & C & D Channel */
#define     DAC_B_C_D_CHAN         			(0xE)
/* Selected DAC A & B & C & D Channel */
#define     DAC_A_B_C_D_CHAN         		(0xF)
/* Hardware Gain */
#define     HW_GAIN                     	(0.4)
/* Communication retry time */
#define     RETRY_TIMES                 	(5)

/* Calibration linearity */
#define     CAB_LINE                        (0.9918f)
/* Calibration offset */
#define     CAB_OFFSET                      (0.0043f)

/* DAC Line mode start voltage */
#define     START_VOLT                      (0)
/* Voltage coefficient */
#define     V_COE                           (1000)    
/* Frequency division coefficient */
#define     F_DIV_COE                       (2.0f)    
/* one steps delay time */
#define     STEP_DELAY                      (90)
/* Cycle timer */
#define     CYCLE_TIMER                     (1)
/* Step compensation value */
#define     STEP_CPS_VLE                    (2)
/* */


/*
  ** Function Name        : Reset_AD5694
  ** Function Description : Reset AD5694 config
  ** Function Parameter   : SlaveAddr 			: AD5694 iic address 
  ** 						Chan                : IIC expand iic channel number
  ** Function Return      : SUCCESS             : Reset success
  ** 						FAILURE             : Reset Failed
  ** Notes				  : None
*/
u8 Reset_AD5694(u8 Bid)
{
	u8 status;
	u8 ch_num;

	for (ch_num = 0; ch_num < 4; ch_num++) {
		status = Set_AD5694(START_VOLT, ch_num, dps_info.AD5694_SlaveAddr, \
				dps_info.DBMsg[Bid].Chan);
		if (SET_SUCCESS != status) {
			debug("Board %d AD5694 channel %d Reset Failed", Bid, ch_num);
			return FAILURE;
		}
		else {

		}
	}

	return SUCCESS;
}


/*
  ** Function Name        : Disable_DPS_CH3_CH4
  ** Function Description : Disable DPS (ADC5694 ChanneL) Channel3 Channel4
  ** Function Parameter   : SlaveAddr 			: AD5694 iic address 
  ** 						Chan                : IIC expand iic channel number
  ** Function Return      : SUCCESS             : Reset success
  ** 						FAILURE             : Reset Failed
  ** Notes				  : None
*/
u8 Disable_DPS_CH3_CH4(u8 Bid)
{
	switch (Bid) {
		case DPS_BD0 ... DPS_BD1: {
			if (SET_SUCCESS != Set_AD5694((float)START_VOLT , 2, dps_info.AD5694_SlaveAddr,\
					dps_info.DBMsg[Bid].Chan)) {
				return FAILURE;
			}
			else {

			}

			if (SET_SUCCESS != Set_AD5694((float)START_VOLT , 3, dps_info.AD5694_SlaveAddr,\
					dps_info.DBMsg[Bid].Chan)) {
				return FAILURE;
			}
			else {
				
			}
		}
			break;
		default:{
			
		}
			break;
	}

	return SUCCESS;
}


/*
  ** Function Name        : Set_AD5694
  ** Function Description : Set AD5694 Channel Voltage Value
  ** Function Parameter   : Vo 					: Set DAC channel voltage value
  ** 						N_dac               : Which DAC channel
  ** 						SlaveAddr           : AD5694 Device IIC slaveaddr
  ** 						Chan                : IIC expand iic channel number
  ** Function Return      : SUCCESS             : Set success
  ** 						FAILURE             : Set Failed
  ** Notes				  : None
*/
u8 Set_AD5694(float Vo, u8 N_dac, u8 SlaveAddr, IIC_Chan Chan)
{
	float data = 0;
	u32 temp = 0;
	u8 Code  = 0x0;
	u8 buf[2]  = {0x0};
	u8 ch = DAC_NO_CHAN;

	/* Choose channel */
	switch (N_dac) {
		case 0: {
				ch = DAC_A_CHAN;
			}
			break;
		case 1: {
				ch = DAC_B_CHAN;
			}
			break;
		case 2: {
				ch = DAC_C_CHAN;
			}
			break;
		case 3: {
				ch = DAC_D_CHAN;
			}
			break;
		default: {
				ch = DAC_NO_CHAN;
			}
			break;
	}

	data = Vo * HW_GAIN;
	/* Calibration */
	data = (data - CAB_OFFSET) / CAB_LINE;
	/* Calculation voltage */
	Code = (u8)((AD5694_CMD_WR_UPDATE_REG << AD5694_CMD_BIT) | (ch & 0x0F));
	temp = (u16)(data * ((1 << AD5694_BIT_WIDTH) - 1) / AD5694_VREF);
	temp = temp << AD5694_INV_REG_VIT;

	buf[0] = (temp >> 8) & 0xFF;
	buf[1] = (temp >> 0) & 0xFF;

	if (SMBus_Write(buf, 2, SlaveAddr, Code, Chan)) {
		debug("Set AD5694 Voltage Failed\r\n");
		return SET_FAILED;
	}

	return SET_SUCCESS;
}


/*
  ** Function Name        : AD5694_Power_On
  ** Function Description : AD5694 set line power on
  ** Function Parameter   : xTimer              : xTimer handler
                            Bid                 : Board id
  ** Function Return      : SET_SUCCESS         : Set success
							SET_FAILED          : Set failed
  ** Notes				  : None
*/
u8 AD5694_Power_On(TimerHandle_t xTimer, u8 Bid)
{
	/* start voltage channel 0 Borad */
	static u16 s_voltage0_on = START_VOLT;
	/* start voltage channel 1 Borad */
	static u16 s_voltage1_on = START_VOLT;

	/* Dps config parameter */
	static Dps_Arg * dps_arg_on = (Dps_Arg *)DPS_CFG_DATA_ADDR;
	/* Timer count */
    static u16 count_on = 0x00;
    /* Max Delay*/
    static u16 max_delay_on = 0x00;
	/* Calculate the step value channel 0 Board */
	static u16 step0_on = 0x00;
	static u16 vol0_on = 0x00;
	static u16 step_v0_on = 0x00;
	static u16 delay0_on = 0x00;

	/* Calculate the step value channel 1 Board */
	static u16 step1_on = 0x00;
	static u16 vol1_on = 0x00;
	static u16 step_v1_on = 0x00;
	static u16 delay1_on = 0x00;	

	/* Calculate the step start value channel0 & channel1 board */
	if (count_on == 0) {
		/* Channel 1 Board */
		step0_on = (dps_arg_on->Dsp[6].Open.Keep / TIMER_DELAY) + STEP_CPS_VLE;
		vol0_on =  dps_arg_on->Dsp[6].SetValue;
		step_v0_on = (vol0_on - START_VOLT) / (((dps_arg_on->Dsp[6].Open.Keep - \
				dps_arg_on->Dsp[6].Open.STime) / TIMER_DELAY) + STEP_CPS_VLE);
		delay0_on = dps_arg_on->Dsp[6].Open.STime / TIMER_DELAY;
		s_voltage0_on = START_VOLT;

		/* Channel 0 Board */
		step1_on = (dps_arg_on->Dsp[7].Open.Keep / TIMER_DELAY) + STEP_CPS_VLE;
		vol1_on =  dps_arg_on->Dsp[7].SetValue;
		step_v1_on = (vol1_on- START_VOLT) / (((dps_arg_on->Dsp[7].Open.Keep - \
				dps_arg_on->Dsp[7].Open.STime) / TIMER_DELAY) + STEP_CPS_VLE);
		delay1_on = dps_arg_on->Dsp[7].Open.STime / TIMER_DELAY;
		s_voltage1_on = START_VOLT;
	
		max_delay_on = (step0_on > step1_on) ? step0_on : step1_on;
	}

	count_on++;

	/* Channel 1 board 0 line mode power on . DPS3 */
	if ((count_on >= delay0_on) && (count_on <= step0_on)) {
		if (count_on == step0_on) {
			/* Channel 1 Board */
			if (SET_SUCCESS != Set_AD5694((float)vol0_on / V_COE, 1, dps_info.AD5694_SlaveAddr, dps_info.DBMsg[Bid].Chan)) {
				return FAILURE;
			}
			else {
		
			}
		}	
		else {
			s_voltage0_on += step_v0_on;
			/* Channel 1 Board */
			if (SET_SUCCESS != Set_AD5694((float)s_voltage0_on / V_COE, 1, dps_info.AD5694_SlaveAddr, dps_info.DBMsg[Bid].Chan)) {
				return FAILURE;
			}
			else {
		
			}
		}	
	}
	else {

	}
	/* Channel 0 board line mode power on . DPS4 */
	if ((count_on >= delay1_on) && (count_on <= step1_on)) {
		if (count_on == step1_on) {
			/* Channel 0 Board */			
			if (SET_SUCCESS != Set_AD5694((float)vol1_on / V_COE, 0, dps_info.AD5694_SlaveAddr, dps_info.DBMsg[Bid].Chan)) {
				return FAILURE;
			}
			else {
			
			}			
		}
		else {
			s_voltage1_on += step_v1_on;
			/* Channel 0 Board */
			if (SET_SUCCESS != Set_AD5694((float)s_voltage1_on / V_COE, 0, dps_info.AD5694_SlaveAddr, dps_info.DBMsg[Bid].Chan)) {
				return FAILURE;
			}
			else {
			
			}
		}
	}
	else {

	}

    if (count_on >= max_delay_on) {
		xTimerDelete(xTimer, 0);
		count_on = 0x00;
	}	

	return SUCCESS;
}


/*
  ** Function Name        : AD5694_Power_Off
  ** Function Description : AD5694 set line power off
  ** Function Parameter   : xTimer              : xTimer handler
                            Bid                 : Board id
  ** Function Return      : SUCCESS             : Set success
							SET_FAILED          : Set failed
  ** Notes				  : None
*/
u8 AD5694_Power_Off(TimerHandle_t xTimer, u8 Bid)
{
	/* start voltage channel 0 Borad */
	static u16 s_voltage0_off = START_VOLT;
	/* start voltage channel 1 Borad */
	static u16 s_voltage1_off = START_VOLT;


	/* Dps config parameter */
	static Dps_Arg * dps_arg_off = (Dps_Arg *)DPS_CFG_DATA_ADDR;
	/* Timer count */
    static u16 count_off = 0x00;
    /* Max Delay*/
    static u16 max_delay_off = 0x00;
	/* Calculate the step value channel 0 Board */
	static u16 step0_off = 0x00;
	static u16 vol0_off = 0x00;
	static u16 step_v0_off = 0x00;
	static u16 delay0_off = 0x00;

	/* Calculate the step value channel 1 Board */
	static u16 step1_off = 0x00;
	static u16 vol1_off = 0x00;
	static u16 step_v1_off = 0x00;
	static u16 delay1_off = 0x00;	

	/* Calculate the step start value channel0 & channel1 board */
	if (count_off == 0) {
		/* Channel 1 Board */
		step0_off = (dps_arg_off->Dsp[6].Close.Keep / TIMER_DELAY) + STEP_CPS_VLE;
		vol0_off =  dps_arg_off->Dsp[6].SetValue;
		step_v0_off = (vol0_off - START_VOLT) / (((dps_arg_off->Dsp[6].Close.Keep - \
				dps_arg_off->Dsp[6].Close.STime) / TIMER_DELAY) + STEP_CPS_VLE);
		delay0_off = dps_arg_off->Dsp[6].Close.STime / TIMER_DELAY;
		s_voltage0_off = vol0_off;

		/* Channel 0 Board */
		step1_off = (dps_arg_off->Dsp[7].Close.Keep / TIMER_DELAY) + STEP_CPS_VLE;
		vol1_off =  dps_arg_off->Dsp[7].SetValue;
		step_v1_off = (vol1_off - START_VOLT) / (((dps_arg_off->Dsp[7].Close.Keep - \
				dps_arg_off->Dsp[7].Close.STime) / TIMER_DELAY) + STEP_CPS_VLE);
		delay1_off = dps_arg_off->Dsp[7].Close.STime / TIMER_DELAY;
		s_voltage1_off = vol1_off;
	
		max_delay_off = (step0_off > step1_off) ? step0_off : step1_off;
	}

	count_off++;

	/* Channel 1 board 0 line mode power on . DPS3 */
	if ((count_off >= delay0_off) && (count_off <= step0_off)) {
		if (count_off == step0_off) {
			/* Channel 1 Board */
			if (SET_SUCCESS != Set_AD5694((float)START_VOLT / V_COE, 1, dps_info.AD5694_SlaveAddr, dps_info.DBMsg[Bid].Chan)) {
				return FAILURE;
			}
			else {
				
			}
		}	
		else {
			s_voltage0_off -= step_v0_off;
			/* Channel 1 Board */
			if (SET_SUCCESS != Set_AD5694((float)s_voltage0_off / V_COE, 1, dps_info.AD5694_SlaveAddr, dps_info.DBMsg[Bid].Chan)) {
				return FAILURE;
			}
			else {
				
			}
		}	
	}
	else {

	}
	/* Channel 0 board line mode power on . DPS4 */
	if ((count_off >= delay1_off) && (count_off <= step1_off)) {
		if (count_off == step1_off) {
			/* Channel 0 Board */			
			if (SET_SUCCESS != Set_AD5694((float)START_VOLT / V_COE, 0, dps_info.AD5694_SlaveAddr, dps_info.DBMsg[Bid].Chan)) {
				return FAILURE;
			}
			else {
			
			}			
		}
		else {
			s_voltage1_off -= step_v1_off;
			/* Channel 0 Board */
			if (SET_SUCCESS != Set_AD5694((float)s_voltage1_off / V_COE, 0, dps_info.AD5694_SlaveAddr, dps_info.DBMsg[Bid].Chan)) {
				return FAILURE;
			}
			else {
			
			}
		}
	}
	else {

	}

    if (count_off >= max_delay_off) {	
		xTimerDelete(xTimer, 0);
		count_off = 0x00;
	}	

	return SUCCESS;
}


/*
  ** Function Name        : AD5694_Set_Line_Power_On_Task_Bd0
  ** Function Description : Board 0 AD5694 set line power on task
  ** Function Parameter   : pvParameters        : Dps configuration parameter pointer
  ** Function Return      : Void
  ** Notes				  : None
*/
void AD5694_Set_Line_Power_On_Task_Bd0(TimerHandle_t xTimer)
{
	AD5694_Power_On(xTimer, DPS_BD0);
}


/*
  ** Function Name        : AD5694_Set_Line_Power_On_Task_Bd0
  ** Function Description : Board 0 AD5694 set line power on task
  ** Function Parameter   : pvParameters        : Dps configuration parameter pointer
  ** Function Return      : Void
  ** Notes				  : None
*/
void AD5694_Set_Line_Power_On_Task_Bd1(TimerHandle_t xTimer)
{
	AD5694_Power_On(xTimer, DPS_BD1);
}

/*
  ** Function Name        : AD5694_Set_Line_Power_Off_Task_Bd0
  ** Function Description : Board 0 AD5694 set line power off task
  ** Function Parameter   : pvParameters        : Dps configuration parameter pointer
  ** Function Return      : Void
  ** Notes				  : None
*/
void AD5694_Set_Line_Power_Off_Task_Bd0(TimerHandle_t xTimer)
{
	AD5694_Power_Off(xTimer, DPS_BD0);
}


/*
  ** Function Name        : AD5694_Set_Line_Power_On_Task_Bd0
  ** Function Description : Board 0 AD5694 set line power off task
  ** Function Parameter   : pvParameters        : Dps configuration parameter pointer
  ** Function Return      : Void
  ** Notes				  : None
*/
void AD5694_Set_Line_Power_Off_Task_Bd1(TimerHandle_t xTimer)
{
	AD5694_Power_Off(xTimer, DPS_BD1);
}


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/

