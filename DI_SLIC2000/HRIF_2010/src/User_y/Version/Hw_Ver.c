




#include "Hw_ver.h"



/////////////////////////////////////////////////////////////////////////////////

/***********************************************************************
	[��	       ��	]��ȡӲ���汾��ϢHW Version: 0001 Board Type: 0001
	[��            ��	]HW_info:�汾��Ϣ, SlaveAddr:�ӻ���ַ, Chan:ͨ����
	[��    ��    ֵ	]�Ƿ�ɹ�
***********************************************************************/
s32 ReadIO_Info(u8 *HW_info, u8 SlaveAddr,  IIC_Chan chan)
{
	if(IO_Group_Get(Group0, HW_info, SlaveAddr, chan))
		return FAILURE;

	return SUCCESS;
}










