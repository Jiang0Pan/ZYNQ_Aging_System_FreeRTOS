





#include "spi.h"



XSpiPs psSpi[2];



/***********************************************************************
	[功	       能	]初始化spi接口
	[参            数	]无
	[返    回    值	]初始化是否成功
***********************************************************************/
s32 Spi_Init(void)
{
	s32 Status = 1;
	XSpiPs_Config *SpiConfig[2];

	SpiConfig[0] = XSpiPs_LookupConfig(SPI0);
	if (NULL == SpiConfig)
		return FAILURE;
	SpiConfig[1] = XSpiPs_LookupConfig(SPI1);
	if (NULL == SpiConfig)
		return FAILURE;

	Status = XSpiPs_CfgInitialize(&psSpi[SPI0], SpiConfig[0], SpiConfig[0]->BaseAddress);
	if (Status != SUCCESS)
		return FAILURE;
	Status = XSpiPs_CfgInitialize(&psSpi[SPI1], SpiConfig[1], SpiConfig[1]->BaseAddress);
	if (Status != SUCCESS)
		return FAILURE;

	Status = XSpiPs_SelfTest(&psSpi[SPI0]);
	if (Status != SUCCESS)
		return FAILURE;
	Status = XSpiPs_SelfTest(&psSpi[SPI1]);
	if (Status != SUCCESS)
		return FAILURE;

	//设置主/从，时钟、相位:CPOL=0,CPHA=0 XSPIPS_CPHA_OPTIONS
	Status = XSpiPs_SetOptions(&psSpi[SPI0], XSPIPS_MASTER_OPTION |
			   XSPIPS_FORCE_SSELECT_OPTION );
	if (Status != SUCCESS)
		return FAILURE;
	//XSPIPS_MANUAL_START_OPTION |
	Status = XSpiPs_SetOptions(&psSpi[SPI1], XSPIPS_MASTER_OPTION |
			XSPIPS_FORCE_SSELECT_OPTION );
	if (Status != SUCCESS)
		return FAILURE;

	Status = XSpiPs_SetClkPrescaler(&psSpi[SPI0], XSPIPS_CLK_PRESCALE_32);
	if (Status != SUCCESS)
		return FAILURE;
	Status = XSpiPs_SetClkPrescaler(&psSpi[SPI1], XSPIPS_CLK_PRESCALE_4);
	if (Status != SUCCESS)
		return FAILURE;

	XSpiPs_SetSlaveSelect(&psSpi[SPI1], 0xF);

	XSpiPs_Enable(&psSpi[SPI0]);
	XSpiPs_Enable(&psSpi[SPI1]);

	return SUCCESS;
}

/***********************************************************************
	[功	       能	]spi传输
	[参            数	]Tx:发送数据指针, Rx:接收数据指针, size:收发字节数, spix:spi编号
	[返    回    值	]传输是否ok
***********************************************************************/
s32 Spi_Transfer(u8 *Tx, u8 *Rx, u8 size, Spi spix)
{
	if(XSpiPs_PolledTransfer(&psSpi[spix], Tx, Rx, size))
		return FAILURE;

	return SUCCESS;
}


/*
  ** Function Name        : Spi_ReadWrite
  ** Function Description : Through spi interface transfer data to fpga dps
  ** Function Parameter   : t_data 			    : Send data point
  ** 						r_data              : Recv data point
  ** 						length              : Send and Recv data lengtg
  ** Function Return      : OPER_SUCCESS        : Transfer success
  ** 						OPER_FAILED         : Transfer Failed
  ** Notes				  : None
*/
u8 Spi_ReadWrite(u8 *Txdata ,u8 *Rxdata, u32 Length)
{
	if (XST_SUCCESS != XSpiPs_PolledTransfer(&psSpi[1],Txdata,Rxdata,Length)) {
		debug("Spi Interface PolledTransfer Data Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	return OPER_SUCCESS;
}


