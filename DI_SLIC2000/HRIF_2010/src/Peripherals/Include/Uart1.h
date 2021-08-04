#ifndef _UART1_H_
#define _UART1_H_

#include "xscugic.h"
#include "xuartps.h"
#include "xuartps_hw.h"
#include "interrupt.h"



#define UART1_ID 		(XPAR_PS7_UART_1_DEVICE_ID)		//����ID



extern XUartPs UartPs_1;

//////////////////////////////////////////////////////////////////////////////

//��ʼ��
s32 Uartps_Init(void);									//���ڳ�ʼ��

void Respon(void);

#endif



