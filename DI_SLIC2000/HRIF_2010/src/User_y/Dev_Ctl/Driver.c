

#include "Driver.h"
#include "ad5694.h"
#include "isl28023.h"
#include "SystemInfo.h"
#include "shell_port.h"
#include "system_state.h"



//同锟斤拷时锟斤拷
void Time_Sync(void * recv, u16 Size)
{
	u8 *ptr = (u8 *)recv;
	memcpy(SysTime.Data, ptr, 8);
}

//锟斤拷锟矫诧拷锟斤拷锟斤拷锟斤拷
void Set_Sample(_SAMP_  * samp)
{

}

void Build_Temp_Sample(short SampleTemp[])
{
	short i = 0;
	for( i = 0; i < DPS_NUM; i++)
	{
		SampleTemp[i] = (short)((35.6)*100);
	}
}

//0XA157 锟斤拷取锟斤拷锟斤拷锟斤拷锟斤拷          锟藉卡锟斤拷锟斤拷    锟斤拷锟阶刺�      锟较伙拷状态
void Set_Sample_packge(Sample_Data *Sample)
{
	//Sample->AgeTime      = AGETIME;
	//Sample.BoardCode   = BoardCode;
	//Sample.BoardStatus = BoardStatus;
	Build_Dps_Info(Sample->DpsVI, Sample->DpsVO, Sample->DpsIO, Sample->DpsTemp, Sample->DpsStat);
	//Build_Temp_Sample(Sample->TempSamp);
}


//锟斤拷锟斤拷64pinDHV
void Set_DHV64(float Vo1, float Vo2)
{
	Set_AD5694(Vo1, 2, dps_info.AD5694_SlaveAddr, dps_info.DBMsg[0].Chan);
	Set_AD5694(Vo2, 3, dps_info.AD5694_SlaveAddr, dps_info.DBMsg[0].Chan);
}

void Set_DHV32(struct _REG_ * Cfg, short Vo1, short Vo2)
{
	Max528_Set_DAC(Cfg, Vo1, DAC3, 0);
	Max528_Set_DAC(Cfg, Vo2, DAC5, 1);
}

void Set_DTV32(struct _REG_ * Cfg, short Vo1, short Vo2)
{
	Max528_Set_DAC(Cfg, Vo1, DAC4, 0);
	Max528_Set_DAC(Cfg, Vo2, DAC2, 1);
}

void Set_DLV32(struct _REG_ * Cfg, short Vo1, short Vo2)
{
	Max528_Set_DAC(Cfg, Vo1, DAC5, 0);
	Max528_Set_DAC(Cfg, Vo2, DAC4, 1);
}

void Set_CPHV32(struct _REG_ * Cfg, short Vo1, short Vo2)
{
	Max528_Set_DAC(Cfg, Vo1, DAC1, 0);
	Max528_Set_DAC(Cfg, Vo2, DAC3, 1);
}

void Set_CPLV32(struct _REG_ * Cfg, short Vo1, short Vo2)
{
	Max528_Set_DAC(Cfg, Vo1, DAC2, 0);
	Max528_Set_DAC(Cfg, Vo2, DAC1, 1);
}

//锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟�
u32 db_Conf(void)
{
	u16 db = 0x0;								//pin 小锟斤拷   64
	Dri_Arg * DB_cfg = (Dri_Arg *)FPGA_CFG1_DATA_ADDR;

	shellprintln("    freq:%x", DB_cfg->freq);
	for(db = 0; db < DRIV_BOARD_NUM; db++) {
		Fpga_Init(&(fpga_obj.Driver[db].Reg), LVDS_MODE);
		//执锟斤拷锟斤拷锟斤拷
		DDR_Period(&(fpga_obj.Driver[db].Reg), DB_cfg->freq);

		if(db < 3)				//64pin锟斤拷锟斤拷
			Set_DHV64((float)(DB_cfg->S_DHV1)/1000, (float)(DB_cfg->S_DHV2)/1000);
		else if(db == 3)		//32pin锟斤拷锟斤拷
		{
			Set_DHV32 (&(fpga_obj.Driver[db].Reg), DB_cfg->D_DHV1,  DB_cfg->D_DHV2 );
			Set_DLV32 (&(fpga_obj.Driver[db].Reg), DB_cfg->D_DLV1,  DB_cfg->D_DLV2 );
			Set_CPHV32(&(fpga_obj.Driver[db].Reg), DB_cfg->D_CPHV1, DB_cfg->D_CPHV2);
			Set_CPLV32(&(fpga_obj.Driver[db].Reg), DB_cfg->D_CPLV1, DB_cfg->D_CPLV2);
		}
		else if(db == 4)		//32pin锟斤拷锟斤拷
		{
			Set_DHV32 (&(fpga_obj.Driver[db].Reg), DB_cfg->D_DHV3,  DB_cfg->D_DHV4 );
			Set_DLV32 (&(fpga_obj.Driver[db].Reg), DB_cfg->D_DLV3,  DB_cfg->D_DLV4 );
			Set_CPHV32(&(fpga_obj.Driver[db].Reg), DB_cfg->D_CPHV3, DB_cfg->D_CPHV4);
			Set_CPLV32(&(fpga_obj.Driver[db].Reg), DB_cfg->D_CPLV3, DB_cfg->D_CPLV4);
		}
	}

	return SUCCESS;
}

//pin锟脚诧拷锟斤拷锟斤拷锟斤拷
u32 pin_Conf(void)
{
	u16 db = 0x0, pin = 0x0, j = 0x0;
	pin_Arg * DB_pin = (pin_Arg *)FPGA_CFG2_DATA_ADDR;

	for(j = 0x0; j < PIN_NUM; j++) {  				//前锟斤拷锟斤拷为64pin, 锟斤拷锟斤拷锟斤拷为32pin
		db  = (j < 192) ? (j/64):(((j-192)/32)+3);	//锟藉卡锟斤拷锟�
		pin = (j < 192) ? (j%64):((j-192)%32);		//锟节诧拷pin锟脚猴拷
		//IO锟戒化位锟斤拷
		IO_Var_WOpt(&(fpga_obj.Driver[db].Reg), pin%64, LE, DB_pin->Arg[j].Roll_LE);

		IO_Var_WOpt(&(fpga_obj.Driver[db].Reg), pin%64, TE, DB_pin->Arg[j].Roll_TE);

		//IO锟饺斤拷位锟斤拷
		if(db > 2){//锟斤拷锟�64pin锟斤拷要写锟饺斤拷位锟斤拷
			IO_Cmp_WOpt(&(fpga_obj.Driver[db].Reg), pin%64, LE, DB_pin->Arg[j].Comp_LE);

			IO_Cmp_WOpt(&(fpga_obj.Driver[db].Reg), pin%64, TE, DB_pin->Arg[j].Comp_TE);

		}
	}
	return SUCCESS;
}

//锟斤拷锟叫板卡锟斤拷锟斤拷为64pin,32通锟斤拷锟斤拷32位锟斤拷锟诫不锟斤拷锟斤拷
u32 Mask_Conf(void)
{
	u32 *pinMask = (u32 *)PIN_MASK_INFO;
	u16 j = 0;

	for(j = 0; j < 6; j+=2)
	{
		IO_SetMasK(&(fpga_obj.Driver[j/2].Reg), pinMask[j], pinMask[j+1]);
	}

	IO_SetMasK(&(fpga_obj.Driver[3].Reg), pinMask[6], 0);
	IO_SetMasK(&(fpga_obj.Driver[4].Reg), pinMask[7], 0);

	return SUCCESS;
}

//锟斤拷锟皆板卡ddr
void TEST_DDR(void)
{

}


//锟斤拷锟斤拷锟斤拷12V锟斤拷源锟斤拷锟斤拷
s32 Switch_12V(SW ON_OFF)
{
	u32 Status = FAILURE;
	Status = IO_Level_Set(&(bbd_info.Ctl_Output), Group1, pin4, ON_OFF, bbd_info.IO_SlaveAddr, bbd_info.TOChan);
	if(FAILURE == Status)
		return FAILURE;

	return SUCCESS;
}

s32 Set_IO_Level(SW ON_OFF, IO_pin io)
{
	u32 Status = FAILURE;
	Status = IO_Level_Set(&(bbd_info.Ctl_Output), io/8, io, ON_OFF, bbd_info.IO_SlaveAddr, bbd_info.TOChan);
	if(FAILURE == Status)
		return FAILURE;

	return SUCCESS;
}

//锟斤拷取IP
s32 ReadBoardIP(u8 *Level)
{
	if(IO_Group_Get(Group0, Level, mbd_info.DIPETH_SlaveAddr, mbd_info.HWChan))
		return FAILURE;

	return SUCCESS;
}


//void ReadDpsOnOff_flag(SW on_off)
//{
//	u16 i = 0, j = 0, id = 0x0;
//	short ret = -1;
//	Dps_Arg * Dps_ptr = (Dps_Arg * )DPS_CFG_DATA_ADDR;
//
//	for(i = 0; i < 2; i++)	//锟斤拷锟斤拷锟较碉拷
//	{
//		for(j = 0; j < (DPS_NUM/2)-2; j++,id++)
//		{
//			if(LINE_MODE == Dps_ptr->Dsp[id].Open.Mode)
//			{
//				if(j < ((DPS_NUM/2) - 2))
//				{
//					ret = ReadON_OFF(id, dps_info.DBMsg[i].DpsAddr[j], dps_info.DBMsg[i].Chan);
//					if(ret >= 0)
//					{
//						if(ON == on_off)
//							dps_info.DBMsg[i].OpenMask |= 0x1 << id;
//						else
//							dps_info.DBMsg[i].CloseMask |= 0x1 << id;
//					}
//					else
//						continue;
//				}
//				else
//				{
//					ret = ReadON_OFF(id, dps_info.AD5694_SlaveAddr, dps_info.DBMsg[i].Chan);
//					if(ret >= 0)
//					{
//						if(ON == on_off)
//							dps_info.DBMsg[i].OpenMask |= 0x1 << id;
//						else
//							dps_info.DBMsg[i].CloseMask |= 0x1 << id;
//					}
//					else
//						continue;
//				}
//			}
//		}
//	}
//}




