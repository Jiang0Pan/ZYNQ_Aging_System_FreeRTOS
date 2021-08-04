#ifndef _ADS8688_H
#define _ADS8688_H



#include "Spi.h"
#include "Gpio.h"


#define ADC_PATHNUM			(8)		//ADC·��
#define VREF				(5.2)	//V

#define Invert(DAT)  ((((DAT)&0x000000ff)<<24)|(((DAT)&0x0000ff00)<<8)|(((DAT)&0x00ff0000)>>8)|(((DAT)&0xff000000)>>24))


//���뷶Χѡ��
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
	NO_OP		= 0x0000,//����ǰģʽ����ִ��
	STDBY		= 0x8200,//�������ģʽ
	PWR_DN		= 0x8300,//�������ģʽ
	SOFTRST		= 0x8500,//��λоƬ
	AUTO_RST	= 0xA000,//������ʹ���Զ�ģʽ
	MAN_Ch_0	= 0xC000,//�ֶ�ѡ��ͨ��0
	MAN_Ch_1	= 0xC400,//�ֶ�ѡ��ͨ��1
	MAN_Ch_2	= 0xC800,//�ֶ�ѡ��ͨ��2
	MAN_Ch_3	= 0xCC00,//�ֶ�ѡ��ͨ��3
	MAN_Ch_4	= 0xD000,//�ֶ�ѡ��ͨ��4
	MAN_Ch_5	= 0xD400,//�ֶ�ѡ��ͨ��5
	MAN_Ch_6	= 0xD800,//�ֶ�ѡ��ͨ��6
	MAN_Ch_7	= 0xDC00,//�ֶ�ѡ��ͨ��7
	MAN_AUX		= 0xE000,//�ֶ�ѡ��ͨ��AUX
}ADS8688_CMD;

//��λ
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

//ͨ�����뷶Χ���üĴ���
#define Chan_Range_REG(chan)   	(0x05 + (chan))

//�ֶ�ͨ��ѡ������
#define Man_Select_CMD(chan)	(MAN_Ch_0 + (chan))


//////////////////////////////////////////////////////////
//��λоƬ
void ADS_Reset(void);
//д�Ĵ���
s32 Spi_Write_Reg(u8 RegAddr, u8 RegVal, u8 *Result);
//���Ĵ���
s32 Spi_Read_Reg (u8 RegAddr, u8 *Result);
//д����(16λ)
s32 Spi_Write_CMD(u16 CMD, u16 *Result);
//��ȡADCֵ(8 + 1·),AUXͨ��
s32 Spi_ReadADC  (u16 *Buff);
//ADS8688��ʼ��
s32 ADS8688_Init (void);

s32 ReadViasADC_Spi(u16 Cmd, u8 *Buff, u8 Size, Spi spix);













#endif






