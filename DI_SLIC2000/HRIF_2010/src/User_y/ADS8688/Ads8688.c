#include "ads8688.h"
#include "error.h"
#include "malloc.h"





/***********************************************************************
	[��	       ��	]д�Ĵ�������Result����д������
	[��            ��	]RegAddr:�Ĵ�����ַ, RegVal:д��ֵ, Result:����ֵָ��
	[��    ��    ֵ	]�������
***********************************************************************/
s32 Spi_Write_Reg(u8 RegAddr, u8 RegVal, u8 *Result)
{
	u8 TXbuf[4] = {0x0};
	u8 RXbuf[4] = {0x0};

	TXbuf[0] = (RegAddr << 1) | 0x1;
	TXbuf[1] = RegVal;

	if(Spi_Transfer(TXbuf, RXbuf, 4, SPI1))
	    return FAILURE;

	*Result = RXbuf[2];

	return SUCCESS;
}

/***********************************************************************
	[��	       ��	]���Ĵ�������Result����д������
	[��            ��	]RegAddr:�Ĵ�����ַ, Result:����ֵָ��
	[��    ��    ֵ	]�������
***********************************************************************/
s32 Spi_Read_Reg(u8 RegAddr, u8 *Result)
{
	u8 TXbuf[4] = {0x0};
	u8 RXbuf[4] = {0x0};

	TXbuf[0] = RegAddr << 1;

	if(Spi_Transfer(TXbuf, RXbuf, 4, SPI1))
		return FAILURE;

	*Result = RXbuf[2];

	return SUCCESS;
}

/***********************************************************************
	[��	       ��	]д����(16λ)����Result����д������
	[��            ��	]CMD:����, Result:����ֵָ��
	[��    ��    ֵ	]�������
***********************************************************************/
s32 Spi_Write_CMD(u16 CMD, u16 *Result)
{
	u8 TXbuf[4] = {0x0};
	u8 RXbuf[4] = {0x0};

	TXbuf[0] = (CMD >> 8) & 0xff;
	TXbuf[1] = (CMD >> 0) & 0xff;

	if(Spi_Transfer(TXbuf, RXbuf, 4, SPI1))
	    return FAILURE;

	*Result = RXbuf[2];
	*Result = ((*Result) << 8) | RXbuf[3];

	return SUCCESS;
}

/***********************************************************************
	[��	       ��	]��ȡADCֵ(8·),AUXͨ��
	[��            ��	]Buff:����ָ��
	[��    ��    ֵ	]�������
***********************************************************************/
s32 Spi_ReadADC(u16 *Buff)
{
	u8 chan = 0;

	for(chan = 0; chan < ADC_PATHNUM; chan++)					//��ȡ8ͨ������
	{
		if(Spi_Write_CMD(Man_Select_CMD(chan), (Buff+chan)))
			return (chan + 1);									//���ش���ͨ����
	}

	return SUCCESS;
}

/***********************************************************************
	[��	       ��	]Ӳ����λ ADS8688
	[��            ��	]��
	[��    ��    ֵ	]��
***********************************************************************/
void ADS_Reset(void)
{
	u8 Timeout = 5;
	Gpio_Set_Dir(ADS_RST, GPIO_OUT);	//�������
	Set_pin_value(ADS_RST, 0);
	while(Timeout--);
	Set_pin_value(ADS_RST, 1);
}

/***********************************************************************
	[��	       ��	]ADS8688��ʼ��
	[��            ��	]��
	[��    ��    ֵ	]�������
***********************************************************************/
s32 ADS8688_Init(void)
{
	u8 Regval = 0x0, Result = 0x0;
	u8 Chan = 0;
	u16 u16Result = 0x0;

	//��λоƬ
	ADS_Reset();

	//�����λ
	if(Spi_Write_CMD(SOFTRST, &u16Result))
		return FAILURE;

	//����ͨ��ȡ�������Զ��������
	Regval = 0x0;
	if(Spi_Write_Reg(AUTO_SEQ_EN, Regval, &Result))
		return (FAILURE);

	//ͨ�����뷶Χѡ��(���Χ)
	for(Chan = 0; Chan < 8; Chan++)
	{
		Regval = 0xff;
		if(Spi_Write_Reg(Chan_Range_REG(Chan), VREFx2_5, &Result))
			return (FAILURE);
	}


	return SUCCESS;
}

/***********************************************************************
	[��	       ��	]ADS�ջ���Spi��ȡ����
	[��            ��	]Cmd:����, Buff:����
	[��    ��    ֵ	]�������
***********************************************************************/
s32 ReadViasADC_Spi(u16 Cmd, u8 *Buff, u8 Size, Spi spix)
{
	u8 *TXbuf = (u8 *)malloc(Size);

	memset(TXbuf, 0, Size);

	TXbuf[0] = (Cmd >> 8) & 0xff;
	TXbuf[1] = (Cmd >> 0) & 0xff;

	if(Spi_Transfer(TXbuf, Buff, Size, spix))
	{
		free(TXbuf);
		return FAILURE;
	}

	free(TXbuf);
	return SUCCESS;
}














