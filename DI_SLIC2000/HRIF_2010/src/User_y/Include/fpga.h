



#ifndef _FPGA_H
#define _FPGA_H



#include "GlobalRef.h"
#include "xil_printf.h"
#include "fpga_reg.h"



typedef enum {
	LOCAL_MODE = 0x0,
	LVDS_MODE  = 0x1,
}FpagMode;

typedef enum {
	ENABLE  = 0x0,
	DISABLE = 0x1,
}Rst_t;


#define Driver_INT_ID              	(61)
#define List_INT_ID              	(62)
#define RISING_EDGE_TYPE    		(0x03)	//������
#define HIGHLEVEL_TYPE      		(0x01)	//�ߵ�ƽ
#define INT_TYPE_MASK          		(0x03)	//�ж�����
#define FIFO_LEN					(0x3E80)
#define SYNC_US						(100000)


//�仯λ��
typedef enum {
	LE = 0x0,
	TE = 0x1,
}VaryPos;


extern volatile u32 HP_STATUS;
/////////////////////////////////////////////////////////////////
void SetOverflag(void);
void ClrOverflag(void);
u32  GetOverflag(void);
void StartList_WOpt(void);
void StopList_WOpt(void);

s32 AXIHP_Init(void);
s32 List_Init (void);

s32 Fpga_WOpt(struct _REG_ * Cfg, u32 Addr, u32  Data);
s32 Fpga_ROpt(struct _REG_ * Cfg, u32 Addr, u32 *Rdata);

//��DDR����
s32 DDR_ROpt (struct _REG_ * Cfg, u32 Addr, u32 *Rdata);

//дDDR����
s32 DDR_WOpt (struct _REG_ * Cfg, u32 Addr, u32 Data);

//����ִ������
s32 DDR_Period(struct _REG_ * Cfg, u32 Period);

//DDR����
s32 Debug_DDR(struct _REG_ * Cfg);

//дddr����(������ŵ�ַ)
s32 DDR_WOpt_Arg(struct _REG_ * Cfg, u32 Waddr, u32 Wlen);

//��ddr����(fpga��������RAM׼����)
s32 DDR_ROpt_Arg(struct _REG_ * Cfg, u32 Waddr, u32 Wlen);

//��У���
s32 DDR_ROpt_Check(struct _REG_ * Cfg, u32 *Rdata);

//��У���
s32 DDR_Clear_Check(struct _REG_ * Cfg);

//����ִ�б�־
s32 DDR_ROpt_Vecflag(struct _REG_ * Cfg, u32 *Rdata);

//��ȡ����ִ������
s32 DDR_ROpt_Counter(struct _REG_ * Cfg, u32 *Rdata);

//��ȡ�����������
s32 DDR_ROpt_Fifo(struct _REG_ * Cfg, u32 *Rdata);
//��ȡlist�������
s32 List_ROpt_Fifo(struct _REG_ * Cfg, u32 *Rdata);

//IO���ԼĴ���
s32 Debug_IO(struct _REG_ * Cfg);

//IO�仯λ��
s32 IO_Var_WOpt(struct _REG_ * Cfg, u16 IO, VaryPos pos, u32 value);

//IO�Ƚ�λ��
s32 IO_Cmp_WOpt(struct _REG_ * Cfg, u16 IO, VaryPos pos, u32 value);

//IO����mask
s32 IO_SetMasK(struct _REG_ * Cfg, u32 MaskL, u32 MaskH);

//дLIST
s32 Write_List(struct _LIST_ * listReg, List *list);

//��������
s32 MV_Vector(Vector_Arg *Vec, u32 Size);

//��ȡ���лؼ���
s32 Read_Back_isp(Vec_Err_Count *Err_ptr);

//������װISP
u32 AnalyzeIsp(BackIsp_Info *Ispinfo);

//��ȡ���а忨����ִ�б�־
u32 Read_Vecflag(Sample_Data  *Sample);

//����������
u8 ReadTrigger(u32 StartAddr, u32 Line);

//��fifo
u8 ReadFifo(void);

//patten�·���ɴ���ACK
void Patten_ACK_Task(void *pvParameters);
/////////////////////////////////////////////////////////////////

s32 Fpga_Init(struct _REG_ * Cfg, FpagMode mode);

//��ʼfpga
s32 StartFpga(struct _REG_ * Cfg);

//ֹͣFPGA
s32 StopFpga(struct _REG_ * Cfg);

//��ͣfpga
s32 PauseFpga(struct _REG_ * Cfg);

//��λfpga
s32 ResetFpga(struct _REG_ * Cfg, Rst_t flag);

//��ʼ����fpga
u8 StartFpgaMask(u8 Mask);

//ֹͣ����FPGA
u8 StopFpgaMask(u8 Mask);

//��ͣ����fpga
u8 PauseFpgaMask(u8 Mask);

//��λ����fpga
u8 ResetFpgaMask(Rst_t flag, u8 Mask);

//��ʼ��fpga bram
s32 Init_Bram(void);

//��λListfifo
void ResetListFifo(void);

//��ȫ��λfpga
u8 RebootFpgaMask(u8 Mask);

fpga_err_t fpga_err(struct _REG_ * Cfg);


#endif

