/*
  ** @file           : system_state.c
  ** @brief          : System State Source File
  **
  ** @attention
  **
  ** Copyright (c) 2020 ChengDuHangke.
  ** All rights reserved.
  **
  ** This software is licensed by ChengDuHangke under Ultimate Liberty license
  **
*/


/*
  ** Include
*/
#include "FreeRTOS.h"
#include "task.h"
#include "return_val.h"
#include "debug_config.h"
#include "GlobalRef.h"
#include "memory_map.h"
#include "operation.h"
#include "system_state.h"
#include "Driver.h"
#include "fpga.h"
#include "ack.h"
#include "shell_port.h"
#include "dps_info.h"
#include "orderstruct.h"
#include "unistd.h"
#include "insert_status.h"
#include "ad5694.h"
#include "isl8272.h"



#define WAIT_FIFO	(10)


/*
  ** Global Variable
*/
u8 OpenFLAG  = 0x0;
u8 CloseFLAG = 0x0;

/*
  ** Static Global Variable
*/
/* System State . Default Wait State */
static volatile u8 sys_sts    = SYS_IDLE;
static volatile u8 stop_even  = NONE;
static volatile u8 start_even = NONE;
static volatile u8 start_flag = NONE;
static volatile u8 stop_flag  = NONE;
static volatile u8 first_flag = NONE;
static volatile u8 config_flag = NONE;




/* Dps Enable Flag */
static u8 dps_enable = 0x00;

static volatile u8 countover = 0;


/*
  ** Function Name        : System_State_Process_Task
  ** Function Description : System State Process Task
  ** Function Parameter   : p 					: Task Parameter()
  ** Function Return      : Void
  ** Notes				  : None
*/
void System_State_Process_Task(void *p)
{
	const TickType_t x1second = pdMS_TO_TICKS(DELAY_1_SECOND);
	static u32 SampleCount = 0;

	u16 i = 0;
	Sample_Data   * Sample    = (Sample_Data   *)SAMPLE_DATA_ADD;		//锟斤拷锟斤拷锟斤拷锟斤拷
	BackIsp_Info  * Ispinfo   = (BackIsp_Info  *)PAT_C_INFO_ADDR;		//锟截硷拷锟斤拷锟斤拷
	VecRun_t      * pat_r     = (VecRun_t      *)PAT_R_STS_ADDR;		//锟斤拷锟斤拷锟斤拷锟斤拷状态
	Dps_Arg       * dps_cfg   = (Dps_Arg       *)DPS_CFG_DATA_ADDR;		//锟斤拷源锟斤拷锟斤拷锟斤拷息
	Aging_Ctl     * aging_ctl = (Aging_Ctl     *)AGING_CTL_ADDR;		//锟较伙拷锟斤拷锟斤拷
	Vec_Err_Count * pat_err   = (Vec_Err_Count *)PAT_ERR_INFO_ADDR;		//锟斤拷锟斤拷锟斤拷锟斤拷统锟斤拷

	/* Sample Data Init */
	memset((void *)Sample,    0, sizeof(Sample_Data));
	/* error mask     */
	memset((void *)Ispinfo,   0, sizeof(BackIsp_Info));
	/* Pattern Running State */
	memset((void *)pat_r,     0, sizeof(VecRun_t));
	/* dps info */
	memset((void *)dps_cfg,   0, sizeof(Dps_Arg));
	/* aging contrl */
	memset((void *)aging_ctl, 0, sizeof(Aging_Ctl));
	/* Pattern Error Information */
	memset((void *)pat_err,   0, sizeof(Vec_Err_Count));

	/* Sample Data */
	Sample->AgeStatus = AGING_WAIT;
	pat_r->RunSta = WAITRUN;	//锟饺达拷锟斤拷锟斤拷
	u8 insert_time = 0x00;
	u8 readcount = 0x0;

	for ( ; ; )
	{
		/* Read Dual Slots Insert status */
restart:insert_time++;
		if (10 == insert_time) {
			if (READ_FAILED != ReadDualSlotStatus(Sample)) {

			}
			else {

			}
			insert_time = 0;
		}

		vTaskDelay(x1second);

		switch (sys_sts) 									//锟斤拷位锟斤拷状态
		{
			case SYS_IDLE: {
				CloseFLAG = 0x0;							//锟铰碉拷锟街撅拷锟轿�
				OpenFLAG  = 0x0;							//锟铰碉拷锟街撅拷锟轿�
				for(i = 0; i < 2; i++)						//锟斤拷源锟斤拷锟诫复位
				{
					dps_info.DBMsg[i].UseMask   = 0x0;
					dps_info.DBMsg[i].OpenMask  = 0x0;
					dps_info.DBMsg[i].CloseMask = 0x0;
				}

				if (Get_Dps_Cfg_Flag())						//锟矫碉拷锟铰碉拷锟斤拷锟矫诧拷锟斤拷
				{
					Init_Bram();
					Sample->AgeStatus = AGING_READY;
					Sample->AgeTime   = 0x00;
					Set_Dps_Enable_Flag();
					Clear_Dps_Cfg_Flag();
					Set_FirstFlag(FIRST_ST);  				//锟窖撅拷锟斤拷一锟轿匡拷始
				}
				else										//锟窖伙拷取锟斤拷锟斤拷,锟饺达拷锟斤拷锟斤拷
				{
					if(M_START == System_Get_StartEven())	//锟斤拷始锟较伙拷
					{
						if(FIRST_ST == Get_FirstFlag())		//锟斤拷锟斤拷锟斤拷锟斤拷锟揭伙拷锟斤拷匣锟�
						{
							Set_Dps_Enable_Flag();
							fpga_obj.list = (List *)LIST_ADDR;      //锟斤拷位list锟斤拷址
							Initloop();
							ResetListFifo();						//锟斤拷位PL fifo
							usleep(1);
							RebootFpgaMask(0x1f);					//锟斤拷位fpga
							ClrOverflag();
							System_State_Set(SYS_RUNNING);
							Set_FirstFlag(FIRST_SP);
						}
						if(RESTART == Get_Startflag())		  //锟斤拷锟斤拷锟较伙拷
						{
							fpga_obj.list = (List *)LIST_ADDR;//锟斤拷位list锟斤拷址
							Initloop();
							ResetListFifo();				  //锟斤拷位PL fifo
							usleep(1);
							RebootFpgaMask(0x1f);			  //锟斤拷位fpga
							ClrOverflag();
							Set_Dps_Enable_Flag();
							System_State_Set(SYS_RUNNING);
							Set_Startflag(NONE);
						}
					}
					else if(RESUME == System_Get_StartEven())//锟街革拷锟较伙拷
					{
						Set_Dps_Enable_Flag();
						ClrOverflag();						 //锟斤拷锟絚ycle锟斤拷锟斤拷锟斤拷媳锟街�
						System_State_Set(SYS_RUNNING);
					}
					System_Set_StartEven(NONE);
				}
			}
			break;
			case SYS_RUNNING: {								 //锟斤拷锟斤拷状态
				/* Sample Data */
				SampleCount += 1;
				if(10 == SampleCount)						 //一锟斤拷锟斤拷锟揭伙拷锟�
				{
					Set_Sample_packge(Sample);
					SampleCount = 0;
				}

				if (Sample->AgeTime >= dps_cfg->AgeTime) {	 //锟较伙拷时锟戒超锟斤拷锟斤拷锟斤拷时锟斤拷
					if (aging_ctl->AutoStop) {				 //锟皆讹拷停止
						System_Set_StopEven(AGE_OK);		 //锟斤拷位锟斤拷锟较伙拷锟斤拷锟�
					}
				}

				if (Get_Dps_Enable_Flag()) {
					u32 flag = 0x00;
					if (CONFIG_SUCCESS != Dps_Config(dps_cfg, Sample, aging_ctl->ProtEnable, DPS_BD0, &flag)) {
					}
					if (flag) {
						Sample->AgeStatus = AGING_DO;
					}
					Clear_Dps_Enable_Flag();
				}

				if(RUNNING == pat_r->RunSta)
				{
					if(GETCFGBIT(8)&0x1)					//config 锟斤拷锟斤拷
					{
						if(GETCFGBIT(0)&0x1)
						{
							db_Conf();
							shellprintln("    db_Conf ok ...");
							CLRCFGBIT(0);
						}
						if(GETCFGBIT(1)&0x1)
						{
							pin_Conf();
							shellprintln("    pin_Conf ok ...");
							CLRCFGBIT(1);
						}
						if(GETCFGBIT(2)&0x1)
						{
							Mask_Conf();
							shellprintln("    Mask_Conf ok ...");
							CLRCFGBIT(2);
						}
						Initloop();
						StartList_WOpt();           		//锟较伙拷锟斤拷锟�
						vTaskDelay(x1second);
						while(1)
						{
							vTaskDelay(100);				//确锟斤拷锟斤拷锟捷碉拷锟斤拷fpga
							shellprintln("    read fpga fifo ...");
							if(0x1f != ReadFifo())			//fpga fifo锟斤拷锟斤拷锟斤拷锟斤拷
								break;
							readcount += 1;
							if(WAIT_FIFO == readcount)
							{
								readcount = 0;
								pat_r->ErrCount = 0x00;
								pat_r->RunSta   = WAITRUN;
								Sample->AgeStatus = ERROR_PROTECT;		//锟斤拷锟斤拷锟斤拷锟斤拷
								shellprintln("    read fpga fifo error ...");
								System_State_Set(SYS_IDLE);
								goto restart;
							}
						}

						shellprintln("    Start fpga ...");
						StartFpgaMask(0x1f);							//锟斤拷始锟斤拷锟斤拷藕锟�

						CLRCFGBIT(8);
					}

					countover += 1;
					if(countover >= 5)
					{
						countover = 0;
						if(GetOverflag()) 								//list锟斤拷锟斤拷锟斤拷锟�
						{
							fpga_obj.list = (List *)LIST_ADDR;      	//锟斤拷位list锟斤拷址

							if(0x1f != Read_Vecflag(Sample))			//锟斤拷取锟斤拷锟斤拷锟斤拷锟斤拷状态
							{
								shellprintln("    Run ok ...");
								ClrOverflag();
								memset((void *)pat_err, 0, sizeof(Vec_Err_Count));
								Read_Back_isp(pat_err);
								AnalyzeIsp(Ispinfo);

								pat_r->ErrCount = 0x00;
								pat_r->RunSta   = RUNOK;
							}
						}
					}
				}
			}
			break;
			case SYS_STOP: {										//停止状态

				if(System_Get_StopFlag())
				{
					StopList_WOpt();								//停止写List
					if(M_PAUSE == System_Get_StopEven())
						PauseFpgaMask(0x1f);						//锟斤拷停fpga
					else
						StopFpgaMask(0x1f);							//停止fpga

					System_Clr_StopFlag();
				}
				else
				{
					if(0x0 == CloseFLAG)
					{
						if(pat_r->RunSta  == RUNOK)
						{
							ClrOverflag();
							memset((void *)pat_err, 0, sizeof(Vec_Err_Count));
							Read_Back_isp(pat_err);
							AnalyzeIsp(Ispinfo);

							pat_r->ErrCount = 0x00;
							u32 flag = 0x00;
							if (SET_SUCCESS != Dps_Disable(dps_cfg, Sample, DPS_BD0, &flag)) {
							}
							if (flag) {
								Sample->AgeStatus = AGING_STOP;
							}
							CloseFLAG = 0x2;
						}
						else
						{
							countover += 1;
							if(countover >= 5)
							{
								countover = 0;
								if(0x1f != Read_Vecflag(Sample))	 		//锟斤拷取锟斤拷锟斤拷锟斤拷锟斤拷状态
								{
									ClrOverflag();
									memset((void *)pat_err, 0, sizeof(Vec_Err_Count));
									Read_Back_isp(pat_err);
									AnalyzeIsp(Ispinfo);

									pat_r->ErrCount = 0x00;
									pat_r->RunSta   = RUNOK;
									u32 flag = 0x00;
									if (SET_SUCCESS != Dps_Disable(dps_cfg, Sample, DPS_BD0, &flag)) {
									}
									if (flag) {
										Sample->AgeStatus = AGING_STOP;
									}
									CloseFLAG = 0x2;
								}
							}
						}
					}
				}

				if(0x2 == CloseFLAG)				//锟铰碉拷锟斤拷锟�
				{
					//锟叫讹拷状态
					switch(System_Get_StopEven())	//停止锟铰硷拷
					{
						case AGE_OK:				//锟较伙拷锟斤拷锟�
						{
							Sample->AgeStatus = AGING_CPL;
							break;
						}
						case M_STOP:				//锟较伙拷停止
						{
							Sample->AgeStatus = AGING_STOP;
							break;
						}
						case M_PAUSE:				//锟斤拷停锟较伙拷
						{
							Sample->AgeStatus = AGING_PAUSE;
							break;
						}
						default:
							break;
					}
					pat_r->ErrCount = 0x00;
					pat_r->RunSta   = WAITRUN;
					System_State_Set(SYS_IDLE);
				}
			}
			break;
			default : {

			}
			break;
		}
	}

    /* Delete Task Itself */
	vTaskDelete(NULL);
}


/*
  ** Function Name        : System_State_Set_Idle
  ** Function Description : Set System State In Idle State
  ** Function Parameter   :	Void
  ** Function Return	  :	SET_SUCCESS         : Return Always
  ** Notes				  : None
*/
void System_State_Set(Sys_State_t stat)
{
	sys_sts = stat;
}


//锟斤拷取系统状态
u8 System_State_Get(void)
{
	return sys_sts;
}


/*
  ** Function Name        : System_State_Set_Stop
  ** Function Description : Set System State In Stop State
  ** Function Parameter   :	Void
  ** Function Return	  :	SET_SUCCESS         : Return Always
  ** Notes				  : None
*/
void System_Set_StopEven(Stop_Even_t even)
{
	System_State_Set(SYS_STOP);
	stop_flag = 0x1;
	stop_even = even;
}

u8 System_Get_StopEven(void)
{
	return stop_even;
}

u8 System_Get_StopFlag(void)
{
	return stop_flag;
}

void System_Clr_StopFlag(void)
{
	stop_flag = 0x0;
}

//锟斤拷锟斤拷锟斤拷锟斤拷锟铰硷拷
void System_Set_StartEven(Start_Even_t even)
{
	start_even = even;
}

//锟斤拷取锟斤拷锟斤拷锟铰硷拷
u8 System_Get_StartEven(void)
{
	return start_even;
}

//锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷志
void Set_Startflag(Start_flag flag)
{
	start_flag = flag;
}

//锟斤拷取锟斤拷锟斤拷锟斤拷志
u8 Get_Startflag(void)
{
	return start_flag;
}

//锟斤拷取锟斤拷一锟斤拷执锟叫憋拷志
u8 Get_FirstFlag(void)
{
	return first_flag;
}

//锟斤拷锟矫碉拷一锟斤拷执锟叫憋拷志
void Set_FirstFlag(First_flag flag)
{
	first_flag = flag;
}

/*
  ** Function Name        : Set_Dps_Enable_Flag
  ** Function Description : Set Dps Enable Flag
  ** Function Parameter   : Void
  ** Function Return      : SET_SUCCESS         : Always
  ** Notes				  : None
*/
u8 Set_Dps_Enable_Flag(void)
{
	dps_enable = 0x01;
	return SET_SUCCESS;
}

/*
  ** Function Name        : Get_Dps_Enable_Flag
  ** Function Description : Get Dps Enable Flag
  ** Function Parameter   : Void
  ** Function Return      : u8			 		: Dps_Cfg_Flag Value
  ** Notes				  : None
*/
u8 Get_Dps_Enable_Flag(void)
{
	return dps_enable;
}

/*
  ** Function Name        : Clear_Dps_Enable_Flag
  ** Function Description : Clear Dps Enable Flag
  ** Function Parameter   : Void
  ** Function Return      : CLEAR_SUCCESS       : Always
  ** Notes				  : None
*/
u8 Clear_Dps_Enable_Flag(void)
{
	dps_enable = 0x00;
	return CLEAR_SUCCESS;
}

void SetFpgaConfig(void)
{
	config_flag = 1;
}

u8 GetFpgaConfig(void)
{
	return config_flag;
}

void ClearFpgaConfig(void)
{
	config_flag = NONE;
}



/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/


