/*
  ** @file           : system_state.h
  ** @brief          : System State Header File
  **
  ** @attention
  **
  ** Copyright (c) 2020 ChengDuHangke.
  ** All rights reserved.
  **
  ** This software is licensed by ChengDuHangke under Ultimate Liberty license
  **
*/


#ifndef SRC_INCLUDE_SYSTEM_STATE_H_
#define SRC_INCLUDE_SYSTEM_STATE_H_


/*
  ** Include
*/
#include "xil_types.h"


/*
  ** Typedef Enum
*/
typedef enum Sys_State {
	/* System Idle State */
	SYS_IDLE = 0x00,
	/* System Running State */
	SYS_RUNNING = 0x01,
	/* System Stop State */
	SYS_STOP = 0x02,
}Sys_State_t;

typedef enum Aging_State {
	/* Aging Wait State */
	AGING_WAIT = 0x00,
	/* Aging Do State */
	AGING_DO = 0x01,
	/* Aging Pause State */
	AGING_PAUSE = 0x02,
	/* Aging Continuance State */
	AGING_CTE = 0x03,
	/* Aging Complete State */
	AGING_CPL = 0x04,
	/* Aging Stop State */
	AGING_STOP = 0x05,
	/* Aging Ready State */
	AGING_READY = 0x06,
	/* error protect */
	ERROR_PROTECT = 0x10,
	/* DPS Power On */
	DPS_PWR_ON = 0x20,
	/* DPS Power Off */
	DPS_PWR_OFF = 0x21,
	/* FPGA Init */
	FPGA_INIT = 0x80,
}Aging_State_t;

typedef enum Dps_Protect {
	/* DPS1 Set Error */
	DPS1_SET_ERR = (0x1 << 0),
	/* DPS2 Set Error */
	DPS2_SET_ERR = (0x1 << 1),
	/* DPS3 Set Error */
	DPS3_SET_ERR = (0x1 << 2),
	/* DPS4 Set Error */
	DPS4_SET_ERR = (0x1 << 3),
	/* DPS5 Set Error */
	DPS5_SET_ERR = (0x1 << 4),
	/* DPS6 Set Error */
	DPS6_SET_ERR = (0x1 << 5),
	/* DPS7 Set Error */
	DPS7_SET_ERR = (0x1 << 6),
	/* DPS8 Set Error */
	DPS8_SET_ERR = (0x1 << 7),
	/* DPS9 Set Error */
	DPS9_SET_ERR = (0x1 << 8),
	/* DPS10 Set Error */
	DPS10_SET_ERR = (0x1 << 9),
	/* DPS11 Set Error */
	DPS11_SET_ERR = (0x1 << 10),
	/* DPS12 Set Error */
	DPS12_SET_ERR = (0x1 << 11),
	/* DPS13 Set Error */
	DPS13_SET_ERR = (0x1 << 12),
	/* DPS14 Set Error */
	DPS14_SET_ERR = (0x1 << 13),
	/* DPS15 Set Error */
	DPS15_SET_ERR = (0x1 << 14),
	/* DPS16 Set Error */
	DPS16_SET_ERR = (0x1 << 15),
	/* ADC Init Abnormal */
	ADC_INIR_ABNAL = (0x1 << 16),
}Dps_Protect_t;


typedef enum DPS_State_Enum {
	/* DPS Channel Normal */
	DPS_NORMAL = 0x00,
	/* DPS CHannel Communication Abnormal*/
	DPS_COM_ABNAL = 0x01,
	/* DPS Channel Over Voltage Abnormal*/
	DPS_OVVOL_ABNAL = 0x02,
	/* DPS Channel Under Voltage Abnormal */
	DPS_UDVOL_VOL = 0x04,
	/* DPS CHannel Over Current Abnormal*/
	DPS_OVTCUR_ABNAL = 0x08,
	/* DPS CHannel Over Temperature Abnormal*/
	DPS_OVTEMP_ABNAL = 0x10,
	/* DPS CHannel Input Over Voltage Abnormal*/
	DPS_IOVVOL_ABNAL = 0x20,
	/* DPS CHannel Input Under Voltage Abnormal*/
	DPS_IUDVOL_VOL = 0x40,
	/* DPS Power Switch ON */
	DPS_PWR_SWH_ON = 0x80,
	/* DPS Voltage Abnormal */
	DPS_VOL_ABNEL = 0x100,
}DPS_State_Enum_t;

enum{
	RUNNING = 0x00,
	RUNOK   = 0x01,
	WAITRUN = 0xff,
};


typedef enum {
	NONE	= 0x0,	//�ղ���
	AGE_OK  = 0x1,	//�ϻ����
	M_STOP  = 0x2,	//�ֶ�ֹͣ
	M_PAUSE = 0X3,	//�ֶ���ͣ
}Stop_Even_t;


typedef enum {
	M_START = 0x1,	//ֱ�ӿ�ʼ
	RESUME  = 0X3,	//�����ϻ�
}Start_Even_t;


typedef enum {
	FIRST_ST = 0x1,
	FIRST_SP = 0x2
}First_flag;


typedef enum {
	RESTART  = 0x1,
}Start_flag;


/*
  ** Function Prototype
*/
void System_State_Process_Task(void *p);
void System_State_Set(Sys_State_t stat);
void System_Set_StopEven(Stop_Even_t even);
  u8 System_Get_StopEven(void);
  u8 System_Get_StopFlag(void);
void System_Clr_StopFlag(void);

void System_Set_StartEven(Start_Even_t even);
  u8 System_Get_StartEven(void);

void Set_Startflag(Start_flag flag);
  u8 Get_Startflag(void);

  u8 Get_FirstFlag(void);
  void Set_FirstFlag(First_flag flag);

  void SetFpgaConfig(void);
  u8 GetFpgaConfig(void);
  void ClearFpgaConfig(void);

  u8 Set_Dps_Enable_Flag(void);
  u8 Get_Dps_Enable_Flag(void);
  u8 Clear_Dps_Enable_Flag(void);
  u8 System_State_Get(void);



#endif /* SRC_INCLUDE_SYSTEM_STATE_H_ */


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
