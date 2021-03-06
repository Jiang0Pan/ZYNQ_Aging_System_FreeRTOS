/*
  ** @file           : dps_info.c
  ** @brief          : Get DPS Channel Information From ISL28023 Source File
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
#include "FreeRTOS.h"
#include "task.h"
#include "lwip/sys.h"
#include "sleep.h"
#include "return_val.h"
#include "debug_config.h"
#include "GlobalRef.h"
#include "ad5694.h"
#include "fpga_ctrl.h"
#include "isl8272.h"
#include "isl28023.h"
#include "system_state.h"
#include "dps_info.h"


/* 
  ** Define
*/
/* Task stack size */
#define 	THREAD_STACKSIZE    (1024)
/* Send data coefficient */
#define     SD_COE              (1000)
/* ISL28023 compare hardware parameter */
/* DPS channel A - F minimum compare coeficiente */
#define		CHA_F_MIN_CCOE		(0.3882f)
/* DPS channel A - F maximum compare coeficiente */
#define		CHA_F_MAX_CCOE		(0.5544f)
/* DPS channel 1 - 4 minimum compare coeficiente */
#define		CH1_4_MIN_CCOE		(0.2f)
/* DPS channel 1 - 4 maximum compare coeficiente */
#define		CH1_4_MAX_CCOE		(0.22f)
/* Isl28023 channel A - F set coeficiente */
#define     ISL_CHA_F_COE       (1.1)
/* Isl28023 channel 1 - 4 set coeficiente */
#define     ISL_CH1_4_COE       (1.05)
/* Error debounce time */
#define     ERR_DBC_TIME        (10)
/* Power on report time */
#define     PO_REPORT_TIME      (5)


/*
  ** static global variable
  ** Debug for DPS channel A
*/
u8 dpsa_disable = 0x01;


/*
  ** Function Name        : Dps_Init
  ** Function Description : Init fpga control dps system  
  ** Function Parameter   : void
  ** Function Return      : SET_SUCCESS         : Clear success
							SET_FAILED          : Clear failed
  ** Notes				  : None
*/
u8 Dps_Init(u8 Bid)
{
	u16 enable = 0x0;
	
	/* Reset DPS board fpga */
	if (SUCCESS != Dps_Fpga_Reset(Bid)) {
		return INIT_FAILED;
	}
	else {

	}

	/* Reset DPS board ad5694 */
	if (SUCCESS != Reset_AD5694(Bid)) {
		return INIT_FAILED;
	}
	else {

	}

	/* Enable DPS board level 1 */
	if (SET_SUCCESS != Set_All_Dps_L1_Enable(0x3E, Bid)) {
		return INIT_FAILED;
	}	
	// if (SET_SUCCESS != Set_All_Dps_L1_Enable(L1_ALL_ENABLE, Bid)) {
	// 	return INIT_FAILED;
	// }

	return INIT_SUCCESS;
}


/*
  ** Function Name        : Read_Error_Info
  ** Function Description : Read Error information
  ** Function Parameter   : Sample              : Aging sample data pointer
  **						Bid                 : Board id
  ** Function Return      : READ_SUCCESS        : Read success
							READ_FAILED         : Read failed
  ** Notes				  : None
*/
u8 Read_Error_Info(Sample_Data * Sample, u8 Bid)
{
	/* Ready data */
	u8 status;
	u16 err_info = 0x00;
	u16 temp = 0x01;
	u16 count = 0x00;

	/* Read dps control fpga */
	status = Read_Dps_Fpga_Reg(ERROR_NUM_REG, &err_info, Bid);
	if (READ_SUCCESS != status) {
		return READ_FAILED;
	}

	/* Judge the error message */
	for (count = DPSA_CHANNEL_BD0; count < DPS3_CHANNEL_BD0; count++) {
		if (temp & err_info) {
			switch (Bid) {
			    Sample->AgeStatus = ERROR_PROTECT;
				case DPS_BD0: {
					Sample->DpsStat[count] = DPS_VOL_ABNEL;
				}
					break;
				case DPS_BD1: {
					Sample->DpsStat[count + DPS3_CHANNEL_BD0] = DPS_VOL_ABNEL;
				}
					break;
				default:
					break;
			}

		}
		else {

		}

		temp = temp << 1;
	}

	return READ_SUCCESS;
}


/*
  ** Function Name        : Dps_Config
  ** Function Description : Configuration Dps Dpm system
  ** Function Parameter   : dps_cfg             : Dps configuration parameter pointer
  **    				    Sample              : Aging sample data pointer
  **                        protect             : Protect enable 
  **						Bid                 : Board id
  **                        PON_FLAG            : Power on flag
  ** Function Return      : CONFIG_SUCCESS      : Config success
							CONFIG_FAILED       : Config failed
  ** Notes				  : None
*/
u8 Dps_Config(Dps_Arg * dps_cfg, Sample_Data * Sample, u8 protect, u8 Bid, u32 * PON_FLAG)
{
	/* Set aging status power on status */
	Sample->AgeStatus = DPS_PWR_ON;	

	/* Ready data */
	u8 id = 0x00;
	u16 BufData= 0x00;

	/* 1ms cover ticks timer usde */
	const TickType_t tdelay = pdMS_TO_TICKS(TIMER_DELAY);

	/* Disable DPSA Channel */
	u8 start = 0x00;
	if (dpsa_disable) {
		start = DPSB_CHANNEL_BD0;
	}
	else {
		start = DPSA_CHANNEL_BD0;
	}
		
	for (id = start; id <= DPSF_CHANNEL_BD0; id++) {
		if (SUCCESS != Isl8272_Config(id, dps_cfg, protect, Bid)) {
			if (protect) {
				Sample->AgeStatus = ERROR_PROTECT;
				if (DPS_BD0 == Bid) {
					Sample->ErrStatus = DPS1_SET_ERR << id;
				}
				else if (DPS_BD1 == Bid) {
					Sample->ErrStatus = DPS9_SET_ERR << id;
				}
				return CONFIG_FAILED;
			}
			else {

			}
		}
	}

	for (id = start; id <= DPS3_CHANNEL_BD0; id++) {
		/* Ready register data . MSB */
		BufData = dps_cfg->Dsp[id].Open.STime;
		/* Write DPS FPGA register data : CHA_ON_DELAY_REG + Ch */
		if (CONFIG_SUCCESS != Config_Dps_Fpga_Reg(CHA_ON_DELAY_REG + id, BufData, Bid)) {
			if (protect) {
				Sample->AgeStatus = ERROR_PROTECT;
				if (DPS_BD0 == Bid) {
					Sample->ErrStatus = DPS1_SET_ERR << id;
				}
				else if (DPS_BD1 == Bid) {
					Sample->ErrStatus = DPS9_SET_ERR << id;
				}
				return CONFIG_FAILED;				
			}
		}
		else {

		}

		/* Ready register data . MSB */
		BufData = dps_cfg->Dsp[id].Close.STime;
		/* Write DPS FPGA register data : CHA_OFF_DELAY_REG + Ch */
		if (CONFIG_SUCCESS != Config_Dps_Fpga_Reg(CHA_OFF_DELAY_REG + id, BufData, Bid)) {
			if (protect) {
				Sample->AgeStatus = ERROR_PROTECT;
				if (DPS_BD0 == Bid) {
					Sample->ErrStatus = DPS1_SET_ERR << id;
				}
				else if (DPS_BD1 == Bid) {
					Sample->ErrStatus = DPS9_SET_ERR << id;
				}
				return CONFIG_FAILED;
			}
		}
		else {

		}
 
		/* Ready register data . MSB */
		BufData = ERR_DBC_TIME;
		/* Write DPS FPGA register data : CHA_ERROR_DELAY_REG + Ch */
		if (CONFIG_SUCCESS != Config_Dps_Fpga_Reg(CHA_ERROR_DELAY_REG + id, BufData, Bid)) {
			if (protect) {
				Sample->AgeStatus = ERROR_PROTECT;
				if (DPS_BD0 == Bid) {
					Sample->ErrStatus = DPS1_SET_ERR << id;
				}
				else if (DPS_BD1 == Bid) {
					Sample->ErrStatus = DPS9_SET_ERR << id;
				}
				return CONFIG_FAILED;
			}
		}
		else {

		}

	}

    /* Set OVP&UVP voltage */
	if (SUCCESS != Dps_Set_Compare_Voltagr(dps_cfg, Bid)) {
		return CONFIG_FAILED;
	}

	u16 enable = 0x00;
	if (dpsa_disable) {
		enable = 0x003E;
	}
	else {
		enable = L3_ALL_ENABLE;
	}
	if (SET_SUCCESS != Set_All_Dps_L3_Enable(enable, Bid)) {
		if (protect) {
			return CONFIG_FAILED;
		}
		else {

		}
	}	

	/* Enable DPS board0 level 2 */
	if (dpsa_disable) {
		enable = 0x03FE;
	}
	else {
		enable = L2_ALL_ENABLE;
	}
	if (SET_SUCCESS != Set_All_Dps_L2_Enable(enable, Bid)) {
		if (protect) {
			return CONFIG_FAILED;
		}
		else {

		}
	}

	/* Config DPS channel A ~ F */
	// for (id = DPSA_CHANNEL_BD0; id <= DPSF_CHANNEL_BD0; id++) {
	// 	if (SUCCESS != Isl8272_Config(id, dps_cfg, protect, Bid)) {
	// 		if (protect) {
	// 			Sample->AgeStatus = ERROR_PROTECT;
	// 			if (DPS_BD0 == Bid) {
	// 				Sample->ErrStatus = DPS1_SET_ERR << id;
	// 			}
	// 			else if (DPS_BD1 == Bid) {
	// 				Sample->ErrStatus = DPS9_SET_ERR << id;
	// 			}
	// 			return CONFIG_FAILED;
	// 		}
	// 		else {

	// 		}
	// 	}
	// }

	// for (id = DPSA_CHANNEL_BD0; id <= DPS3_CHANNEL_BD0; id++) {
	// 	/* Ready register data . MSB */
	// 	BufData = dps_cfg->Dsp[id].Open.STime;
	// 	/* Write DPS FPGA register data : CHA_ON_DELAY_REG + Ch */
	// 	if (CONFIG_SUCCESS != Config_Dps_Fpga_Reg(CHA_ON_DELAY_REG + id, BufData, Bid)) {
	// 		if (protect) {
	// 			Sample->AgeStatus = ERROR_PROTECT;
	// 			if (DPS_BD0 == Bid) {
	// 				Sample->ErrStatus = DPS1_SET_ERR << id;
	// 			}
	// 			else if (DPS_BD1 == Bid) {
	// 				Sample->ErrStatus = DPS9_SET_ERR << id;
	// 			}
	// 			return CONFIG_FAILED;				
	// 		}
	// 	}
	// 	else {

	// 	}

	// 	/* Ready register data . MSB */
	// 	BufData = dps_cfg->Dsp[id].Close.STime;
	// 	/* Write DPS FPGA register data : CHA_OFF_DELAY_REG + Ch */
	// 	if (CONFIG_SUCCESS != Config_Dps_Fpga_Reg(CHA_OFF_DELAY_REG + id, BufData, Bid)) {
	// 		if (protect) {
	// 			Sample->AgeStatus = ERROR_PROTECT;
	// 			if (DPS_BD0 == Bid) {
	// 				Sample->ErrStatus = DPS1_SET_ERR << id;
	// 			}
	// 			else if (DPS_BD1 == Bid) {
	// 				Sample->ErrStatus = DPS9_SET_ERR << id;
	// 			}
	// 			return CONFIG_FAILED;
	// 		}
	// 	}
	// 	else {

	// 	}
 
	// 	/* Ready register data . MSB */
	// 	BufData = ERR_DBC_TIME;
	// 	/* Write DPS FPGA register data : CHA_ERROR_DELAY_REG + Ch */
	// 	if (CONFIG_SUCCESS != Config_Dps_Fpga_Reg(CHA_ERROR_DELAY_REG + id, BufData, Bid)) {
	// 		if (protect) {
	// 			Sample->AgeStatus = ERROR_PROTECT;
	// 			if (DPS_BD0 == Bid) {
	// 				Sample->ErrStatus = DPS1_SET_ERR << id;
	// 			}
	// 			else if (DPS_BD1 == Bid) {
	// 				Sample->ErrStatus = DPS9_SET_ERR << id;
	// 			}
	// 			return CONFIG_FAILED;
	// 		}
	// 	}
	// 	else {

	// 	}

	// }

	/* Clear last error */

    // /* Set OVP&UVP voltage */
	// if (SUCCESS != Dps_Set_Compare_Voltagr(dps_cfg, Bid)) {
	// 	return CONFIG_FAILED;
	// }

	// /* Enable DPS board0 level 3 */
	// if (SET_SUCCESS != Set_All_Dps_L3_Enable(L3_ALL_ENABLE, Bid)) {
	// 	if (protect) {
	// 		return CONFIG_FAILED;
	// 	}
	// 	else {

	// 	}
	// }

	// /* Enable DPS board0 level 2 */
	// if (SET_SUCCESS != Set_All_Dps_L2_Enable(L2_ALL_ENABLE, Bid)) {
	// 	if (protect) {
	// 		return CONFIG_FAILED;
	// 	}
	// 	else {

	// 	}
	// }

	/* Create software timer */
	/* Timer name */
	switch (Bid) {
		case DPS_BD0: {
			/* Timwe id */
			s32 timer_id = 0x01;
			TimerHandle_t Dps_timer;

			Dps_timer = xTimerCreate("Board 0 ad5694 line power on",\
						/* Timer cycle 1ms */
						tdelay,\
						/* Work mode is cycle mode */
						pdTRUE,\
						/* Timwe id is 0x01 */
						(void *)timer_id,
						/* Timwe handler funcation */
						(TimerCallbackFunction_t)AD5694_Set_Line_Power_On_Task_Bd0);
			/* Check create software timer failed or success */
			configASSERT(Dps_timer);
			if (NULL != Dps_timer) {
				/* Success, start timer */
				xTimerStart(Dps_timer, 0);
			}
			else {
				return CONFIG_FAILED;
			}
		}
			break;
		case DPS_BD1: {
			/* Timwe id */
			s32 timer_id = 0x02;
			TimerHandle_t Dps_timer;

			Dps_timer = xTimerCreate("Board 1 ad5694 line power on",\
						/* Timer cycle 1ms */
						tdelay,\
						/* Work mode is cycle mode */
						pdTRUE,\
						/* Timwe id is 0x01 */
						(void *)timer_id,
						/* Timwe handler funcation */
						(TimerCallbackFunction_t)AD5694_Set_Line_Power_On_Task_Bd1);
			/* Check create software timer failed or success */
			configASSERT(Dps_timer);
			if (NULL != Dps_timer) {
				/* Success, start timer */
				xTimerStart(Dps_timer, 0);
			}
			else {
				return CONFIG_FAILED;
			}		
		}
			break;
		default: {

		}
			break;
	}

	if (SUCCESS != Dps_Enable_Check_Error(dps_cfg, Bid, PON_FLAG)) {
		return CONFIG_FAILED;
	}
	// /* Power off carry out */
	// * PON_FLAG = 0x01;

	return CONFIG_SUCCESS;
}


/*
  ** Function Name        : Dps_Disable
  ** Function Description : Disable Dps Dpm system
  ** Function Parameter   : dps_cfg             : Dps configuration parameter pointer
  **    				    Sample              : Aging sample data pointer
  **						Bid                 : Board id
  **                        POFF_FLAG           : Power off flag
  ** Function Return      : SET_SUCCESS         : Disable success
							SET_FAILED          : Disable failed
  ** Notes				  : None
*/
u8 Dps_Disable(Dps_Arg * dps_cfg, Sample_Data * Sample, u8 Bid, u32 * POFF_FLAG)
{	
	/* 1ms cover ticks timer usde */
	u32 delay = 0x00;
	TickType_t x1ms = pdMS_TO_TICKS(1);
	
	/* Set aging status power off status */
	Sample->AgeStatus = DPS_PWR_OFF;

	if (GET_SUCCESS != Get_Max_Close_Delay(dps_cfg, &delay, Bid)) {
		return SET_FAILED;
	}
	const TickType_t tdelay = pdMS_TO_TICKS(TIMER_DELAY);

	/* Disbale error check */
	if (SUCCESS != Dps_Disable_Check_Error(Bid)) {
		return SET_FAILED;
	}

	/* Disable DPS board channel A ~ F level 2 */	
	if (SET_SUCCESS != Set_All_Dps_L2_Enable(L2_CHA_F_DISABLE, Bid)) {
		/* De_bug No Board 1 */		
		return SET_FAILED;
	}

	/* Create software timer */
	/* Timer name */
	switch (Bid) {
		case DPS_BD0: {
			/* Timwe id */
			s32 timer_id = 0x02;
			TimerHandle_t Dps_timer;

			Dps_timer = xTimerCreate("Board 0 ad5694 line power off",\
						/* Timer cycle 1ms */
						tdelay,\
						/* Work mode is cycle mode */
						pdTRUE,\
						/* Timwe id is 0x01 */
						(void *)timer_id,
						/* Timwe handler funcation */
						(TimerCallbackFunction_t)AD5694_Set_Line_Power_Off_Task_Bd0);
			/* Check create software timer failed or success */
			configASSERT(Dps_timer);
			if (NULL != Dps_timer) {
				/* Success, start timer */
				xTimerStart(Dps_timer, 0);
			}
			else {
				return SET_FAILED;
			}
		}
			break;
		case DPS_BD1: {
			/* Timwe id */
			s32 timer_id = 0x03;
			TimerHandle_t Dps_timer;

			Dps_timer = xTimerCreate("Board 1 ad5694 line power off",\
						/* Timer cycle 1ms */
						tdelay,\
						/* Work mode is cycle mode */
						pdTRUE,\
						/* Timwe id is 0x01 */
						(void *)timer_id,
						/* Timwe handler funcation */
						(TimerCallbackFunction_t)AD5694_Set_Line_Power_Off_Task_Bd1);
			/* Check create software timer failed or success */
			configASSERT(Dps_timer);
			if (NULL != Dps_timer) {
				/* Success, start timer */
				xTimerStart(Dps_timer, 0);
			}
			else {
				return SET_FAILED;
			}		
		}
			break;
		default: {

		}
			break;
	}

	vTaskDelay((x1ms * delay + PO_REPORT_TIME));

	/* Disable DPS board level 3 */
	if (SET_SUCCESS != Set_All_Dps_L2_Enable(ALL_DISABLE, Bid)) {
		return SET_FAILED;
	}

	/* Disable DPS board0level 3 */
	if (SET_SUCCESS != Set_All_Dps_L3_Enable(ALL_DISABLE, Bid)) {
		return SET_FAILED;
	}

	/* Disable DPS board ADC Channel1 and Channel2 */
	if (SUCCESS != Disable_DPS_CH3_CH4(Bid))  {
		return SET_FAILED;
	}

	if (CLEAR_SUCCESS != Dps_Info_Clear(Sample)) {
		return SET_FAILED;
	}

	/* Power off carry out */
	* POFF_FLAG = 0x01;

	return SET_SUCCESS;
}


/*
  ** Function Name        : Dps_Info_Clear
  ** Function Description : Clear dps information 
  ** Function Parameter   : Sample              : Aging sample data
  ** Function Return      : CLEAR_SUCCESS       : Clear success
							CLEAR_FAILED        : Clear failed
  ** Notes				  : None
*/
u8 Dps_Info_Clear(Sample_Data * Sample)
{
	u8 i = 0;

	for ( i = 0; i < 16; i ++) {
		Sample->DpsVO[i] = 0x00;
		Sample->DpsIO[i] = 0x00;
		Sample->ErrStatus = 0x00;
		Sample->TempSamp[i] = 0x00;
	}

	return CLEAR_SUCCESS;
}

/*
  ** Function Name        : Get_Max_Open_Delay
  ** Function Description : Get max open delay time
  ** Function Parameter   : dps_cfg             : DPS config parameter
  **                        Delay               : Power on delay time  
  **						Bid                 : Board id 
  ** Function Return      : GET_SUCCESS         : Disable success
							GET_FAILED          : Disable failed
  ** Notes				  : None
*/
u8 Get_Max_Open_Delay(Dps_Arg * dps_cfg, u32 * Delay, u8 Bid)
{
	u32 delay = 0x00;
	u8 count = 0x00;

	switch (Bid) {
		case DPS_BD0: {
			delay = dps_cfg->Dsp[DPSA_CHANNEL_BD0].Open.Keep;

			for (count = DPSB_CHANNEL_BD0; count < DPS3_CHANNEL_BD0; count++) {
				if (delay < dps_cfg->Dsp[count].Open.Keep) {
					delay = dps_cfg->Dsp[count].Open.Keep;
				}
			}
		}
			break;
		case DPS_BD1: {
			delay = dps_cfg->Dsp[DPS3_CHANNEL_BD0].Open.Keep;

			for (count = DPSB_CHANNEL_BD0; count < DPS3_CHANNEL_BD0; count++) {
				if (delay < dps_cfg->Dsp[count + DPS3_CHANNEL_BD0].Open.Keep) {
					delay = dps_cfg->Dsp[count + DPS3_CHANNEL_BD0].Open.Keep;
				}
			}			
		}
			break;
		default: {
			
		}
			break;
	}

	* Delay = delay;

	return GET_SUCCESS;
}


/*
  ** Function Name        : Get_Max_Close_Delay
  ** Function Description : Get max close delay time
  ** Function Parameter   : dps_cfg             : DPS config parameter
  **                        Delay               : Power off delay time  
  **						Bid                 : Board id
  ** Function Return      : GET_SUCCESS         : Disable success
							GET_FAILED          : Disable failed
  ** Notes				  : None
*/
u8 Get_Max_Close_Delay(Dps_Arg * dps_cfg, u32 * Delay, u8 Bid)
{
	u32 delay = 0x00;
	u8 count = 0x00;

	switch (Bid) {
		case DPS_BD0: {
			delay = dps_cfg->Dsp[DPSA_CHANNEL_BD0].Close.Keep;

			for (count = DPSB_CHANNEL_BD0; count < DPS3_CHANNEL_BD0; count++) {
				if (delay < dps_cfg->Dsp[count].Close.Keep) {
					delay = dps_cfg->Dsp[count].Close.Keep;
				}
			}
		}
			break;
		case DPS_BD1: {
			delay = dps_cfg->Dsp[DPS3_CHANNEL_BD0].Close.Keep;

			for (count = DPSB_CHANNEL_BD0; count < DPS3_CHANNEL_BD0; count++) {
				if (delay < dps_cfg->Dsp[count + DPS3_CHANNEL_BD0].Close.Keep) {
					delay = dps_cfg->Dsp[count + DPS3_CHANNEL_BD0].Close.Keep;
				}
			}
		}
			break;
		default: {

		}
			break;
	}

	* Delay = delay;

	return GET_SUCCESS;
}


/*
  ** Function Name        : Dps_Enable_Check_Error
  ** Function Description : Start dps channel error cheek
  ** Function Parameter   : dps_cfg             : DPS config parameter
  **						Bid                 : Board id
  **                        PON_FLAG            : Power on flag  
  ** Function Return      : SET_SUCCESS         : Disable success
							SET_FAILED          : Disable failed
  ** Notes				  : None
*/ 
u8 Dps_Enable_Check_Error(Dps_Arg * dps_cfg, u8 Bid, u32 * PON_FLAG) 
{ 
	/* Ready data */ 
	u8 status; 
	TickType_t x1ms = pdMS_TO_TICKS(1);
	u32 delay_time = 0x00;

	/* Wait power on time */
	status = Get_Max_Open_Delay(dps_cfg, &delay_time, Bid);
	if (GET_SUCCESS != status) {
		return FAILURE;
	}
	vTaskDelay(x1ms * (delay_time + PO_REPORT_TIME));

	/* Start error check */
	u16 enable = 0x00;	
	if (dpsa_disable) {
		enable = 0xFE;
	}
	else {
		enable = ALL_CHECK_ERR;
	}
	status  = Set_All_Dps_Check_Error(enable, Bid);
	if (SET_SUCCESS != status) {
		return FAILURE;
	}
	// status  = Set_All_Dps_Check_Error(ALL_CHECK_ERR, Bid);
	// if (SET_SUCCESS != status) {
	// 	return FAILURE;
	// }
	
	switch (Bid) {
		case DPS_BD0: {
			/* Debug */
			/* Power off carry out */
			* PON_FLAG = 0x01;
		}
			break;
		case DPS_BD1: {
			/* Power off carry out */
			* PON_FLAG = 0x01;			
		}
			break;
		default: {

		}
			break;
	}

	return SUCCESS;
}


/*
  ** Function Name        : Dps_Enable_Check_Error
  ** Function Description : Start dps channel error cheek
  ** Function Parameter   : dps_cfg             : DPS config parameter
  **						Bid                 : Board id
  **                        PON_FLAG            : Power on flag  
  ** Function Return      : SET_SUCCESS         : Disable success
							SET_FAILED          : Disable failed
  ** Notes				  : None
*/ 
u8 Dps_Set_Compare_Voltagr(Dps_Arg * dps_cfg, u8 Bid) 
{ 
	/* Ready data */ 
	u8 status; 
	u8 ch = 0x00;

	/* Set isl28023 compare voltage */
	float voltage;

	/* Set OVP and UVP */
	for (ch = DPSA_CHANNEL_BD0; ch < DPS3_CHANNEL_BD0; ch++) {
		voltage = 0.0;

		switch (ch) {
			case DPSA_CHANNEL_BD0 ... DPSB_CHANNEL_BD0: {
				voltage = (float)dps_cfg->Dsp[ch].SetValue / SD_COE * CHA_F_MIN_CCOE * ISL_CHA_F_COE;
			}
				break;
			case DPSC_CHANNEL_BD0 ... DPSD_CHANNEL_BD0: {
				/* Hardware channel cover user channel */
				u8 temp_ch = 0;
				if (ch == DPSC_CHANNEL_BD0) {
					temp_ch = DPS1_CHANNEL_BD0;
				}
				else if (ch == DPSD_CHANNEL_BD0) {
					temp_ch = DPS2_CHANNEL_BD0;
				}
				else {

				}

				voltage = (float)dps_cfg->Dsp[temp_ch].SetValue / SD_COE * CH1_4_MIN_CCOE * ISL_CH1_4_COE;
			}
				break;
			case DPSE_CHANNEL_BD0 ... DPS2_CHANNEL_BD0: {
				/* Hardware channel cover user channel */
				u8 temp_ch = 0;
	            temp_ch = ch - DPSC_CHANNEL_BD0;

				voltage = (float)dps_cfg->Dsp[temp_ch].SetValue / SD_COE * CHA_F_MIN_CCOE * ISL_CHA_F_COE;
			}
				break;			
			default: {
				return FAILURE;
			}
				break;
		}
		status = Set_dac_out(voltage, dps_info.DBMsg[Bid].Dpm[ch].DpmAddr , \
				dps_info.DBMsg[Bid].Chan);
		if (SUCCESS != status) {
			return FAILURE;
		}
	}

	return SUCCESS;
}


/*
  ** Function Name        : Dps_Disable_Check_Error
  ** Function Description : Stop dps channel error cheek
  ** Function Parameter   : Bid                 : Board id
  ** Function Return      : SET_SUCCESS         : Disable success
							SET_FAILED          : Disable failed
  ** Notes				  : None
*/ 
u8 Dps_Disable_Check_Error(u8 Bid) 
{ 
	/* Ready data */ 
	u8 status; 
	u8 ch = 0x00;

	/* Set isl28023 compare voltage */
	float voltage = 0.0f;

	/* Disable error check */
	status  = Set_All_Dps_Check_Error(ALL_DISABLE, Bid);
	if (SET_SUCCESS != status) {
		return FAILURE;
	}

	/* Disable OVP and UVP */
	for (ch = DPSA_CHANNEL_BD0; ch < DPS3_CHANNEL_BD0; ch++) {
		status = Set_dac_out(voltage, dps_info.DBMsg[Bid].Dpm[ch].DpmAddr , \
				dps_info.DBMsg[Bid].Chan);
		if (SUCCESS != status) {
			return FAILURE;
		}
	}

	return SUCCESS;
}


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
