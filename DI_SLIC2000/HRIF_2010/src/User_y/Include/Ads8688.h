#ifndef _ADS8688_H
#define _ADS8688_H



#include "Spi.h"
#include "Gpio.h"


#define ADC_PATHNUM			(8)		//ADC路数
#define VREF				(5.2)	//V

#define Invert(DAT)  ((((DAT)&0x000000ff)<<24)|(((DAT)&0x0000ff00)<<8)|(((DAT)&0x00ff0000)>>8)|(((DAT)&0xff000000)>>24))


//输入范围选择
typedef enum {
	VREFx2_5		= 0x0,
	VREFx1_25		= 0x1,
	VREFx0_625		= 0x2,
	VREFx0_3125		= 0x3,
	VREFx0_15625	= 0xB,
	VREFx2_5_		= 0x5,
	VREFx1_25_		= 0x6,
	VREFx0_625_		= 0x7,
	VREFx0_3125_	= 0xf,
}ADC_Input_Range;


typedef enum{
	NO_OP		= 0x0000,//按以前模式继续执行
	STDBY		= 0x8200,//进入待机模式
	PWR_DN		= 0x8300,//进入掉电模式
	SOFTRST		= 0x8500,//复位芯片
	AUTO_RST	= 0xA000,//重启后使用自动模式
	MAN_Ch_0	= 0xC000,//手动选择通道0
	MAN_Ch_1	= 0xC400,//手动选择通道1
	MAN_Ch_2	= 0xC800,//手动选择通道2
	MAN_Ch_3	= 0xCC00,//手动选择通道3
	MAN_Ch_4	= 0xD000,//手动选择通道4
	MAN_Ch_5	= 0xD400,//手动选择通道5
	MAN_Ch_6	= 0xD800,//手动选择通道6
	MAN_Ch_7	= 0xDC00,//手动选择通道7
	MAN_AUX		= 0xE000,//手动选择通道AUX
}ADS8688_CMD;

//置位
#define BIT(X)    				(0x1 << (X))

typedef enum {
	AUTO_SEQ_EN				= 0x01,
	Channel_Power_Down		= 0x02,
	Channel0_Input_Range	= 0x05,
	Channel1_Input_Range	= 0x06,
	Channel2_Input_Range	= 0x07,
	Channel3_Input_Range	= 0x08,
	Channel4_Input_Range	= 0x09,
	Channel5_Input_Range	= 0x0A,
	Channel6_Input_Range	= 0x0B,
	Channel7_Input_Range	= 0x0C,
}ADS8688_REG;

//通道输入范围设置寄存器
#define Chan_Range_REG(chan)   	(0x05 + (chan))

//手动通道选择命令
#define Man_Select_CMD(chan)	(MAN_Ch_0 + (chan))


//////////////////////////////////////////////////////////
//复位芯片
void ADS_Reset(void);
//写寄存器
s32 Spi_Write_Reg(u8 RegAddr, u8 RegVal, u8 *Result);
//读寄存器
s32 Spi_Read_Reg (u8 RegAddr, u8 *Result);
//写命令(16位)
s32 Spi_Write_CMD(u16 CMD, u16 *Result);
//读取ADC值(8 + 1路),AUX通道
s32 Spi_ReadADC  (u16 *Buff);
//ADS8688初始化
s32 ADS8688_Init (void);

s32 ReadViasADC_Spi(u16 Cmd, u8 *Buff, u8 Size, Spi spix);













#endif






