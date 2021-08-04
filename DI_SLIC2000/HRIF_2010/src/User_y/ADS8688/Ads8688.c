#include "ads8688.h"
#include "error.h"
#include "malloc.h"





/***********************************************************************
	[功	       能	]写寄存器并从Result返回写入数据
	[参            数	]RegAddr:寄存器地址, RegVal:写入值, Result:返回值指针
	[返    回    值	]操作结果
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
	[功	       能	]读寄存器并从Result返回写入数据
	[参            数	]RegAddr:寄存器地址, Result:返回值指针
	[返    回    值	]操作结果
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
	[功	       能	]写命令(16位)并从Result返回写入数据
	[参            数	]CMD:命令, Result:返回值指针
	[返    回    值	]操作结果
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
	[功	       能	]读取ADC值(8路),AUX通道
	[参            数	]Buff:缓存指针
	[返    回    值	]操作结果
***********************************************************************/
s32 Spi_ReadADC(u16 *Buff)
{
	u8 chan = 0;

	for(chan = 0; chan < ADC_PATHNUM; chan++)					//读取8通道数据
	{
		if(Spi_Write_CMD(Man_Select_CMD(chan), (Buff+chan)))
			return (chan + 1);									//返回错误通道号
	}

	return SUCCESS;
}

/***********************************************************************
	[功	       能	]硬件复位 ADS8688
	[参            数	]无
	[返    回    值	]无
***********************************************************************/
void ADS_Reset(void)
{
	u8 Timeout = 5;
	Gpio_Set_Dir(ADS_RST, GPIO_OUT);	//设置输出
	Set_pin_value(ADS_RST, 0);
	while(Timeout--);
	Set_pin_value(ADS_RST, 1);
}

/***********************************************************************
	[功	       能	]ADS8688初始化
	[参            数	]无
	[返    回    值	]操作结果
***********************************************************************/
s32 ADS8688_Init(void)
{
	u8 Regval = 0x0, Result = 0x0;
	u8 Chan = 0;
	u16 u16Result = 0x0;

	//复位芯片
	ADS_Reset();

	//软件复位
	if(Spi_Write_CMD(SOFTRST, &u16Result))
		return FAILURE;

	//所有通道取消加入自动排序队列
	Regval = 0x0;
	if(Spi_Write_Reg(AUTO_SEQ_EN, Regval, &Result))
		return (FAILURE);

	//通道输入范围选择(最大范围)
	for(Chan = 0; Chan < 8; Chan++)
	{
		Regval = 0xff;
		if(Spi_Write_Reg(Chan_Range_REG(Chan), VREFx2_5, &Result))
			return (FAILURE);
	}


	return SUCCESS;
}

/***********************************************************************
	[功	       能	]ADS菊花链Spi读取数据
	[参            数	]Cmd:命令, Buff:缓存
	[返    回    值	]操作结果
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














