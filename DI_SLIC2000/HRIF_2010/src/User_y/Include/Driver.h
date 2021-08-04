


#ifndef _DRIVER_H
#define _DRIVER_H



#include "GlobalRef.h"
#include "isl28023.h"


typedef enum {
	io0 = 0x0,
	io1,
	io2,
	io3,
	io4,
	io5,
	io6,
	io7,
	io8,
	io9,
	ioA,
	ioB,
	ioC,
	ioD,
	ioE,
	ioF,
}IO_pin;


void Time_Sync(void * recv, u16 Size);

//设置采样数据
void Set_Sample(_SAMP_  * samp);

//0XA157 读取采样数据          板卡编码    插板状态      老化状态
void Set_Sample_packge(Sample_Data *Sample);

//用户电源输出控制
s32 UserDps_Switch(u16 bid, u16 ID, SW ON_OFF);

//驱动板电源输出控制
s32 DrivDps_Switch(u16 bid, u16 ID, SW ON_OFF);

//打开DPS电源输入
void OpenDpsIn(void);

//Dps线性上电
u32 DpsLineOpen (struct _DpsArg_ *Dpsx, u16 id, u8 ProtectEn, u8 DpsAddr, IIC_Chan Chan);

//Dps线性下电
u32 DpsLineClose(struct _DpsArg_ *Dpsx, u16 id, u8 ProtectEn, u8 DpsAddr, IIC_Chan Chan);

//驱动板参数配置
u32 db_Conf(void);

//pin脚参数配置
u32 pin_Conf(void);


u32 Mask_Conf(void);

//测试板卡ddr
void TEST_DDR(void);

//关闭用户电源
void CloseDps(void);

//打开12V
s32 Switch_12V(SW ON_OFF);

//设置IO
s32 Set_IO_Level(SW ON_OFF, IO_pin io);

//读取IP
s32 ReadBoardIP(u8 *Level);



//配置并打开电源
void DpsConfig(Sample_Data *Sample, u8 ProtectEn);

//关闭上电时序
void CloseDpsSequence(Sample_Data *Sample, u8 ProtectEn);

void ReadDpsOnOff_flag(SW on_off);

void Set_DHV32(struct _REG_ * Cfg, short Vo1, short Vo2);

void Set_DTV32(struct _REG_ * Cfg, short Vo1, short Vo2);

void Set_DLV32(struct _REG_ * Cfg, short Vo1, short Vo2);

void Set_CPHV32(struct _REG_ * Cfg, short Vo1, short Vo2);

void Set_CPLV32(struct _REG_ * Cfg, short Vo1, short Vo2);


#endif







