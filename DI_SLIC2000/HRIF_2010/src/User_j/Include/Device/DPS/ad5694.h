/*
  ** @file           : ad5694.h
  ** @brief          : AD5694 Device (ADC Device) Header File
  **
  ** @attention
  **
  ** Copyright (c) 2020 ChengDuHangke.
  ** All rights reserved.
  **
  ** This software is licensed by ChengDuHangke under Ultimate Liberty license
  **
*/


#ifndef AD5694_H_
#define AD5694_H_


/*
  ** Include
*/
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "xil_types.h"
#include "IIC.h"


/*
  ** Define
*/


/*
  ** Function Prototype
*/
u8 Reset_AD5694(u8 Bid);
u8 Disable_DPS_CH1_CH2(u8 Bid);
u8 Set_AD5694(float Vo, u8 N_dac, u8 SlaveAddr, IIC_Chan Chan);
u8 AD5694_Power_On(TimerHandle_t xTimer, u8 Bid);
u8 AD5694_Power_Off(TimerHandle_t xTimer, u8 Bid);
u8 Disable_DPS_CH3_CH4(u8 Bid);
void AD5694_Set_Line_Power_On_Task_Bd0(TimerHandle_t xTimer);
void AD5694_Set_Line_Power_On_Task_Bd1(TimerHandle_t xTimer);
void AD5694_Set_Line_Power_Off_Task_Bd0(TimerHandle_t xTimer);
void AD5694_Set_Line_Power_Off_Task_Bd1(TimerHandle_t xTimer);


#endif /* AD5694_H_ */


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
