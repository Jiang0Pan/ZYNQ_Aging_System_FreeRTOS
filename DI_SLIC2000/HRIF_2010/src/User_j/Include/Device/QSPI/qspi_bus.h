/*
  ** @file           : qspi_bus.h
  ** @brief          : Qspi Bus Interface Header File
  **
  ** @attention
  **
  ** Copyright (c) 2020 ChengDuHangke.
  ** All rights reserved.
  **
  ** This software is licensed by ChengDuHangke under Ultimate Liberty license
  **
*/


#ifndef SRC_INCLUDE_DEVICE_QSPI_QSPI_BUS_H_
#define SRC_INCLUDE_DEVICE_QSPI_QSPI_BUS_H_


/*
  ** Include
*/
#include "xil_types.h"
#include "xqspips.h"


/*
  ** Function Prototype
*/
u8 Qspi_Init(void);
u8 Qspi_Write_Read(u8 * Writedata, u8 * Readdata, u32 Length);
XQspiPs * Get_Qspi_Point(void);


#endif /* SRC_INCLUDE_DEVICE_QSPI_QSPI_BUS_H_ */


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
