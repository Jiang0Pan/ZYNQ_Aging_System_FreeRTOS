



#ifndef _SYSTEMINFO_H
#define _SYSTEMINFO_H


#include "GlobalRef.h"
#include "fpga.h"
#include "Hw_ver.h"
#include "eeprom.h"
#include "Max528.h"
#include "Max9972.h"
#include "Driver.h"

#define     ZYNQ_VERSION	"1.5"



#define		PL_SYSDEBUG		(*((volatile u32 *)(AXI_BASE_ADDR + 0x00)))
#define 	PL_VERSION 	 	(*((volatile u32 *)(AXI_BASE_ADDR + 0x04)))
#define		PL_DATE    	 	(*((volatile u32 *)(AXI_BASE_ADDR + 0x08)))
#define		PL_TIME    	 	(*((volatile u32 *)(AXI_BASE_ADDR + 0x0C)))
#define		PL_TEMP    	 	(*((volatile u32 *)(AXI_BASE_ADDR + 0x18)))
#define		PL_DAN1   	 	(*((volatile u32 *)(AXI_BASE_ADDR + 0x1C)))
#define		PL_DAN2 	 	(*((volatile u32 *)(AXI_BASE_ADDR + 0x20)))


//读取一块驱动板的信息
void ReadDBinfo(u8 Bid);

//读取电源信息
void ReadDpsinfo(void);

//读取背板信息
void ReadBKinfo(void);

//读取主板信息
void ReadMBinfo(void);

//板卡硬件初始化
void BoardHW_Init(void);

//读取系统硬件信息
void ReadSysteminfo(void);



#endif

