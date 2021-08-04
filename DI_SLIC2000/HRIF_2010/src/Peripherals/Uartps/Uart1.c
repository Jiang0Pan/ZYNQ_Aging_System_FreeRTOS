





#include "uart1.h"
#include "error.h"
#include "string.h"


XUartPs UartPs_1;

XUartPsFormat uart_format =
{
	.BaudRate = XUARTPS_DFT_BAUDRATE,   	//Ĭ�ϲ����� 115200
	.DataBits = XUARTPS_FORMAT_8_BITS,		//����λ
	.Parity   = XUARTPS_FORMAT_NO_PARITY,	//У��λ
	.StopBits = XUARTPS_FORMAT_1_STOP_BIT,	//ֹͣλ
};

/***********************************************************************
	[��	       ��	]���ڳ�ʼ��
	[��            ��	]��
	[��    ��    ֵ	]��ʼ���Ƿ�OK
***********************************************************************/

s32 Uartps_Init(void)
{
	s32 Status = 0;
	XUartPs_Config *Uart1;

	//���ڳ�ʼ��
	Uart1 = XUartPs_LookupConfig(UART1_ID);
	Status = XUartPs_CfgInitialize(&UartPs_1, Uart1, Uart1->BaseAddress);
	if(Status != SUCCESS)
		return FAILURE;

	Status = XUartPs_SetDataFormat(&UartPs_1, &uart_format);	//���ô��ڲ���
	if(Status != SUCCESS)
		return FAILURE;

	XUartPs_SetOperMode(&UartPs_1, XUARTPS_OPER_MODE_NORMAL); 	//����ģʽ

	XUartPs_SetFifoThreshold(&UartPs_1, 1);						//����fifo��ֵ

	XUartPs_SetRecvTimeout(&UartPs_1, 4);						//���ó�ʱ����8*4=32��

	return SUCCESS;
}


