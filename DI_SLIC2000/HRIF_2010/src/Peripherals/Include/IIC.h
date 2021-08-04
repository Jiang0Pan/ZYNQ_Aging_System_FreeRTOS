#ifndef _IIC_H
#define _IIC_H



#include "xparameters.h"
#include "xiicps.h"
#include "error.h"
#include "gpio.h"


#define IIC_WAITE (1000)


//IIC编号，IIC1用作扩展，IIC0由ps用作eeprom
enum {
	DEV_IIC0 = XPAR_XIICPS_0_DEVICE_ID,
	DEV_IIC1 = XPAR_XIICPS_1_DEVICE_ID,
};

#define IIC0_SCLK_RATE		(100000) //100khz
#define IIC1_SCLK_RATE   	(100000) //100khz

/* Expand iic device 0 */
#define TCA0_ADDR			(0x70)
/* Expand iic device 1 */
#define TCA1_ADDR			(0x71)


typedef enum{
	IIC00 = 0x0,		//IIC00~IIC15为扩展IIC
	IIC01 = 0x1,
	IIC02 = 0x2,
	IIC03 = 0x3,
	IIC04 = 0x4,
	IIC05 = 0x5,
	IIC06 = 0x6,
	IIC07 = 0x7,
	IIC08 = 0x8,
	IIC09 = 0x9,
	IIC10 = 0xA,
	IIC11 = 0xB,
	IIC12 = 0xC,
	IIC13 = 0xD,
	IIC14 = 0xE,
	IIC15 = 0xF,
	PSIIC,				//ps的IIC
	INVAILD_IIC,
}IIC_Chan;


#define IIC_REST_IO     (6)
extern XIicPs IIC[2];


//////////////////////////////////////////////////////

void Reset_ExpIIC(void);

s32 IIC_Init(void);

//IIC轮询发送
s32 My_Write(u8 *MsgPtr, s32 ByteCount, u16 SlaveAddr, XIicPs* iicx);

//IIC轮询接收
s32 My_Read (u8 *MsgPtr, s32 ByteCount, u16 SlaveAddr, XIicPs* iicx);

//选择通道
s32 Select_IIC(IIC_Chan Chan);

//写通道
s32 IIC_Write(u8 *MsgPtr, s32 ByteCount, u16 SlaveAddr, IIC_Chan Chan);

//读通道
s32 IIC_Read (u8 *MsgPtr, s32 ByteCount, u16 SlaveAddr, IIC_Chan Chan);












#endif

