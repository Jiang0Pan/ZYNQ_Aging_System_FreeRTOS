/*
  ** @file           : qspi_bus.c
  ** @brief          : Qspi Bus Interface Source File
  **
  ** @attention
  **
  ** Copyright (c) 2020 ChengDuHangke.
  ** All rights reserved.
  **
  ** This software is licensed by ChengDuHangke under Ultimate Liberty license
  **
*/


/*
  ** Include
*/
#include "xqspips.h"
#include "debug_config.h"
#include "return_val.h"
#include "qspi_bus.h"


/*
  ** Define
*/
#define 	QSPI_DEVICE_ID		 		(XPAR_XQSPIPS_0_DEVICE_ID)
/*
 * The following defines are for dual flash stacked mode interface.
 */
/* Fast Quad Read Output */
#define 	LQSPI_CR_FAST_QUAD_READ		(0x0000006B)
/* 1 Dummy Byte Between Address And Return Data */
#define 	LQSPI_CR_1_DUMMY_BYTE		(0x00000100)
#define 	DUAL_STACK_CONFIG_WRITE		(XQSPIPS_LQSPI_CR_TWO_MEM_MASK | \
					 	 	 	 	 	 LQSPI_CR_1_DUMMY_BYTE | \
					 	 	 	 	 	 LQSPI_CR_FAST_QUAD_READ)

#define 	DUAL_QSPI_CONFIG_WRITE		(XQSPIPS_LQSPI_CR_TWO_MEM_MASK | \
					 	 	 	 	 	 XQSPIPS_LQSPI_CR_SEP_BUS_MASK | \
					 	 	 	 	 	 LQSPI_CR_1_DUMMY_BYTE | \
					 	 	 	 	 	 LQSPI_CR_FAST_QUAD_READ)

/*
  ** Static Global Variable
*/
static 		XQspiPs 					QspiInstance;


/*
  ** Function Name        : Qspi_Init
  ** Function Description : Qspi Bus Initialize
  ** Function Parameter   : Void
  ** Function Return      : INIT_SUCCESS        : Initialize Success
  **                        INIT_FAILED         : Initialize Failed
  ** Notes				  : None
*/
u8 Qspi_Init(void)
{
	u8 status;
	XQspiPs_Config *QspiConfig;

	/* Initialize The Qspi Driver So That It's Ready To Use */
	QspiConfig = XQspiPs_LookupConfig(QSPI_DEVICE_ID);
	if (NULL == QspiConfig) {
		debug("Qspi Init Failed In LookupConfig In %s Files,%d Line\n",__FILE__,__LINE__);
		return INIT_FAILED;
	}

	/* Default The Qspi Driver */
	status = XQspiPs_CfgInitialize(&QspiInstance, QspiConfig,
					QspiConfig->BaseAddress);
	if (XST_SUCCESS != status) {
		debug("Qspi Init Failed In CfgInitialize In %s Files,%d Line\n",__FILE__,__LINE__);
		return INIT_FAILED;
	}

	/* Perform A Self-Test To Check Hardware Build */
	status = XQspiPs_SelfTest(&QspiInstance);
	if (XST_SUCCESS != status) {
		debug("Qspi Init Failed In SelfTest In %s Files,%d Line\n",__FILE__,__LINE__);
		return INIT_FAILED;
	}

	/* Set The Pre-scaler For Qspi Clock */
	status = XQspiPs_SetClkPrescaler(&QspiInstance, XQSPIPS_CLK_PRESCALE_8);
	if (XST_SUCCESS != status) {
		debug("Qspi Init Failed In SetClkPrescaler In %s Files,%d Line\n",__FILE__,__LINE__);
		return INIT_FAILED;
	}

	/*
	   ** Set Manual Start And Manual Chip Select Options And Drive The
	   ** HOLD_B high.
    */
	status = XQspiPs_SetOptions(&QspiInstance, XQSPIPS_FORCE_SSELECT_OPTION |
			XQSPIPS_MANUAL_START_OPTION | XQSPIPS_HOLD_B_DRIVE_OPTION);
	if (XST_SUCCESS != status) {
		debug("Qspi Init Failed In SetOptions In %s Files,%d Line\n",__FILE__,__LINE__);
		return INIT_FAILED;
	}

	if(QspiConfig->ConnectionMode == XQSPIPS_CONNECTION_MODE_STACKED) {
		/*
		  ** Enable Two Flash Memories, Shared Bus (NOT Separate Bus),
		  ** L_PAGE Selected By Default
		*/
		XQspiPs_SetLqspiConfigReg(&QspiInstance, DUAL_STACK_CONFIG_WRITE);

	}

	if(QspiConfig->ConnectionMode == XQSPIPS_CONNECTION_MODE_PARALLEL) {
		/* Enable Two Flash Memories On Separate Buses */
		XQspiPs_SetLqspiConfigReg(&QspiInstance, DUAL_QSPI_CONFIG_WRITE);
	}

	/* Assert The Flash Chip Select */
	status = XQspiPs_SetSlaveSelect(&QspiInstance);
	if (XST_SUCCESS != status) {
		debug("Qspi Init Failed In SetSlaveSelect In %s Files,%d Line\n",__FILE__,__LINE__);
		return INIT_FAILED;
	}

	return INIT_SUCCESS;
}


/*
  ** Function Name        : Qspi_Read
  ** Function Description : Qspi Bus Read And Write Data
  ** Function Parameter   : Writedata 			: Write Data Address
  ** 					    Readdata            : Read Data Address
  ** 					    Length              : Data Length
  ** Function Return      : WRITE_READ_SUCCESS  : Write And Read Success
  ** 						WRITE_READ_FAILED   : Write And Read Failed
  ** Notes				  : None
*/
u8 Qspi_Write_Read(u8 * Writedata, u8 * Readdata, u32 Length)
{
	if (XST_SUCCESS != XQspiPs_PolledTransfer(&QspiInstance, Writedata,\
			Readdata,Length)) {
		debug("Qspi Write And Read Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		return WRITE_READ_FAILED;
	}
	return WRITE_READ_SUCCESS;
}


/*
  ** Function Name        : Get_Qspi_Point
  ** Function Description : Get Point Qspi Pointer
  ** Function Parameter   : Void
  ** Function Return      : XQspiPs *           : The Pointer To Qspi Bus Point
  ** Notes				  : None
*/
XQspiPs * Get_Qspi_Point(void)
{
	return &QspiInstance;
}


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
