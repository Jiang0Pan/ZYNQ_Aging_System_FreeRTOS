#include "interrupt.h"
#include "error.h"




/***********************************************************************
	[功	       能	]注册中断异常处理
	[参            数	]无
	[返    回    值	]无
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
	[功	       能	]中断初始化
	[参            数	]无
	[返    回    值	]初始化是否成功
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
	[功	       能	]映射中断ID到当前CPU
	[参            数	]IntrId:中断号
	[返    回    值	]无
***********************************************************************/
void Mask_CPU_IntrID(u16 IntrId)
{
	u32 RegVal = XScuGic_DistReadReg(&xInterruptController, XSCUGIC_SPI_TARGET_OFFSET_CALC(IntrId));
	u32 val = (IntrId % 4)*8;
	RegVal = (RegVal & (~(0xff << val))) | ((XPAR_CPU_ID + 1) << val);
	XScuGic_DistWriteReg(&xInterruptController, XSCUGIC_SPI_TARGET_OFFSET_CALC(IntrId), RegVal);
}

/***********************************************************************
	[功	       能	]设置中断触发类型
	[参            数	]IntrId:中断号, Type:中断类型
	[返    回    值	]无
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











