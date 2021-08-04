#ifndef _INTERRUPT_H
#define _INTERRUPT_H



#include "xscugic.h"
#include "xil_exception.h"


//�жϿ�����ƫ����
#define INT_CFG0_OFFSET 0x00000C00


///////////////////////////////////////////////////////

extern XScuGic xInterruptController;

void Setup_Intr_Exception(void);

s32 Interrupt_Init(void);

void Mask_CPU_IntrID(u16 IntrId);

void IntrTypeSetup(u16 IntrId, u16 Type);


#endif







///////////////////////////////////////////////////////


//CPU0 FIQ�жϳ�ʼ��
s32 InitScuGic(void);
