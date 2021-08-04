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


/*
  ** Include
*/
#include <stdio.h>
#include "sleep.h"
#include "return_val.h"
#include "Error.h"
#include "debug_config.h"
#include "spi.h"
#include "dps_info.h"
#include "system_emio.h"
#include "GlobalRef.h"
#include "IO.h"
#include "fpga_ctrl.h"


/*
  ** Define
*/
/* Transfer Length */
#define     TRANSFER_LENGTH     (0x03)
/* Command Byte */
#define     COMMAND_DATA        (0x00)
/* First Data Byte */
#define     FIRST_DATA          (0x01)
/* Second Data Byte */
#define     SECOND_DATA         (0x02)
/* Read And Write Bits */
#define     READ_WRITE_BIT      (0x07)
/* Reset DPS Control Command */
#define     RESET_COMMAND       (0x01)
/* Version Byte Size */
#define     VER_SIZE            (20)      


/*
  ** Function Name        : Fpga_dps_Transfer
  ** Function Description : Through spi interface transfer data to fpga dps
  ** Function Parameter   : t_data 			    : Send data point
  ** 						r_data              : Recv data point
  ** 						length              : Send and Recv data lengtg
  ** 						bid                 : Board id
  ** 											  DPS Board 0
  ** 											  DPS Board 1
  ** Function Return      : SUCCESS             : Transfer success
  ** 						FAILURE             : Transfer Failed
  ** Notes				  : None
*/
s32 Fpga_Dps_Transfer(u8 * t_data, u8 * r_data,u16 length, u8 bid)
{
	XGpioPs * spi_cs = NULL;
	spi_cs = Get_Emio_Point();

	/* SPI switch on */
	if (IO_Level_Set(&(bbd_info.Ctl_Output), Group0, \
			0, 0, bbd_info.IO_SlaveAddr, bbd_info.TOChan)) {
		debug("Set back board control spi error!\n");
		return FAILURE;
	}
	if (IO_Level_Set(&(bbd_info.Ctl_Output), Group0, \
			1, 0, bbd_info.IO_SlaveAddr, bbd_info.TOChan)) {
		debug("Set back board control spi error!\n");
		return FAILURE;
	}
	if (IO_Level_Set(&(dps_info.DBMsg[bid].Output_LVL), Group1, \
			1, 1, dps_info.PCA953_SlaveAddr, dps_info.DBMsg[bid].Chan)) {
		debug("Set dps fpga control spi error!\n");
		return FAILURE;
	}
	if (IO_Level_Set(&(dps_info.DBMsg[bid].Output_LVL), Group1, \
			2, 0, dps_info.PCA953_SlaveAddr, dps_info.DBMsg[bid].Chan)) {
		debug("Set dps fpga control spi error!\n");
		return FAILURE;
	}

	/* Spi transfer */
	Set_Spi_Cs(spi_cs,LOW_LEVEL);
	if (SUCCESS != Spi_Transfer(t_data, r_data, length, CPLD_SPI)) {
		return FAILURE;
	}
	Set_Spi_Cs(spi_cs,HIGH_LEVEL);

	/* SPI switch off */
	if (IO_Level_Set(&(bbd_info.Ctl_Output), Group0, \
			1, 1, bbd_info.IO_SlaveAddr, bbd_info.TOChan)) {
		debug("Set CPLD SPI error!\r\n");
		return FAILURE;
	}
	if (IO_Level_Set(&(dps_info.DBMsg[bid].Output_LVL), Group1, \
			2, 1, dps_info.PCA953_SlaveAddr, dps_info.DBMsg[bid].Chan)) {
		debug("Set CPLD SPI error!\r\n");
		return FAILURE;
	}

	return SUCCESS;
}


/*
  ** Function Name        : Write_Dps_Fpga_Reg
  ** Function Description : Through spi interface write fpga register
  ** Function Parameter   : reg   			    : Write fpga register address
  ** 						reg_data            : Write fpga register data
  ** 						bid                 : Board id
  ** 											  DPS Board 0
  ** 											  DPS Board 1
  ** Function Return      : CONFIG_SUCCESS      : Config success
  ** 						CONFIG_FAILED       : Config Failed
  ** Notes				  : None
*/
u8 Config_Dps_Fpga_Reg(u8 reg, u16 reg_data, u8 bid)
{
	u16 t_data = 0x00;
	u16 r_data = 0x00;

	t_data = reg_data;

	do {
		if (WRITE_SUCCESS != Write_Dps_Fpga_Reg(reg, t_data, bid)) {
			return CONFIG_FAILED;
		}
		else {

		}

		if (READ_SUCCESS != Read_Dps_Fpga_Reg(reg, &r_data, bid)) {
			return CONFIG_FAILED;
		}
		else {

		}

	} while(t_data != r_data);

	return CONFIG_SUCCESS;
}


/*
  ** Function Name        : Write_Dps_Fpga_Reg
  ** Function Description : Through spi interface write fpga register
  ** Function Parameter   : reg   			    : Write fpga register address
  ** 						reg_data            : Write fpga register data
  ** 						bid                 : Board id
  ** 											  DPS Board 0
  ** 											  DPS Board 1
  ** Function Return      : WRITE_SUCCESS       : Write success
  ** 						WRITE_FAILED        : Write Failed
  ** Notes				  : None
*/
u8 Write_Dps_Fpga_Reg(u8 reg, u16 reg_data, u8 bid)
{
	u8 t_data[TRANSFER_LENGTH] = {0x00};
	u8 r_data[TRANSFER_LENGTH] = {0x00};

	t_data[COMMAND_DATA] = (WRITE_BITS << READ_WRITE_BIT);
	t_data[COMMAND_DATA] |= reg;
	t_data[FIRST_DATA] = (u8)(reg_data >> 8) & 0xFF;
	t_data[SECOND_DATA] = (u8)reg_data & 0xFF;

	if (SUCCESS != Fpga_Dps_Transfer(t_data, r_data, TRANSFER_LENGTH, bid)) {
		debug("Write Fpga Dps Register %x Error In %s Files,%d Line\n", reg,__FILE__, __LINE__);
		return WRITE_FAILED;
	}

	return WRITE_SUCCESS;
}


/*
  ** Function Name        : Read_Dps_Fpga_Version
  ** Function Description : Through spi interface read fpga version
  ** Function Parameter   : reg   			    : Read fpga register address
  ** 						reg_data            : Read fpga register data
  ** 						bid                 : Board id
  ** 											  DPS Board 0
  ** 											  DPS Board 1
  ** Function Return      : READ_SUCCESS        : Read success
  ** 						READ_FAILED         : Read Failed
  ** Notes				  : None
*/
u8 Read_Dps_Fpga_Version(u8 * reg_data, u8 bid)
{
	u16 r_data = 0x00;
	u16 count = 0x00;
	u8 major = 0x00;
	u8 minor = 0x00;

	for (count = 0; count < VER_SIZE;count++) {
		reg_data[count] = 0x00;
	}

	if (READ_SUCCESS != Read_Dps_Fpga_Reg(VER_REG, &r_data, bid)) {
		return READ_FAILED;
	}

	/* major version */
	major = (r_data >> 9);
	/* minor version */
	minor = (r_data >> 8) & 0x01;

	sprintf(reg_data, "%d.%d", major, minor);
	
	return READ_SUCCESS;
}


/*
  ** Function Name        : Read_Dps_Fpga_Reg
  ** Function Description : Through spi interface read fpga register
  ** Function Parameter   : reg   			    : Read fpga register address
  ** 						reg_data            : Read fpga register data
  ** 						bid                 : Board id
  ** 											  DPS Board 0
  ** 											  DPS Board 1
  ** Function Return      : READ_SUCCESS        : Read success
  ** 						READ_FAILED         : Read Failed
  ** Notes				  : None
*/
u8 Read_Dps_Fpga_Reg(u8 reg, u16 * reg_data, u8 bid)
{
	u8 t_data[TRANSFER_LENGTH] = {0x00};
	u8 r_data[TRANSFER_LENGTH] = {0x00};

	t_data[COMMAND_DATA] = (READ_BITS << READ_WRITE_BIT);
	t_data[COMMAND_DATA] |= reg;

	if (SUCCESS != Fpga_Dps_Transfer(t_data, r_data, TRANSFER_LENGTH, bid)) {
		debug("Read Fpga Dps Register %x Error In %s Files,%d Line\n", reg,__FILE__, __LINE__);
		return READ_FAILED;
	}

	*reg_data = r_data[FIRST_DATA];
	*reg_data = (*reg_data) << 8;
	*reg_data |= r_data[SECOND_DATA];

	return READ_SUCCESS;
}


/*
  ** Function Name        : Read_Dps_Fpga_Reg
  ** Function Description : Through spi interface read fpga register
  ** Function Parameter   : reg   			    : Read fpga register address
  ** 						reg_data            : Read fpga register data
  ** 						bid                 : Board id
  ** 											  DPS Board 0
  ** 											  DPS Board 1
  ** Function Return      : FAILURE             : Reset failed
  ** 						SUCCESS             : Read success
  ** Notes				  : None
*/
u8 Dps_Fpga_Reset(u8 bid)
{
	u8 t_data[TRANSFER_LENGTH] = {0x00};
	u8 r_data[TRANSFER_LENGTH] = {0x00};

	t_data[COMMAND_DATA] = (WRITE_BITS << READ_WRITE_BIT);
	t_data[COMMAND_DATA] |= RESET_REG;
	t_data[SECOND_DATA] = RESET_COMMAND;

	if (SUCCESS != Fpga_Dps_Transfer(t_data, r_data, TRANSFER_LENGTH, bid)) {
		debug("Fpga Dps Board %d Transfer Error In %s Files,%d Line\n", bid, __FILE__, __LINE__);
		return FAILURE;
	}

	return SUCCESS;
}


/*
  ** Function Name        : Set_All_Dps_Check_Error
  ** Function Description : Through spi interface operation fpga all dps check error
  ** 						en                  : Enable check error Flag
  ** 						bid                 : Board id
  ** 											  DPS Board 0
  ** 											  DPS Board 1
  ** Function Return      : SET_SUCCESS         : Set success
  ** 						SET_FAILED          : Set Failed
  ** Notes				  : None
*/
u8 Set_All_Dps_Check_Error(All_Enable_Status en, u8 bid)
{
	u16 t_data = 0x00;

	t_data = en;

	if (CONFIG_SUCCESS != Config_Dps_Fpga_Reg(ERR_START_REG, t_data, bid)) {
		return SET_FAILED;
	}
	else {

	} 

	return SET_SUCCESS;
}


/*
  ** Function Name        : Set_Dps_Channel_L1_Enable
  ** Function Description : Through spi interface operation fpga dps L1 enable
  ** Function Parameter   : ch   			    : Send data point
  ** 						en                  : Enable Flag
  ** 						bid                 : Board id
  ** 											  DPS Board 0
  ** 											  DPS Board 1
  ** Function Return      : SET_SUCCESS         : Set success
  ** 						SET_FAILED          : Set Failed
  ** Notes				  : None
*/
u8 Set_Dps_Channel_L1_Enable(Dps_channel ch, Enable_status en, u8 bid)
{
	u8 t_data[TRANSFER_LENGTH] = {0x00};
	u8 r_data[TRANSFER_LENGTH] = {0x00};

	if ((ch >= DPSA_CHANNEL_BD0) && (ch <= DPSF_CHANNEL_BD0)) {
		t_data[COMMAND_DATA] = (READ_BITS << READ_WRITE_BIT);
		t_data[COMMAND_DATA] |= ENABLE_LV1_REG;

		if (SUCCESS != Fpga_Dps_Transfer(t_data, r_data, TRANSFER_LENGTH, bid)) {
			debug("Fpga Dps Transfer Error In %s Files,%d Line\n",__FILE__,__LINE__);
			return SET_FAILED;
		}

		t_data[FIRST_DATA] = r_data[FIRST_DATA];
		t_data[SECOND_DATA] = r_data[SECOND_DATA];
		if (en) {
			t_data[SECOND_DATA] |= 1 << ch;
		}
		else {
			t_data[SECOND_DATA] &= ~(1 << ch);
		}
		t_data[COMMAND_DATA] = (WRITE_BITS << READ_WRITE_BIT);
		t_data[COMMAND_DATA] |= ENABLE_LV1_REG;

		if (SUCCESS != Fpga_Dps_Transfer(t_data, r_data, TRANSFER_LENGTH, bid)) {
			debug("Fpga Dps Transfer Error In %s Files,%d Line\n",__FILE__,__LINE__);
			return SET_FAILED;
		}
	}
	else {
		debug("Dps Channel Error In %s Files,%d Line\n",__FILE__,__LINE__);
		return SET_FAILED;
	}

	return SET_SUCCESS;
}


/*
  ** Function Name        : Set_All_Dps_L1_Enable
  ** Function Description : Through spi interface operation fpga all dps L1 enable
  ** 						en                  : Enable Flag
  ** 						bid                 : Board id
  ** 											  DPS Board 0
  ** 											  DPS Board 1
  ** Function Return      : SET_SUCCESS         : Set success
  ** 						SET_FAILED          : Set Failed
  ** Notes				  : None
*/
u8 Set_All_Dps_L1_Enable(All_Enable_Status en, u8 bid)
{
	u16 t_data = 0x00;

	t_data = en;

	if (CONFIG_SUCCESS != Config_Dps_Fpga_Reg(ENABLE_LV1_REG, t_data, bid)) {
		return SET_FAILED;
	}
	else {

	} 

	return SET_SUCCESS;
}


/*
  ** Function Name        : Set_Dps_Channel_L2_Enable
  ** Function Description : Through spi interface operation fpga dps L2 enable
  ** Function Parameter   : ch   			    : Send data point
  ** 						en                  : Enable Flag
  ** 						bid                 : Board id
  ** 											  DPS Board 0
  ** 											  DPS Board 1
  ** Function Return      : SET_SUCCESS         : Set success
  ** 						SET_FAILED          : Set Failed
  ** Notes				  : None
*/
u8 Set_Dps_Channel_L2_Enable(Dps_channel ch, Enable_status en, u8 bid)
{
	u8 t_data[TRANSFER_LENGTH] = {0x00};
	u8 r_data[TRANSFER_LENGTH] = {0x00};

	if ((ch >= DPSA_CHANNEL_BD0) && (ch <= DPS4_CHANNEL_BD0)) {
		t_data[COMMAND_DATA] = (READ_BITS << READ_WRITE_BIT);
		t_data[COMMAND_DATA] |= ENABLE_LV2_REG;

		if (SUCCESS != Fpga_Dps_Transfer(t_data, r_data, TRANSFER_LENGTH, bid)) {
			debug("Fpga Dps Transfer Error In %s Files,%d Line\n",__FILE__,__LINE__);
			return SET_FAILED;
		}


		t_data[FIRST_DATA] = r_data[FIRST_DATA];
		t_data[SECOND_DATA] = r_data[SECOND_DATA];
		if ((ch >= DPSA_CHANNEL_BD0) && (ch <= DPSF_CHANNEL_BD0)) {
			if (en) {
				t_data[SECOND_DATA] |= 1 << ch;
			}
			else {
				t_data[SECOND_DATA] &= ~(1 << ch);
			}
		}
		else if ((ch >= DPS3_CHANNEL_BD0) && (ch <= DPS4_CHANNEL_BD0)) {
			if (en) {
				t_data[1] |= 1 << (ch - 8);
			}
			else {
				t_data[1] &= ~(1 << (ch - 8));
			}
		}
		else {

		}

		t_data[COMMAND_DATA] = (WRITE_BITS << 7);
		t_data[COMMAND_DATA] |= ENABLE_LV2_REG;

		if (SUCCESS != Fpga_Dps_Transfer(t_data, r_data, TRANSFER_LENGTH, bid)) {
			debug("Fpga Dps Transfer Error In %s Files,%d Line\n",__FILE__,__LINE__);
			return SET_FAILED;
		}
	}
	else {
		debug("Dps Channel Error In %s Files,%d Line\n",__FILE__,__LINE__);
		return SET_FAILED;
	}

	return SET_SUCCESS;
}


/*
  ** Function Name        : Set_All_Dps_L2_Enable
  ** Function Description : Through spi interface operation fpga all dps L2 enable
  ** 						en                  : Enable Flag
  ** 						bid                 : Board id
  ** 											  DPS Board 0
  ** 											  DPS Board 1
  ** Function Return      : SET_SUCCESS         : Set success
  ** 						SET_FAILED          : Set Failed
  ** Notes				  : None
*/
u8 Set_All_Dps_L2_Enable(All_Enable_Status en, u8 bid)
{
	u16 t_data = 0x00;

	t_data = en;

	if (CONFIG_SUCCESS != Config_Dps_Fpga_Reg(ENABLE_LV2_REG, t_data, bid)) {
		return SET_FAILED;
	}
	else {

	} 

	return SET_SUCCESS;
}


/*
  ** Function Name        : Set_Dps_Channel_L2_Enable
  ** Function Description : Through spi interface operation fpga dps L2 enable
  ** Function Parameter   : ch   			    : Send data point
  ** 						en                  : Enable Flag
  ** 						bid                 : Board id
  ** 											  DPS Board 0
  ** 											  DPS Board 1
  ** Function Return      : SET_SUCCESS         : Set success
  ** 						SET_FAILED          : Set Failed
  ** Notes				  : None
*/
u8 Set_Dps_Channel_L3_Enable(Dps_channel ch, Enable_status en, u8 bid)
{
	u8 t_data[TRANSFER_LENGTH] = {0x00};
	u8 r_data[TRANSFER_LENGTH] = {0x00};

	if ((ch >= DPSA_CHANNEL_BD0) && (ch <= DPSF_CHANNEL_BD0)) {
		t_data[COMMAND_DATA] = (READ_BITS << READ_WRITE_BIT);
		t_data[COMMAND_DATA] |= ENABLE_LV3_REG;

		if (SUCCESS != Fpga_Dps_Transfer(t_data, r_data, TRANSFER_LENGTH, bid)) {
			debug("Fpga Dps Transfer Error In %s Files,%d Line\n",__FILE__,__LINE__);
			return SET_FAILED;
		}

		t_data[FIRST_DATA] = r_data[FIRST_DATA];
		t_data[SECOND_DATA] = r_data[SECOND_DATA];
		if (en) {
			t_data[SECOND_DATA] |= 1 << ch;
		}
		else {
			t_data[SECOND_DATA] &= ~(1 << ch);
		}
		t_data[COMMAND_DATA] = (WRITE_BITS << 7);
		t_data[COMMAND_DATA] |= ENABLE_LV3_REG;

		if (SUCCESS != Fpga_Dps_Transfer(t_data, r_data, TRANSFER_LENGTH, bid)) {
			debug("Fpga Dps Transfer Error In %s Files,%d Line\n",__FILE__,__LINE__);
			return SET_FAILED;
		}
	}
	else {
		debug("Dps Channel Error In %s Files,%d Line\n",__FILE__,__LINE__);
		return SET_FAILED;
	}

	return SET_SUCCESS;
}


/*
  ** Function Name        : Set_All_Dps_L3_Enable
  ** Function Description : Through spi interface operation fpga all dps L3 enable
  ** 						en                  : Enable Flag
  ** 						bid                 : Board id
  ** 											  DPS Board 0
  ** 											  DPS Board 1
  ** Function Return      : SET_SUCCESS         : Set success
  ** 						SET_FAILED          : Set Failed
  ** Notes				  : None
*/
u8 Set_All_Dps_L3_Enable(All_Enable_Status en, u8 bid)
{
	u16 t_data = 0x00;

	t_data = en;

	if (CONFIG_SUCCESS != Config_Dps_Fpga_Reg(ENABLE_LV3_REG, t_data, bid)) {
		return SET_FAILED;
	}
	else {

	} 

	return SET_SUCCESS;
}


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
