



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
#define RISING_EDGE_TYPE    		(0x03)	//上升沿
#define HIGHLEVEL_TYPE      		(0x01)	//高电平
#define INT_TYPE_MASK          		(0x03)	//中断掩码
#define FIFO_LEN					(0x3E80)
#define SYNC_US						(100000)


//变化位置
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

//读DDR数据
s32 DDR_ROpt (struct _REG_ * Cfg, u32 Addr, u32 *Rdata);

//写DDR数据
s32 DDR_WOpt (struct _REG_ * Cfg, u32 Addr, u32 Data);

//配置执行周期
s32 DDR_Period(struct _REG_ * Cfg, u32 Period);

//DDR测试
s32 Debug_DDR(struct _REG_ * Cfg);

//写ddr参数(向量存放地址)
s32 DDR_WOpt_Arg(struct _REG_ * Cfg, u32 Waddr, u32 Wlen);

//读ddr参数(fpga读向量到RAM准备好)
s32 DDR_ROpt_Arg(struct _REG_ * Cfg, u32 Waddr, u32 Wlen);

//读校验和
s32 DDR_ROpt_Check(struct _REG_ * Cfg, u32 *Rdata);

//清校验和
s32 DDR_Clear_Check(struct _REG_ * Cfg);

//向量执行标志
s32 DDR_ROpt_Vecflag(struct _REG_ * Cfg, u32 *Rdata);

//读取向量执行数量
s32 DDR_ROpt_Counter(struct _REG_ * Cfg, u32 *Rdata);

//读取向量缓存深度
s32 DDR_ROpt_Fifo(struct _REG_ * Cfg, u32 *Rdata);
//读取list缓存深度
s32 List_ROpt_Fifo(struct _REG_ * Cfg, u32 *Rdata);

//IO调试寄存器
s32 Debug_IO(struct _REG_ * Cfg);

//IO变化位置
s32 IO_Var_WOpt(struct _REG_ * Cfg, u16 IO, VaryPos pos, u32 value);

//IO比较位置
s32 IO_Cmp_WOpt(struct _REG_ * Cfg, u16 IO, VaryPos pos, u32 value);

//IO设置mask
s32 IO_SetMasK(struct _REG_ * Cfg, u32 MaskL, u32 MaskH);

//写LIST
s32 Write_List(struct _LIST_ * listReg, List *list);

//搬移向量
s32 MV_Vector(Vector_Arg *Vec, u32 Size);

//读取所有回检结果
s32 Read_Back_isp(Vec_Err_Count *Err_ptr);

//分析组装ISP
u32 AnalyzeIsp(BackIsp_Info *Ispinfo);

//读取所有板卡向量执行标志
u32 Read_Vecflag(Sample_Data  *Sample);

//读向量触发
u8 ReadTrigger(u32 StartAddr, u32 Line);

//读fifo
u8 ReadFifo(void);

//patten下发完成处理ACK
void Patten_ACK_Task(void *pvParameters);
/////////////////////////////////////////////////////////////////

s32 Fpga_Init(struct _REG_ * Cfg, FpagMode mode);

//开始fpga
s32 StartFpga(struct _REG_ * Cfg);

//停止FPGA
s32 StopFpga(struct _REG_ * Cfg);

//暂停fpga
s32 PauseFpga(struct _REG_ * Cfg);

//复位fpga
s32 ResetFpga(struct _REG_ * Cfg, Rst_t flag);

//开始所有fpga
u8 StartFpgaMask(u8 Mask);

//停止所有FPGA
u8 StopFpgaMask(u8 Mask);

//暂停所有fpga
u8 PauseFpgaMask(u8 Mask);

//复位所有fpga
u8 ResetFpgaMask(Rst_t flag, u8 Mask);

//初始化fpga bram
s32 Init_Bram(void);

//复位Listfifo
void ResetListFifo(void);

//完全复位fpga
u8 RebootFpgaMask(u8 Mask);

fpga_err_t fpga_err(struct _REG_ * Cfg);


#endif

