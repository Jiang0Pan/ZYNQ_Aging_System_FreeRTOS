/*
  ** @file           : SMBus.h
  ** @brief          : The IIC BUS Driver Header File
  **
  ** @attention
  **
  ** Copyright (c) 2020 ChengDuHangke.
  ** All rights reserved.
  **
  ** This software is licensed by ChengDuHangke under Ultimate Liberty license
  **
*/


#ifndef _SMBUS_H
#define _SMBUS_H


/*
  ** Include
*/
#include "iic.h"


/*
  ** Function Prototype
*/
s32 SMBus_Write(u8 *Wbuf, u8 WSize, u8 SlaveAddr, u8 Code, IIC_Chan Chan);
s32 SMBus_Read(u8 *Rbuf, u8 RSize, u8 SlaveAddr, u8 Code, IIC_Chan Chan);
s32 SMBus_Code(u8 SlaveAddr, u8 Code, IIC_Chan Chan);


#endif


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
