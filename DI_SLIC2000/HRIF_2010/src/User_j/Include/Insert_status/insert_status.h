/*
  ** @file           : insert_status.h
  ** @brief          : Dual Slots board insert status information header file
  **
  ** @attention
  **
  ** Copyright (c) 2020 ChengDuHangke.
  ** All rights reserved.
  **
  ** This software is licensed by ChengDuHangke under Ultimate Liberty license
  **
*/


#ifndef INSERT_STATUS_H_
#define INSERT_STATUS_H_

/*
  ** Include
*/
#include "xil_types.h"


/*
  ** Function Prototype
*/
u8 ReadDualSlotInfo(void);
u8 ReadDualSlotStatus(Sample_Data * Sample);


#endif /* INSERT_STATUS_H_ */


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
