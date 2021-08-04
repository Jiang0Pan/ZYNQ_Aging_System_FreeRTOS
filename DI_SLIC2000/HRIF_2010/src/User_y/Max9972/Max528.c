



#include "max528.h"
#include "fpga_reg.h"
#include "shell_port.h"


//Max528������
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
	[��	       ��	]Max528��ʼ��
	[��            ��	]spix:spi���
	[��    ��    ֵ	]�Ƿ�ok
***********************************************************************/
s32 Max528_Init(struct _REG_ * Cfg)
{
	Max528 BuffMode = {
		.Data = 0xff,
		.Addr = 0x00,
	};

	if(Max528_WROpt(Cfg, &BuffMode, NULL, 0)) 	//����BUffer mode ȫ����
		return FAILURE;

	if(Max528_WROpt(Cfg, &BuffMode, NULL, 1)) 	//����BUffer mode ȫ����
		return FAILURE;

	return SUCCESS;
}

/***********************************************************************
	[��	       ��	]Max528����DAC
	[��            ��	]Vo:��ѹ, Chan:adcͨ��, CS:528���
	[��    ��    ֵ	]�Ƿ�ok
***********************************************************************/
s32 Max528_Set_DAC(struct _REG_ * Cfg, short Vo, DAC_Chan Chan, u8 CS)
{
	float tmp = (float)(Vo + Absolute(MAX528_REFL))/MAX528_REFV * 255;

	Max528  DAC = {
		.Data = ((u16)tmp)&0xff,
		.Addr = 0x1 << Chan,
	};

	if(Max528_WROpt(Cfg, &DAC, NULL, CS)) 						//����DAC
		return FAILURE;

	return SUCCESS;
}


