#include "eeprom.h"
#include <malloc.h>
#include "unistd.h"



/***********************************************************************
	[��	       ��	]eeprom��ȡһ���ֽ�
	[��            ��	]Addr:��ȡ��ַ, Byte:����, SlaveAddr:�ӻ���ַ, i2cX:iicͨ��
	[��    ��    ֵ	]��ȡ���
***********************************************************************/
s32 EEPROM_ReadByte(u16 Addr, u8 *Byte, u8 SlaveAddr, IIC_Chan i2cX)
{
	s32 Status = FAILURE;
	u8 buf[2] = {0x0};
	buf[0] = (Addr >> 8) & 0xff;				//�ߵ�ַ��ǰ
	buf[1] = Addr & 0xff;

	Status = IIC_Write(buf, 2, SlaveAddr, i2cX);//���͵�ַ
	if(Status == FAILURE)
		return FAILURE;

	Status = IIC_Read(Byte, 1, SlaveAddr, i2cX);//��ȡ����
	if(Status == FAILURE)
		return FAILURE;

	return SUCCESS;
}

/***********************************************************************
	[��	       ��	]eepromдһ���ֽ�
	[��            ��	]Addr:��ȡ��ַ, Byte:����, SlaveAddr:�ӻ���ַ, i2cX:iicͨ��
	[��    ��    ֵ	]д����
***********************************************************************/
s32 EEPROM_WriteByte(u16 Addr, u8 Byte, u8 SlaveAddr, IIC_Chan i2cX)
{
	s32 Status = FAILURE;
	u8 buf[3] = {0x0, 0x0, Byte};
	buf[0] = (Addr >> 8) & 0xff;				//�ߵ�ַ��ǰ
	buf[1] = Addr & 0xff;

	Status = IIC_Write(buf, 3, SlaveAddr, i2cX);//���͵�ַ������
	if(Status == FAILURE)
		return FAILURE;

	return SUCCESS;
}

/***********************************************************************
	[��	       ��	]eeprom��ȡn���ֽ�(n < 64)
	[��            ��	]Addr:��ȡ��ַ, Byte:����, Size:��С, SlaveAddr:�ӻ���ַ, i2cX:iicͨ��
	[��    ��    ֵ	]��ȡ���
***********************************************************************/
s32 EEPROM_ReadBuff(u16 Addr, u8 *Buff, u8 Size, u8 SlaveAddr, IIC_Chan i2cX)
{
	s32 Status = FAILURE;
	u8 buf[2] = {0x0};
	buf[0] = (Addr >> 8) & 0xff;					//�ߵ�ַ��ǰ
	buf[1] = (Addr >> 0) & 0xff;

	Status = IIC_Write(buf, 2, SlaveAddr, i2cX);	//���͵�ַ
	if(Status == FAILURE)
		return FAILURE;

	Status = IIC_Read(Buff, Size, SlaveAddr, i2cX);	//��ȡ����
	if(Status == FAILURE)
		return FAILURE;

	return SUCCESS;
}

/***********************************************************************
	[��	       ��	]eepromдn���ֽ�(n < 64)
	[��            ��	]Addr:��ȡ��ַ, Byte:����, Size:��С, SlaveAddr:�ӻ���ַ, i2cX:iicͨ��
	[��    ��    ֵ	]д����
***********************************************************************/
s32 EEPROM_WriteBuff(u16 Addr, u8 *Buff, u8 Size, u8 SlaveAddr, IIC_Chan i2cX)
{
	s32 Status = FAILURE;

	u8 *buf = NULL;


	buf = (u8 *)malloc(Size+2);
	if(buf == NULL)
		return FAILURE;
	memset(buf, 0, Size+2);

	buf[0] = (Addr >> 8) & 0xff;	//�ߵ�ַ��ǰ
	buf[1] = Addr & 0xff;
	memcpy(buf+2, Buff, Size);

	Status = IIC_Write(buf, Size+2, SlaveAddr, i2cX);//���͵�ַ������
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
	[��	       ��	]����һƬ����
	[��            ��	]Addr:��ȡ��ַ, Size:��С, SlaveAddr:�ӻ���ַ, i2cX:iicͨ��
	[��    ��    ֵ	]�������
***********************************************************************/
s32 EEPROM_EraseBuff(u16 Addr, u8 Size, u8 SlaveAddr, IIC_Chan i2cX)
{
	s32 Status = FAILURE;

	u8 *buf = NULL;

	buf = (u8 *)malloc(Size+2);
	if(buf == NULL)
		return FAILURE;
	memset(buf, 0, Size+2);

	buf[0] = (Addr >> 8) & 0xff;	//�ߵ�ַ��ǰ
	buf[1] = Addr & 0xff;

	Status = IIC_Write(buf, Size+2, SlaveAddr, i2cX);//���͵�ַ������
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
	[��	       ��	]���д����
	[��            ��	]IO:IO������Ϣ, SlaveAddr:�ӻ���ַ, i2cX:iicͨ��
	[��    ��    ֵ	]��
***********************************************************************/
void CLEAR_WP(_Output_ * IO, u8 SlaveAddr, IIC_Chan i2cX)
{
	IO_Level_Set(IO, Group1, 0, 0, SlaveAddr, i2cX);
}

/***********************************************************************
	[��	       ��	]����д����
	[��            ��	]IO:IO������Ϣ, SlaveAddr:�ӻ���ַ, i2cX:iicͨ��
	[��    ��    ֵ	]��
***********************************************************************/
void ADD_WP  (_Output_ * IO, u8 SlaveAddr, IIC_Chan i2cX)
{
	IO_Level_Set(IO, Group1, 0, 1, SlaveAddr, i2cX);
}

/***********************************************************************
	[��	       ��	]�жϵ�ַ�Ƿ�Ϸ�
	[��            ��	]IO:IO������Ϣ, SlaveAddr:�ӻ���ַ, i2cX:iicͨ��
	[��    ��    ֵ	]�������
***********************************************************************/
u8 Addr_Legal(u16 Addr)
{
	if(Addr % PAGE_SIZE)
		return 1;
	return 0;
}

///////////////////////////////////////////////////////////////////////

/***********************************************************************
	[��	       ��	]crc8У��
	[��            ��	]ptr:����ָ��, len:���ݳ���
	[��    ��    ֵ	]У����
***********************************************************************/
u8 CRC8(u8 *ptr, u8 len)
{
    u8 i;
    u8 crc=0x00; 	/*crcֵ */
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






