
#ifndef _MAX9972_H
#define _MAX9972_H



#include "error.h"
#include "fpga.h"

//9972¿ØÖÆ
typedef union {
	struct{
		u16 TERM:		1;
		u16 LLEAK:		1;
		u16 SENSE_EN:	1;
		u16 FORCE_EN:	1;
		u16 EN_LOAD_L:	1;
		u16 EN_LOAD_H:	1;
		u16 :			2;
		u16 CH1:		1;
		u16 CH2:		1;
		u16 CH3:		1;
		u16 CH4:		1;
		u16 :			4;
	};
	u16  Data;
}Max9972;

typedef enum {
	CS0 = 0x0,
	CS1 = 0x1,
	CS2 = 0x2,
	CS3 = 0x3,
	CS4 = 0x4,
	CS5 = 0x5,
	CS6 = 0x6,
	CS7 = 0x7,
}MAX9972_CS;


////////////////////////////////////////////////////////////////////////////


s32 Max9972_InitCfg(struct _REG_ * Cfg);
   s32 Max9972_WOpt(struct _REG_ * Cfg, Max9972 *Tx, MAX9972_CS cs);
   s32 Max9972_ROpt(struct _REG_ * Cfg, u32 *Rdata, MAX9972_CS cs);
  s32 Max9972_WROpt(struct _REG_ * Cfg, Max9972 *Tx, u32 *Rdata, MAX9972_CS cs);



#endif






