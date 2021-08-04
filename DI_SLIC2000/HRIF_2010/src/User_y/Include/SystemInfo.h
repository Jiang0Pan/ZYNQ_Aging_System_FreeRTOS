



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


//��ȡһ�����������Ϣ
void ReadDBinfo(u8 Bid);

//��ȡ��Դ��Ϣ
void ReadDpsinfo(void);

//��ȡ������Ϣ
void ReadBKinfo(void);

//��ȡ������Ϣ
void ReadMBinfo(void);

//�忨Ӳ����ʼ��
void BoardHW_Init(void);

//��ȡϵͳӲ����Ϣ
void ReadSysteminfo(void);



#endif

