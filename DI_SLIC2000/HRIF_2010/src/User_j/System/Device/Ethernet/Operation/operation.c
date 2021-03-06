/*
  ** @file           : operation.c
  ** @brief          : Tcp Data Operation Source File
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
#include "return_val.h"
#include "debug_config.h"
#include "app_layer_protocol.h"
#include "tcp_server.h"
#include "ack.h"
#include "verify.h"
#include "GlobalRef.h"
#include "system_emio.h"
#include "main.h"
#include "system_state.h"
#include "memory_map.h"
#include "operation.h"
#include "fpga.h"
#include "unistd.h"
#include "Gpio.h"
#include "xil_cache.h"
#include "Driver.h"
#include "shell_port.h"
#include "mt25ql512.h"






volatile Cfg_Mask cfgmask = 0;

/*
  ** Define
*/

/* Check Crc */
#define     CHECK_CRC           (0x0)
/* AXI LITE */
#define     AXI_LITE            (0x0)
/* First Frame */
#define     FIRST_FRAME         (0x1)
/* Crc Initial Value */
#define     CRC_INIT_VAL        (0xFFFFFFFF)
/* Crc Size */
#define     CRC_VAL_SIZE        (0x4)
/* Management Firmware Other Overhead Size */
#define     MB_FR_OVER_SIZE     (0x4)
/* Flash Start Address */
#define     FLASH_START_ADDR    (0x000000)
/* Retransmission Firmware Ack */
#define     RET_FIM_ACK         (0x01)
/* Update Successed Ack */
#define     UPT_SUCS_ACK        (0x00)
/* u32 Type Bits Number */
#define     U32_BITS_NUM        (0x20)


/*
  ** Static Funtion
*/
static u8 Beep_Control(void * Data);
static u8 Time_Synchronous(void * Data);
static u8 DPS_Config(void * Data);
static u8 Pattern_Handle(void * Data);
static u8 Control_Command(void * Data);
static u8 Timer_Restart(void * Data);
static u8 Pin_Config(void * Data);
static u8 S_Sample_Data(void * Data);
static u8 DPS_Channel_Status(void * Data);
static u8 Realtime_Status(void * Data);
static u8 Pattern_Error_Information(void * Data);
static u8 Set_Pattern_Status(void * Data);
static u8 Driver_Board_Version(void * Data);
static u8 Pattern_Status_Debug(void * Data);
static u8 Pattern_Status(void * Data);
static u8 Pin_Config_Arg(void * Data);
static u8 Max9972_Debug(void * Data);
static u8 Read_PS_DDR(void * Data);
static u8 Management_Board_Firmware(void * Data);
static u8 Driver_Board_Firmware(void * Data);
static u8 DDR_Test(void * Data);
static u8 DDR_Test_Status(void * Data);
static u8 Pin_Mask(void * Data);
static u8 Cycle_Info(void * Data);
static u8 Config_File(void * Data);


/*
  ** Static Global Variable
*/
static u8 Dps_Cfg_Flag  = 0x00;
static u8 FPGA_Cfg_Flag = 0x00;
static u32 CycleSize    = 0x00;

/*
  ** Function Name        : Set_Dps_Cfg_Flag
  ** Function Description : Set Dps Config Flag
  ** Function Parameter   : Void
  ** Function Return      : SET_SUCCESS         : Always
  ** Notes				  : None
*/
u8 Set_Dps_Cfg_Flag(void)
{
	Dps_Cfg_Flag = 0x01;
	return SET_SUCCESS;
}


/*
  ** Function Name        : Get_Dps_Cfg_Flag
  ** Function Description : Get Dps Config Flag
  ** Function Parameter   : Void
  ** Function Return      : u8			 		: Dps_Cfg_Flag Value
  ** Notes				  : None
*/
u8 Get_Dps_Cfg_Flag(void)
{
	return Dps_Cfg_Flag;
}


/*
  ** Function Name        : Clear_Dps_Cfg_Flag
  ** Function Description : Clear Dps Config Flag
  ** Function Parameter   : Void
  ** Function Return      : CLEAR_SUCCESS       : Always
  ** Notes				  : None
*/
u8 Clear_Dps_Cfg_Flag(void)
{
	Dps_Cfg_Flag = 0x00;
	return CLEAR_SUCCESS;
}


/*
  ** Function Name        : Set_FPGA_Cfg1_Flag
  ** Function Description : Set FPGA Config1 Flag
  ** Function Parameter   : Void
  ** Function Return      : SET_SUCCESS         : Always
  ** Notes				  : None
*/
u8 Set_FPGA_Cfg_Flag(void)
{
	FPGA_Cfg_Flag = 0x01;
	return SET_SUCCESS;
}


/*
  ** Function Name        : Get_FPGA_Cfg1_Flag
  ** Function Description : Get FPGA Config1 Flag
  ** Function Parameter   : Void
  ** Function Return      : u8			 		: FPGA_Cfg1_Flag Value
  ** Notes				  : None
*/
u8 Get_FPGA_Cfg_Flag(void)
{
	return FPGA_Cfg_Flag;
}


/*
  ** Function Name        : Clear_FPGA_Cfg1_Flag
  ** Function Description : Clear FPGA Config1 Flag
  ** Function Parameter   : Void
  ** Function Return      : CLEAR_SUCCESS       : Always
  ** Notes				  : None
*/
u8 Clear_FPGA_Cfg_Flag(void)
{
	FPGA_Cfg_Flag = 0x00;
	return CLEAR_SUCCESS;
}


/*
  ** Function Name        : Data_Operation
  ** Function Description : Operation Tcp Server Receive Data
  ** Function Parameter   : Data                : Point Receive Data Pointer
  ** Function Return      : OPER_FAILED 		: Operation Failed
  **                        OPER_SUCCESS	    : Operation Success
  ** Notes				  : None
*/
u8 Data_Operation(void * Data)
{
	/* Check Parameter (Data) */
	if (NULL == Data) {
		debug("The Data Point Is NULL In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	static Frame * temp_data = NULL;
	static u8 status = 0;
	temp_data = (Frame *)Data;
	switch (temp_data->com) {
	    /* Beep Control Order */
		case R_BEEP_CONTROL: {
			status = Beep_Control(Data);
			if (OPER_SUCCESS != status) {
				debug("Beep Control Order Error In %s Files,%d Line\n",__FILE__,__LINE__);
			}
		}
		break;
		/* Time Synchronous Order */
		case R_TIME_SYNC: {
			status = Time_Synchronous(Data);
			if (OPER_SUCCESS != status) {
				debug("Time Synchronous Order Error In %s Files,%d Line\n",__FILE__,__LINE__);
			}
		}
		break;
		/* DPS Channel Config Order */
		case R_DPS_CONFIG: {
			status = DPS_Config(Data);
			if (OPER_SUCCESS != status) {
				debug("DPS Channel Config Order Error In %s Files,%d Line\n",__FILE__,__LINE__);
			}
		}
		break;
		/* Pattern File Order */
		case R_PATTERN: {
			status = Pattern_Handle(Data);
			if (OPER_SUCCESS != status) {
				debug("Pattern File Order Error In %s Files,%d Line\n",__FILE__,__LINE__);
			}
		}
		break;
		/* Control Command Order */
		case R_CONTROL_COM: {
			status = Control_Command(Data);
			if (OPER_SUCCESS != status) {
				debug("Control Order Error In %s Files,%d Line\n",__FILE__,__LINE__);
			}
		}
		break;
		/* Time Restart Order */
		case R_TIME_RESTART: {
			status = Timer_Restart(Data);
			if (OPER_SUCCESS != status) {
				debug("Time Restart Order Error In %s Files,%d Line\n",__FILE__,__LINE__);
			}
		}
		break;
		/* Pin Config Order */
		case R_PIN_CONFIG: {
			status = Pin_Config(Data);
			if (OPER_SUCCESS != status) {
				debug("Pin Config Order Error In %s Files,%d Line\n",__FILE__,__LINE__);
			}
		}
		break;
		/* Not Used Order */
		case R_ALARM: {

		}
		break;
		/* Sample Data Order */
		case S_SAMPLE_DATA: {
			status = S_Sample_Data(Data);
			if (OPER_SUCCESS != status) {
				debug("Sample Data Order Error In %s Files,%d Line\n",__FILE__,__LINE__);
			}
		}
		break;
		/* Not Used Order */
		case S_MAX9979_DATA: {

		}
		break;
		/* DPS Channel Status */
		case R_DPS_CH_STATUS: {
			status = DPS_Channel_Status(Data);
			if (OPER_SUCCESS != status) {
				debug("Pin Config Order Error In %s Files,%d Line\n",__FILE__,__LINE__);
			}
		}
		break;
		/* Not Used Order */
		case R_TEMP_SAMPLE_DATA: {

		}
		break;
	    /* Realtime Status */
		case S_REALTIME_STATUS: {
			status = Realtime_Status(Data);
			if (OPER_SUCCESS != status) {
				debug("Realtime Status Order Error In %s Files,%d Line\n",__FILE__,__LINE__);
			}
		}
		break;
		/* Pattern Error Informaion */
		case S_PATTERN_ERR_INFO: {
			status = Pattern_Error_Information(Data);
			if (OPER_SUCCESS != status) {
				debug("Pattern Error Information Order Error In %s Files,%d Line\n",__FILE__,__LINE__);
			}
		}
		break;
		/* Set Pattern Status */
		case R_SET_PATTERN_STS: {
			status = Set_Pattern_Status(Data);
			if (OPER_SUCCESS != status) {
				debug("Pattern Error Information Order Error In %s Files,%d Line\n",__FILE__,__LINE__);
			}
		}
		break;
		/* Driver Board Version */
		case S_DB_VERSION: {
			status = Driver_Board_Version(Data);
			if (OPER_SUCCESS != status) {
				debug("Pattern Error Information Order Error In %s Files,%d Line\n",__FILE__,__LINE__);
			}
		}
		break;
		/* Not Used Order */
		case S_DB_PATTERN_STS: {

		}
		break;
		/* Pattern Status (Debug) */
		case R_PATTERN_DSTS: {
			status = Pattern_Status_Debug(Data);
			if (OPER_SUCCESS != status) {
				debug("Pattern Status Debug Order Error In %s Files,%d Line\n",__FILE__,__LINE__);
			}
		}
		break;
		/* Pattern Status */
		case S_PATTERN_STS: {
			status = Pattern_Status(Data);
			if (OPER_SUCCESS != status) {
					debug("Pattern Status Order Error In %s Files,%d Line\n",__FILE__,__LINE__);
			}
		}
		break;
		/* Pin Config (Debug) */
		case R_PIN_DCONFIG: {
			status = Pin_Config_Arg(Data);
			if (OPER_SUCCESS != status) {
					debug("Pin Config Debug Order Error In %s Files,%d Line\n",__FILE__,__LINE__);
			}
		}
		break;
		case R_PIN_MASK: {
			status = Pin_Mask(Data);
			if (OPER_SUCCESS != status) {
					debug("Pin Mask Order Error In %s Files,%d Line\n",__FILE__,__LINE__);
			}
		}
		break;
		case R_CYCLE_INFO: {
			status = Cycle_Info(Data);
			if (OPER_SUCCESS != status) {
					debug("Pin Mask Order Error In %s Files,%d Line\n",__FILE__,__LINE__);
			}
		}
		break;
		/* Max9972 Debug */
		case R_9972_DEBUG: {
			status = Max9972_Debug(Data);
			if (OPER_SUCCESS != status) {
					debug("Pin Config Debug Order Error In %s Files,%d Line\n",__FILE__,__LINE__);
			}
		}
		break;
		/* Read ZYNQ (Management Board) PS DDR Data */
		case R_PS_DDR: {
			status = Read_PS_DDR(Data);
			if (OPER_SUCCESS != status) {
					debug("Management Board Firmware Order Error In %s Files,%d Line\n",__FILE__,__LINE__);
			}
		}
		break;
		/* Management Board Firmware */
		case R_MB_FIRMWARE: {
			status = Management_Board_Firmware(Data);
			if (OPER_SUCCESS != status) {
					debug("Management Board Firmware Order Error In %s Files,%d Line\n",__FILE__,__LINE__);
			}
		}
		break;
		/* Driver Board Firmware */
		case R_DB_FIRMWARE: {
			status = Driver_Board_Firmware(Data);
			if (OPER_SUCCESS != status) {
					debug("Driver Board Firmware Order Error In %s Files,%d Line\n",__FILE__,__LINE__);
			}
		}
		break;
		/* Not Used Order */
		case S_VERSION: {

		}
		break;
		/* DDR */
		case S_DDR_TEST: {
			status = DDR_Test(Data);
			if (OPER_SUCCESS != status) {
					debug("DDR Test Order Error In %s Files,%d Line\n",__FILE__,__LINE__);
			}
		}
		break;
		case S_DDR_TEST_STS: {
			status = DDR_Test_Status(Data);
			if (OPER_SUCCESS != status) {
					debug("DDR Test Status Order Error In %s Files,%d Line\n",__FILE__,__LINE__);
			}
		}
		break;
		/* Not Used Order */
		case CONFIG_FILE: {
			status = Config_File(Data);
			if (OPER_SUCCESS != status) {
					debug("Config Files Order Error In %s Files,%d Line\n",__FILE__,__LINE__);
			}
		}
		break;
		default : {
			status = Tcp_Nack();
			if (SEND_SUCCESS != status) {
				debug("Tcp Server Send Nack Failed In %s Files,%d Line\n",__FILE__,__LINE__);
				status = OPER_FAILED;
			}
			else {
				status = OPER_SUCCESS;
			}
		}
		break;
	}

	return status;
}


/*
  ** Function Name        : Beep_Control
  ** Function Description : Beep Control Operation
  ** Function Parameter   : Data                : Point Receive Data Pointer
  ** Function Return      : OPER_FAILED 		: Operation Failed
  **                        OPER_SUCCESS	    : Operation Success
  ** Notes				  : Static
*/
static u8 Beep_Control(void * Data)
{
	/* Check Parameter (Data) */
	if (NULL == Data) {
		return OPER_FAILED;
	}

	/* Frame End */
	Frame * temp = (Frame *)Data;
	Beep_Control_t * point = (Beep_Control_t *)temp->data;
	XGpioPs * emio = NULL;
	emio = Get_Emio_Point();

	/* Beep Disable */
	if ((BEEP_DISABLE == point->enable)) {
		/* Nothing To Do */
	}
	/* Beep Enable */
	else if (BEEP_ENABLE == point->enable) {
		/* Beep Off */
		if (BEEP_OFF == point->state) {
			Set_System_Beep(emio,OFF);
		}
		/* Beep On */
		else if (BEEP_0N == point->state) {
			Set_System_Beep(emio,ON);
		}
		/* Others . Error */
		else {
			debug("Beep Control Order Error In %s Files,%d Line\n",__FILE__,__LINE__);
			/* Tcp Server Send Nack */
			if (SEND_SUCCESS != Tcp_Nack()) {
				debug("Send Nack Failed In Beep Control In %s Files,%d Line\n",__FILE__,__LINE__);
				return OPER_FAILED;
			}
			return OPER_FAILED;
		}
	}
	/* Others . Error */
	else {
		debug("Beep Control Order Error In %s Files,%d Line\n",__FILE__,__LINE__);
		/* Tcp Server Send Nack */
		if (SEND_SUCCESS != Tcp_Nack()) {
			debug("Send Nack Failed In Beep Control In %s Files,%d Line\n",__FILE__,__LINE__);
			return OPER_FAILED;
		}
		return OPER_FAILED;
	}

	/* Tcp Server Send Ack */
	if (SEND_SUCCESS != Tcp_Ack()) {
		debug("Send Ack Failed In Beep Control In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	return OPER_SUCCESS;
}


/*
  ** Function Name        : Time_Synchronous
  ** Function Description : ZYNQ Synchronous Time
  ** Function Parameter   : Data                : Point Receive Data Pointer
  ** Function Return      : OPER_FAILED 		: Operation Failed
  **                        OPER_SUCCESS	    : Operation Success
  ** Notes				  : Static
*/
static u8 Time_Synchronous(void * Data)
{
	/* Check Parameter (Data) */
	if (NULL == Data) {
		return OPER_FAILED;
	}

	/* Synchronous Time Process */
	//Frame * temp = (Frame *)Data;
	//Time_Sync_t * point = (Time_Sync_t *)temp->data;




	/* Tcp Server Send Ack */
	if (SEND_SUCCESS != Tcp_Ack()) {
		debug("Send Ack Failed In Time Synchronous In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	return OPER_SUCCESS;
}


/*
  ** Function Name        : DPS_Config
  ** Function Description : DPS Channel Config Operation
  ** Function Parameter   : Data                : Point Receive Data Pointer
  ** Function Return      : OPER_FAILED 		: Operation Failed
  **                        OPER_SUCCESS	    : Operation Success
  ** Notes				  : Static
*/
static u8 DPS_Config(void * Data)
{
	/* Check Parameter (Data) */
	if (NULL == Data) {
		return OPER_FAILED;
	}

	/* DPS Channel Config */
	Frame * temp = (Frame *)Data;
	/* Copy DPS Config Data */
	u8 * point = (u8 *)temp->data;
	u8 * desaddr = (void *)DPS_CFG_DATA_ADDR;

	u32 count = 0x00;
	u32 size = sizeof(Dps_Arg);

	for (count = 0x00; count < size; count++) {
		desaddr[count] = point[count];
	}

	Set_Cycle_Size(0);		//list????
	/* Set DPS Config Flag */
	Set_Dps_Cfg_Flag();//DPS????OK

	/* Tcp Server Send Ack */
	if (SEND_SUCCESS != Tcp_Ack()) {
		debug("Send Ack Failed In DPS Config In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	return OPER_SUCCESS;
}

/*
  ** Function Name        : Pattern_Handle
  ** Function Description : Pattern File Operation
  ** Function Parameter   : Data                : Point Receive Data Pointer
  ** Function Return      : OPER_FAILED 		: Operation Failed
  **                        OPER_SUCCESS	    : Operation Success
  ** Notes				  : Static
*/
static u8 Pattern_Handle(void * Data)
{
	/* Check Parameter (Data) */
	if (NULL == Data) {
		return OPER_FAILED;
	}

	s32 status = FAILURE;
	Frame * temp = (Frame *)Data;
	Vector_Arg * temp_1 = (Vector_Arg *)temp->data;

	u32 Length = (temp->length) - HEAD_SIZE - COM_SIZE - LENGTH_SIZE - 8;


	//????HP????
	if(0 == HP_STATUS)
	{
		HP_STATUS = 0x1;
		status = MV_Vector(temp_1, Length);
		if(status == FAILURE)			//??????
		{
			HP_STATUS = 0x0;
			if (SEND_SUCCESS != Tcp_Nack()) {
				debug("Send Nack Failed In %s Files,%d Line\r\n",__FILE__,__LINE__);
				return OPER_FAILED;
			}
		}
	}
	else
	{ //PL??
		if (SEND_SUCCESS != Tcp_Nack())
		{
			debug("Send Nack Failed In %s Files,%d Line\r\n",__FILE__,__LINE__);
			return OPER_FAILED;
		}
	}

	return OPER_SUCCESS;
}


/*
  ** Function Name        : Control_Command
  ** Function Description : Control Command Operation
  ** Function Parameter   : Data                : Point Receive Data Pointer
  ** Function Return      : OPER_FAILED 		: Operation
  ** Failed
  **                        OPER_SUCCESS	    : Operation Success
  ** Notes				  : Static
*/
static u8 Control_Command(void * Data)
{
	/* Check Parameter (Data) */
	if (NULL == Data) {
		return OPER_FAILED;
	}

	/* Get Control Command */
	Frame * temp = (Frame *)Data;
	Aging_Ctl * point     = (Aging_Ctl *)temp->data;
	u8 * point1 = (u8 *)temp->data;
	u8 * desdaddr = (void *)AGING_CTL_ADDR;

	u32 count = 0x00;
	u32 size = sizeof(Aging_Ctl);

	for (count = 0x00; count < size; count++) {
		desdaddr[count] = point1[count];
	}

	/* Set Systrm State */
	switch (point->AgeStatus) {
		case AGING_DO:
			if(FIRST_SP == Get_FirstFlag())	//??????????????
				Set_Startflag(RESTART);
			System_Set_StartEven(M_START);
			break;
		case AGING_PAUSE:
			System_Set_StopEven(M_PAUSE);
			break;
		case AGING_CTE:
			System_Set_StartEven(RESUME);
			break;
		case AGING_CPL:
			System_Set_StopEven(AGE_OK);	//??????????????????
			break;
		case AGING_STOP:
			System_Set_StopEven(M_STOP);
			break;
		default:

			break;
	}

	/* Tcp Server Send Ack */
	if (SEND_SUCCESS != Tcp_Ack()) {
		debug("Send Ack Failed In Control Command In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}
	/* Tcp Server Send Data */
	/* Control State */
	u8 con_sts = 0x00;
	if (SEND_SUCCESS != Tcp_Server_Send((const char *)&con_sts,sizeof(con_sts))) {
		debug("Send Control Command Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	return OPER_SUCCESS;
}


/*
  ** Function Name        : Timer_Restart
  ** Function Description : Patern Status Operation
  ** Function Parameter   : Void
  ** Function Return      : OPER_FAILED 		: Operation Failed
  **                        OPER_SUCCESS	    : Operation Success
  ** Notes				  : Static
*/
static u8 Timer_Restart(void * Data)
{
	/* Check Parameter (Data) */
	if (NULL == Data) {
		return OPER_FAILED;
	}

	/* Timer Restart */
	Frame * temp = (Frame *)Data;
	Retime_Enable_t * point = (Retime_Enable_t *)temp->data;
	Sample_Data * samp_data = (Sample_Data *)SAMPLE_DATA_ADD;
	/* Clear Aging Timer */
	if	(point->clr_timer) {
		samp_data->AgeTime = 0x00;
	}
	/* Nothing To Do */
	else {

	}

	/* Tcp Server Send Ack */
	if (SEND_SUCCESS != Tcp_Ack()) {
		debug("Send Ack Failed In Timer Restart In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	return OPER_SUCCESS;
}


/*
  ** Function Name        : Pin_Config
  ** Function Description : Pin Config Operation
  ** Function Parameter   : Data                : Point Receive Data Pointer
  ** Function Return      : OPER_FAILED 		: Operation Failed
  **                        OPER_SUCCESS	    : Operation Success
  ** Notes				  : Static
*/
static u8 Pin_Config(void * Data)
{
	/* Check Parameter (Data) */
	if (NULL == Data) {
		return OPER_FAILED;
	}

	/* Pin Config */
	Frame * temp = (Frame *)Data;
	/* Copy FPGA Config1 Data */
	u8 * point = (u8 *)temp->data;

	u8 * desdaddr = (void *)FPGA_CFG1_DATA_ADDR;

	u32 count = 0x00;
	u32 size = sizeof(Dri_Arg);

	for (count = 0x00; count < size; count++) {
		desdaddr[count] = point[count];
	}

	SETCFGBIT(0);

	/* Tcp Server Send Ack */
	if (SEND_SUCCESS != Tcp_Ack()) {
		debug("Send Ack Failed In Pin Config In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	return OPER_SUCCESS;
}


/*
  ** Function Name        : Sample_Data
  ** Function Description : Sample Data Operation
  ** Function Parameter   : Void
  ** Function Return      : OPER_FAILED 		: Operation Failed
  **                        OPER_SUCCESS	    : Operation Success
  ** Notes				  : Static
*/
static u8 S_Sample_Data(void * Data)
{
	/* Check Parameter (Data) */
	if (NULL == Data) {
		return OPER_FAILED;
	}

	/* Pin Config */
	/* Tcp Server Send Sample Data */
	Sample_Data * samp_data = (Sample_Data *)SAMPLE_DATA_ADD;


	/* Tcp Server Send Ack */
	if (SEND_SUCCESS != Tcp_Ack()) {
		debug("Send Ack Failed In Realtime Status In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}
	/* Tcp Server Send Data*/
	if (SEND_SUCCESS != Tcp_Server_Send((const char *)samp_data,sizeof(Sample_Data))) {
		debug("Send Realtime Status Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	return OPER_SUCCESS;
}


/*
  ** Function Name        : DPS_Channel_Status
  ** Function Description : Control DPS Channel Status Operation
  ** Function Parameter   : Data                : Point Receive Data Pointer
  **                        Length				: Receive Data Length
  ** Function Return      : OPER_FAILED 		: Operation Failed
  **                        OPER_SUCCESS	    : Operation Success
  ** Notes				  : Static
*/
static u8 DPS_Channel_Status(void * Data)
{
	/* Check Parameter (Data) */
	if (NULL == Data) {
		return OPER_FAILED;
	}

	/* DPS Channel Status */
//	Frame * temp = (Frame *)Data;
//	DPS_Channel_Sts_t * point = (DPS_Channel_Sts_t *)temp->data;


	/* Tcp Server Send Ack */
	if (SEND_SUCCESS != Tcp_Ack()) {
		debug("Send Ack Failed In DPS Channel Status In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	return OPER_SUCCESS;
}


/*
  ** Function Name        : Realtime_Status
  ** Function Description : System Realtime Status Operation
  ** Function Parameter   : Void
  ** Function Return      : OPER_FAILED 		: Operation Failed
  **                        OPER_SUCCESS	    : Operation Success
  ** Notes				  : Static
*/
static u8 Realtime_Status(void * Data)
{
	/* Check Parameter (Data) */
	if (NULL == Data) {
		return OPER_FAILED;
	}

	/* Tcp Server Send Pattern Status */
	Vec_Err_Count * point = (Vec_Err_Count *)PAT_ERR_INFO_ADDR;

	/* Tcp Server Send Ack */
	if (SEND_SUCCESS != Tcp_Ack()) {
		debug("Send Ack Failed In Realtime Status In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	/* Tcp Server Send Data */
	if (SEND_SUCCESS != Tcp_Server_Send((const char *)point,sizeof(Vec_Err_Count))) {
		debug("Send Realtime Status Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	return OPER_SUCCESS;
}


/*
  ** Function Name        : Pattern_Error_Information
  ** Function Description : Pattern Error Information Operation
  ** Function Parameter   : Void
  ** Function Return      : OPER_FAILED 		: Operation Failed
  **                        OPER_SUCCESS	    : Operation Success
  ** Notes				  : Static
*/
static u8 Pattern_Error_Information(void * Data)
{
	/* Check Parameter (Data) */
	if (NULL == Data) {
		return OPER_FAILED;
	}

	Frame * temp = (Frame *)Data;
	Error_cmd * cmdptr = (Error_cmd *)temp->data;

	BackIsp_Info * point = (BackIsp_Info *)PAT_C_INFO_ADDR;

	u8 *errinfo = (u8 *)(point + cmdptr->Sline);


	/* Tcp Server Send Ack */
	if (SEND_SUCCESS != Tcp_Ack()) {
		debug("Send Ack Failed In Pattern Error Information In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	/* Tcp Server Send Data */
	if (SEND_SUCCESS != Tcp_Server_Send((const char *)errinfo, sizeof(struct _isp_) * (cmdptr->Number))) {
		debug("Send Realtime Status Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	/* Tcp Server Send Pattern Status */
	return OPER_SUCCESS;
}

/*
  ** Function Name        : Set_Pattern_Status
  ** Function Description : Set Pattern Status Operation
  ** Function Parameter   : Data                : Point Receive Data Pointer
  **                        Length				: Receive Data Length
  ** Function Return      : OPER_FAILED 		: Operation Failed
  **                        OPER_SUCCESS	    : Operation Success
  ** Notes				  : Static
*/
static u8 Set_Pattern_Status(void * Data)
{
	/* Check Parameter (Data) */
	if (NULL == Data) {
		return OPER_FAILED;
	}

	/* Set_Pattern_Status */

	Frame * temp = (Frame *)Data;
	Pat_Set_Sts_t * setptr = (Pat_Set_Sts_t *)temp->data;
	VecRun_t      * pat_r  = (VecRun_t      *)PAT_R_STS_ADDR;
	u8 status = 0x0;

	if(0x8 == setptr->Run_state)	//????????
	{
		shellprintln("    Start aging ...");
		SETCFGBIT(8);			    //????fpga????
		pat_r->RunSta = RUNNING;
		status = 0x0;

	}
	else if(0x0 == setptr->Run_state)   //????
	{
		status = RebootFpgaMask(0x1f);
		status = ~status;
	}

	/* Tcp Server Send Ack */
	if (SEND_SUCCESS != Tcp_Ack()) {
		debug("Send Ack Failed In Set Pattern Status In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	if (SEND_SUCCESS != Tcp_Server_Send((const char *)&status,1)) {
		debug("Send Set Pattern Status FailedI In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	return OPER_SUCCESS;
}


/*
  ** Function Name        : Deiver_Board_Version
  ** Function Description : Driver Operation
  ** Function Parameter   : Void
  ** Function Return      : OPER_FAILED 		: Operation Failed
  **                        OPER_SUCCESS	    : Operation Success
  ** Notes				  : Static
*/
static u8 Driver_Board_Version(void * Data)
{
	/* Check Parameter (Data) */
	if (NULL == Data) {
		return OPER_FAILED;
	}

	/* Tcp Server Send Driver Board Version */


	/* Tcp Server Send Ack */
	if (SEND_SUCCESS != Tcp_Ack()) {
		debug("Send Ack Failed In Driver Board Version In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	return OPER_SUCCESS;
}


/*
  ** Function Name        : Pattern_Status_Debug
  ** Function Description : Pattern Status Debug Operation
  ** Function Parameter   : Void
  ** Function Return      : OPER_FAILED 		: Operation Failed
  **                        OPER_SUCCESS	    : Operation Success
  ** Notes				  : Static
*/
static u8 Pattern_Status_Debug(void * Data)
{
	/* Check Parameter (Data) */
	if (NULL == Data) {
		return OPER_FAILED;
	}

	/* Set Pattern Status Debug */


	/* Tcp Server Send Ack */
	if(SEND_SUCCESS != Tcp_Ack()) {
		debug("Send Ack Failed In Pattern Status Debug In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	return OPER_SUCCESS;
}


/*
  ** Function Name        : Pattern_Status
  ** Function Description : Pattern Status Operation
  ** Function Parameter   : Void
  ** Function Return      : OPER_FAILED 		: Operation Failed
  **                        OPER_SUCCESS	    : Operation Success
  ** Notes				  : Static
*/
static u8 Pattern_Status(void * Data)
{
	/* Check Parameter (Data) */
	if (NULL == Data) {
		return OPER_FAILED;
	}

	/* Tcp Server Send Pattern Status */
	VecRun_t * point = (VecRun_t *)PAT_R_STS_ADDR;

	/* Tcp Server Send Ack */
	if(SEND_SUCCESS != Tcp_Ack()) {
		debug("Send Ack Failed In Pattern Status In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	if (SEND_SUCCESS != Tcp_Server_Send((const char *)point,\
			sizeof(VecRun_t))) {
		debug("Send Pattern Status Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	return OPER_SUCCESS;
}


/*
  ** Function Name        : Pin_Config_Debug
  ** Function Description : Pin Config Debug Operation
  ** Function Parameter   : Data                : Point Receive Data Pointer
  **                        Length				: Receive Data Length
  ** Function Return      : OPER_FAILED 		: Operation Failed
  **                        OPER_SUCCESS	    : Operation Success
  ** Notes				  : Static
*/
static u8 Pin_Config_Arg(void * Data)
{
	/* Check Parameter (Data) */
	if (NULL == Data) {
		return OPER_FAILED;
	}

	/* Pin Config Debug */
	Frame * temp = (Frame *)Data;
	/* Copy FPGA Config2 Data */
	u8 * point = (u8 *)temp->data;

	u8 * desdaddr = (void *)FPGA_CFG2_DATA_ADDR;

	u32 count = 0x00;
	u32 size = 0x00;
	size = sizeof(pin_Arg);

	for (count = 0x00; count < size; count++) {
		desdaddr[count] = point[count];
	}

	SETCFGBIT(1);

	/* Tcp Server Send Ack */
	if (SEND_SUCCESS != Tcp_Ack()) {
		debug("Send Ack Failed In Pin Config Debug In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	return OPER_SUCCESS;
}

/*
		0xA261;
*/
static u8 Cycle_Info(void * Data)
{
	/* Check Parameter (Data) */
	if (NULL == Data) {
		return OPER_FAILED;
	}

	u32 Length = 0x0;
	Frame * temp = (Frame *)Data;
	CycleInfo_t * cycle = (CycleInfo_t *)temp->data;
	u8 *point = cycle->cycle_fir_data;
	u8 * desdaddr = (void *)(CYCLE_DATA_ADDR + CycleSize);

	Length = (temp->length) - HEAD_SIZE - COM_SIZE - LENGTH_SIZE - 4;//??????4??????????

	if(cycle->cycle_crt_fir_frame !=  cycle->cycle_total_fir_frame)
	{
		if(cycle->cycle_crt_fir_frame == 1)
		{
			fpga_obj.list = (List *)LIST_ADDR;//????list????
			CycleSize = 0x00;
		}
		memcpy(desdaddr, point, Length);
		CycleSize += Length;
	}
	else if(cycle->cycle_crt_fir_frame ==  cycle->cycle_total_fir_frame)
	{
		if((CycleSize + Length) >= CYCLE_DATA_SIZE)
		{
			CycleSize = CYCLE_DATA_SIZE;
			Length = CYCLE_DATA_SIZE-CycleSize; //????????
			memcpy(desdaddr, point, Length);
		}
		else
		{
			memcpy(desdaddr, point, Length);
			CycleSize += Length;
		}
		shellprintln("  CycleSize:%d", CycleSize);
	}

	if (SEND_SUCCESS != Tcp_Ack())
	{
		debug("Send ack Failed In %s Files,%d Line\r\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	return OPER_SUCCESS;
}

/*
  ** Function Name        : Max9972_Debug
  ** Function Description : Max9972 Debug Operation
  ** Function Parameter   : Data                : Point Receive Data Pointer
  **                        Length				: Receive Data Length
  ** Function Return      : OPER_FAILED 		: Operation Failed
  **                        OPER_SUCCESS	    : Operation Success
  ** Notes				  : Static
*/
static u8 Max9972_Debug(void * Data)
{
	/* Check Parameter (Data) */
	if (NULL == Data) {
		return OPER_FAILED;
	}

	/* Max9972 Debug */


	/* Tcp Server Send Ack */
	if (SEND_SUCCESS != Tcp_Ack()) {
		debug("Send Ack Failed In Max9972 Debug In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	return OPER_SUCCESS;
}


/*
  ** Function Name        : Driver_Board_Firmware
  ** Function Description : Driver Board Firmware Operation
  ** Function Parameter   : Data                : Point Receive Data Pointer
  **                        Length				: Receive Data Length
  ** Function Return      : OPER_FAILED 		: Operation Failed
  **                        OPER_SUCCESS	    : Operation Success
  ** Notes				  : Static
*/
static u8 Driver_Board_Firmware(void * Data)
{
	/* Check Parameter (Data) */
	if (NULL == Data) {
		return OPER_FAILED;
	}

	/* Driver Board Firmware */


	/* Tcp Server Send Ack */
	if (SEND_SUCCESS != Tcp_Ack()) {
		debug("Send Ack Failed In Driver Board Firmware In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	return OPER_SUCCESS;
}


/*
  ** Function Name        : DDR_Test
  ** Function Description : DDR Test Operation
  ** Function Parameter   : Data                : Point Receive Data Pointer
  **                        Length				: Receive Data Length
  ** Function Return      : OPER_FAILED 		: Operation Failed
  **                        OPER_SUCCESS	    : Operation Success
  ** Notes				  : Static
*/
static u8 DDR_Test(void * Data)
{
	/* Check Parameter (Data) */
	if (NULL == Data) {
		return OPER_FAILED;
	}

	/* DDR Test */


	/* Tcp Server Send Ack */
	if (SEND_SUCCESS != Tcp_Ack()) {
		debug("Send Ack Failed In DDR Test In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	return OPER_SUCCESS;
}


/*
  ** Function Name        : DDR_Test_Status
  ** Function Description : DDR Test Operation
  ** Function Parameter   : Data                : Point Receive Data Pointer
  **                        Length				: Receive Data Length
  ** Function Return      : OPER_FAILED 		: Operation Failed
  **                        OPER_SUCCESS	    : Operation Success
  ** Notes				  : Static
*/
static u8 DDR_Test_Status(void * Data)
{
	/* Check Parameter (Data) */
	if (NULL == Data) {
		return OPER_FAILED;
	}

	/* DDR Test Status */


	/* Tcp Server Send Ack */
	if (SEND_SUCCESS != Tcp_Ack()) {
		debug("Send Ack Failed In DDR Test Status In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	return OPER_SUCCESS;
}

/*
  ** Function Name        : Pin_Mask
  ** Function Description : Pin Mask Operation
  ** Function Parameter   : Data                : Point Receive Data Pointer
  ** Function Return      : OPER_FAILED 		: Operation Failed
  **                        OPER_SUCCESS	    : Operation Success
  ** Notes				  : Static
*/
static u8 Pin_Mask(void * Data)
{
	/* Check Parameter (Data) */
	if (NULL == Data) {
		return OPER_FAILED;
	}

	/* Pin Mask */
	Frame * temp = (Frame *)Data;
	/* Copy FPGA Config2 Data */
	Recv_Pin_Mask_Info_t * point = (Recv_Pin_Mask_Info_t *)temp->data;
	Pin_To_Info_t * point_temp = (Pin_To_Info_t *)PIN_MASK_INFO;

	u16 count;
	for (count = 0; count < PIN_NUM; count++) {
		if (point->pin_mask[count]) {
			point_temp->pin_mask[count / 32] |= (0x1 << (count % 32));
		}
		else {
			point_temp->pin_mask[count / 32] &= ~(0x1 << (count % 32));
		}
	}
	SETCFGBIT(2);

	/* Tcp Server Send Ack */
	if (SEND_SUCCESS != Tcp_Ack()) {
		debug("Send Ack Failed In Pin Mask In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	/* Debug */
	//debug("recv:A259 Order\n");
	return OPER_SUCCESS;
}

/*
  ** Function Name        : Read_PS_DDR
  ** Function Description : PC Read ZYNQ(Management Board) PS DDR Data
  ** Function Parameter   : Data                : Point Receive Data Pointer
  ** Function Return      : OPER_FAILED 		: Operation Failed
  **                        OPER_SUCCESS	    : Operation Success
  ** Notes				  : Static
*/
static u8 Read_PS_DDR(void * Data)
{
	/* Check Parameter (Data) */
	if (NULL == Data) {
		return OPER_FAILED;
	}

	/* Read ZYNQ PS DDR */
	Frame * temp = (Frame *)Data;
	Read_DDR_t * point = (Read_DDR_t *)temp->data;

	/* Temp Data */
	u8 data[1024] = {0x00};
	u8 * temp_p = (u8 *)point;
	u8 * temp_p1 = (u8 *)point->ddr_addr;
	u8 length = sizeof(Read_DDR_t);
	u32 leng = point->ddr_length;
	u32 count = 0x00;

	/* Ack Data */
	memcpy((void *)data , (void *)Read_Ack(),ACK_SIZE);
    /* Command Data */
	for (count = 0; count < length; count++) {
		data[count + ACK_SIZE] = temp_p[count];
	}
	/* DDR Data */
	for (count = 0; count < leng; count++) {
		data[count + length + ACK_SIZE] = temp_p1[count];
	}

	/* Tcp Server Send Ack And Data*/
	if (SEND_SUCCESS != Tcp_Server_Send((const char *)data,	\
			point->ddr_length + ACK_SIZE + length)) {
		debug("Send Realtime Status Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	return OPER_SUCCESS;
}

/*
  ** Function Name        : Management_Board_Firmware
  ** Function Description : Management Board Firmware Operation
  ** Function Parameter   : Data                : Point Receive Data Pointer
  ** Function Return      : OPER_FAILED 		: Operation Failed
  **                        OPER_SUCCESS	    : Operation Success
  ** Notes				  : Static
*/
static u8 Management_Board_Firmware(void * Data)
{
	/* Check Parameter (Data) */
	if (NULL == Data) {
		return OPER_FAILED;
	}

	/* Management Board Firmware */
	Frame * temp = (Frame *)Data;
	MB_Firmware_t * point = (MB_Firmware_t *)temp->data;
	/* Temp Data */
	static u32 temp_point = 0x00;
	/* Current Frame */
	static u16 c_frame = 0x00;
	/* Firmware Size */
	static u32 fir_size = 0x00;
	/* Cache Management Board Firmware */
	/* First Frame Firmware */
	if (FIRST_FRAME == point->mb_crt_fir_frame) {
		/* Get Frame Size */
		u32 frame_size = temp->length - LENGTH_SIZE - HEAD_SIZE - COM_SIZE - MB_FR_OVER_SIZE;
		/* Get Next Frame Count */
		c_frame = point->mb_crt_fir_frame + 1;
		/* Clear Firmware Size */
		fir_size = 0x00;
		/* Erase Flash */
		if (ERASE_SUCCESS != Erase_Flash(FLASH_START_ADDR ,point->mb_total_fir_frame * frame_size, \
				Get_Write_Buffer())) {
			debug("Flash Erase Failed In %s Files,%d Line\n",__FILE__,__LINE__);
			return OPER_SUCCESS;
		}
		else {

		}
		/* Write Flash */
		u32 page_cnt = Get_Page_Count(frame_size);
		u32 page;
		u32 page_size = Get_Page_Size();
		for (page = 0; page < page_cnt; page++) {
			if (WRITE_SUCCESS != Write_Flash((page * page_size) + FLASH_START_ADDR + fir_size,
					page_size, WRITE_CMD, (u8 *)point + (page * page_size))) {
				debug("Qspi Write Flash Failed In Management Board Firmware In %s Files,%d Line\n",\
						__FILE__,__LINE__);
				return OPER_FAILED;
			}
		}
		/* Get Firmware Size */
		fir_size += frame_size;
		/* Tcp Server Send Ack */
		if (SEND_SUCCESS != Tcp_Ack()) {
			debug("Send Ack Failed In Driver Management Firmware In %s Files,%d Line\n",__FILE__,__LINE__);
			return OPER_FAILED;
		}
	}
	/* Last Frame Firmware */
	else if (point->mb_crt_fir_frame == point->mb_total_fir_frame) {
		/* Get Frame Size */
		u32 frame_size = temp->length - LENGTH_SIZE - HEAD_SIZE - COM_SIZE - MB_FR_OVER_SIZE - \
				CRC_VAL_SIZE;
		/* Write Flash */
		u32 page_cnt = Get_Page_Count(frame_size);
		u32 page;
		u32 page_size = Get_Page_Size();
		for (page = 0; page < page_cnt; page++) {
			if (WRITE_SUCCESS != Write_Flash((page * page_size) + FLASH_START_ADDR + fir_size,
					page_size, WRITE_CMD, (u8 *)point + (page * page_size))) {
				debug("Qspi Write Flash Failed In Management Board Firmware In %s Files,%d Line\n",\
						__FILE__,__LINE__);
				return OPER_FAILED;
			}
		}
		/* Get Firmware Size */
		fir_size += frame_size;
		/* Read Flash Data */
		temp_point = MB_FRW_INFO_ADDR;
		Read_Flash(FLASH_START_ADDR, fir_size, QUAD_READ_CMD, Get_Write_Buffer(),\
				(u8 *)temp_point);
		/* Crc Check Firmware */
		u32 crc_temp = *(u32 *)&point->mb_fir_data[frame_size];
		u32 crc_val = CRC_INIT_VAL;
		crc_val = Get_Crc32_Val(crc_val, (u8 *)MB_FRW_INFO_ADDR, fir_size);
		/* Ready Ack */
		u8 retr[ACK_SIZE + 1] = {0x00};
		u8 * ack = Read_Ack();
		u8 count;
		for (count = 0; count < ACK_SIZE; count++) {
			retr[count] = ack[count];
		}
		/* According To CRC32 Result Process */
		if (crc_temp != crc_val) {
			/* Debug Updata Failed */
			debug("Firmware Updata Failed, Please Retry!\n");
			/* Tcp Server Send Retransmission Ack */
			retr[ACK_SIZE] = RET_FIM_ACK;
			if (SEND_SUCCESS != Tcp_Server_Send(retr, ACK_SIZE + 1)) {
				debug("Send Nack Failed In Management Board Firmware In %s Files,%d Line\n",__FILE__,__LINE__);
				return OPER_FAILED;
			}
		}
		else {
			/* Debug Updata Failed */
			debug("Firmware Updata Success, Please Reboot!\n");
			/* Tcp Server Send Success Ack */
			retr[ACK_SIZE] = UPT_SUCS_ACK;
			if (SEND_SUCCESS != Tcp_Server_Send(retr, ACK_SIZE + 1)) {
				debug("Send Ack Failed In Management Board Firmware In %s Files,%d Line\n",__FILE__,__LINE__);
				return OPER_FAILED;
			}
		}
	}
	/* Middle Frame Firmware */
	else if (c_frame == point->mb_crt_fir_frame) {
		/* Get Frame Size */
		u32 frame_size = temp->length - LENGTH_SIZE - HEAD_SIZE - COM_SIZE - MB_FR_OVER_SIZE;
		/* Get Next Frame Count */
		c_frame = point->mb_crt_fir_frame + 1;
		/* Write Flash */
		u32 page_cnt = Get_Page_Count(frame_size);
		u32 page;
		u32 page_size = Get_Page_Size();
		for (page = 0; page < page_cnt; page++) {
			if (WRITE_SUCCESS != Write_Flash((page * page_size) + FLASH_START_ADDR + fir_size,
					page_size, WRITE_CMD, (u8 *)point + (page * page_size))) {
				debug("Qspi Write Flash Failed In Management Board Firmware In %s Files,%d Line\n",\
						__FILE__,__LINE__);
				return OPER_FAILED;
			}
		}

		/* Get Firmware Size */
		fir_size += frame_size;

		/* Tcp Server Send Ack */
		if (SEND_SUCCESS != Tcp_Ack()) {
			debug("Send Ack Failed In Management Board Firmware In %s Files,%d Line\n",__FILE__,__LINE__);
			return OPER_FAILED;
		}
	}
	else {

	}

	return OPER_SUCCESS;
}

//????Cycle????size
u32 Get_Cycle_Size(void)
{
	return CycleSize;
}

//????Cycle????size
u32 Get_Cycle_Num(void)
{
	return (CycleSize/12);
}

void Set_Cycle_Size(u32 size)
{
	CycleSize = size;
}


void NetDataCopy(u8 *dest, u8 *src, size_t size)
{
	u32 i = 0;
	for(i = 0; i < size; i++)
	{
		dest[i] = src[i];
	}
}

static u8 Config_File(void * Data)
{
	/* Tcp Server Send Ack */
	if (SEND_SUCCESS != Tcp_Ack()) {
		debug("Send Ack Failed In Config File In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	return OPER_SUCCESS;
}
/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
