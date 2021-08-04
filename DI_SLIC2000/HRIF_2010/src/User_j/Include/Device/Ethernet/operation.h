/*
  ** @file           : operation.h
  ** @brief          : Tcp Data Operation Header File
  **
  ** @attention
  **
  ** Copyright (c) 2020 ChengDuHangke.
  ** All rights reserved.
  **
  ** This software is licensed by ChengDuHangke under Ultimate Liberty license
  **
*/


#ifndef OPERATION_H_
#define OPERATION_H_


typedef u32 Cfg_Mask;
extern volatile Cfg_Mask cfgmask;

#define GETCFGBIT(bit)		((cfgmask >> (bit))&0x1)
#define SETCFGBIT(bit)		(cfgmask |= (0x1 << (bit)))
#define CLRCFGBIT(bit)		(cfgmask &= ~(0x1 << (bit)))


/*
  ** Function Prototype
*/
u8 Set_Dps_Cfg_Flag(void);
u8 Get_Dps_Cfg_Flag(void);
u8 Clear_Dps_Cfg_Flag(void);

u8 Set_FPGA_Cfg_Flag(void);
u8 Get_FPGA_Cfg_Flag(void);
u8 Clear_FPGA_Cfg_Flag(void);
u32 Get_Cycle_Size(void);
u32 Get_Cycle_Num(void);
void Set_Cycle_Size(u32 size);
u8 Data_Operation(void * Data);
void NetDataCopy(u8 *dest, u8 *src, size_t size);


#endif /* OPERATION_H_ */


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
