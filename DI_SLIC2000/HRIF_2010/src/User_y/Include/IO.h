


#ifndef _IO_H
#define _IO_H

#include "iic.h"
#include "GlobalRef.h"


//IO组号定义
typedef enum {
	Group0	= 0x0,
	Group1	= 0x1,
}GroupX;


typedef enum{
	pin0,
	pin1,
	pin2,
	pin3,
	pin4,
	pin5,
	pin6,
	pin7,
}PIN_Num;


//IO方向定义
typedef enum {
	OUTPUT	= 0x0,
	INPUT	= 0x1,
}GroupXDir;


//PCA9535BS寄存器定义
typedef enum {
	PORT0READ  = 0x0,	//0组IO状态读取
	PORT1READ  = 0x1,	//1组IO状态读取
	PORT0WRITE = 0x2, 	//0组IO写数据
	PORT1WRITE = 0x3,	//1组IO写数据
	PORT0INVER = 0x4,	//0组IO数据极性翻转
	PORT1INVER = 0x5,	//1组IO数据极性翻转
	PORT0CFG   = 0x6,	//0组IO配置
	PORT1CFG   = 0x7,	//1组IO配置
}PCA_Reg;

typedef enum {
	FPGA_0,
	FPGA_1,
	FPGA_2,
	FPGA_3,
	FPGA_4,
}UpdateCS;

typedef enum {
	DPS_UPDATE = 0x0,
	DPS_CTL    = 0x1,
}DpsCS;



/////////////////////////////////////////////////////////////////////////////

//PCA芯片IO方向配置
s32 IO_Dir_Config(GroupXDir dir0, GroupXDir dir1, u8 SlaveAddr, IIC_Chan Chan);

//设置PCA的IO电平
s32 IO_Level_Set(_Output_ * IO, GroupX Gpx, PIN_Num pin, u8 Level, u8 SlaveAddr, IIC_Chan Chan);

//设置所有IO输出
s32 ALL_IO_Level_Set(_Output_ * IO, u8 pin, u8 Level, u8 SlaveAddr, IIC_Chan Chan);

//读取PCA的IO电平
s32 IO_Level_Get(GroupX Gpx, PIN_Num pin, u8 *Level, u8 SlaveAddr, IIC_Chan Chan);

//读取PCA的一组IO电平
s32 IO_Group_Get(GroupX Gpx, u8 *Level, u8 SlaveAddr, IIC_Chan Chan);

//初始化PCA IO
s32 PCA_IO_Init(GroupXDir dir0, GroupXDir dir1, _Output_ *GIO, u8 SlaveAddr, IIC_Chan Chan);

 s32 fpgaSpi_Open(UpdateCS CS);

s32 fpgaSpi_Close(UpdateCS CS);

s32 DpsSpi_Open(u8 bid, DpsCS cs);

s32 DpsSpi_Close(u8 bid);







#endif

