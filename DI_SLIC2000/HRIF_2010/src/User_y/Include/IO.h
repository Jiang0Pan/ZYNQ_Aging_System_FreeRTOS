


#ifndef _IO_H
#define _IO_H

#include "iic.h"
#include "GlobalRef.h"


//IO��Ŷ���
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


//IO������
typedef enum {
	OUTPUT	= 0x0,
	INPUT	= 0x1,
}GroupXDir;


//PCA9535BS�Ĵ�������
typedef enum {
	PORT0READ  = 0x0,	//0��IO״̬��ȡ
	PORT1READ  = 0x1,	//1��IO״̬��ȡ
	PORT0WRITE = 0x2, 	//0��IOд����
	PORT1WRITE = 0x3,	//1��IOд����
	PORT0INVER = 0x4,	//0��IO���ݼ��Է�ת
	PORT1INVER = 0x5,	//1��IO���ݼ��Է�ת
	PORT0CFG   = 0x6,	//0��IO����
	PORT1CFG   = 0x7,	//1��IO����
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

//PCAоƬIO��������
s32 IO_Dir_Config(GroupXDir dir0, GroupXDir dir1, u8 SlaveAddr, IIC_Chan Chan);

//����PCA��IO��ƽ
s32 IO_Level_Set(_Output_ * IO, GroupX Gpx, PIN_Num pin, u8 Level, u8 SlaveAddr, IIC_Chan Chan);

//��������IO���
s32 ALL_IO_Level_Set(_Output_ * IO, u8 pin, u8 Level, u8 SlaveAddr, IIC_Chan Chan);

//��ȡPCA��IO��ƽ
s32 IO_Level_Get(GroupX Gpx, PIN_Num pin, u8 *Level, u8 SlaveAddr, IIC_Chan Chan);

//��ȡPCA��һ��IO��ƽ
s32 IO_Group_Get(GroupX Gpx, u8 *Level, u8 SlaveAddr, IIC_Chan Chan);

//��ʼ��PCA IO
s32 PCA_IO_Init(GroupXDir dir0, GroupXDir dir1, _Output_ *GIO, u8 SlaveAddr, IIC_Chan Chan);

 s32 fpgaSpi_Open(UpdateCS CS);

s32 fpgaSpi_Close(UpdateCS CS);

s32 DpsSpi_Open(u8 bid, DpsCS cs);

s32 DpsSpi_Close(u8 bid);







#endif

