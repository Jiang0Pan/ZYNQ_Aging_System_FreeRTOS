


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

//���ò�������
void Set_Sample(_SAMP_  * samp);

//0XA157 ��ȡ��������          �忨����    ���״̬      �ϻ�״̬
void Set_Sample_packge(Sample_Data *Sample);

//�û���Դ�������
s32 UserDps_Switch(u16 bid, u16 ID, SW ON_OFF);

//�������Դ�������
s32 DrivDps_Switch(u16 bid, u16 ID, SW ON_OFF);

//��DPS��Դ����
void OpenDpsIn(void);

//Dps�����ϵ�
u32 DpsLineOpen (struct _DpsArg_ *Dpsx, u16 id, u8 ProtectEn, u8 DpsAddr, IIC_Chan Chan);

//Dps�����µ�
u32 DpsLineClose(struct _DpsArg_ *Dpsx, u16 id, u8 ProtectEn, u8 DpsAddr, IIC_Chan Chan);

//�������������
u32 db_Conf(void);

//pin�Ų�������
u32 pin_Conf(void);


u32 Mask_Conf(void);

//���԰忨ddr
void TEST_DDR(void);

//�ر��û���Դ
void CloseDps(void);

//��12V
s32 Switch_12V(SW ON_OFF);

//����IO
s32 Set_IO_Level(SW ON_OFF, IO_pin io);

//��ȡIP
s32 ReadBoardIP(u8 *Level);



//���ò��򿪵�Դ
void DpsConfig(Sample_Data *Sample, u8 ProtectEn);

//�ر��ϵ�ʱ��
void CloseDpsSequence(Sample_Data *Sample, u8 ProtectEn);

void ReadDpsOnOff_flag(SW on_off);

void Set_DHV32(struct _REG_ * Cfg, short Vo1, short Vo2);

void Set_DTV32(struct _REG_ * Cfg, short Vo1, short Vo2);

void Set_DLV32(struct _REG_ * Cfg, short Vo1, short Vo2);

void Set_CPHV32(struct _REG_ * Cfg, short Vo1, short Vo2);

void Set_CPLV32(struct _REG_ * Cfg, short Vo1, short Vo2);


#endif







