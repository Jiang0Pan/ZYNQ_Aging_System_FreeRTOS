/*
  ** @file           : mt25ql512.h
  ** @brief          : The Device Of MT25QL512(QSPI FLASH Device) Header File
  **
  ** @attention
  **
  ** Copyright (c) 2020 ChengDuHangke.
  ** All rights reserved.
  **
  ** This software is licensed by ChengDuHangke under Ultimate Liberty license
  **
*/


#ifndef MT25QL512_H_
#define MT25QL512_H_


/*
  ** Include
*/
#include "xil_types.h"


/*
  ** Define
*/
/*
  ** The following constants define the commands which may be
  ** sent to the Flash device.
*/
#define 	WRITE_STATUS_CMD	(0x01)
#define 	WRITE_CMD			(0x02)
#define 	READ_CMD			(0x03)
#define 	WRITE_DISABLE_CMD	(0x04)
#define 	READ_STATUS_CMD		(0x05)
#define 	WRITE_ENABLE_CMD	(0x06)
#define 	FAST_READ_CMD		(0x0B)
#define 	DUAL_READ_CMD		(0x3B)
#define 	QUAD_READ_CMD		(0x6B)
#define 	BULK_ERASE_CMD		(0xC7)
#define		SEC_ERASE_CMD		(0xD8)
#define 	READ_ID				(0x9F)
#define 	READ_CONFIG_CMD		(0x35)
#define 	WRITE_CONFIG_CMD	(0x01)
#define 	BANK_REG_RD			(0x16)
#define 	BANK_REG_WR			(0x17)
/* Bank register is called Extended Address Register in Micron */
#define 	EXTADD_REG_RD		(0xC8)
#define 	EXTADD_REG_WR		(0xC5)
#define		DIE_ERASE_CMD		(0xC4)
#define 	READ_FLAG_STS_CMD	(0x70)


/*
  ** Function Prototype
*/
u8 * Get_Write_Buffer(void);
u8 * Get_Read_Buffer(void);
u8 Mt25ql512_Init(void);
u32 Get_Page_Count(u32 Length);
u32 Get_Page_Size(void);
u8 Erase_Flash(u32 Address, u32 ByteCount, u8 *WriteBfrPtr);
u8 Write_Flash(u32 Address, u32 ByteCount, u8 Command, u8 *WriteBfrPtr);
u8 Read_Flash_ID(u8 *WriteBfrPtr, u8 *ReadBfrPtr);
u8 Read_Flash(u32 Address, u32 ByteCount, u8 Command, u8 *WriteBfrPtr, u8 *ReadBfrPtr);
u8 SendBankSelect(u8 *WriteBfrPtr, u32 BankSel);
u8 Erase_Bulk(u8 *WriteBfrPtr);
u8 Erase_Die(u8 *WriteBfrPtr);
u32 GetRealAddr(u32 Address);


#endif /* MT25QL512_H_ */


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
