



#ifndef _EEPROM_H
#define _EEPROM_H


#include "Hw_ver.h"



#define  	IPINFO_ADDR		(0x0040)


/////////////////////////////////////////////////////////////////////
//清除写保护
void CLEAR_WP(_Output_ * IO, u8 SlaveAddr, IIC_Chan i2cX);
//加入写保护
void ADD_WP  (_Output_ * IO, u8 SlaveAddr, IIC_Chan i2cX);
//判断地址是否合法
u8 Addr_Legal(u16 Addr);

//eeprom读取一个字节
s32 EEPROM_ReadByte(u16 Addr, u8 *Byte, u8 SlaveAddr, IIC_Chan i2cX);

//eeprom写一个字节
s32 EEPROM_WriteByte(u16 Addr, u8 Byte, u8 SlaveAddr, IIC_Chan i2cX);

//eeprom读取n个字节(n < 64)
s32 EEPROM_ReadBuff(u16 Addr, u8 *Buff, u8 Size, u8 SlaveAddr, IIC_Chan i2cX);

//eeprom写n个字节(n < 64)
s32 EEPROM_WriteBuff(u16 Addr, u8 *Buff, u8 Size, u8 SlaveAddr, IIC_Chan i2cX);

//擦除一片区域
s32 EEPROM_EraseBuff(u16 Addr, u8 Size, u8 SlaveAddr, IIC_Chan i2cX);

u8 CRC8(u8 *ptr, u8 len);

//读取IP地址
s32 Read_IPAddr(u8 SlaveAddr, IIC_Chan i2cX);

//存取IP地址
s32 Set_IPAddr(u8 ip, u8 SlaveAddr, IIC_Chan i2cX);























#endif













































