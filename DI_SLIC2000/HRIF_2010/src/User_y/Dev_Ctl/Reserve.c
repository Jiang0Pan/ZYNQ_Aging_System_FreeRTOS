


#include "GlobalRef.h"

#define		Arry2U16(ptr)	(((*(ptr+0)) <<  8)|((*(ptr+1)) << 0))
#define		Arry2U32(ptr)	(((*(ptr+0)) << 24)|((*(ptr+1)) << 16)|((*(ptr+2)) <<  8)|((*(ptr+3)) << 0))

float Arry2F32(u8 *ptr)
{
	u16 udata = Arry2U16(ptr);
	return (udata*1000.0);
}


Dri_Arg DB_cfg;
pin_Arg DB_pin;
Dps_Arg	Cfg_Info;


//0XA050
void Recv_Conf_Arg(void * recv)
{
	u8 i = 0, j = 0;
	u8 *Data = (u8 *)recv;

	for(i = 0 ; i < DPS_NUM; i++) {
		Cfg_Info.Dsp[i].SetValue    = Arry2F32(Data+j);j += 2;
		Cfg_Info.Dsp[i].DpsMaxV     = Arry2F32(Data+j);j += 2;
		Cfg_Info.Dsp[i].DpsMinV     = Arry2F32(Data+j);j += 2;
		Cfg_Info.Dsp[i].InputMaxV   = Arry2F32(Data+j);j += 2;
		Cfg_Info.Dsp[i].InputMinV   = Arry2F32(Data+j);j += 2;
		Cfg_Info.Dsp[i].Protect_I   = Arry2F32(Data+j);j += 2;

		Cfg_Info.Dsp[i].Open.Mode   = Data[j];j += 1;
		Cfg_Info.Dsp[i].Open.STime  = Arry2U32(Data+j);j += 4;
		Cfg_Info.Dsp[i].Open.Step   = Arry2F32(Data+j);j += 2;
		Cfg_Info.Dsp[i].Open.Keep   = Arry2F32(Data+j);j += 2;

		Cfg_Info.Dsp[i].Close.Mode  = Data[j];j += 1;
		Cfg_Info.Dsp[i].Close.STime = Arry2U32(Data+j);j += 4;
		Cfg_Info.Dsp[i].Close.Step  = Arry2F32(Data+j);j += 2;
		Cfg_Info.Dsp[i].Close.Keep  = Arry2F32(Data+j);j += 2;
	}

	Cfg_Info.TempSet    = Arry2F32(Data+j);j += 2;
	Cfg_Info.AgeBoardID = Arry2U16(Data+j);j += 2;
	Cfg_Info.AgeTime    = Arry2U32(Data+j);j += 4;
	Cfg_Info.Templimit  = Arry2F32(Data+j);j += 2;
}

//读取驱动板配置参数
void Recv_DBConf(void *recv, u16 Size)
{
	u16 j = 0;
	u8 *ptr = (u8 *)recv;

	DB_cfg.freq    = Arry2U32(ptr+j); j += 4;
	DB_cfg.S_DHV1  = Arry2F32(ptr+j); j += 2;
	DB_cfg.S_DLV1  = Arry2F32(ptr+j); j += 2;
	DB_cfg.S_DHV2  = Arry2F32(ptr+j); j += 2;
	DB_cfg.S_DLV2  = Arry2F32(ptr+j); j += 2;
	DB_cfg.D_DHV1  = Arry2F32(ptr+j); j += 2;
	DB_cfg.D_DLV1  = Arry2F32(ptr+j); j += 2;
	DB_cfg.D_DHV2  = Arry2F32(ptr+j); j += 2;
	DB_cfg.D_DLV2  = Arry2F32(ptr+j); j += 2;
	DB_cfg.D_DHV3  = Arry2F32(ptr+j); j += 2;
	DB_cfg.D_DLV3  = Arry2F32(ptr+j); j += 2;
	DB_cfg.D_DHV4  = Arry2F32(ptr+j); j += 2;
	DB_cfg.D_DLV4  = Arry2F32(ptr+j); j += 2;
	DB_cfg.D_CPHV1 = Arry2F32(ptr+j); j += 2;
	DB_cfg.D_CPLV1 = Arry2F32(ptr+j); j += 2;
	DB_cfg.D_CPHV2 = Arry2F32(ptr+j); j += 2;
	DB_cfg.D_CPLV2 = Arry2F32(ptr+j); j += 2;
	DB_cfg.D_CPHV3 = Arry2F32(ptr+j); j += 2;
	DB_cfg.D_CPLV3 = Arry2F32(ptr+j); j += 2;
	DB_cfg.D_CPHV4 = Arry2F32(ptr+j); j += 2;
	DB_cfg.D_CPLV4 = Arry2F32(ptr+j); j += 2;
}

//读取pin脚参数
void Recv_pinConf(void *recv, u16 Size)
{
	u16 i = 0, j = 0;
	u8 *ptr = (u8 *)recv;

	for(i = 0; i < PIN_NUM; i++)
	{
		DB_pin.Arg[i].Roll_LE = Arry2U32(ptr+j);
		j += 4;
		DB_pin.Arg[i].Roll_TE = Arry2U32(ptr+j);
		j += 4;
		DB_pin.Arg[i].Comp_LE = Arry2U32(ptr+j);
		j += 4;
		DB_pin.Arg[i].Comp_TE = Arry2U32(ptr+j);
		j += 4;
	}
}






























