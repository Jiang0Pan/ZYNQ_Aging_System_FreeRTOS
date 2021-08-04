


#ifndef _FPGA_REG_H
#define _FPGA_REG_H



#include "xil_types.h"



//fpga寄存器
#define SYS_BASE		(0x0000)
#define DDR3_BASE		(0x1000)
#define MAX528_BASE		(0x3000)
#define MAX9972_BASE	(0x5000)
#define IO_BASE			(0x2000)
#define IO64_BASE		(0x2000)
typedef enum {
	VERSION 	 = (SYS_BASE     + 0x000),
	SYSDEBUG	 = (SYS_BASE	 + 0x004),
	DATE    	 = (SYS_BASE     + 0x008),
	TIME    	 = (SYS_BASE     + 0x00C),
	REST    	 = (SYS_BASE     + 0x010),
	TEMP    	 = (SYS_BASE     + 0x014),
	DAN1   	     = (SYS_BASE     + 0x018),
	DAN2 	     = (SYS_BASE     + 0x01C),

	PERIOD  	 = (DDR3_BASE    + 0x000),
	DDRDEBUG	 = (DDR3_BASE    + 0x004),
	WSADDR  	 = (DDR3_BASE    + 0x010),
	WLENGTH 	 = (DDR3_BASE    + 0x014),
	WTIRG   	 = (DDR3_BASE    + 0x018),
	RSADDR  	 = (DDR3_BASE    + 0x01C),
	RLENGTH 	 = (DDR3_BASE    + 0x020),
	RTIRG   	 = (DDR3_BASE    + 0x024),
	EXEFLAG 	 = (DDR3_BASE    + 0x028),
	COUNTER 	 = (DDR3_BASE    + 0x02C),
	STATUS       = (DDR3_BASE    + 0x030),
	VECFIFOLEN 	 = (DDR3_BASE    + 0x034),
	LISTFIFOLEN  = (DDR3_BASE    + 0x038),
	STREAMFLAG   = (DDR3_BASE    + 0x03C),
	CHECKSUM	 = (DDR3_BASE    + 0x064),
	CLRCHECK	 = (DDR3_BASE    + 0x068),
	CYCLE        = (DDR3_BASE    + 0x070),
	RESET		 = (DDR3_BASE    + 0xFD0),

	MAX528DEBUG  = (MAX528_BASE  + 0x004),
	MAX528TX     = (MAX528_BASE  + 0x008),
	MAX528BUSY   = (MAX528_BASE  + 0x00C),
	MAX528RX     = (MAX528_BASE  + 0x010),
	MAX528MODE   = (MAX528_BASE  + 0x014),

	MAX9972DEBUG = (MAX9972_BASE + 0x004),
	MAX9972BUSY  = (MAX9972_BASE + 0x008),
	MAX9972LOAD  = (MAX9972_BASE + 0x00C),
	MAX9972TX    = (MAX9972_BASE + 0x020),
	MAX9972RX    = (MAX9972_BASE + 0x040),

	IODEBUG	     = (IO_BASE      + 0x004),
	CHANVAR	     = (IO_BASE      + 0x008),
	CHANCMP	     = (IO_BASE      + 0x088),

	//回检信息
	RAMADDR	     = (IO_BASE      + 0x508),	//写RAM地址
	ERRORADDR    = (IO_BASE      + 0x510),	//出错地址
	ERRORMASK    = (IO_BASE      + 0x50c),	//出错通道掩码
	CHANMASK	 = (IO_BASE      + 0x500),	//通道掩码(关闭某个通道)

	INITBRAM     = (IO_BASE      + 0x518),	//初始化测试信息

	IOCFG		 = (IO_BASE      + 0x000),	//64路IO配置



}FPGA_REG;


#define	AGETIME			(*((volatile u32 *)(0x43C07000)))
#define	TIMECTL			(*((volatile u32 *)(0x43C07004)))



#endif



