/*
  ** @file           : isl28023.c
  ** @brief          : ISL28023 Information Source File
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


/*
  ** Include
*/
#include "xil_types.h"
#include "sleep.h"
#include "return_val.h"
#include "debug_config.h"
#include "IIC.h"
#include "SMBus.h"
#include "GlobalRef.h"
#include "dps_info.h"
#include "isl28023.h"


/*
  ** Define
*/
/* Send data coefficient */
#define     SD_COE                           (1000) 
/* DPS Channel1 Sampling resistance value */
#define     SAMPLE_CH1_R                     (0.001f)
/* DPS Channel2、 Sampling resistance value */
#define     SAMPLE_CH2_R                     (0.002f)
/* DPS Channel3-4 Sampling resistance value */
#define     SAMPLE_CH3_R                     (0.004f)
/* DPS Board number */
#define     BOARD_NUM                        (2)
/* One DPS Board number */
#define     CHANNEL_NUM                      (8)


/*
  ** Function Name        : Build_Dps_Info
  ** Function Description : Build Dps Information
  ** Function Parameter   : DpsVI   			: DPS input voltage                 
  ** 						DpsVO    		    : DPS output voltage 
  **                        DpsIO               : DPS output current 
  **                        DpsTemp 		    : DPS temperature
  **                        DpsStat             : DPS status
  ** Function Return	  :	Void
  ** Notes				  : None
*/
u8 Dpm_Init(u8 Bid)
{
	u8 status;
	u8 count;

	for (count = CHANNEL_NUM; count < CHANNEL_NUM; count++) {
		status = Dpm_Config(&dps_info.DBMsg[Bid].Dpm[count] ,dps_info.DBMsg[Bid].Chan);
		if (SUCCESS != status)  {
			debug("Dpm Channel %d Board %d init failed In %s Files,%d Line\n", count, Bid, __FILE__, __LINE__);
			return INIT_FAILED;	
		}
	}

	return INIT_SUCCESS;
}


/*
  ** Function Name        : Build_Dps_Info
  ** Function Description : Build Dps Information
  ** Function Parameter   : DpsVI   			: DPS input voltage                 
  ** 						DpsVO    		    : DPS output voltage 
  **                        DpsIO               : DPS output current 
  **                        DpsTemp 		    : DPS temperature
  **                        DpsStat             : DPS status
  ** Function Return	  :	Void
  ** Notes				  : None
*/
void Build_Dps_Info(short DpsVI[], short DpsVO[], short DpsIO[], short DpsTemp[], u16 DpsStat[])
{
	/* Ready data */
	u16 id = 0, i = 0, j = 0;
	float vo = 0.0 , io = 0.0;
	/* Sample data address */
	Sample_Data   * Sample    = (Sample_Data   *)SAMPLE_DATA_ADD;

	/* Get current and voltage */
	for (i = 0; i < (BOARD_NUM -1) ; i++) {
		for (j = 0; j < CHANNEL_NUM; j++, id++) {
			/* Get Sampling resistance voltage value */
			vo = Dpm_VBus(&dps_info.DBMsg[i].Dpm[j], dps_info.DBMsg[i].Chan);
			/* Get Sampling voltage value */			
			io = Dpm_VShunt(&dps_info.DBMsg[i].Dpm[j], dps_info.DBMsg[i].Chan);

            /* Send data coefficient */
			vo = vo * SD_COE;
			io = io * SD_COE;

			/* Calculate the current value */
			switch (j) {
				case 0: {
					io = io  / SAMPLE_CH1_R;
				}
					break;
				case 1: {
					io = io / SAMPLE_CH2_R;
				}
					break;
				case 2 ... 3: {
					io = io / SAMPLE_CH3_R;				
				}	
					break;
				case 4 ... 7 : {
					io = io / SAMPLE_CH2_R;				
				}		
					break;
				default:
					break;
			}
			
			if (0 == i) {
				DpsVO[j] = (s16)vo;
				DpsIO[j] = (s16)io;
			}
			else if (1 == i) {
				DpsVO[j + CHANNEL_NUM] = (s16)vo;
				DpsIO[j + CHANNEL_NUM] = (s16)io;
			}
			else {
				
			}
		}
	}

	/* Read Error information */
	if (READ_SUCCESS != Read_Error_Info(Sample, DPS_BD0)) {

	}
}


/*
  ** Function Name        : Dpm_Gain_Count
  ** Function Description : Get Dpm Gain Value
  ** Function Parameter   : Void
  ** Function Return	  :	u16                 : Dpm Gain Value
  ** Notes				  : None
*/
u16 Dpm_Gain_Count(void)
{
	return ((u16)((DPM_SCALE / DPM_VSHUNT_FS) * DPM_ADC_RES));
}


/*
  ** Function Name        : Dpm_Reg_Val
  ** Function Description : Build Dps Information
  ** Function Parameter   : Regval   			: DPS input voltage                 
  ** 						EQ_N    		    : DPS output voltage 
  ** Function Return	  :	Void
  ** Notes				  : None
*/
int Dpm_Reg_Val(u16 Regval, u8 EQ_N)
{
	int val = 0;
	u8 bitn = 0;

	for (bitn = 0; bitn <= 14; bitn++) {
		val +=  ((Regval >> bitn) & 0x1) * (0x1 << bitn);
	}
	
	if (EQ_N > 1) {
		val -= ((Regval >> 15) & 0x1) * (0x1 << 15);
	}
	else {
		val += ((Regval >> 15) & 0x1) * (0x1 << 15);
	}

	return val;
}


/*
  ** Function Name        : Dpm_VBus
  ** Function Description : Collection Vbus voltage , Easy to calculate current value
  ** Function Parameter   : dpm      			: Dpm object               
  ** 						Chan    		    : IIC expand channel  
  ** Function Return	  :	Void
  ** Notes				  : None
*/
float Dpm_VBus(Dpm_Info *dpm, IIC_Chan Chan)
{
	/* Ready data */
	u16 Regval = 0x0;
	u8 buf[2] = {0x0, 0x0};
	s32 Status = FAILURE;
	float vbus = 0.0;

    /* Read Register */
	Status = SMBus_Read(buf, 2, dpm->DpmAddr, DPM_READ_VOUT, Chan);
	if(Status == FAILURE) {
		return 0.0;
	}

	/* Cover data */
	Regval = buf[0];
	Regval = (Regval << 8);
	Regval |= buf[1];

	/* Determine different resolutions based on version information */
	// if (dpm->ChipVer) {
	// 	vbus = Regval * (DPM_VBUSLSB_60);
	// }
	// else {
	// 	vbus = Regval * (DPM_VBUSLSB_12);
	// }

	vbus = Regval * (DPM_VBUSLSB_12);

	/* Return float data */
	return vbus;
}


/*
  ** Function Name        : Dpm_VShunt
  ** Function Description : Collection VShunt voltage , Equal to the power supply voltage
  ** Function Parameter   : dpm      			: Dpm object               
  ** 						Chan    		    : IIC expand channel  
  ** Function Return	  :	Void
  ** Notes				  : None
*/
float Dpm_VShunt(Dpm_Info *dpm, IIC_Chan Chan)
{
	/* Ready data */
	s16 Regval = 0x0;
	u8 buf[2] = {0x0, 0x0};
	s32 Status = FAILURE;
	float vshunt = 0.0;

    /* Read Register */
	Status = SMBus_Read(buf, 2, dpm->DpmAddr, DPM_READ_VSHUNT_OUT, Chan);
	if(Status == FAILURE) {
		return 0.0;
	}

	/* Cover data */
	Regval = buf[0];
	Regval = (Regval << 8);
	Regval |= buf[1];

	vshunt = Regval * DPM_VSHUNTLSB;

	/* Return float data */
	return vshunt;
}


/*
  ** Function Name        : Dpm_IOUT
  ** Function Description : Collection output current
  ** Function Parameter   : Dpm_Info   			: object
  ** 						Chan    		    : IIC expand channel
  ** Function Return	  :	Void
  ** Notes				  : None
*/
float Dpm_IOUT(Dpm_Info *dpm, IIC_Chan Chan)
{
	/* Ready data */
	u16 Regval = 0x0;
	u8 buf[2] = {0x0, 0x0};
	s32 Status = FAILURE;
	float iout = 0.0;

    /* Read Register */
	Status = SMBus_Read(buf, 2, dpm->DpmAddr, DPM_READ_IOUT, Chan);
	if(Status == FAILURE) {
		return 0.0;
	}

	/* Cover data */
	Regval |= buf[0];
	Regval = (Regval << 8) | buf[1];

	int val = Dpm_Reg_Val(Regval, 2);

	/* Return float data */
	iout = val * DPM_CURRENTLSB;

	return iout;
}


/*
  ** Function Name        : Dpm_POUT
  ** Function Description : Collection output power
  ** Function Parameter   : Dpm_Info   			: object
  ** 						Chan    		    : IIC expand channel
  ** Function Return	  :	Void
  ** Notes				  : None
*/
float Dpm_POUT(Dpm_Info *dpm, IIC_Chan Chan)
{
	/* Ready data */
	u16 Regval = 0x0;
	u8 buf[2] = {0x0, 0x0};
	s32 Status = FAILURE;
	float pout = 0.0;

    /* Read Register */
	Status = SMBus_Read(buf, 2, dpm->DpmAddr, DPM_READ_POUT, Chan);
	if (Status == FAILURE) {
		return 0.0;
	}

	/* Cover data */
	Regval |= buf[0];
	Regval = (Regval << 8) | buf[1];

	int val = Dpm_Reg_Val(Regval, 2);

	if (dpm->ChipVer > 0) {
		pout = val * (DPM_VBUSLSB_12 * DPM_CURRENTLSB) * 40000;
	}
	else {
		pout = val * (DPM_VBUSLSB_60 * DPM_CURRENTLSB) * 40000;
	}

	/* Return float data */
	return pout;
}


/*
  ** Function Name        : Dpm_TEMP
  ** Function Description : Collection temperature
  ** Function Parameter   : Dpm_Info   			: object
  ** 						Chan    		    : IIC expand channel
  ** Function Return	  :	Void
  ** Notes				  : None
*/
float Dpm_TEMP(Dpm_Info *dpm, IIC_Chan Chan)
{
	/* Ready data */
	u16 Regval = 0x0;
	u8 buf[2] = {0x0, 0x0};
	s32 Status = FAILURE;
	float temp = 0.0;

    /* Read Register */
	Status = SMBus_Read(buf, 2, dpm->DpmAddr, DPM_READ_TEMPERATURE_1, Chan);
	if (Status == FAILURE) {
		return 0.0;
	}

	/* Cover data */
	Regval |= buf[0];
	Regval = (Regval << 8) | buf[1];

	int val = Dpm_Reg_Val(Regval, 2);

	temp = val * 0.016;

	/* Return float data */
	return temp;
}


/*
  ** Function Name        : Dpm_Max_IOUT
  ** Function Description : Collection output max current
  ** Function Parameter   : Dpm_Info   			: object
  ** 						Chan    		    : IIC expand channel
  ** Function Return	  :	Void
  ** Notes				  : None
*/
float Dpm_Max_IOUT(Dpm_Info *dpm, IIC_Chan Chan)
{
	/* Ready data */
	u16 Regval = 0x0;
	u8 buf[2] = {0x0, 0x0};
	s32 Status = FAILURE;
	float maxiout = 0.0;

    /* Read Register */
	Status = SMBus_Read(buf, 2, dpm->DpmAddr, DPM_READ_PEAK_MAX_IOUT, Chan);
	if (Status == FAILURE) {
		return 0.0;
	}

	/* Cover data */
	Regval |= buf[0];
	Regval = (Regval << 8) | buf[1];

	/* Return float data */
	return maxiout;
}


/*
  ** Function Name        : Dpm_Max_IOUT
  ** Function Description : Collection output min current
  ** Function Parameter   : Dpm_Info   			: object
  ** 						Chan    		    : IIC expand channel
  ** Function Return	  :	Void
  ** Notes				  : None
*/
float Dpm_Min_IOUT(Dpm_Info *dpm, IIC_Chan Chan)
{
	/* Ready data */
	u16 Regval = 0x0;
	u8 buf[2] = {0x0, 0x0};
	s32 Status = FAILURE;
	float miniout = 0.0;

    /* Read Register */
	Status = SMBus_Read(buf, 2, dpm->DpmAddr, DPM_READ_PEAK_MIN_IOUT, Chan);
	if(Status == FAILURE)
		return -1;

	/* Cover data */
	Regval |= buf[0];
	Regval = (Regval << 8) | buf[1];

	/* Return float data */
	return miniout;
}


/*
  ** Function Name        : Config_Isl28023_Reg
  ** Function Description : Config isl28023 register
  ** Function Parameter   : Slave_addr   		: IIC slave device address
  ** 						Chan    		    : IIC expand channel
  ** Function Return	  :	Void
  ** Notes				  : None
*/
u8 Config_Isl28023_Reg(u8 Reg, u16 S_buf, u16 S_byte,u8 Slave_addr,IIC_Chan Chan)
{
	u8 status;
	u16 t_data;
	u16 r_data;

	u8 t_buf[2] = {0x00};
	u8 r_buf[2] = {0x00};

	t_buf[0] = S_buf & 0xFF;
	t_buf[1] = (S_buf >> 8) & 0xFF;	
	t_data = S_buf;

	do {
		usleep(1000);

		status = SMBus_Write(t_buf, S_byte, Slave_addr, Reg, Chan);
		if (FAILURE == status) {
			return FAILURE;
		}

		usleep(1000);

		status = SMBus_Read(r_buf, S_byte, Slave_addr, Reg, Chan);
		if (FAILURE == status) {
			return FAILURE;
		}

		r_data = r_buf[0];
		r_data = r_data << 8;
		r_data |= r_buf[1];

	} while(t_data != r_data);

	return CONFIG_SUCCESS;

}

/*
  ** Function Name        : Dpm_Init
  ** Function Description : Dpm Initialization
  ** Function Parameter   : Dpm_Info   			: object
  ** 						Chan    		    : IIC expand channel
  ** Function Return	  :	Void
  ** Notes				  : None
*/
s32 Dpm_Config(Dpm_Info *dpm, IIC_Chan Chan)
{
	s32 Status  = FAILURE;
	u8 value[4]  = {0x0, 0x0, 0x0, 0x0};

	/* Reset Default */
	Status = SMBus_Code(dpm->DpmAddr, DPM_RESTORE_DEFAULT_LL, Chan);
	if (Status == FAILURE) {
		return FAILURE;
	}
	/* Wait for delay */
	usleep(100000);

	/* Read isl28023 version */
	Status = SMBus_Read(value, 4, dpm->DpmAddr, DPM_IC_DEVICE_REV, Chan);
	if (Status == FAILURE) {
		return FAILURE;
	}
	dpm->ChipVer = value[2] & 0x08;
	/* Wait for delay */
	usleep(1000);

	/* Set DPM Mode */
	value[0] = 0x4F;
	Status = SMBus_Write(value, 1, dpm->DpmAddr, DPM_SET_DPM_MODE, Chan);
	if (Status == FAILURE) {
		return FAILURE;
	}
	/* Wait for delay */
	usleep(1000);

	/* Set DAC output voltage threshold */
	value[0] = 0x00;
	value[1] = 0x40;
	Status = SMBus_Write(value, 2, dpm->DpmAddr, DPM_VOUT_OV_THRESHOLD_SET, Chan);
	if (Status == FAILURE) {
		return FAILURE;
    }
	/* Wait for delay */
	usleep(1000);

	/* Set DAC current threshold */
	value[0] = 0x0;
	value[1] = 0x7F;
	Status = SMBus_Write(value, 2, dpm->DpmAddr, DPM_IOUT_OC_THRESHOLD_SET, Chan);
	if (Status == FAILURE) {
		return FAILURE;
	}
	/* Wait for delay */
	usleep(1000);

	/* 
	  ** Aux Shunt Sample AVG:16 , not used
	  ** Aux Shunt Conversion time:2.048ms , not used
	  ** Prim Shunt Sample AVG:1
	  ** Prim Shunt Conversion time:2.048ms 
	*/
	value[0] = 0x13;
	value[1] = 0x8E;
	Status = SMBus_Write(value, 2, dpm->DpmAddr, DPM_CONFIG_ICHANNEL, Chan);
	if (Status == FAILURE) {
		return FAILURE;
	}
	/* Wait for delay */
	usleep(1000);

	/* 
	  ** AuxV Sample AVG:32 , not used
	  ** AuxV Conversion time:2.048ms , not used
	  ** Prim Shunt Sample AVG:32
	  ** Vbus Conversion time:2.048ms 
	*/
	value[0] = 0x17;
	value[1] = 0xAF;
	Status = SMBus_Write(value, 2, dpm->DpmAddr, DPM_CONFIG_VCHANNEL, Chan);
	if (Status == FAILURE) {
		return FAILURE;
	}
	/* Wait for delay */
	usleep(1000);

	/* Set adc operation mode (all mode) */
	value[0] = 0xF;
	Status = SMBus_Write(value, 1, dpm->DpmAddr, DPM_SET_DPM_MODE, Chan);
	if (Status == FAILURE) {
		return FAILURE;
	}
	/* Wait for delay */
	usleep(1000);

	/* Set ADC (DAC) output voltage */
	value[0] = 0x3B;
	Status = SMBus_Write(value, 1, dpm->DpmAddr, DPM_CONFIG_VOL_MARGIN, Chan);
	if (Status == FAILURE) {
		return FAILURE;
	}
	/* Wait for delay */
	usleep(1000);

	return SUCCESS;
}


/*
  ** Function Name        : Set_dac_out
  ** Function Description : Config Isl8272(ISL8277) Register 
  ** Function Parameter   : voltage   			: Isl8272 register address                   
  ** 						Reg_data 		    : The point to write data pointer
  **                        Data_Len 		    : Write data length
  **                        Slaveaddr           : IIC slave address
  **                        Chan                : Expand iic channel
  ** Function Return	  :	WRITE_SUCEESS       : Write Isl8272 register data success
  **                      : WRITE_FAILED        : Write Isl8272 register data failed
  ** Notes				  : None
*/
u8 Set_dac_out(float voltage, u8 SlaveAddr, IIC_Chan Chan)
{
	/* Ready data */
	u8 value[1]  = {0x0};
	u8 rdata = 0x00;
	s32 Status  = FAILURE;

	value[0] = 0x3B;
	do {
		Status = SMBus_Write(value, 1, SlaveAddr, DPM_CONFIG_VOL_MARGIN, Chan);
		if (Status == FAILURE) {
			return FAILURE;
		}
		
		Status = SMBus_Read(&rdata, 1, SlaveAddr, DPM_CONFIG_VOL_MARGIN, Chan);
		if (Status == FAILURE) {
			return FAILURE;
		}
	} while (value[0] != rdata);

	if ((voltage >= 0) && (voltage <= 2.4)) {
		value[0] = (u8)((voltage / 2.4) * 0xFF);
	}
	do {
		Status = SMBus_Write(value, 1, SlaveAddr, DPM_SET_VOL_MARGIN, Chan);
		if (Status == FAILURE) {
			return FAILURE;
		}

		Status = SMBus_Read(&rdata, 1, SlaveAddr, DPM_SET_VOL_MARGIN, Chan);
		if (Status == FAILURE) {
			return FAILURE;
		}

	} while (value[0] != rdata);

	value[0] = 0x3F;
	do {
		Status = SMBus_Write(value, 1, SlaveAddr, DPM_CONFIG_VOL_MARGIN, Chan);
		if (Status == FAILURE) {
			return FAILURE;
		}

		Status = SMBus_Read(&rdata, 1, SlaveAddr, DPM_CONFIG_VOL_MARGIN, Chan);
		if (Status == FAILURE) {
			return FAILURE;
		}

	} while (value[0] != rdata);
	
	value[0] = 0x3B;
	do {
		Status = SMBus_Write(value, 1, SlaveAddr, DPM_CONFIG_VOL_MARGIN, Chan);
		if (Status == FAILURE) {
			return FAILURE;
		}

		Status = SMBus_Read(&rdata, 1, SlaveAddr, DPM_CONFIG_VOL_MARGIN, Chan);
		if (Status == FAILURE) {
			return FAILURE;
		}
	} while (value[0] != rdata);

	return SUCCESS;
}


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
