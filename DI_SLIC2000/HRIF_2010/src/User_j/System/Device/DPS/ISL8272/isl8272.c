/*
  ** @file           : isl8272.c
  ** @brief          : The Device Of ISL8272(IIC PWR Device) Source File
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
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "sleep.h"
#include "debug_config.h"
#include "return_val.h"
#include "IIC.h"
#include "SMBus.h"
#include "fpga_ctrl.h"
#include "GlobalRef.h"
#include "memory_map.h"
#include "isl8272.h"


/*
  ** Define
*/
/* Vout coefficient */
#define     VOUT_COE        (1.06f)
/* Max output current protect . 10A */
#define		MAX_OCP			(10000)
/* Max Vout */
#define     MAX_VOUT        (5.5f)
/* Float cover coefficient */
#define     FLOAT_COE       (8192.0f)
/* U16 cover coefficient */
#define     U16_COE         (1000)
/* Power good on coefficient */
#define     PG_COE          (0.8f)
/* Under current protect value */
#define     UCP             (0xECE0)
/* Input over voltage value */
#define     IOV             (13000)
/* Input under voltage value */ 
#define     IUV             (7000)


/*
  ** Static Global Variable
*/
static u32 CalBuf1[16];


/*
  ** Function Name        : Config_Isl8272_Reg
  ** Function Description : Config Isl8272(ISL8277) Register 
  ** Function Parameter   : Reg_Addr   			: Isl8272 register address                   
  ** 						Reg_data 		    : The point to write data pointer
  **                        Data_Len 		    : Write data length
  **                        Slaveaddr           : IIC slave address
  **                        Chan                : Expand iic channel
  ** Function Return	  :	WRITE_SUCEESS       : Write Isl8272 register data success
  **                      : WRITE_FAILED        : Write Isl8272 register data failed
  ** Notes				  : None
*/
u8 Config_Isl8272_Reg(u8 Reg_Addr, u8 * Reg_data, u8 Data_Len ,u8 Slaveaddr, IIC_Chan Chan)
{
	u8 status;
	/* Read Data Buf */
	u16 Write_data, Read_data = 0x00;
	u8 Write_buf[2] = {0x00};
	u8 Read_buf[2] = {0x00};
	TickType_t x1ms = pdMS_TO_TICKS(1);

	Write_buf[0] = Reg_data[0];
	Write_buf[1] = Reg_data[1];
	Write_data = Reg_data[1];
	Write_data = Write_data >> 8;
	Write_data |= Reg_data[0];

	do {
		vTaskDelay(x1ms);

		status = Write_Isl8272_Reg(Reg_Addr, Write_buf, Data_Len, Slaveaddr, Chan);
		if (WRITE_SUCCESS != status) {			
			return CONFIG_FAILED;
		}

	    vTaskDelay(x1ms);

		status = Read_Isl8272_Reg(Reg_Addr, Read_buf, Data_Len, Slaveaddr, Chan);
		if (READ_SUCCESS != status) {
			return CONFIG_FAILED;
		}

		Read_data = Read_buf[1];
		Read_data = Read_data >> 8;
		Read_data |= Read_buf[0];

	} while (Write_data != Read_data);

	return CONFIG_SUCCESS;
}


/*
  ** Function Name        : Write_Isl8272_Reg
  ** Function Description : Write Data To Isl8272(ISL8277) Register 
  ** Function Parameter   : Reg_Addr   			: Isl8272 register address                   
  ** 						Reg_data 		    : The point to write data pointer
  **                        Data_Len 		    : Write data length
  **                        Slaveaddr           : IIC slave address
  **                        Chan                : Expand iic channel
  ** Function Return	  :	WRITE_SUCEESS       : Write Isl8272 register data success
  **                      : WRITE_FAILED        : Write Isl8272 register data failed
  ** Notes				  : None
*/
u8 Write_Isl8272_Reg(u8 Reg_Addr, u8 * Reg_data, u8 Data_Len ,u8 Slaveaddr, IIC_Chan Chan)
{
	u8 status;

	status = SMBus_Write(Reg_data, Data_Len, Slaveaddr, Reg_Addr, Chan);
	if (SUCCESS != status) {
 		debug("iic write isl8272 register %x failed in %s Files,%d Line\n", Reg_Addr,  __FILE__, __LINE__);	
		return WRITE_FAILED;	
	}

	return WRITE_SUCCESS;
}


/*
  ** Function Name        : Read_Isl8272_Reg
  ** Function Description : Read Data To Isl8272(ISL8277) Register 
  ** Function Parameter   : Reg_Addr   			: Isl8272 register address                   
  ** 						Reg_data 		    : The point to write data pointer
  **                        Data_Len 		    : Read data length
  **                        Slaveaddr           : IIC slave address
  **                        Chan                : Expand iic channel
  ** Function Return	  :	READ_SUCEESS        : Read Isl8272 register data success
  **                      : READ_FAILED         : Read Isl8272 register data failed
  ** Notes				  : None
*/
u8 Read_Isl8272_Reg(u8 Reg_Addr, u8 * Reg_data, u8 Data_Len ,u8 Slaveaddr, IIC_Chan Chan)
{
	u8 status;

	status = SMBus_Read(Reg_data, Data_Len, Slaveaddr, Reg_Addr, Chan);
	if (SUCCESS != status) {
 		debug("iic read isl8272 register %x failed in %s Files,%d Line\n", Reg_Addr,  __FILE__, __LINE__);	
		return READ_FAILED;	
	}

	return READ_SUCCESS;
}


/*
  ** Function Name        : Isl8272_Config
  ** Function Description : Isl8272(Isl8277) Config
  ** Function Parameter   : Ch   			    : Dps Channel                  
  ** 						Dps_cfg 		    : Dps Config parameter
  **                      : Protect 		    : Protect enable
  ** Function Return	  :	SUCEESS             : Configuration success
  **                      : FAILURE             : Configuration failed
  ** Notes				  : None
*/
u8 Isl8272_Config(Dps_channel Ch, Dps_Arg * Dps_cfg, u8 Protect, u8 Bid)
{
	TickType_t x100ms = pdMS_TO_TICKS(100);

	u8 data[2] = {0x00};
	u16 BufData = 0x00;
	u8 status;

    /* Ready register data . LSB */
	data[0] = 0x17;
	/* Write ISL8272(ISL8277) register data : ON_OFF_CONFIG */
	status = Write_Isl8272_Reg(ON_OFF_CONFIG, data, 1, dps_info.DBMsg[Bid].DpsAddr[Ch],\
			dps_info.DBMsg[Bid].Chan);
	if (WRITE_SUCCESS != status) {
		if (Protect) {
			goto error;
		}
	}
	else {

	}
    vTaskDelay(x100ms);

    /* Ready register data . LSB */
	data[0] = 0x04;
	/* Write ISL8272(ISL8277) register data : OPERATION */
	status = Write_Isl8272_Reg(OPERATION, data, 1, dps_info.DBMsg[Bid].DpsAddr[Ch],\
			dps_info.DBMsg[Bid].Chan);
	if (WRITE_SUCCESS != status) {
		if (Protect) {
			goto error;
		}
	}
	else {

	}
	vTaskDelay(x100ms);

    /* Ready register data . LSB */
	data[0] = 0x04;
	/* Write ISL8272(ISL8277) register data : CLEAR_FAULTS */
	status = Write_Isl8272_Reg(CLEAR_FAULTS, data, 1, dps_info.DBMsg[Bid].DpsAddr[Ch],\
			dps_info.DBMsg[Bid].Chan);
	if (WRITE_SUCCESS != status) {
		if (Protect) {
			goto error;
		}
	}
	else {

	}
	vTaskDelay(x100ms);

	/* Ready register data . LSB */
 	BufData = CoverToL11(IOV);
 	data[0] = BufData & 0xFF;
 	data[1] = (BufData >> 8) & 0xFF;
	/* Write ISL8272(ISL8277) register data : VIN_OV_FAULT_LIMIT */
	status = Config_Isl8272_Reg(VIN_OV_FAULT_LIMIT, data, 2, dps_info.DBMsg[Bid].DpsAddr[Ch],\
			dps_info.DBMsg[Bid].Chan);
	if (CONFIG_SUCCESS != status) {
		if (Protect) {
			goto error;
		}
	}
	else {

	}

    /* Ready register data . LSB */
 	BufData = CoverToL11(IUV);
 	data[0] = BufData & 0xFF;
 	data[1] = (BufData >> 8) & 0xFF;
	/* Write ISL8272(ISL8277) register data : VIN_UV_FAULT_LIMIT */
	status = Config_Isl8272_Reg(VIN_UV_FAULT_LIMIT, data, 2, dps_info.DBMsg[Bid].DpsAddr[Ch],\
			dps_info.DBMsg[Bid].Chan);
	if (CONFIG_SUCCESS != status) {
		if (Protect) {
			goto error;
		}
	}
	else {

	}

    /* Ready register data . LSB */
 	BufData = CoverToL11(Dps_cfg->Dsp[Ch].InputMaxV);
 	data[0] = BufData & 0xFF;
 	data[1] = (BufData >> 8) & 0xFF;
	/* Write ISL8272(ISL8277) register data : VIN_OV_WARN_LIMIT */
	status = Config_Isl8272_Reg(VIN_OV_WARN_LIMIT, data, 2, dps_info.DBMsg[Bid].DpsAddr[Ch],\
			dps_info.DBMsg[Bid].Chan);
	if (CONFIG_SUCCESS != status) {
		if (Protect) {
			goto error;
		}
	}
	else {

	}

	/* Ready register data . LSB */
 	BufData = CoverToL11(Dps_cfg->Dsp[Ch].InputMinV);
 	data[0] = BufData & 0xFF;
 	data[1] = (BufData >> 8) & 0xFF;
	/* Write ISL8272(ISL8277) register data : VIN_UV_WARN_LIMIT */
	status = Config_Isl8272_Reg(VIN_UV_WARN_LIMIT, data, 2, dps_info.DBMsg[Bid].DpsAddr[Ch],\
			dps_info.DBMsg[Bid].Chan);
	if (CONFIG_SUCCESS != status) {
		if (Protect) {
			goto error;
		}
	}
	else {

	}

	/* Ready register data . LSB */
 	BufData = (float)(FLOAT_COE * MAX_VOUT * VOUT_COE);
 	data[0] = BufData & 0xFF;
 	data[1] = (BufData >> 8) & 0xFF;
	/* Write ISL8272(ISL8277) register data : VOUT_MAX */
	status = Config_Isl8272_Reg(VOUT_MAX, data, 2, dps_info.DBMsg[Bid].DpsAddr[Ch],\
			dps_info.DBMsg[Bid].Chan);
	if (CONFIG_SUCCESS != status) {
		if (Protect) {
			goto error;
		}
	}
	else {

	}

	/* Ready register data . LSB */
 	BufData = (float)(FLOAT_COE * ((float)Dps_cfg->Dsp[Ch].DpsMaxV / U16_COE) * VOUT_COE);
 	data[0] = BufData & 0xFF;
 	data[1] = (BufData >> 8) & 0xFF;
	/* Write ISL8272(ISL8277) register data : VOUT_OV_FAULT_LIMIT */
	status = Config_Isl8272_Reg(VOUT_OV_FAULT_LIMIT, data, 2, dps_info.DBMsg[Bid].DpsAddr[Ch],\
			dps_info.DBMsg[Bid].Chan);
	if (CONFIG_SUCCESS != status) {
		if (Protect) {
			goto error;
		}
	}
	else {

	}

	/* Ready register data . LSB */
 	BufData = (float)(FLOAT_COE * ((float)Dps_cfg->Dsp[Ch].DpsMaxV / U16_COE) * VOUT_COE);
 	data[0] = BufData & 0xFF;
 	data[1] = (BufData >> 8) & 0xFF;
	/* Write ISL8272(ISL8277) register data : VOUT_OV_WARN_LIMIT */
	status = Config_Isl8272_Reg(VOUT_OV_WARN_LIMIT, data, 2, dps_info.DBMsg[Bid].DpsAddr[Ch],\
			dps_info.DBMsg[Bid].Chan);
	if (CONFIG_SUCCESS != status) {
		if (Protect) {
			goto error;
		}
	}
	else {

	}

	/* Ready register data . LSB */
 	BufData = (float)(FLOAT_COE * ((float)Dps_cfg->Dsp[Ch].DpsMinV / U16_COE));
 	data[0] = BufData & 0xFF;
 	data[1] = (BufData >> 8) & 0xFF;
	/* Write ISL8272(ISL8277) register data : VOUT_UV_WARN_LIMIT */
	status = Config_Isl8272_Reg(VOUT_UV_WARN_LIMIT, data, 2, dps_info.DBMsg[Bid].DpsAddr[Ch],\
			dps_info.DBMsg[Bid].Chan);
	if (CONFIG_SUCCESS != status) {
		if (Protect) {
			goto error;
		}
	}
	else {

	}

	/* Ready register data . LSB */
 	BufData = (float)(FLOAT_COE * ((float)Dps_cfg->Dsp[Ch].DpsMinV / U16_COE));
 	data[0] = BufData & 0xFF;
 	data[1] = (BufData >> 8) & 0xFF;
	/* Write ISL8272(ISL8277) register data : VOUT_UV_FAULT_LIMIT */
	status = Config_Isl8272_Reg(VOUT_UV_FAULT_LIMIT, data, 2, dps_info.DBMsg[Bid].DpsAddr[Ch],\
			dps_info.DBMsg[Bid].Chan);
	if (CONFIG_SUCCESS != status) {
		if (Protect) {
			goto error;
		}
	}
	else {

	}

	/* Ready register data . LSB*/
 	BufData = (float)(VOUT_COE * FLOAT_COE * ((float)Dps_cfg->Dsp[Ch].SetValue / U16_COE));
 	data[0] = BufData & 0xFF;
 	data[1] = (BufData >> 8) & 0xFF;
	/* Write ISL8272(ISL8277) register data : VOUT_MARGIN_HIGH */
	status = Config_Isl8272_Reg(VOUT_MARGIN_HIGH, data, 2, dps_info.DBMsg[Bid].DpsAddr[Ch],\
			dps_info.DBMsg[Bid].Chan);
	if (CONFIG_SUCCESS != status) {
		if (Protect) {
			goto error;
		}
	}
	else {

	}

	/* Ready register data . LSB */
 	BufData = (float)(FLOAT_COE * ((float)Dps_cfg->Dsp[Ch].SetValue / U16_COE));
 	data[0] = BufData & 0xFF;
 	data[1] = (BufData >> 8) & 0xFF;
	/* Write ISL8272(ISL8277) register data : VOUT_MARGIN_LOW */
	status = Config_Isl8272_Reg(VOUT_MARGIN_LOW, data, 2, dps_info.DBMsg[Bid].DpsAddr[Ch],\
			dps_info.DBMsg[Bid].Chan);
	if (CONFIG_SUCCESS != status) {
		if (Protect) {
			goto error;
		}
	}
	else {

	}

	/* Ready register data . LSB */
	BufData = CoverToL11(Dps_cfg->Dsp[Ch].Protect_I);
 	data[0] = BufData & 0xFF;
 	data[1] = (BufData >> 8) & 0xFF;
	/* Write ISL8272(ISL8277) register data : IOUT_OC_FAULT_LIMIT */
	status = Config_Isl8272_Reg(IOUT_OC_FAULT_LIMIT, data, 2, dps_info.DBMsg[Bid].DpsAddr[Ch],\
			dps_info.DBMsg[Bid].Chan);
	if (CONFIG_SUCCESS != status) {
		if (Protect) {
			goto error;
		}
	}
	else {

	}	

	/* Ready register data . LSB */
	BufData = UCP;
 	data[0] = BufData & 0xFF;
 	data[1] = (BufData >> 8) & 0xFF;
	/* Write ISL8272(ISL8277) register data : IOUT_UC_FAULT_LIMIT */
	status = Config_Isl8272_Reg(IOUT_UC_FAULT_LIMIT, data, 2, dps_info.DBMsg[Bid].DpsAddr[Ch],\
			dps_info.DBMsg[Bid].Chan);
	if (CONFIG_SUCCESS != status) {
		if (Protect) {
			goto error;
		}
	}
	else {

	}	

	/* Ready register data . LSB */
	BufData = (float)(PG_COE * FLOAT_COE * ((float)Dps_cfg->Dsp[Ch].SetValue / U16_COE));
 	data[0] = BufData & 0xFF;
 	data[1] = (BufData >> 8) & 0xFF;
	/* Write ISL8272(ISL8277) register data : POWER_GOOD_ON */
	status = Config_Isl8272_Reg(POWER_GOOD_ON, data, 2, dps_info.DBMsg[Bid].DpsAddr[Ch],\
			dps_info.DBMsg[Bid].Chan);
	if (CONFIG_SUCCESS != status) {
		if (Protect) {
			goto error;
		}
	}
	else {

	}		

	/* Ready register data . LSB */
	BufData = (float)(FLOAT_COE * ((float)Dps_cfg->Dsp[Ch].SetValue / U16_COE));
 	data[0] = BufData & 0xFF;
 	data[1] = (BufData >> 8) & 0xFF;
	/* Write ISL8272(ISL8277) register data : VOUT_COMMAND */
	status = Config_Isl8272_Reg(VOUT_COMMAND, data, 2, dps_info.DBMsg[Bid].DpsAddr[Ch],\
			dps_info.DBMsg[Bid].Chan);
	if (CONFIG_SUCCESS != status) {
		if (Protect) {
			goto error;
		}
	}
	else {

	}	
	
	/* Ready register data . LSB */
	BufData = CoverToL11((Dps_cfg->Dsp[Ch].Open.Keep - Dps_cfg->Dsp[Ch].Open.STime) * U16_COE);	
 	data[0] = BufData & 0xFF;
 	data[1] = (BufData >> 8) & 0xFF;
	/* Write ISL8272(ISL8277) register data : TON_RISE */
	status = Config_Isl8272_Reg(TON_RISE, data, 2, dps_info.DBMsg[Bid].DpsAddr[Ch],\
			dps_info.DBMsg[Bid].Chan);
	if (CONFIG_SUCCESS != status) {
		if (Protect) {
			goto error;
		}
	}
	else {

	}	

	/* Ready register data . LSB */
	BufData = CoverToL11(0000);
 	data[0] = BufData & 0xFF;
 	data[1] = (BufData >> 8) & 0xFF;
	/* Write ISL8272(ISL8277) register data : TON_DELAY */
	status = Config_Isl8272_Reg(TON_DELAY, data, 2, dps_info.DBMsg[Bid].DpsAddr[Ch],\
			dps_info.DBMsg[Bid].Chan);
	if (CONFIG_SUCCESS != status) {
		if (Protect) {
			goto error;
		}
	}
	else {

	}	

	/* Ready register data . LSB */
	BufData = CoverToL11((Dps_cfg->Dsp[Ch].Close.Keep - Dps_cfg->Dsp[Ch].Close.STime) * U16_COE);
 	data[0] = BufData & 0xFF;
 	data[1] = (BufData >> 8) & 0xFF;
	/* Write ISL8272(ISL8277) register data : TOFF_FALL */
	status = Config_Isl8272_Reg(TOFF_FALL, data, 2, dps_info.DBMsg[Bid].DpsAddr[Ch],\
			dps_info.DBMsg[Bid].Chan);
	if (CONFIG_SUCCESS != status) {
		if (Protect) {
			goto error;
		}
	}
	else {

	}

	/* Ready register data . LSB */
	BufData = CoverToL11(0000);
 	data[0] = BufData & 0xFF;
 	data[1] = (BufData >> 8) & 0xFF;
	/* Write ISL8272(ISL8277) register data : TOFF_DELAY */
	status = Config_Isl8272_Reg(TOFF_DELAY, data, 2, dps_info.DBMsg[Bid].DpsAddr[Ch],\
			dps_info.DBMsg[Bid].Chan);
	if (CONFIG_SUCCESS != status) {
		if (Protect) {
			goto error;
		}
	}
	else {

	}	

	return CONFIG_SUCCESS;

error:
	return CONFIG_FAILED;
}


/*
  ** Function Name        : CoverToL11
  ** Function Description : u16 cover to l11
  ** Function Parameter   : Data   			    : need to cover data                  
  ** Function Return	  :	u16                 : cover over data
  ** Notes				  : None
*/
u16 CoverToL11(u32 Data)
{
	u8 SubSquare,i;
	u16 DataBuf;
	float GetSbuSquare,Min;
	u16 Watch1;

	for (SubSquare = 0; SubSquare <= 11; SubSquare++) {
		DataBuf = 1 << SubSquare;
		CalBuf1[SubSquare] = Data * DataBuf;
	}

	Min = 1000;
	for (i = 0; i < 12; i++) {
		if (CalBuf1[i] < 2048000) {
			DataBuf = CalBuf1[i] % 1000;
			GetSbuSquare = (float)DataBuf / (1 << i);
			if (GetSbuSquare < Min) {
				Min = GetSbuSquare;
				SubSquare = i;
			}
		}
	}

	Watch1 = SubSquare;
	DataBuf = CalBuf1[SubSquare] / 1000;
	Watch1 = DataBuf;
	SubSquare = ~SubSquare + 1;
	Watch1 = DataBuf | (SubSquare << 11);

	return DataBuf | (SubSquare << 11);
}


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/

