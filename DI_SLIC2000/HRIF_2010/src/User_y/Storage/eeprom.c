#include "eeprom.h"
#include <malloc.h>
#include "unistd.h"



/***********************************************************************
	[功	       能	]eeprom读取一个字节
	[参            数	]Addr:读取地址, Byte:缓存, SlaveAddr:从机地址, i2cX:iic通道
	[返    回    值	]读取结果
***********************************************************************/
s32 EEPROM_ReadByte(u16 Addr, u8 *Byte, u8 SlaveAddr, IIC_Chan i2cX)
{
	s32 Status = FAILURE;
	u8 buf[2] = {0x0};
	buf[0] = (Addr >> 8) & 0xff;				//高地址在前
	buf[1] = Addr & 0xff;

	Status = IIC_Write(buf, 2, SlaveAddr, i2cX);//发送地址
	if(Status == FAILURE)
		return FAILURE;

	Status = IIC_Read(Byte, 1, SlaveAddr, i2cX);//读取数据
	if(Status == FAILURE)
		return FAILURE;

	return SUCCESS;
}

/***********************************************************************
	[功	       能	]eeprom写一个字节
	[参            数	]Addr:读取地址, Byte:缓存, SlaveAddr:从机地址, i2cX:iic通道
	[返    回    值	]写入结果
***********************************************************************/
s32 EEPROM_WriteByte(u16 Addr, u8 Byte, u8 SlaveAddr, IIC_Chan i2cX)
{
	s32 Status = FAILURE;
	u8 buf[3] = {0x0, 0x0, Byte};
	buf[0] = (Addr >> 8) & 0xff;				//高地址在前
	buf[1] = Addr & 0xff;

	Status = IIC_Write(buf, 3, SlaveAddr, i2cX);//发送地址及数据
	if(Status == FAILURE)
		return FAILURE;

	return SUCCESS;
}

/***********************************************************************
	[功	       能	]eeprom读取n个字节(n < 64)
	[参            数	]Addr:读取地址, Byte:缓存, Size:大小, SlaveAddr:从机地址, i2cX:iic通道
	[返    回    值	]读取结果
***********************************************************************/
s32 EEPROM_ReadBuff(u16 Addr, u8 *Buff, u8 Size, u8 SlaveAddr, IIC_Chan i2cX)
{
	s32 Status = FAILURE;
	u8 buf[2] = {0x0};
	buf[0] = (Addr >> 8) & 0xff;					//高地址在前
	buf[1] = (Addr >> 0) & 0xff;

	Status = IIC_Write(buf, 2, SlaveAddr, i2cX);	//发送地址
	if(Status == FAILURE)
		return FAILURE;

	Status = IIC_Read(Buff, Size, SlaveAddr, i2cX);	//读取数据
	if(Status == FAILURE)
		return FAILURE;

	return SUCCESS;
}

/***********************************************************************
	[功	       能	]eeprom写n个字节(n < 64)
	[参            数	]Addr:读取地址, Byte:缓存, Size:大小, SlaveAddr:从机地址, i2cX:iic通道
	[返    回    值	]写入结果
***********************************************************************/
s32 EEPROM_WriteBuff(u16 Addr, u8 *Buff, u8 Size, u8 SlaveAddr, IIC_Chan i2cX)
{
	s32 Status = FAILURE;

	u8 *buf = NULL;


	buf = (u8 *)malloc(Size+2);
	if(buf == NULL)
		return FAILURE;
	memset(buf, 0, Size+2);

	buf[0] = (Addr >> 8) & 0xff;	//高地址在前
	buf[1] = Addr & 0xff;
	memcpy(buf+2, Buff, Size);

	Status = IIC_Write(buf, Size+2, SlaveAddr, i2cX);//发送地址及数据
	if(Status == FAILURE)
	{
		free(buf);
		return FAILURE;
	}

	free(buf);
	usleep(10000);

	return SUCCESS;
}

/***********************************************************************
	[功	       能	]擦除一片区域
	[参            数	]Addr:读取地址, Size:大小, SlaveAddr:从机地址, i2cX:iic通道
	[返    回    值	]操作结果
***********************************************************************/
s32 EEPROM_EraseBuff(u16 Addr, u8 Size, u8 SlaveAddr, IIC_Chan i2cX)
{
	s32 Status = FAILURE;

	u8 *buf = NULL;

	buf = (u8 *)malloc(Size+2);
	if(buf == NULL)
		return FAILURE;
	memset(buf, 0, Size+2);

	buf[0] = (Addr >> 8) & 0xff;	//高地址在前
	buf[1] = Addr & 0xff;

	Status = IIC_Write(buf, Size+2, SlaveAddr, i2cX);//发送地址及数据
	if(Status == FAILURE)
	{
		free(buf);
		return FAILURE;
	}

	free(buf);
	usleep(10000);

	return SUCCESS;
}

/***********************************************************************
	[功	       能	]清除写保护
	[参            数	]IO:IO控制信息, SlaveAddr:从机地址, i2cX:iic通道
	[返    回    值	]无
***********************************************************************/
void CLEAR_WP(_Output_ * IO, u8 SlaveAddr, IIC_Chan i2cX)
{
	IO_Level_Set(IO, Group1, 0, 0, SlaveAddr, i2cX);
}

/***********************************************************************
	[功	       能	]加入写保护
	[参            数	]IO:IO控制信息, SlaveAddr:从机地址, i2cX:iic通道
	[返    回    值	]无
***********************************************************************/
void ADD_WP  (_Output_ * IO, u8 SlaveAddr, IIC_Chan i2cX)
{
	IO_Level_Set(IO, Group1, 0, 1, SlaveAddr, i2cX);
}

/***********************************************************************
	[功	       能	]判断地址是否合法
	[参            数	]IO:IO控制信息, SlaveAddr:从机地址, i2cX:iic通道
	[返    回    值	]操作结果
***********************************************************************/
u8 Addr_Legal(u16 Addr)
{
	if(Addr % PAGE_SIZE)
		return 1;
	return 0;
}

///////////////////////////////////////////////////////////////////////

/***********************************************************************
	[功	       能	]crc8校验
	[参            数	]ptr:数据指针, len:数据长度
	[返    回    值	]校验码
***********************************************************************/
u8 CRC8(u8 *ptr, u8 len)
{
    u8 i;
    u8 crc=0x00; 	/*crc值 */
    while(len--)
    {
		crc ^= *(ptr++);
		for(i=8; i > 0; --i)
		{
			if(crc & 0x80)
				crc = (crc << 1) ^ 0x31;
			else
				crc = (crc << 1);
		}
    }
    return (crc);
}

s32 Read_IPAddr(u8 SlaveAddr, IIC_Chan i2cX)
{
	u8 ipaddr = 0x0;
	if(EEPROM_ReadByte(IPINFO_ADDR, &ipaddr, SlaveAddr, i2cX))
		return -1;

	return ipaddr;
}

s32 Set_IPAddr(u8 ip, u8 SlaveAddr, IIC_Chan i2cX)
{
	if(EEPROM_WriteByte(IPINFO_ADDR, ip, SlaveAddr, i2cX))
		return FAILURE;
	usleep(10000);
	return SUCCESS;
}






