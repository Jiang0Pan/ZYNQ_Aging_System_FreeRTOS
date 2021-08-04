




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

//����ͨ��
#define SAMP_NUM				(8)

//Dps��Դ����
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

//���ս�׼ʱ�� 0xA050
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

//����ϵͳ������Ϣ0xA151
typedef struct {
	struct _DpsArg_ {
		short SetValue;		//����ֵ
		short DpsMaxV;		//�������ֵ
		short DpsMinV;		//�������ֵ
		short InputMaxV;	//��������ֵ
		short InputMinV;	//��������ֵ
		  u16 Protect_I;	//��������
		struct _Open_ {		//�ϵ����
			u16 Mode;		//�ϵ�ģʽ
			u16 STime;		//��ʼʱ��
			u16 Step;		//������
			u16 Keep;		//����ʱ��
		}Open;
		struct _Close_ {	//�ϵ����
			u16 Mode;		//�µ�ģʽ
			u16 STime;		//��ʼʱ��
			u16 Step;		//������
			u16 Keep;		//����ʱ��
		}Close;
	}Dsp[DPS_NUM];

	short TempSet;		//�¶��趨ֵ
	short AgeBoardID;	//�ϻ������
	u32   AgeTime;		//�ϻ�ʱ��
	short Templimit;	//�¶�����
}Dps_Arg;

//��������
typedef struct {
	u32 fpga_CS;				//fpgaƬѡ
	u32 lseek;					//�ļ��е�ƫ����
	u8  Data[VEC_OFFSET];		//��������
}Vector_Arg;

//�ϻ�״̬����Command:0xA153
typedef struct {
	u8  AgeStatus;				//�ϻ�״̬
	u8  AutoStop;				//�Զ�ֹͣ
	u16 ProtEnable;				//����ʹ��
	u32 AgeTime;				//�ϻ�ʱ��
}Aging_Ctl;


typedef struct Retime_Enable {
	/* System Error Protect enable */
	u16 clr_timer;
}Retime_Enable_t;

//��������״̬Command:0xA257 */
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


//����������ͨ������
typedef struct {
	u32 freq;			//��Ƶϵ��
	short S_DHV1;		//�����ߵ�ƽ
	short S_DLV1;		//�����͵�ƽ
	short S_DHV2;		//�����ߵ�ƽ
	short S_DLV2;		//�����͵�ƽ

	short D_DHV1;		//�����ߵ�ƽ
	short D_DLV1;		//�����͵�ƽ
	short D_DHV2;		//�����ߵ�ƽ
	short D_DLV2;		//�����͵�ƽ
	short D_DHV3;		//�����ߵ�ƽ
	short D_DLV3;		//�����͵�ƽ
	short D_DHV4;		//�����ߵ�ƽ
	short D_DLV4;		//�����͵�ƽ

	short D_CPHV1;	//�Ƚϸߵ�ƽ
	short D_CPLV1;	//�Ƚϵ͵�ƽ
	short D_CPHV2;	//�Ƚϸߵ�ƽ
	short D_CPLV2;	//�Ƚϵ͵�ƽ
	short D_CPHV3;	//�Ƚϸߵ�ƽ
	short D_CPLV3;	//�Ƚϵ͵�ƽ
	short D_CPHV4;	//�Ƚϸߵ�ƽ
	short D_CPLV4;	//�Ƚϵ͵�ƽ
}Dri_Arg;

/*8·��������*/
typedef struct {
	short Temperature[DPS_NUM];
	 u32 WarnState;
}_SAMP_;

//���Ͳ�����Ϣ0xA157
typedef struct _Sample_data{
	  u32 AgeTime;				//�ϻ�ʱ��
	  u16 BoardCode;			//�ϻ������
	  u8  BoardStatus;			//���״̬
	  u8  AgeStatus;			//�ϻ�״̬
	  u32 ErrStatus;			//����״̬
	short DpsVI[DPS_NUM];		//�����ѹ
	short DpsVO[DPS_NUM];		//�����ѹ
	short DpsIO[DPS_NUM];		//�������
	short DpsTemp[DPS_NUM];		//��Դ�¶�
	u16 DpsStat[DPS_NUM];		//��Դ״̬
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

//����pin�Ų�������
typedef struct {
	struct _Pin_ {
		u32 Roll_LE;	//��ת��ʱLE
		u32 Roll_TE;	//��ת��ʱTE
		u32 Comp_LE;	//�Ƚ���ʱLE
		u32 Comp_TE;	//�Ƚ���ʱTE
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

//�ؼ���Ϣ
typedef struct {
	struct {
		u32 count;
		struct _Mask_ {
			u32 Addr;		//�����ַ
			u32 Mask;		//ͨ������
		}isp[NUM_ISP];
	}db[2];
}BackIsp;

//pca9548 IO����
typedef union {
	struct {
		u8 Reg_Addr;
		u8 Group0_Data;
		u8 Group1_Data;
	};
	u8 IO_Data[3];
}_Output_;

//dpm��Ϣ
typedef struct {
	u8 DpmAddr;	//IIC��ַ
	u8 ChipVer;	//оƬ�汾
}Dpm_Info;

//оƬ��Ϣ
typedef struct {
	union {
		struct {				//4byte
			struct {
				u8 Minor: 4;	//�ΰ汾��
				u8 Major: 4;	//���屾��
			} Test_V;			//���԰汾
			struct {
				u8 Minor: 4;	//�ΰ汾��
				u8 Major: 4;	//���屾��
			} Publish_V;		//�����汾
		};
		u32 V_Data;
	} Version;	//�汾����

	union {
		struct {	//4byte
			u32	Day:   8;		//��
			u32	Month: 8;		//��
			u32 Year: 16;		//��
		};
		u32 D_Data;
	}Date;						//��������

	union {
		struct {	//2byte
			u32	Minute:8;		//��
			u32	Hour:  8;		//��
		};
		u32 T_Data;
	}Time;						//ʱ������

	u32 Core_temp;				//�ں��¶�

	struct{
		u32 DANL;
		u32 DANH;
	} DAN;

}ChipInfo;

//pl����fpga�Ĵ���(��������Ϣ)
typedef struct {

	u8  EEPROM_SlaveAddr;			//EEPROM IIC��ַ
	u8  PCA953_SlaveAddr;			//IPCA95 IIC��ַ
	List * list;					//list��ַ

	struct _FPGA_ {
		u32	   BoardType;			//������ID
		u32	   VecSize;				//�����ֽ���

		struct _Msg_ {
			u8  HWChan;				//IICͨ��
			u16 HWVersion;			//Ӳ���汾
			u32 PCBType;			//PCB�忨����
			u8  PN[32];				//PN
			u8  SN[32];    			//SN

			u8 	Version[20];		//�̼��汾
			u8 	CompTime[20];		//�̼�����ʱ��
			float ChipTemp;			//оƬ�¶�
			struct{
				u32 DANL;
				u32 DANH;
			} ChipDAN;
			_Output_ Output_LVL;	//IO�����ƽ״̬
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
		volatile u32 *List_L;		//ָ�����ݵ�λ
		volatile u32 *List_M;		//ָ��������λ
		volatile u32 *List_H;		//ָ�����ݸ�λ
		volatile u32 *WOpt_OK;		//����д�����
		volatile u32 *Fifo_Depth;	//fifo���
		volatile u32 *IRQ_Mask;		//list�ж����α�־
		volatile u32 *Fifo_rst;		//��λFifo
	}__attribute__((aligned(4)))ListReg;

}Driver_fpga;

//Dps�忨��Ϣ(����)
typedef struct {
	u8  GDQ54_SlaveAddr;
	u8  EEPROM_SlaveAddr;	//EEPROM SlaveAddr
	u8  PCA953_SlaveAddr;	//IPCA95 SlaveAddr
	u8  AD5694_SlaveAddr;	//AD5694 SlaveAddr
	struct {
		u8  Chan;			//IICͨ��
		u16 UseMask;		//��Դ״̬
		u16 OpenMask;		//�ѿ���Դ
		u16 CloseMask;		//��Դ״̬
		u8  DpsAddr[6];		//Dps��Դ��ַ
		Dpm_Info Dpm[10];	//Dpm��ص�ַ	//ABCDEF1234
		////////////////////////////////////////////////////////////
		u16 HWVersion;		//Ӳ���汾
		u32 PCBType;		//PCB�忨����
		u8  PN[32];			//PN
		u8  SN[32];    		//SN
		u8 	Version[20];	//�̼��汾
		u8 	CompTime[20];	//�̼�����ʱ��
		float ChipTemp;		//оƬ�¶�
		struct{
			u32 DANL;
			u32 DANH;
		} ChipDAN;			//DAN

		//�汾IO�����ƽ״̬
		_Output_ Output_LVL;
	}DBMsg[2];
}DpsMsg;


//������Ϣ
typedef struct {
	u8 HWChan;				//Ӳ���汾IICͨ��
	u8 EEPROM_SlaveAddr;	//EEPROM IIC��ַ
	u8 PCA953_SlaveAddr;	//PCA953 IIC��ַ
	u8 TOChan;				//��չIO IICͨ��
	u8 IO_SlaveAddr;		//PCA953 IO��չоƬ

	u16 HWVersion;			//Ӳ���汾
	u32 PCBType;			//PCB�忨����
	u8  PN[32];				//PN
	u8  SN[32];    			//SN

	//�汾IO�����ƽ״̬
	_Output_ Output_LVL;

	//����IO�����ƽ״̬
	_Output_ Ctl_Output;
}BKBMsg;

//������Ϣ
typedef struct {
	u8 HWChan;				//Ӳ���汾IICͨ��
	u8 EEPROM_SlaveAddr;	//EEPROM IIC��ַ
	u8 PCA953_SlaveAddr;	//PCA953 IIC��ַ
	u8 DIPETH_SlaveAddr;	//PCA953 IIC��ַ(DIP)
	u8 DS1337_SlaveAddr;	//DS1337 IIC��ַ(RTC)

	u16 HWVersion;			//Ӳ���汾
	u32 PCBType;			//PCB�忨����
	u8  PN[20];				//PN
	u8  SN[20];    			//SN

	u8 	Version0[20];		//�̼��汾
	u8 	CompTime0[20];		//�̼�����ʱ��
	u8 	Version1[20];		//�̼��汾
	u8 	CompTime1[20];		//�̼�����ʱ��
	float ChipTemp;			//оƬ�¶�
	struct{
		u32 DANL;
		u32 DANH;
	} ChipDAN;			//DAN
	//IO�����ƽ״̬
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


extern Time_Sync_t 	SysTime;	//ϵͳʱ��
extern Driver_fpga	fpga_obj;	//fpga���ƶ���
extern DpsMsg		dps_info;	//dps��ԴӲ����Ϣ
extern BKBMsg		bbd_info;	//������Ϣ
extern MBMsg		mbd_info;	//������Ϣ
extern Dual_Slot_Info dual_slot_info;
extern BackIsp		All_isp;	//�ؼ���Ϣ


#endif




