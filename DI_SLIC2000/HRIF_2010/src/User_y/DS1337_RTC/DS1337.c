




#include "GlobalRef.h"
#include "DS1337.h"



s32 Set_Time(Time_Sync_t *time, u8 SlaveAddr, IIC_Chan Chan)
{
	s32 Status = FAILURE;
	u8 byte[2] = {0x0};

	CTRL ctr = {
		.AxIE = 0,
		.INTCN= 0,
		.RS   = 3,
		.EOSC = 1,
	};

	byte[0] = ds_ctrl_reg;
	byte[1] = ctr.Ctrl[0];

	Status = IIC_Write(byte, 2, SlaveAddr, Chan);
	if(FAILURE == Status)
		return FAILURE;

	DS_Time ti = {
		.Addr = Seconds,
		.SEC  = {
			._sec   = (time->Sec)%10,
			._10sec = (time->Sec)/10,
		},
		.MIN  = {
			._min   = (time->Min)%10,
			._10min = (time->Min)/10,
		},
		.HOU = {
			._hou   = (time->Hou)%10,
			._10hou = (time->Hou)/10,
			._wk    = 0,
		},
		.DATE = {
			._date  = (time->Day)%10,
			._10date= (time->Day)/10,
		},
		.MON  = {
			._mon   = (time->Mon)%10,
			._10mon = (time->Mon)/10,
		},
		.YEA  = {
			._yea   = (time->Yea)/10,
			._10yea = (time->Yea)/10,
		}
	};

	Status = IIC_Write(ti.Byte, 7, SlaveAddr, Chan);
	if(FAILURE == Status)
		return FAILURE;

	ctr.EOSC = 0;
	byte[0] = ds_ctrl_reg;
	byte[1] = ctr.Ctrl[0];

	Status = IIC_Write(byte, 2, SlaveAddr, Chan);
	if(FAILURE == Status)
		return FAILURE;

	return SUCCESS;
}

s32 Read_Time(DS_Time *time, u8 SlaveAddr, IIC_Chan Chan)
{
	s32 Status = FAILURE;
	STAT st = {.Stat = {0}};
	u8 addr = ds_stat_reg;

	Status = IIC_Write(&addr, 1, SlaveAddr, Chan);
		if(FAILURE == Status)
			return FAILURE;

	Status = IIC_Read(st.Stat, 1, SlaveAddr, Chan);
	if(FAILURE == Status)
		return FAILURE;

	if(st.OSF)
		return FAILURE;

	time->Addr = Seconds;

	Status = IIC_Write(time->Byte, 1, SlaveAddr, Chan);
	if(FAILURE == Status)
		return FAILURE;

	Status = IIC_Read(time->Byte+1, 7, SlaveAddr, Chan);
	if(FAILURE == Status)
		return FAILURE;


	return 0;
}









