





#include "uart1.h"
#include "error.h"
#include "string.h"


XUartPs UartPs_1;

XUartPsFormat uart_format =
{
	.BaudRate = XUARTPS_DFT_BAUDRATE,   	//默认波特率 115200
	.DataBits = XUARTPS_FORMAT_8_BITS,		//数据位
	.Parity   = XUARTPS_FORMAT_NO_PARITY,	//校验位
	.StopBits = XUARTPS_FORMAT_1_STOP_BIT,	//停止位
};

/***********************************************************************
	[功	       能	]串口初始化
	[参            数	]无
	[返    回    值	]初始化是否OK
***********************************************************************/

s32 Uartps_Init(void)
{
	s32 Status = 0;
	XUartPs_Config *Uart1;

	//串口初始化
	Uart1 = XUartPs_LookupConfig(UART1_ID);
	Status = XUartPs_CfgInitialize(&UartPs_1, Uart1, Uart1->BaseAddress);
	if(Status != SUCCESS)
		return FAILURE;

	Status = XUartPs_SetDataFormat(&UartPs_1, &uart_format);	//设置串口参数
	if(Status != SUCCESS)
		return FAILURE;

	XUartPs_SetOperMode(&UartPs_1, XUARTPS_OPER_MODE_NORMAL); 	//正常模式

	XUartPs_SetFifoThreshold(&UartPs_1, 1);						//设置fifo阈值

	XUartPs_SetRecvTimeout(&UartPs_1, 4);						//设置超时周期8*4=32个

	return SUCCESS;
}


