



#ifndef _ERROR_H
#define _ERROR_H


#include "xil_types.h"

//iic传输超时
#define 	IIC_TIMEOUT			(5)
#define     TIMEOUT				(2)


typedef union _err_ {
	struct{
		u32 err0:	1;
		u32 err1:	1;
		u32 err2:	1;
		u32 err3:	1;
		u32 err4:	1;
		u32 err5:	1;
		u32 err6:	1;
		u32 err7:	1;
		u32 err8:	1;
		u32 err9:	1;
		u32 err10:	1;
		u32 err11:	1;
		u32 err12:	1;
		u32 err13:	1;
		u32 err14:	1;
		u32 err15:	1;
	}err;
	u32 errbit;
}err_tp;





enum {SUCCESS = 0x0, FAILURE = 0x1,};


u8 TimeOutUS(u16 *us);

u8 TimeOutCount(u16 *count);












#endif





































