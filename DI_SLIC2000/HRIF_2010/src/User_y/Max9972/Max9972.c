

#include "max9972.h"


/********************************************
//9972Œ™ÀƒÕ®µ¿
//«˝∂Ø∞Âπ≤8∆¨ 16channel
//∞¥«˝∂Ø∞Âºƒ¥Ê∆˜≈‰÷√
*********************************************/


//∂¡–¥“ª∆¨9972
s32 Max9972_WOpt(struct _REG_ * Cfg, Max9972 *Tx, MAX9972_CS cs)
{
	u32 busy = 0x0;

	if(Fpga_ROpt(Cfg, MAX9972BUSY, &busy))
		return FAILURE;
	if((busy >> cs)&0x1)
		return FAILURE;

	if(Fpga_WOpt(Cfg, MAX9972LOAD, 0x1 << cs))			//load–≈∫≈
		return FAILURE;

	if(Fpga_WOpt(Cfg, MAX9972TX + (0x4 * cs), Tx->Data))
		return FAILURE;

	return SUCCESS;
}

//∂¡–¥“ª∆¨9972
s32 Max9972_ROpt(struct _REG_ * Cfg, u32 *Rdata, MAX9972_CS cs)
{
	u32 busy = 0xff;
	u16 timeout = WAITE_TIME;

	if(Fpga_ROpt(Cfg, MAX9972BUSY, &busy))
		return FAILURE;
	if((busy >> cs)&0x1)
		return FAILURE;

	if(Fpga_WOpt(Cfg, MAX9972LOAD, 0))			//load–≈∫≈
		return FAILURE;

	if(Fpga_WOpt(Cfg, MAX9972TX + (0x4 * cs), 0x0))
		return FAILURE;

	do{
		if(SUCCESS == Fpga_ROpt(Cfg, MAX9972BUSY, &busy))
		{
			if(0 == ((busy >> cs)&0x1))
				break;
		}
		if(TimeOutUS(&timeout))
			return FAILURE;
	}while(1);

	if(NULL != Rdata)
		return Fpga_ROpt(Cfg, MAX9972RX + (0x4 * cs), Rdata);

	return SUCCESS;
}


//∂¡–¥“ª∆¨9972
s32 Max9972_WROpt(struct _REG_ * Cfg, Max9972 *Tx, u32 *Rdata, MAX9972_CS cs)
{
	s32 status = FAILURE;

	status = Max9972_WOpt(Cfg, Tx, cs);
	if(status == FAILURE)
		return FAILURE;

	status = Max9972_ROpt(Cfg, Rdata, cs);
	if(status == FAILURE)
		return FAILURE;

	return SUCCESS;
}

//9972≥ı ºªØ≈‰÷√
s32 Max9972_InitCfg(struct _REG_ * Cfg)
{
	u16 i = 0x0;
	Max9972 Tx = {
		.TERM		= 0,
		.LLEAK      = 0,
		.SENSE_EN   = 0,
		.FORCE_EN   = 0,
		.EN_LOAD_L  = 0,
		.EN_LOAD_H  = 0,
		.CH1      	= 1,
		.CH2      	= 1,
		.CH3      	= 1,
		.CH4      	= 1,
	};

	for(i = 0; i < 8; i++)
	{
		if(Max9972_WROpt(Cfg, &Tx, NULL, i))
			return FAILURE;
	}

	return SUCCESS;
}
