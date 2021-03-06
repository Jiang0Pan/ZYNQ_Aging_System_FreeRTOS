/*
  ** @file           : SMBus.c
  ** @brief          : The IIC BUS Driver Source File
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
#include "SMBus.h"


/*
  ** Define
*/
#define		COMMAN_OFFSET		(0)
#define     DATA_OFFSET         (1)
#define  	BUF_SZIE    		(20)


/*
  ** Function Name        : SMBus_Write
  ** Function Description : SMBbus Write Data
  ** Function Parameter   : Wbuf   			    : The point to write data pointer
  **                        WSize   		    : Write data length
  **                        Slaveaddr           : IIC slave address
  **                        Code                : Command Byte
  **                        Chan                : Expand iic channel
  ** Function Return	  :	SUCEESS       		: SMBbus write data success
  **                      : FAILURE        		: SMBbus write data failed
  ** Notes				  : None
*/
s32 SMBus_Write(u8 *Wbuf, u8 WSize, u8 SlaveAddr, u8 Code, IIC_Chan Chan)
{
	s32 Status = FAILURE;
	u8 count;
	u8 buf[BUF_SZIE] = {0x00};

	/* Ready write data */
	buf[COMMAN_OFFSET] = Code;
	for (count = 0; count < WSize; count++) {
		buf[count + DATA_OFFSET] = Wbuf[count];
	}

	/* IIC bus write data to slave */
	Status = IIC_Write(buf, WSize + 1, SlaveAddr, Chan);

	return Status;
}


/*
  ** Function Name        : SMBus_Read
  ** Function Description : SMBbus Read Data (Repeat start)
  ** Function Parameter   : Rbuf   			    : The point to read data pointer
  **                        RSize   		    : Read data length
  **                        Slaveaddr           : IIC slave address
  **                        Code                : Command Byte
  **                        Chan                : Expand iic channel
  ** Function Return	  :	SUCEESS       		: SMBbus read data success
  **                      : FAILURE        		: SMBbus read data failed
  ** Notes				  : None
*/
s32 SMBus_Read (u8 *Rbuf, u8 RSize, u8 SlaveAddr, u8 Code, IIC_Chan Chan)
{
	s32 Status = FAILURE;

	u16 timeout = IIC_WAITE;

	/* Select expand iic channel */
	Status = Select_IIC(Chan);
	if (Status != XST_SUCCESS) {
		return FAILURE;
	}

	/* Set repeat start option */
	Status = XIicPs_SetOptions(&IIC[1], XIICPS_REP_START_OPTION);
	if (Status != XST_SUCCESS) {
		return FAILURE;
	}

	/* Write command data */
	Status = XIicPs_MasterSendPolled(&IIC[1], &Code, 1, SlaveAddr);
	if (Status != XST_SUCCESS) {
		Status = XIicPs_ClearOptions(&IIC[1], XIICPS_REP_START_OPTION);
		if (Status != XST_SUCCESS) {
			return FAILURE;
		}
	}
	else {

	}

	/* Wait repeat start flag */
	if (!(IIC[1].IsRepeatedStart)) {
		while (XIicPs_BusIsBusy(&IIC[1])) {
			if (TimeOutUS(&timeout)) {
				return FAILURE;
			}
		}
	}

	/* Clear repeat start option */
	Status = XIicPs_ClearOptions(&IIC[1], XIICPS_REP_START_OPTION);
	if (Status != XST_SUCCESS) {
		return FAILURE;
	}

	/* Read data */
	vTaskSuspendAll();
	Status = XIicPs_MasterRecvPolled(&IIC[1], Rbuf, RSize, SlaveAddr);
	if (Status != XST_SUCCESS) {
		xTaskResumeAll();
		return FAILURE;
	}
	else {
		xTaskResumeAll();
	}

	/* Wait busy flag */
	while (XIicPs_BusIsBusy(&IIC[1])) {
		if (TimeOutUS(&timeout)) {
			return FAILURE;
		}
	}

	return SUCCESS;
}


/*
  ** Function Name        : SMBus_Code
  ** Function Description : SMBbus Write Command
  ** Function Parameter   : Slaveaddr           : IIC slave address
  **                        Code                : Command Byte
  **                        Chan                : Expand iic channel
  ** Function Return	  :	SUCEESS       		: SMBbus write command success
  **                      : FAILURE        		: SMBbus write command failed
  ** Notes				  : None
*/
s32 SMBus_Code(u8 SlaveAddr, u8 Code, IIC_Chan Chan)
{
	s32 Status = FAILURE;

	Status = IIC_Write(&Code, 1, SlaveAddr, Chan);
	if (FAILURE == Status) {
		return FAILURE;
	}

	return SUCCESS;
}


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/

