#ifndef _SPI_H_
#define _SPI_H_



#include "xspips.h"
#include "error.h"
#include "debug_config.h"
#include "return_val.h"


#define	nop()      							asm("nop") 			//延时一个指令周期

typedef enum {
	SPI0 = 0,
	SPI1 = 1,
}Spi;


#define SpiPs_RecvByte(BaseAddress) 		((u8)XSpiPs_In32((BaseAddress) + XSPIPS_RXD_OFFSET))
#define SpiPs_SendByte(BaseAddress, Data) 	(XSpiPs_Out32((BaseAddress) + XSPIPS_TXD_OFFSET, (Data)))


////////////////////////////////////////////////////////////////////////////
 s32 Spi_Init(void);						//初始化spi

 s32 Spi_Transfer(u8 *Tx, u8 *Rx, u8 size, Spi spix);

 u8 Spi_ReadWrite(u8 *Txdata ,u8 *Rxdata, u32 Length);

#endif



