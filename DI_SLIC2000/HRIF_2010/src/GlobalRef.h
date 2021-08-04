




#ifndef _GLOBALREF_H
#define _GLOBALREF_H



#include "xil_types.h"
#include "error.h"
#include "memory_map.h"


#define	AXI_BASE_ADDR			(0x43C00000U)
#define LIST_ADDR				(0x30000000U)


#define RegValue(reg)			(*((volatile u32 *)(reg)))  //u32 reg
#define Reg_Opt(reg)			(*(reg))					//u32 *reg

#define AddrRWOpt(ptr)			(*(ptr))					//



#define VEC_UNIT				(32)

#define WAITE_TIME				(5)

#ifndef FAILURE
#define FAILURE					(0x1)
#endif

#ifndef SUCCESS
#define SUCCESS					(0x0)
#endif

#ifndef OK
#define OK						(0x1)
#endif

typedef enum {
	OFF = 0x0,
	ON  = 0x1,
}SW;


#define ERROR_VL				(0xffffffff)
#define TEST_VAL				(0x1)

#define DRIVDPS0ADDR			(0x18)
#define DRIVDPS1ADDR			(0x19)

/* Pin Channel Number */
#define NUM_ISP					(512)

//采样通道
#define SAMP_NUM				(8)

//Dps电源数量
#define DPS_NUM					(16)


#define	VEC_OFFSET				(0x100020)
#define PIN_NUM					(256)
#define DPS_ADDR				(0x10)

#define K7_PATH					(0x01)
#define A7_PATH					(0xf0)

#define DRIV_BOARD_NUM			(5)
#define PAGE_SIZE				(64)


#define CORE_TEMP_COE			(503.975)
#define CORE_TEMP_OFFSET		(273.15)


/*
  ** Define
*/
/* Min Data Length */
#define     MINLENGTH           (0x00)
/* Beep Control */
#define     BEEP_DISABLE        (0x00)
#define     BEEP_ENABLE         (0x01)
#define     BEEP_OFF            (0x00)
#define     BEEP_0N             (0x01)
/* Frame Pattern Max Size */
#define     PATTERN_SIZE        (0x100004)
/* Firmware Frame Max Size */
#define     FIRWARE_MAX_SIZE    (0x100004)

/* Dps Channel Number */


#define 	DELAY_1_SECOND		(100UL)
#define 	MAX_ISP				(1024)


typedef u32 fpga_err_t;

typedef union{
	float fdata;
	u32 udata;
}Data_t;


enum {NO_OPT = 0x0, W_OPT  = 0x1, R_OPT  = 0x2,};

///////////////////////////////////////////////////////////////////////////////////

#pragma pack(push)
#pragma pack(1)

typedef struct Beep_Control {
	/* Beep State Control */
	u16 state;
	/* Beep Enable Control */
	u16 enable;
}Beep_Control_t;

//接收较准时间 0xA050
typedef union {
	struct{
		u8 Sec;
		u8 Min;
		u8 Hou;
		u8 Day;
		u8 Mon;
		u8 Yea;
		u8 Cen;
		u8 Res;
	};
	u8  Data[8];
}Time_Sync_t;

//接收系统配置信息0xA151
typedef struct {
	struct _DpsArg_ {
		short SetValue;		//设置值
		short DpsMaxV;		//输出上限值
		short DpsMinV;		//输出下限值
		short InputMaxV;	//输入上限值
		short InputMinV;	//输入下限值
		  u16 Protect_I;	//保护电流
		struct _Open_ {		//上电参数
			u16 Mode;		//上电模式
			u16 STime;		//开始时间
			u16 Step;		//步进量
			u16 Keep;		//保持时间
		}Open;
		struct _Close_ {	//上电参数
			u16 Mode;		//下电模式
			u16 STime;		//开始时间
			u16 Step;		//步进量
			u16 Keep;		//保持时间
		}Close;
	}Dsp[DPS_NUM];

	short TempSet;		//温度设定值
	short AgeBoardID;	//老化板编码
	u32   AgeTime;		//老化时间
	short Templimit;	//温度上限
}Dps_Arg;

//发送向量
typedef struct {
	u32 fpga_CS;				//fpga片选
	u32 lseek;					//文件中的偏移量
	u8  Data[VEC_OFFSET];		//数据内容
}Vector_Arg;

//老化状态控制Command:0xA153
typedef struct {
	u8  AgeStatus;				//老化状态
	u8  AutoStop;				//自动停止
	u16 ProtEnable;				//保护使能
	u32 AgeTime;				//老化时间
}Aging_Ctl;


typedef struct Retime_Enable {
	/* System Error Protect enable */
	u16 clr_timer;
}Retime_Enable_t;

//向量运行状态Command:0xA257 */
typedef struct {
	/* Pattern Runing State */
	u32 RunSta;
	/* Pattern Runing Position */
	u32 RunLine;
	/* Pattern Runing Current Position */
	u32 RunCycle;
	/* Pattern Runing Error Count */
	u32 ErrCount;
}VecRun_t;


//接收驱动板通道配置
typedef struct {
	u32 freq;			//分频系数
	short S_DHV1;		//驱动高电平
	short S_DLV1;		//驱动低电平
	short S_DHV2;		//驱动高电平
	short S_DLV2;		//驱动低电平

	short D_DHV1;		//驱动高电平
	short D_DLV1;		//驱动低电平
	short D_DHV2;		//驱动高电平
	short D_DLV2;		//驱动低电平
	short D_DHV3;		//驱动高电平
	short D_DLV3;		//驱动低电平
	short D_DHV4;		//驱动高电平
	short D_DLV4;		//驱动低电平

	short D_CPHV1;	//比较高电平
	short D_CPLV1;	//比较低电平
	short D_CPHV2;	//比较高电平
	short D_CPLV2;	//比较低电平
	short D_CPHV3;	//比较高电平
	short D_CPLV3;	//比较低电平
	short D_CPHV4;	//比较高电平
	short D_CPLV4;	//比较低电平
}Dri_Arg;

/*8路采样参数*/
typedef struct {
	short Temperature[DPS_NUM];
	 u32 WarnState;
}_SAMP_;

//发送采样信息0xA157
typedef struct _Sample_data{
	  u32 AgeTime;				//老化时间
	  u16 BoardCode;			//老化板编码
	  u8  BoardStatus;			//插板状态
	  u8  AgeStatus;			//老化状态
	  u32 ErrStatus;			//错误状态
	short DpsVI[DPS_NUM];		//输入电压
	short DpsVO[DPS_NUM];		//输出电压
	short DpsIO[DPS_NUM];		//输出电流
	short DpsTemp[DPS_NUM];		//电源温度
	u16 DpsStat[DPS_NUM];		//电源状态
	short TempSamp[DPS_NUM];
	u32 Sample_timer;
}Sample_Data;

typedef struct DPS_Channel_Sts {
	/* DPS Channel ID */
	u16 dps_chl;
	/*
	  ** DPS Channel State .
	  ** 0x5555 : Close DPS Channel Power
	  ** 0xAAAA : Open  DPS Channel Power
	*/
	u16 dps_chl_sts;
}DPS_Channel_Sts_t;

typedef union Pat_Err_Info {
	struct {
		u16 DB0[64];
		u16 DB1[64];
		u16 DB2[64];
		u16 DB3[32];
		u16 DB4[32];
	};
	/* Pin Error Count */
	u16 Err_Cnt[PIN_NUM];
}Vec_Err_Count;

//接收pin脚参数配置
typedef struct {
	struct _Pin_ {
		u32 Roll_LE;	//翻转延时LE
		u32 Roll_TE;	//翻转延时TE
		u32 Comp_LE;	//比较延时LE
		u32 Comp_TE;	//比较延时TE
	}Arg[PIN_NUM];
}pin_Arg;

/* Set Pattern Output State . Command:0xA253 */
typedef struct Pat_Set_Sts {
	/* which fpga  */
	u16 fpga_cs;
	/*
	  ** Pattern Running State
	  ** 0x08 : Start
	  ** 0x00 : Stop
	*/
	u16 Run_state;
	/* Pattern Running Start Number */
	u32 StartAddr;
	/* Pattern Running Count */
	u32 LineNum;
	/* Pattern Running Times */
	u32 pat_r_tiems;
	/* Pattern Running Max Timer */
	u32 pat_Limit_timer;
	/* Pattern Running Blank */
	u32 pat_r_blank;
}Pat_Set_Sts_t;

/* Pattern Compare Information . Command:0xA254 */
typedef struct Fpga_Version {
	/* Fpga0 Version */
	u16 fpga0_ver;
	/* Fpga1 Version */
	u16 fpga1_ver;
	/* Fpga2 Version */
	u16 fpga2_ver;
	/* Fpga3 Version */
	u16 fpga3_ver;
	/* Fpga4 Version */
	u16 fpga4_ver;
}Fpga_Version_t;

/* Pattern Compare Information (Debug) . Command:0xA256 */
typedef struct Pat_Set_Sts_Debug{
	/* which fpga  */
	u16 fpga_cs;
	/*
	  ** Pattern Running State
	  ** 0x08 : Start
	  ** 0x00 : Stop
	*/
	u16 run_state;
	/* Pattern Running Start Number */
	u32 pat_r_num;
	/* Pattern Running Count */
	u32 pat_r_cnt;
	/* Pattern Running Times */
	u32 pat_r_tiems;
	/* Pattern Running Max Timer */
	u32 pat_r_timer;
	/* Pattern Running Blank */
	u32 pat_r_blank;
}Pat_Set_Sts_Debug_t;

/* Get Pattern Running State . Command:0xA257 */
//typedef struct Pattern_Running_Sts {
//	/* Pattern Runing State */
//	u32 pat_r_sts;
//	/* Pattern Runing Position */
//	u32 pat_r_pos;
//	/* Pattern Runing Current Position */
//	u32 pat_r_c_pos;
//	/* Pattern Runing Error Count */
//	u32 pat_err_count;
//}Pattern_Running_Sts_t;

/* Send Max9972 Debug Information . Command:0xACC0 */
typedef struct Send_Max9972_Info {
	/* Channel ID */
	u16 channel;
	/* Register Address */
	u16 reg;
	/* Register Data */
	u16 reg_data;
}Send_Max9972_Info_t;

/* Upgrade Fpga Firmware . Command:0xACCE */
typedef struct Fpga_Firmware {
	/*
	  ** Fpga Firmware Frame
	  ** Last Frame Is 0xFFFF
	*/
	u16 fir_frame;
	/* Fpga Firmware */
	u8 fir_data[FIRWARE_MAX_SIZE];
}Fpga_Firmware_t;

/* Recv Pin Mask Information From PC. Command:0xA259 */
typedef struct Recv_Pin_Mask_Info {
	/* Pin Channel Mask */
	u8 pin_mask[PIN_NUM];
}Recv_Pin_Mask_Info_t;

/* Pin Mask Information To ZYNQ */
typedef struct Pin_To_Info {
	/* Pin Channel Mask */
	u32 pin_mask[8];
}Pin_To_Info_t;

/* PC Read DDR Data . Command:0xACCC */
typedef struct Read_DDR {
	/* Read DDR Data Address */
	u32 ddr_addr;
	/* Read DDR Data Length */
	u32 ddr_length;
}Read_DDR_t;

/* Upgrade Management Board Firmware . Command:0xACCD */
typedef struct MB_Firmware {
	/* Management Board Total Firmware Frame */
	u16 mb_total_fir_frame;
	/* Management Board Current Firmware Frame */
	u16 mb_crt_fir_frame;
	/* Management Board Firmware Data */
	u8 mb_fir_data[FIRWARE_MAX_SIZE];
}MB_Firmware_t;

/* CycleInfo . Command:0xA261 */
typedef struct CycleInfo {
	/* Management Board Total Firmware Frame */
	u16 cycle_total_fir_frame;
	/* Management Board Current Firmware Frame */
	u16 cycle_crt_fir_frame;
	/* Management Board Firmware Data */
	u8 cycle_fir_data[0x200000];
}CycleInfo_t;

/* DDR Test . Command:0xAF00 */
typedef struct DDR_Test {
	/* Which Fpga In Board */
	u16 fpga_cs;
	/*
	  ** Test State
	  ** 0x00 : Stop
	  ** 0x01 : Start
	*/
	u16 test_sts;
}DDR_Test_t;

/* Get DDR Test State . Command:0xAF01 */
typedef struct DDR_Test_State {
	/* Which Fpga In Board */
	u16 fpga_cs;
	/*
	  ** Test State
	  ** 0x00 : Stop
	  ** 0x01 : Testing
	  ** 0x02 : Test Complete
	  ** 0x03 : Test Abnormal
	*/
	u16 test_sts;
	/* Already Test DDR Address */
	u32	alr_test_addr;
}DDR_Test_State_t;

typedef struct {
	struct _isp_ {
		/* Pin Compare Error Position */
		u32 Addr;
		/* Pin Mask */
		u32 Mask[8];
	}info[MAX_ISP];
}BackIsp_Info;

/*
 * A252
*/
typedef struct {
	short Sline;
	short Number;
}Error_cmd;


typedef struct MB_Info {
	/* EEPROM Information : Product Number */
	u8 MB_ERP_PN[MB_ERP_PN_SIZE];
	/* EEPROM Information : Serial Number */
	u8 MB_ERP_SN[MB_ERP_SN_SIZE];
	/* EEPROM Information : Reserved */
	u8 MB_ERP_RSD[MB_ERP_RSD_SIZE];
	/* Version Information : Hardware Version */
	u8 MB_VSN_HW[MB_VSN_HW_SIZE];
	/* Version Information : Chip0 Firmware Version */
	u8 MB_VSN_FW0[MB_VSN_FW0_SIZE];
	/* Version Information : Chip0 Firmware Compilation Time */
	u8 MB_VSN_FWCT0[MB_VSN_FWCT0_SIZE];
	/* Version Information : Chip1 Firmware Version */
	u8 MB_VSN_FW1[MB_VSN_FW1_SIZE];
	/* Version Information : Chip1 Firmware Compilation Time */
	u8 MB_VSN_FWCT1[MB_VSN_FWCT1_SIZE];
	/* Version Information : Reserved */
	u8 MB_VSN_RSD[MB_VSN_RSD_SIZE];
	/* Other Information : Board Type */
	u8 MB_OTR_BT[MB_OTR_BT_SIZE];
	/* Other Information : Reserved */
	u8 MB_OTR_RSD[MB_OTR_RSD_SIZE];
}MB_Info_t;

/* Management Board Information Command:0xACCC */
typedef struct DB_Info {
	/* EEPROM Information : Product Number */
	u8 DB_ERP_PN[DB_ERP_PN_SIZE];
	/* EEPROM Information : Serial Number */
	u8 DB_ERP_SN[DB_ERP_SN_SIZE];
	/* EEPROM Information : Reserved */
	u8 DB_ERP_RSD[DB_ERP_RSD_SIZE];
	/* Version Information : Hardware Version */
	u8 DB_VSN_HW[DB_VSN_HW_SIZE];
	/* Version Information : Chip0 Firmware Version */
	u8 DB_VSN_FW[DB_VSN_FW_SIZE];
	/* Version Information : Chip0 Firmware Compilation Time */
	u8 DB_VSN_FWCT[DB_VSN_FWCT_SIZE];
	/* Version Information : Reserved */
	u8 DB_VSN_RSD[DB_VSN_RSD_SIZE];
	/* Other Information : Board Type */
	u8 DB_OTR_BT[DB_OTR_BT_SIZE];
	/* Other Information : Reserved */
	u8 DB_OTR_RSD[DB_OTR_RSD_SIZE];
}DB_Info_t;

typedef struct {
	u32 Saddr;
	u32 Slength;
	u32 Repeat;
}List;

/////////////////////////////////////////////////////////////////////////////////////////

//回检信息
typedef struct {
	struct {
		u32 count;
		struct _Mask_ {
			u32 Addr;		//出错地址
			u32 Mask;		//通道掩码
		}isp[NUM_ISP];
	}db[2];
}BackIsp;

//pca9548 IO数据
typedef union {
	struct {
		u8 Reg_Addr;
		u8 Group0_Data;
		u8 Group1_Data;
	};
	u8 IO_Data[3];
}_Output_;

//dpm信息
typedef struct {
	u8 DpmAddr;	//IIC地址
	u8 ChipVer;	//芯片版本
}Dpm_Info;

//芯片信息
typedef struct {
	union {
		struct {				//4byte
			struct {
				u8 Minor: 4;	//次版本号
				u8 Major: 4;	//主板本号
			} Test_V;			//测试版本
			struct {
				u8 Minor: 4;	//次版本号
				u8 Major: 4;	//主板本号
			} Publish_V;		//发布版本
		};
		u32 V_Data;
	} Version;	//版本数据

	union {
		struct {	//4byte
			u32	Day:   8;		//日
			u32	Month: 8;		//月
			u32 Year: 16;		//年
		};
		u32 D_Data;
	}Date;						//日期数据

	union {
		struct {	//2byte
			u32	Minute:8;		//日
			u32	Hour:  8;		//月
		};
		u32 T_Data;
	}Time;						//时间数据

	u32 Core_temp;				//内核温度

	struct{
		u32 DANL;
		u32 DANH;
	} DAN;

}ChipInfo;

//pl控制fpga寄存器(驱动板信息)
typedef struct {

	u8  EEPROM_SlaveAddr;			//EEPROM IIC地址
	u8  PCA953_SlaveAddr;			//IPCA95 IIC地址
	List * list;					//list地址

	struct _FPGA_ {
		u32	   BoardType;			//驱动板ID
		u32	   VecSize;				//向量字节数

		struct _Msg_ {
			u8  HWChan;				//IIC通道
			u16 HWVersion;			//硬件版本
			u32 PCBType;			//PCB板卡类型
			u8  PN[32];				//PN
			u8  SN[32];    			//SN

			u8 	Version[20];		//固件版本
			u8 	CompTime[20];		//固件编译时间
			float ChipTemp;			//芯片温度
			struct{
				u32 DANL;
				u32 DANH;
			} ChipDAN;
			_Output_ Output_LVL;	//IO输出电平状态
		}Msg;

		struct _REG_ {
			volatile u32 *Debug;
			volatile u32 *Opt_Addr;
			volatile u32 *W_Data;
			volatile u32 *R_Data;
			volatile u32 *Data_Des;
			volatile u32 *Opt_RW;
			volatile u32 *Trigger;
			volatile u32 *R_Update;
			volatile u32 *WOpt_OK;
		}__attribute__((aligned(4)))Reg;

	}Driver[DRIV_BOARD_NUM];

	struct _Vec_Addr_{
		volatile u32 *Debug;
		volatile u32 *S_Addr;
		volatile u32 *D_Len;
		volatile u32 *W_Trig;
		volatile u32 *Serial;
		volatile u32 *MV_Flag;
		volatile u32 *Clr_fifo;
	}__attribute__((aligned(4)))Vec_Addr;

	struct _LIST_ {
		volatile u32 *Debug;
		volatile u32 *List_L;		//指令数据低位
		volatile u32 *List_M;		//指令数据中位
		volatile u32 *List_H;		//指令数据高位
		volatile u32 *WOpt_OK;		//数据写入完成
		volatile u32 *Fifo_Depth;	//fifo深度
		volatile u32 *IRQ_Mask;		//list中断屏蔽标志
		volatile u32 *Fifo_rst;		//复位Fifo
	}__attribute__((aligned(4)))ListReg;

}Driver_fpga;

//Dps板卡信息(两块)
typedef struct {
	u8  GDQ54_SlaveAddr;
	u8  EEPROM_SlaveAddr;	//EEPROM SlaveAddr
	u8  PCA953_SlaveAddr;	//IPCA95 SlaveAddr
	u8  AD5694_SlaveAddr;	//AD5694 SlaveAddr
	struct {
		u8  Chan;			//IIC通道
		u16 UseMask;		//电源状态
		u16 OpenMask;		//已开电源
		u16 CloseMask;		//电源状态
		u8  DpsAddr[6];		//Dps电源地址
		Dpm_Info Dpm[10];	//Dpm监控地址	//ABCDEF1234
		////////////////////////////////////////////////////////////
		u16 HWVersion;		//硬件版本
		u32 PCBType;		//PCB板卡类型
		u8  PN[32];			//PN
		u8  SN[32];    		//SN
		u8 	Version[20];	//固件版本
		u8 	CompTime[20];	//固件编译时间
		float ChipTemp;		//芯片温度
		struct{
			u32 DANL;
			u32 DANH;
		} ChipDAN;			//DAN

		//版本IO输出电平状态
		_Output_ Output_LVL;
	}DBMsg[2];
}DpsMsg;


//背板信息
typedef struct {
	u8 HWChan;				//硬件版本IIC通道
	u8 EEPROM_SlaveAddr;	//EEPROM IIC地址
	u8 PCA953_SlaveAddr;	//PCA953 IIC地址
	u8 TOChan;				//扩展IO IIC通道
	u8 IO_SlaveAddr;		//PCA953 IO扩展芯片

	u16 HWVersion;			//硬件版本
	u32 PCBType;			//PCB板卡类型
	u8  PN[32];				//PN
	u8  SN[32];    			//SN

	//版本IO输出电平状态
	_Output_ Output_LVL;

	//控制IO输出电平状态
	_Output_ Ctl_Output;
}BKBMsg;

//主板信息
typedef struct {
	u8 HWChan;				//硬件版本IIC通道
	u8 EEPROM_SlaveAddr;	//EEPROM IIC地址
	u8 PCA953_SlaveAddr;	//PCA953 IIC地址
	u8 DIPETH_SlaveAddr;	//PCA953 IIC地址(DIP)
	u8 DS1337_SlaveAddr;	//DS1337 IIC地址(RTC)

	u16 HWVersion;			//硬件版本
	u32 PCBType;			//PCB板卡类型
	u8  PN[20];				//PN
	u8  SN[20];    			//SN

	u8 	Version0[20];		//固件版本
	u8 	CompTime0[20];		//固件编译时间
	u8 	Version1[20];		//固件版本
	u8 	CompTime1[20];		//固件编译时间
	float ChipTemp;			//芯片温度
	struct{
		u32 DANL;
		u32 DANH;
	} ChipDAN;			//DAN
	//IO输出电平状态
	_Output_ Output_LVL;
	_Output_ IP_ADDR;
}MBMsg;

/* Dual Slot Information */
typedef struct {
	/* IIC expand channel */
	u8 Chan;
	/* Eeprom slave address, P/N,S/N Information */
	u8 EEPROM_SlaveAddr;
	/* IIC expand IO Chip 0, Hardware version */
	u8 PCA953_SlaveAddr_0;
	/* IIC expand IO Chip 1, SLot board insert status */
	u8 PCA953_SlaveAddr_1;

	/* Hardware version */
	u16 HWVersion;
	/* Board ID */
	u32 PCBType;
	/* Product Number */
	u8  PN[20];
	/* Serial Number */
	u8  SN[20];

	/* IO expand input status, hardware version */
	_Output_ Output_LVL;
	/* IO expand input status, slot insert */
	_Output_ Slot_Insert;
}Dual_Slot_Info;

#pragma pack(pop)


#define SetAgeStatus(status)	(((Sample_Data *)SAMPLE_DATA_ADD)->AgeStatus = (status);)



//////////////////////////////////////////////////////////////////////


extern Time_Sync_t 	SysTime;	//系统时间
extern Driver_fpga	fpga_obj;	//fpga控制对象
extern DpsMsg		dps_info;	//dps电源硬件信息
extern BKBMsg		bbd_info;	//背板信息
extern MBMsg		mbd_info;	//主板信息
extern Dual_Slot_Info dual_slot_info;
extern BackIsp		All_isp;	//回检信息


#endif




