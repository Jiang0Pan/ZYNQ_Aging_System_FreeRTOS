






#include "fpga.h"
#include "interrupt.h"
#include "ack.h"
#include "return_val.h"
#include "debug_config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "shell_port.h"
#include "unistd.h"
#include "operation.h"
#include "orderstruct.h"






typedef u32 QueueData_t;

volatile u32 HP_STATUS = 0x0;
volatile u32 listoverflag  = 0x0;

void SetOverflag(void) {listoverflag = 0x1;}
void ClrOverflag(void) {listoverflag = 0x0;}
u32  GetOverflag(void) {return listoverflag;}


QueueHandle_t xQueue;


void Patten_ACK_Task(void *pvParameters)
{
	BaseType_t xHigherPriorityTaskWoken;

	xQueue = xQueueCreate(1, sizeof(QueueData_t));
	if(NULL == xQueue)
	{
		debug("Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		vTaskDelete(NULL);
	}
	else
	{
		u32 Bid = 0x0;
		u32 CheckSum = 0x0;
		while(1)
		{
			if(pdTRUE == xQueueReceiveFromISR(xQueue, &Bid, &xHigherPriorityTaskWoken))
			{
				//shellprintln("Recv OK ...");
				//У�����
				DDR_ROpt_Check(&(fpga_obj.Driver[Bid-1].Reg), &CheckSum);

				//��У���
				DDR_Clear_Check(&(fpga_obj.Driver[Bid-1].Reg));

				if (SEND_SUCCESS != Tcp_Ack())
					debug("Send Ack Failed In %s Files,%d Line\r\n",__FILE__,__LINE__);

				HP_STATUS = 0;
			}
			vTaskDelay(10);
		}
	}
}

static void HP_IntrHanedler(void *arg)
{
	u32 Bid = 0x0;

	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if(Reg_Opt(fpga_obj.Vec_Addr.MV_Flag))
	{
		Bid = (Reg_Opt(fpga_obj.Vec_Addr.MV_Flag))&0xf;
		Reg_Opt(fpga_obj.Vec_Addr.Serial)   = 0x0;			//���ѡ��忨�ż����ذ忨��
		Reg_Opt(fpga_obj.Vec_Addr.MV_Flag)  = 0x0;			//����OK����
	}

	//������Ϣ
	xQueueSendFromISR(xQueue, &Bid, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/***********************************************************************
	[��	       ��	]�����������·��жϳ�ʼ��
	[��            ��	]��
	[��    ��    ֵ	]��ʼ���Ƿ�ɹ�
***********************************************************************/
s32 AXIHP_Init(void)
{
	s32 Status = FAILURE;

	//ע���жϻص�����
	Status = XScuGic_Connect(&xInterruptController, Driver_INT_ID,
							(Xil_ExceptionHandler)HP_IntrHanedler,
	                        (void *)NULL);
	if(Status != XST_SUCCESS)
		return FAILURE;

	//�����ж�����
	IntrTypeSetup(Driver_INT_ID, RISING_EDGE_TYPE);

	//ʹ���ж�
	XScuGic_Enable(&xInterruptController, Driver_INT_ID);

	return SUCCESS;
}

void StartList_WOpt(void)
{
	Reg_Opt(fpga_obj.ListReg.IRQ_Mask) = 0x0;
}

void StopList_WOpt(void)
{
	Reg_Opt(fpga_obj.ListReg.IRQ_Mask) = 0x1;//�����ж�
}

static u32 line = 0;
static List *listbase = ((List *)LIST_ADDR);
static u32 i = 0;
static List *backup = NULL;
u32 loopid = 1;
int stack_status = FAILURE;
static void List_IntrHanedler(void)
{
	i = 0;
	Reg_Opt(fpga_obj.ListReg.IRQ_Mask) = 0x1;										//�����ж��ź�

	if(0 == loop_ptr->Repeat)														//repeatΪ0 ����ѭ��
	{
		for(i = (FIFO_LEN - Reg_Opt(fpga_obj.ListReg.Fifo_Depth)); i > 0; i++)
		{
			//shellprintln("   loop(%d): addr:%u ", loopid, loop_ptr->Cline->Saddr);
			Write_List(&(fpga_obj.ListReg), loop_ptr->Cline);
			if((loop_ptr->Cline - loop_ptr->Sline) + 1 == loop_ptr->Depth)			//loop���һ��
			{
				loop_ptr->Cline = loop_ptr->Sline;
			}
			else
			{
				loop_ptr->Cline += 1;
			}
		}
		Reg_Opt(fpga_obj.ListReg.IRQ_Mask) = 0x0;									//���ж��ź�
	}
	else
	{
		if(loop_ptr->count == 0)
		{
			stack_status = PopStack(loop_ptr);										//�����ϼ�loop
			if(FAILURE == stack_status)												//ջ��(����loopִ����)
			{
				shellprintln("    List Send over(1) ...");
				Reg_Opt(fpga_obj.ListReg.IRQ_Mask) = 0x1;							//�ر�list
				SetOverflag();														//list ok
				return;
			}
			else
				loopid -= 1;
		}

		if(loop_ptr->count > 0)														//��ǰѭ��repeatδִ�����
		{
			for(i = (FIFO_LEN - Reg_Opt(fpga_obj.ListReg.Fifo_Depth)); i > 0; i--)
			{
				if((loop_ptr->Cline->Saddr)&0x80000000)								//loopָ��
				{
					backup = loop_ptr->Cline;										//���ݵ�ǰloopָ����
					loop_ptr->Cline += 1;											//��ǰ��+1����ջ��֤��ջʱ������һ��
					PushStack(loop_ptr);											//��ջ��ǰloop
					loopid += 1;
					/**************************��ת���µ�ָ����****************************/
					line = (u32)((backup->Saddr)&0x7ffffff);								//�ҵ���ʼ���±�(27bit)line��1��ʼ
					loop_ptr->Sline  = &listbase[line-1];
					loop_ptr->Cline  = &listbase[line-1];
					loop_ptr->Depth  = backup->Slength;
					loop_ptr->Repeat = backup->Repeat;
					loop_ptr->count  = backup->Repeat;
				}
				else																//����ָ��
				{
					//shellprintln("   loop(%d): addr:%u ", loopid, loop_ptr->Cline->Saddr);
					Write_List(&(fpga_obj.ListReg), loop_ptr->Cline);

					if((loop_ptr->Cline - loop_ptr->Sline + 1) == loop_ptr->Depth)	//loopһ��ok
					{
						(loop_ptr->count) -= 1;
						if(loop_ptr->count == 0)									//repeat OK
						{
							stack_status = PopStack(loop_ptr);						//�����ϼ�loop
							if(FAILURE == stack_status)								//ջ��(����loopִ����)
							{
								shellprintln("    List Send over(0) ...");
								Reg_Opt(fpga_obj.ListReg.IRQ_Mask) = 0x1;			//�ر�list
								SetOverflag();										//list ok
								return;
							}
							else
							{
								loopid -= 1;
							}
						}
						else
							loop_ptr->Cline = loop_ptr->Sline;
					}
					else
						loop_ptr->Cline += 1;
				}
			}
			Reg_Opt(fpga_obj.ListReg.IRQ_Mask) = 0x0;								//���ж��ź�
		}
	}
}

/***********************************************************************
	[��	       ��	]List�жϳ�ʼ��
	[��            ��	]��
	[��    ��    ֵ	]��ʼ���Ƿ�ɹ�
***********************************************************************/
s32 List_Init(void)
{
	s32 Status = FAILURE;

	//ע���жϻص�����
	Status = XScuGic_Connect(&xInterruptController, List_INT_ID,
							(Xil_ExceptionHandler)List_IntrHanedler,
	                        (void *)NULL);
	if(Status != XST_SUCCESS)
		return FAILURE;

	//�����ж�����
	IntrTypeSetup(List_INT_ID, RISING_EDGE_TYPE);

	//ʹ���ж�
	XScuGic_Enable(&xInterruptController, List_INT_ID);

	return SUCCESS;
}

s32 Fpga_WOpt(struct _REG_ * Cfg, u32 Addr, u32  Data)
{
	u16 timeout = 1000;

	Reg_Opt(Cfg->WOpt_OK ) = 0x0;
	Reg_Opt(Cfg->Opt_Addr) = Addr;
	Reg_Opt(Cfg->W_Data  ) = Data;
	Reg_Opt(Cfg->Data_Des) = 0x01;
	Reg_Opt(Cfg->Opt_RW  ) = W_OPT;
	Reg_Opt(Cfg->Trigger ) = 0x1;
	Reg_Opt(Cfg->Trigger ) = 0x0;
	while(OK != Reg_Opt(Cfg->WOpt_OK))
	{
		if(TimeOutUS(&timeout))
			return (TIMEOUT);
	}

	return (SUCCESS);
}

s32 Fpga_ROpt(struct _REG_ * Cfg, u32 Addr, u32 *Rdata)
{
	u16 timeout = 1000;

	Reg_Opt(Cfg->Opt_Addr) = Addr;
	Reg_Opt(Cfg->W_Data  ) = 0x0;
	Reg_Opt(Cfg->Data_Des) = 0x01;
	Reg_Opt(Cfg->Opt_RW  ) = R_OPT;
	Reg_Opt(Cfg->Trigger ) = 0x1;
	Reg_Opt(Cfg->Trigger ) = 0x0;

	while(OK != (Reg_Opt(Cfg->R_Update)))
	{
		if(TimeOutUS(&timeout))
			return FAILURE;
	}

	Reg_Opt(Cfg->R_Update) = 0x0;
	Reg_Opt(Rdata) = Reg_Opt(Cfg->R_Data);

	return SUCCESS;
}

//��DDR����
s32 DDR_ROpt(struct _REG_ * Cfg, u32 Addr, u32 *Rdata)
{
	return (Fpga_ROpt(Cfg, Addr, Rdata));
}

//дDDR����
s32 DDR_WOpt(struct _REG_ * Cfg, u32 Addr, u32 Data)
{
	if(Fpga_WOpt(Cfg, Addr, Data))
		return FAILURE;
	return SUCCESS;
}

//����ִ������
s32 DDR_Period(struct _REG_ * Cfg, u32 Period)
{
	if(Fpga_WOpt(Cfg, PERIOD, Period))
		return FAILURE;

	return SUCCESS;
}

//DDR����
s32 Debug_DDR(struct _REG_ * Cfg)
{
	u32 Rdata = 0x0;
	u16 timeout = 10;
	if(Fpga_WOpt(Cfg, DDRDEBUG, (u32)TEST_VAL))
		return FAILURE;

	while(timeout--);

	if(Fpga_ROpt(Cfg, DDRDEBUG, &Rdata))
		return FAILURE;

	if((TEST_VAL + 1) != Rdata)
		return FAILURE;

	return SUCCESS;
}

//дddr����(������ŵ�ַ)
s32 DDR_WOpt_Arg(struct _REG_ * Cfg, u32 Waddr, u32 Wlen)
{
	if(Fpga_WOpt(Cfg, WTIRG, 0x0))
		return FAILURE;

	if(Fpga_WOpt(Cfg, WSADDR, Waddr))
		return FAILURE;

	if(Fpga_WOpt(Cfg, WLENGTH, Wlen))
		return FAILURE;

	if(Fpga_WOpt(Cfg, WTIRG, 0x1))
		return FAILURE;

	return SUCCESS;
}

//��ddr����(fpga��������RAM׼����)
s32 DDR_ROpt_Arg(struct _REG_ * Cfg, u32 Waddr, u32 Wlen)
{
	if(Fpga_WOpt(Cfg, RTIRG, 0x0))
		return FAILURE;

	if(Fpga_WOpt(Cfg, RSADDR, Waddr))
		return FAILURE;

	if(Fpga_WOpt(Cfg, RLENGTH, Wlen))
		return FAILURE;

	if(Fpga_WOpt(Cfg, RTIRG, 0x1))
		return FAILURE;

	return SUCCESS;
}

//��У���
s32 DDR_ROpt_Check(struct _REG_ * Cfg, u32 *Rdata)
{
	return (Fpga_ROpt(Cfg, CHECKSUM, Rdata));
}

//��У���
s32 DDR_Clear_Check(struct _REG_ * Cfg)
{
	if(Fpga_WOpt(Cfg, CLRCHECK, 0x1))
		return FAILURE;
	return SUCCESS;
}

//����ִ�б�־
s32 DDR_ROpt_Vecflag(struct _REG_ * Cfg, u32 *Rdata)
{
	return (Fpga_ROpt(Cfg, EXEFLAG, Rdata));
}

//��ȡ����ִ������
s32 DDR_ROpt_Counter(struct _REG_ * Cfg, u32 *Rdata)
{
	return (Fpga_ROpt(Cfg, COUNTER, Rdata));
}

//��ȡ�����������
s32 DDR_ROpt_Fifo(struct _REG_ * Cfg, u32 *Rdata)
{
	return (Fpga_ROpt(Cfg, VECFIFOLEN, Rdata));
}

//��ȡlist�������
s32 List_ROpt_Fifo(struct _REG_ * Cfg, u32 *Rdata)
{
	return (Fpga_ROpt(Cfg, LISTFIFOLEN, Rdata));
}

//IO���ԼĴ���
s32 Debug_IO(struct _REG_ * Cfg)
{
	u32 Rdata = 0x0;
	if(Fpga_WOpt(Cfg, IODEBUG, TEST_VAL))
		return FAILURE;

	if(Fpga_ROpt(Cfg, IODEBUG, &Rdata))
		return FAILURE;

	if((TEST_VAL + 1) != Rdata)
		return FAILURE;

	return SUCCESS;
}

//IO�仯λ��
s32 IO_Var_WOpt(struct _REG_ * Cfg, u16 IO, VaryPos pos, u32 value)
{
	u32 Reg = ((IO > 192) ? CHANVAR : IOCFG) + (IO/8 * 64) + (32 * pos) + ((IO%8) *4);
	if(Fpga_WOpt(Cfg, Reg, value))
		return FAILURE;

	return SUCCESS;
}

//IO�Ƚ�λ��
s32 IO_Cmp_WOpt(struct _REG_ * Cfg, u16 IO, VaryPos pos, u32 value)
{
	if(Fpga_WOpt(Cfg, CHANCMP + (IO/8 * 64) + (32 * pos) + ((IO%8) *4) , value))
		return FAILURE;

	return SUCCESS;
}

//��ʼ��fpga bram
s32 Init_Bram(void)
{
	if(Fpga_WOpt(&(fpga_obj.Driver[3].Reg), INITBRAM , 0x1))
		return FAILURE;

	if(Fpga_WOpt(&(fpga_obj.Driver[3].Reg), INITBRAM , 0x0))
		return FAILURE;

	if(Fpga_WOpt(&(fpga_obj.Driver[4].Reg), INITBRAM , 0x1))
		return FAILURE;

	if(Fpga_WOpt(&(fpga_obj.Driver[4].Reg), INITBRAM , 0x0))
		return FAILURE;

	return SUCCESS;
}


//ͨ����������
s32 IO_SetMasK(struct _REG_ * Cfg, u32 MaskL, u32 MaskH)
{
	if(Fpga_WOpt(Cfg, CHANMASK, MaskL))
		return FAILURE;

	if(Fpga_WOpt(Cfg, CHANMASK+4, MaskH))
		return FAILURE;

	return SUCCESS;
}

s32 MV_Vector(Vector_Arg *Vec, u32 Size)
{
	u16 timeout = 20;							//��ʱʱ��

	//����DDR�Ƿ�OK
	// if(Debug_DDR(&(fpga_obj.Driver[Vec->fpga_CS].Reg)))
	// 	return FAILURE;

	//����ַ
	if(DDR_WOpt_Arg(&(fpga_obj.Driver[Vec->fpga_CS].Reg), Vec->lseek, Size/VEC_UNIT))
		return FAILURE;

	(fpga_obj.Driver[(Vec->fpga_CS)].VecSize) += (Size);	//ÿ��������С�ۼ�

	//Ϊ��֤��������׼ȷÿ�ΰ��ƶ�д����ֵ
	Reg_Opt(fpga_obj.Vec_Addr.Debug) = TEST_VAL;
	while(Reg_Opt(fpga_obj.Vec_Addr.Debug) != TEST_VAL+1)	//�ȴ���Ӧ
	{
		if(TimeOutUS(&timeout))
			return FAILURE;
	}

	Reg_Opt(fpga_obj.Vec_Addr.Clr_fifo) = 0x1;				//���fifo
	Reg_Opt(fpga_obj.Vec_Addr.MV_Flag ) = 0x0;				//��ȡ��־OK��0
	Reg_Opt(fpga_obj.Vec_Addr.Serial  ) = 0x0;				//����忨���
	Reg_Opt(fpga_obj.Vec_Addr.S_Addr  ) = (u32)(Vec->Data);	//�������ݵ�ַ
	Reg_Opt(fpga_obj.Vec_Addr.D_Len   ) = Size;				//�������ݳ���
	Reg_Opt(fpga_obj.Vec_Addr.Serial  ) = (Vec->fpga_CS)+1; //Ŀ��忨��,PL��Ŵ�1��ʼ
	Reg_Opt(fpga_obj.Vec_Addr.W_Trig  ) = 0x1;				//����PL

	return SUCCESS;
}

//��ȡ��������
s32 Read_Back_isp(Vec_Err_Count *Err_ptr)
{
	u16 i = 0x0, bit = 0;
	u16 j = 0x0, pin = 192;
	u32 Mask = 0x0;
	u32 Line = 0x0;

	memset(&All_isp, 0, sizeof(All_isp));

	for(i = 3; i < DRIV_BOARD_NUM; i++)
	{
		for(j = 0; j < NUM_ISP; j++)
		{
			Mask = 0x0;
			Line = 0x0;
			Fpga_WOpt(&(fpga_obj.Driver[i].Reg), j, RAMADDR);
			Fpga_ROpt(&(fpga_obj.Driver[i].Reg), ERRORMASK, &Mask);
			if(!Mask)												//����
				break;

			Fpga_ROpt(&(fpga_obj.Driver[i].Reg), ERRORADDR, &Line);
			if(!Line)
				break;

			All_isp.db[i/4].isp[j].Mask = Mask;
			All_isp.db[i/4].isp[j].Addr = Line;
			All_isp.db[i/4].count += 1;
			shellprintln("Addr:%x\tMask:%x", Line, Mask);
			for(bit = 0; bit < 32; bit++)
			{
				if((Mask >> bit)&0x1)
					Err_ptr->Err_Cnt[pin+bit] += 1;
			}
		}
		pin += 32;
	}
	Init_Bram();
	return SUCCESS;
}


void Build256BIT(u32 *bitarray, u8 bit)
{
	if(bit > 31 )
		bitarray[7] |= (0x1 << (bit-32));
	else
		bitarray[6] |= (0x1 << bit);
}


#define swap(a, b) {(a) ^= (b); (b) ^= (a); (a) ^= (b);}

void SortArray(struct _Mask_ Array[], u32 length)
{
	u32 i = 0, j = 0;

	for (i = 0; i < length -1; i++)
	{
		for (j = 0; j < length -1 -i; j++)
		{
			if (Array[j].Addr > Array[j + 1].Addr)
			{
				swap(Array[j].Addr, Array[j + 1].Addr);
				swap(Array[j].Mask, Array[j + 1].Mask);
			}
		}
	}
}

//����ISP
u32 AnalyzeIsp(BackIsp_Info *Ispinfo)
{
	u32 i = 0, j = 0, num = 0;

	//����
	if(All_isp.db[0].count)
		SortArray(All_isp.db[0].isp, All_isp.db[0].count);

	if(All_isp.db[1].count)
		SortArray(All_isp.db[1].isp, All_isp.db[1].count);

	for(i = 0; i < All_isp.db[0].count; i++)
	{
		Ispinfo->info[num].Addr = All_isp.db[0].isp[i].Addr;

		Ispinfo->info[num].Mask[6] |= All_isp.db[0].isp[i].Mask;

		for(i = 0; i < All_isp.db[1].count; i++)
		{
			if(All_isp.db[0].isp[i].Addr == All_isp.db[1].isp[j].Addr)
			{
				Ispinfo->info[num].Mask[7] |= All_isp.db[1].isp[i].Mask;
			}
			num++;
		}
	}

	return num;
}

u32 Read_Vecflag(Sample_Data  *Sample)
{
	u16 i = 0;
	u32 Rdata = 0x0, ret = 0x0;
	for(i = 0; i < DRIV_BOARD_NUM; i++)
	{
		Rdata = 0x0;
		if(DDR_ROpt_Vecflag(&(fpga_obj.Driver[i].Reg), &Rdata))
		{
			Sample->ErrStatus |= 0x1 << (16+i);
			ret |= (0x1 << i);
		}
		else
		{
			if(0 == ((Rdata >> 1)&0x1))
				ret |= (0x1 << i);
		}
	}

	return ret;
}

//����������
u8 ReadTrigger(u32 StartAddr, u32 Line)
{
	u16 i = 0;
	u8 Stat = 0;

	for(i = 0; i < DRIV_BOARD_NUM; i++)
	{
		if(Fpga_WOpt(&(fpga_obj.Driver[i].Reg), STREAMFLAG, 0))
			Stat |= 0x1 << i;

		if(DDR_ROpt_Arg(&(fpga_obj.Driver[i].Reg), StartAddr, Line))
			Stat |= 0x1 << i;
	}

	return Stat;
}

//��fifo
u8 ReadFifo(void)
{
	u16 i = 0;
	u16 timeout = 10;
	u8 Stat = 0b11111;
	u32 Rdata = 0x0;
	do {
		for(i = 0; i < DRIV_BOARD_NUM; i++)
		{
			if((Stat >> i) & 0x1)
			{
				Rdata = 0x0;
				if(SUCCESS == DDR_ROpt_Fifo(&(fpga_obj.Driver[i].Reg), &Rdata))
				{
					if(Rdata)
						Stat &= ~(0x1 << i);
				}
			}
			else
				continue;
		}
		if(TimeOutUS(&timeout))
			break;

	}while(Stat);

	return Stat;
}

//List write
s32 Write_List(struct _LIST_ * listReg, List *list)
{
	Reg_Opt(listReg->List_L ) = list->Saddr;
	Reg_Opt(listReg->List_M ) = list->Slength;
	Reg_Opt(listReg->List_H ) = list->Repeat;
	Reg_Opt(listReg->WOpt_OK) = 0x1;

	return SUCCESS;
}

///////////////////////////////////Fpga ctl/////////////////////////////////////////

s32 Fpga_Init(struct _REG_ * Cfg, FpagMode mode)
{
	u32 get_mod = LOCAL_MODE;

	if(Fpga_ROpt(Cfg, STREAMFLAG, &get_mod))
		return FAILURE;

	if(mode == get_mod)
		return SUCCESS;

	if(Fpga_WOpt(Cfg, STREAMFLAG, (u32)mode))
		return FAILURE;

	return SUCCESS;
}

//��ʼfpga
s32 StartFpga(struct _REG_ * Cfg)
{
	if(Fpga_WOpt(Cfg, CYCLE, (u32)0x0))
		return FAILURE;

	return SUCCESS;
}

//ֹͣFPGA
s32 StopFpga(struct _REG_ * Cfg)
{
	if(Fpga_WOpt(Cfg, CYCLE, (u32)0x1))
		return FAILURE;

	return SUCCESS;
}

//��ͣfpga
s32 PauseFpga(struct _REG_ * Cfg)
{
	if(Fpga_WOpt(Cfg, CYCLE, 0x2))
		return FAILURE;

	return SUCCESS;
}

//��λfpga
s32 ResetFpga(struct _REG_ * Cfg, Rst_t flag)
{
	if(Fpga_WOpt(Cfg, RESET, (u32)flag))
		return FAILURE;

	return SUCCESS;
}

//ͬ���ź�
static void SyncFpga(u32 us)
{
	usleep(500);
	Set_pin_value(SYS_TRIG, 1);
	usleep(us);
	Set_pin_value(SYS_TRIG, 0);
}


//��ʼ����fpga
u8 StartFpgaMask(u8 Mask)
{
	u16 i = 0;
	u8 ret = 0x0;
	for(i = 0; i < DRIV_BOARD_NUM; i++)
	{
		if((Mask >> i)&0x1)
		{
			if(SUCCESS == StartFpga(&(fpga_obj.Driver[i].Reg)))
				ret |= 0x1 <i;
		}
	}
	SyncFpga(SYNC_US);

	return ret;
}

//ֹͣ����FPGA
u8 StopFpgaMask(u8 Mask)
{
	u16 i = 0;
	u8 ret = 0x0;
	for(i = 0; i < DRIV_BOARD_NUM; i++)
	{
		if((Mask >> i)&0x1)
		{
			if(SUCCESS == StopFpga(&(fpga_obj.Driver[i].Reg)))
				ret |= 0x1 <i;
		}
	}
	SyncFpga(SYNC_US);

	return ret;
}

//��ͣ����fpga
u8 PauseFpgaMask(u8 Mask)
{
	u16 i = 0;
	u8 ret = 0x0;
	for(i = 0; i < DRIV_BOARD_NUM; i++)
	{
		if((Mask >> i)&0x1)
		{
			if(SUCCESS == PauseFpga(&(fpga_obj.Driver[i].Reg)))
				ret |= 0x1 <i;
		}
	}
	SyncFpga(SYNC_US);

	return ret;
}

//��λ����fpga
u8 ResetFpgaMask(Rst_t flag, u8 Mask)
{
	u16 i = 0;
	u8 ret = 0x0;
	for(i = 0; i < DRIV_BOARD_NUM; i++)
	{
		if((Mask >> i)&0x1)
		{
			if(SUCCESS == ResetFpga(&(fpga_obj.Driver[i].Reg), flag))
				ret |= 0x1 <i;
		}
	}

	return ret;
}

//��λPL fifo
void ResetListFifo(void)
{
	Reg_Opt(fpga_obj.ListReg.IRQ_Mask) = 0x1;//����PL�ж�
	Reg_Opt(fpga_obj.ListReg.Fifo_rst) = 0x1;//��λPL fifo
	Reg_Opt(fpga_obj.ListReg.Fifo_rst) = 0x0;//��λPL fifo
}

//��ȫ��λfpga
u8 RebootFpgaMask(u8 Mask)
{
	u8 ret = 0x0;
	ret = ResetFpgaMask(ENABLE, Mask);
	usleep(10000);							//�ȴ���λ
	ret |= ResetFpgaMask(DISABLE, Mask);

	return ret;
}

//��ȡִ��״̬
fpga_err_t fpga_err(struct _REG_ * Cfg)
{
	fpga_err_t err = 0;
	if(Fpga_ROpt(Cfg, STATUS, &err))
		return 0xff;

	return err;
}




