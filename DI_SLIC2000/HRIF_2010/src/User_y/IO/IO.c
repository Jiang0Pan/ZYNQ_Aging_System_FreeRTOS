



#include "IO.h"





/***********************************************************************
	[��	       ��	]PCAоƬIO��������
	[��            ��	]dir0:��0����, dir1:��1����, SlaveAddr:�ӻ���ַ, Chan:ͨ����
	[��    ��    ֵ	]�Ƿ�ɹ�
***********************************************************************/
s32 IO_Dir_Config(GroupXDir dir0, GroupXDir dir1, u8 SlaveAddr, IIC_Chan Chan)
{
	u8 RegVal[3] = {PORT0CFG, 0x00, 0x00};
	if(dir0 == INPUT)
		RegVal[1] = 0Xff;
	if(dir1 == INPUT)
		RegVal[2] = 0Xff;

	if(IIC_Write(RegVal, 3, SlaveAddr, Chan))
		return FAILURE;

	return SUCCESS;
}

s32 IO_Group_Set(_Output_ * IO, GroupX Gpx, u8 Mask, u8 SlaveAddr, IIC_Chan Chan)
{
	IO->IO_Data[0] = PORT0WRITE;
	IO->IO_Data[Gpx+1] = Mask;
	if(IIC_Write(IO->IO_Data, 3, SlaveAddr, Chan))
		return FAILURE;

	return SUCCESS;
}

/***********************************************************************
	[��	       ��	]����PCA��IO��ƽ
	[��            ��	]IO:IO������Ϣ, Gpx:���, pin:���ű��, Level:��ƽ, SlaveAddr:�ӻ���ַ, Chan:ͨ����
	[��    ��    ֵ	]�Ƿ�ɹ�
***********************************************************************/
s32 IO_Level_Set(_Output_ * IO, GroupX Gpx, PIN_Num pin, u8 Level, u8 SlaveAddr, IIC_Chan Chan)
{
	switch(Gpx)
	{
		case Group0:
			if(Level)
				IO->IO_Data[1] |= 0x1 << pin;
			else
				IO->IO_Data[1] &= ~(0x1 << pin);
			break;

		case Group1:
			if(Level)
				IO->IO_Data[2] |= 0x1 << pin;
			else
				IO->IO_Data[2] &= ~(0x1 << pin);
			break;
	}

	IO->IO_Data[0] = PORT0WRITE;
	if(IIC_Write(IO->IO_Data, 3, SlaveAddr, Chan))
		return FAILURE;

	return SUCCESS;
}

//����IO�������
s32 ALL_IO_Level_Set(_Output_ * IO, u8 pin, u8 Level, u8 SlaveAddr, IIC_Chan Chan)
{
	if(pin <= 7)
	{
		if(Level)
			IO->IO_Data[1] |= 0x1 << pin;
		else
			IO->IO_Data[1] &= ~(0x1 << pin);
	}
	else if(pin > 7 && pin < 16)
	{
		pin = pin - 8;
		if(Level)
			IO->IO_Data[2] |= 0x1 << pin;
		else
			IO->IO_Data[2] &= ~(0x1 << pin);
	}
	else
		return FAILURE;
	IO->IO_Data[0] = PORT0WRITE;
	if(IIC_Write(IO->IO_Data, 3, SlaveAddr, Chan))
		return FAILURE;

	return SUCCESS;
}


/***********************************************************************
	[��	       ��	]��ȡPCA��IO��ƽ
	[��            ��	]Gpx:���, pin:���ű��, Level:��ƽ, SlaveAddr:�ӻ���ַ, Chan:ͨ����
	[��    ��    ֵ	]�Ƿ�ɹ�
***********************************************************************/
s32 IO_Level_Get(GroupX Gpx, PIN_Num pin, u8 *Level, u8 SlaveAddr, IIC_Chan Chan)
{
	u8 Data[2] = {0x0};
	u8 code = PORT0READ;

	if(IIC_Write(&code, 1, SlaveAddr, Chan))
		return FAILURE;

	if(IIC_Read(Data, 2, SlaveAddr, Chan))
		return FAILURE;

	switch(Gpx)
	{
		case Group0:
			if(Data[0] >> pin)
				*Level = 1;
			else
				*Level = 0;
			break;

		case Group1:
			if(Data[1] >> pin)
				*Level = 1;
			else
				*Level = 0;
			break;
	}
	return SUCCESS;
}

/***********************************************************************
	[��	       ��	]��ȡPCA��һ��IO��ƽ
	[��            ��	]Gpx:���, Level:��ƽ, SlaveAddr:�ӻ���ַ, Chan:ͨ����
	[��    ��    ֵ	]�Ƿ�ɹ�
***********************************************************************/
s32 IO_Group_Get(GroupX Gpx, u8 *Level, u8 SlaveAddr, IIC_Chan Chan)
{
	u8 Data[2] = {0x0};
	u8 code = PORT0READ;

	if(IIC_Write(&code, 1, SlaveAddr, Chan))
		return FAILURE;

	if(IIC_Read(Data, 2, SlaveAddr, Chan))
		return FAILURE;

	switch(Gpx)
	{
		case Group0:
			*Level = Data[0];
			break;

		case Group1:
			*Level = Data[1];
			break;
	}
	return SUCCESS;
}

/***********************************************************************
	[��	       ��	]��ʼ��PCA IO
	[��            ��	]IO:IO������Ϣ, SlaveAddr:�ӻ���ַ, Chan:ͨ����
	[��    ��    ֵ	]�Ƿ�ɹ�
***********************************************************************/
s32 PCA_IO_Init(GroupXDir dir0, GroupXDir dir1, _Output_ *IO, u8 SlaveAddr, IIC_Chan Chan)
{
	if(IO_Dir_Config(dir0, dir1, SlaveAddr, Chan))
		return FAILURE;

	if(IIC_Write(IO->IO_Data, 3, SlaveAddr, Chan))
		return FAILURE;

	return SUCCESS;
}

s32 fpgaSpi_Open(UpdateCS cs)
{
	s32 Status = FAILURE;
	//�л�spi��fpga update
	Status = IO_Level_Set(&(bbd_info.Ctl_Output), Group0, pin1, 1, bbd_info.IO_SlaveAddr, bbd_info.TOChan);
	if(FAILURE == Status)
		return FAILURE;

	//�ر�����fpga spi
	Status = IO_Level_Set(&(fpga_obj.Driver[FPGA_0].Msg.Output_LVL), Group1, pin2, 1, fpga_obj.PCA953_SlaveAddr, fpga_obj.Driver[FPGA_0].Msg.HWChan);
	if(FAILURE == Status)
		return FAILURE;

	Status = IO_Level_Set(&(fpga_obj.Driver[FPGA_1].Msg.Output_LVL), Group1, pin2, 1, fpga_obj.PCA953_SlaveAddr, fpga_obj.Driver[FPGA_1].Msg.HWChan);
	if(FAILURE == Status)
		return FAILURE;

	Status = IO_Level_Set(&(fpga_obj.Driver[FPGA_2].Msg.Output_LVL), Group1, pin2, 1, fpga_obj.PCA953_SlaveAddr, fpga_obj.Driver[FPGA_2].Msg.HWChan);
	if(FAILURE == Status)
		return FAILURE;

	Status = IO_Level_Set(&(fpga_obj.Driver[FPGA_3].Msg.Output_LVL), Group1, pin2, 1, fpga_obj.PCA953_SlaveAddr, fpga_obj.Driver[FPGA_3].Msg.HWChan);
	if(FAILURE == Status)
		return FAILURE;

	Status = IO_Level_Set(&(fpga_obj.Driver[FPGA_4].Msg.Output_LVL), Group1, pin2, 1, fpga_obj.PCA953_SlaveAddr, fpga_obj.Driver[FPGA_4].Msg.HWChan);
	if(FAILURE == Status)
		return FAILURE;

	//��Ƭѡ
	Status = IO_Level_Set(&(fpga_obj.Driver[cs].Msg.Output_LVL), Group1, pin2, 0, fpga_obj.PCA953_SlaveAddr, fpga_obj.Driver[cs].Msg.HWChan);
	if(FAILURE == Status)
		return FAILURE;

	return SUCCESS;
}

s32 fpgaSpi_Close(UpdateCS cs)
{
	s32 Status = FAILURE;

	//�л�spi��fpga update
	Status = IO_Level_Set(&(bbd_info.Ctl_Output), Group0, pin1, 1, bbd_info.IO_SlaveAddr, bbd_info.TOChan);
	if(FAILURE == Status)
		return FAILURE;

	Status = IO_Level_Set(&(fpga_obj.Driver[FPGA_0].Msg.Output_LVL), Group1, pin2, 1, fpga_obj.PCA953_SlaveAddr, fpga_obj.Driver[FPGA_0].Msg.HWChan);
	if(FAILURE == Status)
		return FAILURE;

	Status = IO_Level_Set(&(fpga_obj.Driver[FPGA_1].Msg.Output_LVL), Group1, pin2, 1, fpga_obj.PCA953_SlaveAddr, fpga_obj.Driver[FPGA_1].Msg.HWChan);
	if(FAILURE == Status)
		return FAILURE;

	Status = IO_Level_Set(&(fpga_obj.Driver[FPGA_2].Msg.Output_LVL), Group1, pin2, 1, fpga_obj.PCA953_SlaveAddr, fpga_obj.Driver[FPGA_2].Msg.HWChan);
	if(FAILURE == Status)
		return FAILURE;

	Status = IO_Level_Set(&(fpga_obj.Driver[FPGA_3].Msg.Output_LVL), Group1, pin2, 1, fpga_obj.PCA953_SlaveAddr, fpga_obj.Driver[FPGA_3].Msg.HWChan);
	if(FAILURE == Status)
		return FAILURE;

	Status = IO_Level_Set(&(fpga_obj.Driver[FPGA_4].Msg.Output_LVL), Group1, pin2, 1, fpga_obj.PCA953_SlaveAddr, fpga_obj.Driver[FPGA_4].Msg.HWChan);
	if(FAILURE == Status)
		return FAILURE;

	//�ر�Ƭѡ
	Status = IO_Level_Set(&(fpga_obj.Driver[cs].Msg.Output_LVL), Group1, pin2, 1, fpga_obj.PCA953_SlaveAddr, fpga_obj.Driver[cs].Msg.HWChan);
	if(FAILURE == Status)
		return FAILURE;

	return SUCCESS;
}

s32 DpsSpi_Open(u8 bid, DpsCS cs)
{
	s32 Status = FAILURE;
	//�л�spi��fpga update
	Status = IO_Level_Set(&(bbd_info.Ctl_Output), Group0, pin1, 0, bbd_info.IO_SlaveAddr, bbd_info.TOChan);
	if(FAILURE == Status)
		return FAILURE;

	Status = IO_Level_Set(&(bbd_info.Ctl_Output), Group0, pin0, 0, bbd_info.IO_SlaveAddr, bbd_info.TOChan);
	if(FAILURE == Status)
		return FAILURE;

	//��spi
	Status = IO_Level_Set(&(dps_info.DBMsg[bid].Output_LVL), Group1, pin2,  0, dps_info.PCA953_SlaveAddr, dps_info.DBMsg[bid].Chan);
	if(FAILURE == Status)
		return FAILURE;

	Status = IO_Level_Set(&(dps_info.DBMsg[bid].Output_LVL), Group1, pin1, cs, dps_info.PCA953_SlaveAddr, dps_info.DBMsg[bid].Chan);
	if(FAILURE == Status)
		return FAILURE;

	return SUCCESS;
}

s32 DpsSpi_Close(u8 bid)
{
	s32 Status = FAILURE;
	//�л�spi��fpga update
	Status = IO_Level_Set(&(bbd_info.Ctl_Output), Group0, pin1, 1, bbd_info.IO_SlaveAddr, bbd_info.TOChan);
	if(FAILURE == Status)
		return FAILURE;

	Status = IO_Level_Set(&(bbd_info.Ctl_Output), Group0, pin0, 1, bbd_info.IO_SlaveAddr, bbd_info.TOChan);
	if(FAILURE == Status)
		return FAILURE;

	//��spi
	Status = IO_Level_Set(&(dps_info.DBMsg[bid].Output_LVL), Group1, pin2,  1, dps_info.PCA953_SlaveAddr, dps_info.DBMsg[bid].Chan);
	if(FAILURE == Status)
		return FAILURE;

	Status = IO_Level_Set(&(dps_info.DBMsg[bid].Output_LVL), Group1, pin1,  1, dps_info.PCA953_SlaveAddr, dps_info.DBMsg[bid].Chan);
	if(FAILURE == Status)
		return FAILURE;

	return SUCCESS;
}





