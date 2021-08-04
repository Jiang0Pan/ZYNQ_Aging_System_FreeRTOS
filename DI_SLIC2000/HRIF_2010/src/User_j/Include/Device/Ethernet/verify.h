/*
  ** @file           : verify.h
  ** @brief          : Verify Data Header File
  **
  ** @attention
  **
  ** Copyright (c) 2020 ChengDuHangke.
  ** All rights reserved.
  **
  ** This software is licensed by ChengDuHangke under Ultimate Liberty license
  **
*/


#ifndef VERIFY_H_
#define VERIFY_H_


/*
  ** Include
*/
#include "xil_types.h"


/*
  ** Function Prototype
*/
u8 Verify_Data(void *Data,u32 Length);
u8 Check_Sum(void *Data,u32 Length,u32 *CheckSum);
void Crc32_Table_Init(void);
u32 Get_Crc32_Val(u32 Crc_Val, u8 * Data, u32 Length);
u32 Get_Data_Crc32_Val(void);


#endif /* VERIFY_H_ */


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
