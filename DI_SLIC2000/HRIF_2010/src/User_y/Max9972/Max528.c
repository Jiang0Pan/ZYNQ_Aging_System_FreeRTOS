



#include "max528.h"
#include "fpga_reg.h"
#include "shell_port.h"


//Max528读数据
s32 Max528_WROpt(struct _REG_ * Cfg, Max528 *Tx, u32 *Rdata, u8 CS)
{
	u32 busy = 0x0;

	if(Fpga_ROpt(Cfg, MAX528BUSY + (0x1000 * ((CS > 0) ? 1 : 0)), &busy))
		return FAILURE;
	if(busy)
		return FAILURE;

	if(Fpga_WOpt(Cfg, MAX528TX + (0x1000 * ((CS > 0) ? 1 : 0)), (u32)Tx->TxDat))
		return FAILURE;

	if(NULL != Rdata)
		return Fpga_ROpt(Cfg, MAX528RX + (0x1000 * ((CS > 0) ? 1 : 0)), Rdata);

	return SUCCESS;
}

/***********************************************************************
	[功	       能	]Max528初始化
	[参            数	]spix:spi编号
	[返    回    值	]是否ok
***********************************************************************/
s32 Max528_Init(struct _REG_ * Cfg)
{
	Max528 BuffMode = {
		.Data = 0xff,
		.Addr = 0x00,
	};

	if(Max528_WROpt(Cfg, &BuffMode, NULL, 0)) 	//设置BUffer mode 全缓冲
		return FAILURE;

	if(Max528_WROpt(Cfg, &BuffMode, NULL, 1)) 	//设置BUffer mode 全缓冲
		return FAILURE;

	return SUCCESS;
}

/***********************************************************************
	[功	       能	]Max528设置DAC
	[参            数	]Vo:电压, Chan:adc通道, CS:528编号
	[返    回    值	]是否ok
***********************************************************************/
s32 Max528_Set_DAC(struct _REG_ * Cfg, short Vo, DAC_Chan Chan, u8 CS)
{
	float tmp = (float)(Vo + Absolute(MAX528_REFL))/MAX528_REFV * 255;

	Max528  DAC = {
		.Data = ((u16)tmp)&0xff,
		.Addr = 0x1 << Chan,
	};

	if(Max528_WROpt(Cfg, &DAC, NULL, CS)) 						//设置DAC
		return FAILURE;

	return SUCCESS;
}


