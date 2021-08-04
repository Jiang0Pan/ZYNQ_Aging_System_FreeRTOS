#include "interrupt.h"
#include "error.h"




/***********************************************************************
	[��	       ��	]ע���ж��쳣����
	[��            ��	]��
	[��    ��    ֵ	]��
***********************************************************************/
void Setup_Intr_Exception(void)
{
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler)XScuGic_InterruptHandler,
			(void *)&xInterruptController);
	Xil_ExceptionEnable();
}

/***********************************************************************
	[��	       ��	]�жϳ�ʼ��
	[��            ��	]��
	[��    ��    ֵ	]��ʼ���Ƿ�ɹ�
***********************************************************************/
s32 Interrupt_Init(void)
{
	s32 Status = 1;
	XScuGic_Config *IntcConfig;

	IntcConfig = XScuGic_LookupConfig(XPAR_SCUGIC_SINGLE_DEVICE_ID);
	if (NULL == IntcConfig)
		return FAILURE;

	Status = XScuGic_CfgInitialize(&xInterruptController, IntcConfig,
					IntcConfig->CpuBaseAddress);
	if (Status != SUCCESS)
		return FAILURE;

	return SUCCESS;
}

/***********************************************************************
	[��	       ��	]ӳ���ж�ID����ǰCPU
	[��            ��	]IntrId:�жϺ�
	[��    ��    ֵ	]��
***********************************************************************/
void Mask_CPU_IntrID(u16 IntrId)
{
	u32 RegVal = XScuGic_DistReadReg(&xInterruptController, XSCUGIC_SPI_TARGET_OFFSET_CALC(IntrId));
	u32 val = (IntrId % 4)*8;
	RegVal = (RegVal & (~(0xff << val))) | ((XPAR_CPU_ID + 1) << val);
	XScuGic_DistWriteReg(&xInterruptController, XSCUGIC_SPI_TARGET_OFFSET_CALC(IntrId), RegVal);
}

/***********************************************************************
	[��	       ��	]�����жϴ�������
	[��            ��	]IntrId:�жϺ�, Type:�ж�����
	[��    ��    ֵ	]��
***********************************************************************/
void IntrTypeSetup(u16 IntrId, u16 Type)
{
    s32 mask;

    mask = XScuGic_DistReadReg(&xInterruptController, INT_CFG0_OFFSET + (IntrId/16)*4);
    mask &= ~(0x03 << (IntrId%16)*2);
    Type &= 0x03;
    mask |= Type << ((IntrId%16)*2);
    XScuGic_DistWriteReg(&xInterruptController, INT_CFG0_OFFSET + (IntrId/16)*4, mask);
}











