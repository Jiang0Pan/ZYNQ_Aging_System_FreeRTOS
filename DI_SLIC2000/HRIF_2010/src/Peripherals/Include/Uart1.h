#ifndef _UART1_H_
#define _UART1_H_

#include "xscugic.h"
#include "xuartps.h"
#include "xuartps_hw.h"
#include "interrupt.h"



#define UART1_ID 		(XPAR_PS7_UART_1_DEVICE_ID)		//串口ID



extern XUartPs UartPs_1;

//////////////////////////////////////////////////////////////////////////////

//初始化
s32 Uartps_Init(void);									//串口初始化

void Respon(void);

#endif



