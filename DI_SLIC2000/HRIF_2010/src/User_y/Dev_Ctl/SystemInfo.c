

#include "SystemInfo.h"
#include "string.h"
#include "stdio.h"
#include "insert_status.h"
#include "inttypes.h"



//读取一块驱动板的信息
void ReadDBinfo(u8 Bid)
{
	u8 HwVer = 0;

	ChipInfo Chip;
	memset(&Chip, 0, sizeof(Chip));

	Fpga_ROpt(&(fpga_obj.Driver[Bid].Reg),  VERSION, &Chip.Version.V_Data);
	Fpga_ROpt(&(fpga_obj.Driver[Bid].Reg),     DATE, &Chip.Date.D_Data   );
	Fpga_ROpt(&(fpga_obj.Driver[Bid].Reg),     TIME, &Chip.Time.T_Data   );
	Fpga_ROpt(&(fpga_obj.Driver[Bid].Reg),     TEMP, &Chip.Core_temp     );
	Fpga_ROpt(&(fpga_obj.Driver[Bid].Reg),     DAN1, &Chip.DAN.DANL      );
	Fpga_ROpt(&(fpga_obj.Driver[Bid].Reg),     DAN2, &Chip.DAN.DANH      );


	sprintf((char *)(fpga_obj.Driver[Bid].Msg.Version), "%d.%d",
			((Chip.Version.Publish_V.Major)&0xf),
			((Chip.Version.Publish_V.Minor)&0xf));

	sprintf((char *)(fpga_obj.Driver[Bid].Msg.CompTime), "%d%d%d%d/%d%d/%d%d %d%d:%d%d",
			((Chip.Date.Year  )>>12)&0xf, ((Chip.Date.Year  )>>8)&0xf,
			((Chip.Date.Year  )>> 4)&0xf, ((Chip.Date.Year  )>>0)&0xf,
			((Chip.Date.Month )>> 4)&0xf, ((Chip.Date.Month )>>0)&0xf,
			((Chip.Date.Day   )>> 4)&0xf, ((Chip.Date.Day   )>>0)&0xf,
			((Chip.Time.Hour  )>> 4)&0xf, ((Chip.Time.Hour  )>>0)&0xf,
			((Chip.Time.Minute)>> 4)&0xf, ((Chip.Time.Minute)>>0)&0xf);

	fpga_obj.Driver[Bid].Msg.ChipTemp  = ((Chip.Core_temp) * CORE_TEMP_COE)/4096 - CORE_TEMP_OFFSET;
	fpga_obj.Driver[Bid].Msg.ChipDAN.DANH = Chip.DAN.DANH;
	fpga_obj.Driver[Bid].Msg.ChipDAN.DANL = Chip.DAN.DANL;

	ReadIO_Info(&HwVer, fpga_obj.PCA953_SlaveAddr, fpga_obj.Driver[Bid].Msg.HWChan);
	fpga_obj.Driver[Bid].Msg.HWVersion = (HwVer >> 4) & 0xf;
	fpga_obj.Driver[Bid].Msg.PCBType   = (HwVer >> 0) & 0xf;

	EEPROM_ReadBuff( 0, fpga_obj.Driver[Bid].Msg.PN, 32, fpga_obj.EEPROM_SlaveAddr, fpga_obj.Driver[Bid].Msg.HWChan);
	CRC8(fpga_obj.Driver[Bid].Msg.PN, 31);
	EEPROM_ReadBuff(32, fpga_obj.Driver[Bid].Msg.SN, 32, fpga_obj.EEPROM_SlaveAddr, fpga_obj.Driver[Bid].Msg.HWChan);
	CRC8(fpga_obj.Driver[Bid].Msg.SN, 31);
}

//读取电源信息
void ReadDpsinfo(void)
{
	u8 i = 0, tp = 0x0;

	DB_Info_t  *dp[2];
	dp[0] = (DB_Info_t *)DPSB0_INFO_ADDR;
	dp[1] = (DB_Info_t *)DPSB1_INFO_ADDR;

	for(i = 0; i < 2; i++) {
		IO_Group_Get(Group0, &tp, dps_info.PCA953_SlaveAddr, dps_info.DBMsg[i].Chan);
		dps_info.DBMsg[i].HWVersion = (tp >> 4) & 0xf;
		dps_info.DBMsg[i].PCBType   = (tp >> 0) & 0xf;
		EEPROM_ReadBuff( 0, dps_info.DBMsg[i].PN, 32, dps_info.EEPROM_SlaveAddr, dps_info.DBMsg[i].Chan);
		CRC8(dps_info.DBMsg[i].PN, 31);
		EEPROM_ReadBuff(32, dps_info.DBMsg[i].SN, 32, dps_info.EEPROM_SlaveAddr, dps_info.DBMsg[i].Chan);
		CRC8(dps_info.DBMsg[i].SN, 31);

		memcpy(dp[i]->DB_ERP_PN,   dps_info.DBMsg[i].PN, 32);
		memcpy(dp[i]->DB_ERP_SN,   dps_info.DBMsg[i].SN, 32);
		memcpy(dp[i]->DB_VSN_HW,  &dps_info.DBMsg[i].HWVersion, 2);
		memcpy(dp[i]->DB_OTR_BT,  &dps_info.DBMsg[i].PCBType, 4);
	}
}

//读取背板信息
void ReadBKinfo(void)
{
	u8 tp = 0x0;

	DB_Info_t * bk = (DB_Info_t *)BKB_INFO_ADDR;

	IO_Group_Get(Group0, &tp, bbd_info.PCA953_SlaveAddr, bbd_info.HWChan);
	bbd_info.HWVersion = (tp >> 4) & 0xf;
	bbd_info.PCBType   = (tp >> 0) & 0xf;
	EEPROM_ReadBuff( 0, bbd_info.PN, 32, bbd_info.EEPROM_SlaveAddr, bbd_info.HWChan);
	CRC8(bbd_info.PN, 31);
	EEPROM_ReadBuff(32, bbd_info.SN, 32, bbd_info.EEPROM_SlaveAddr, bbd_info.HWChan);
	CRC8(bbd_info.SN, 31);

	memcpy(bk->DB_ERP_PN,   bbd_info.PN, 32);
	memcpy(bk->DB_ERP_SN,   bbd_info.SN, 32);
	memcpy(bk->DB_VSN_HW,  &bbd_info.HWVersion, 2);
	memcpy(bk->DB_OTR_BT,  &bbd_info.PCBType, 4);
}

//读取主板信息
void ReadMBinfo(void)
{
	u8 tp = 0x0;
	char Str[16] = {0};

	MB_Info_t *mb = (MB_Info_t *)MB_INFO_ADDR;

	//pl芯片信息
	ChipInfo plMsg;
	memset(&plMsg, 0, sizeof(ChipInfo));

	plMsg.Version.V_Data = PL_VERSION;
	plMsg.Date.D_Data    = PL_DATE;
	plMsg.Time.T_Data    = PL_TIME;
	plMsg.Core_temp		 = PL_TEMP;
	plMsg.DAN.DANL       = PL_DAN1;
	plMsg.DAN.DANH       = PL_DAN2;

	IO_Group_Get(Group0, &tp, mbd_info.PCA953_SlaveAddr, mbd_info.HWChan);
	mbd_info.HWVersion = (tp >> 4) & 0xf;
	mbd_info.PCBType   = (tp >> 0) & 0xf;
	EEPROM_ReadBuff( 0, mbd_info.PN, 32, mbd_info.EEPROM_SlaveAddr, mbd_info.HWChan);
	CRC8(mbd_info.PN, 31);
	EEPROM_ReadBuff(32, mbd_info.SN, 32, mbd_info.EEPROM_SlaveAddr, mbd_info.HWChan);
	CRC8(mbd_info.SN, 31);

	memcpy(mbd_info.Version0, ZYNQ_VERSION, strlen(ZYNQ_VERSION));

	memcpy(Str, __DATE__, strlen(__DATE__));
	int year  = ((((Str[7]-'0')*10+(Str[8]-'0'))*10+(Str[9]-'0'))*10+(Str[10]-'0'));
	int month = (Str[2] == 'n' ? (Str[1] == 'a' ? 1 : 6) \
							 : Str[2] == 'b' ? 2 \
							 : Str[2] == 'r' ? (Str[0] == 'M' ? 3 : 4) \
							 : Str[2] == 'y' ? 5 \
							 : Str[2] == 'l' ? 7 \
							 : Str[2] == 'g' ? 8 \
							 : Str[2] == 'p' ? 9 \
							 : Str[2] == 't' ? 10\
							 : Str[2] == 'v' ? 11 : 12);
	int day   = ((Str [4] == ' ' ? 0 : Str[4] - '0') * 10 + (Str[5] - '0'));

	memcpy(Str, __TIME__, strlen(__TIME__));
	int hour  = ((Str[0] - '0') * 10 + (Str[1] - '0'));
	int minute= ((Str[3] - '0') * 10 + (Str[4] - '0'));
	sprintf((char*)(mbd_info.CompTime0), "%d/%02d/%02d %02d:%02d", year, month, day, hour, minute);


	sprintf((char *)(mbd_info.Version1), "%d.%d",
			((plMsg.Version.Publish_V.Major)&0xf),
			((plMsg.Version.Publish_V.Minor)&0xf));

	sprintf((char *)(mbd_info.CompTime1), "%d%d%d%d/%d%d/%d%d %d%d:%d%d",
			   ((plMsg.Date.Year  )>>12)&0xf, ((plMsg.Date.Year  )>>8)&0xf,
			   ((plMsg.Date.Year  )>> 4)&0xf, ((plMsg.Date.Year  )>>0)&0xf,
			   ((plMsg.Date.Month )>> 4)&0xf, ((plMsg.Date.Month )>>0)&0xf,
			   ((plMsg.Date.Day   )>> 4)&0xf, ((plMsg.Date.Day   )>>0)&0xf,
			   ((plMsg.Time.Hour  )>> 4)&0xf, ((plMsg.Time.Hour  )>>0)&0xf,
			   ((plMsg.Time.Minute)>> 4)&0xf, ((plMsg.Time.Minute)>>0)&0xf);

	mbd_info.ChipTemp = ((plMsg.Core_temp) * CORE_TEMP_COE)/4096 - CORE_TEMP_OFFSET;
	mbd_info.ChipDAN.DANH  = plMsg.DAN.DANH;
	mbd_info.ChipDAN.DANL  = plMsg.DAN.DANL;

	memcpy(mb->MB_ERP_PN,     mbd_info.PN,        32);
	memcpy(mb->MB_ERP_SN,     mbd_info.SN,        32);
	memcpy(mb->MB_VSN_HW,    &mbd_info.HWVersion,  2);
	memcpy(mb->MB_VSN_FW0,    mbd_info.Version0,  16);
	memcpy(mb->MB_VSN_FWCT0,  mbd_info.CompTime0, 16);
	memcpy(mb->MB_VSN_FW1,    mbd_info.Version1,  16);
	memcpy(mb->MB_VSN_FWCT1,  mbd_info.CompTime1, 16);
	memcpy(mb->MB_OTR_BT,    &mbd_info.PCBType,    4);
}

//系统硬件设备初始化
void BoardHW_Init(void)
{
	u16 i = 0;

	//主板初件初始化
	PCA_IO_Init(INPUT, OUTPUT, &(mbd_info.Output_LVL), mbd_info.PCA953_SlaveAddr, mbd_info.HWChan);
	PCA_IO_Init(INPUT, OUTPUT, &(mbd_info.IP_ADDR),    mbd_info.PCA953_SlaveAddr, mbd_info.HWChan);

	//背板硬件初始化
	PCA_IO_Init( INPUT, OUTPUT, &(bbd_info.Output_LVL), bbd_info.PCA953_SlaveAddr, bbd_info.HWChan);
	PCA_IO_Init(OUTPUT, OUTPUT, &(bbd_info.Ctl_Output),     bbd_info.IO_SlaveAddr, bbd_info.TOChan);
	Switch_12V(ON);//打开12V

	//电源板0硬件初始化
	PCA_IO_Init(INPUT, OUTPUT, &(dps_info.DBMsg[0].Output_LVL), dps_info.PCA953_SlaveAddr, dps_info.DBMsg[0].Chan);
	//Set_AD5694(0, 2, dps_info.AD5694_SlaveAddr, dps_info.DBMsg[0].Chan);
	//Set_AD5694(0, 3, dps_info.AD5694_SlaveAddr, dps_info.DBMsg[0].Chan);

	//电源板1硬件初始化
	PCA_IO_Init(INPUT, OUTPUT, &(dps_info.DBMsg[1].Output_LVL), dps_info.PCA953_SlaveAddr, dps_info.DBMsg[1].Chan);
	//Set_AD5694(0, 2, dps_info.AD5694_SlaveAddr, dps_info.DBMsg[1].Chan);
	//Set_AD5694(0, 3, dps_info.AD5694_SlaveAddr, dps_info.DBMsg[1].Chan);

	//驱动板硬件初始化
	for(i = 0; i < DRIV_BOARD_NUM; i++) {

		PCA_IO_Init(INPUT, OUTPUT, &(fpga_obj.Driver[i].Msg.Output_LVL), fpga_obj.PCA953_SlaveAddr, fpga_obj.Driver[i].Msg.HWChan);

		//关闭fpga spi
		IO_Level_Set(&(fpga_obj.Driver[i].Msg.Output_LVL), Group1, pin2, 1, fpga_obj.PCA953_SlaveAddr, fpga_obj.Driver[i].Msg.HWChan);

		//初始化528
		Max528_Init(&(fpga_obj.Driver[i].Reg));

		//初始化9972
		Max9972_InitCfg(&(fpga_obj.Driver[i].Reg));
	}

	/* Dual slot information */
	PCA_IO_Init(INPUT, OUTPUT, &(dual_slot_info.Output_LVL), dual_slot_info.PCA953_SlaveAddr_0, dual_slot_info.Chan);
	PCA_IO_Init(INPUT, OUTPUT, &(dual_slot_info.Slot_Insert), dual_slot_info.PCA953_SlaveAddr_1, dual_slot_info.Chan);
}

//读取系统设备信息
void ReadSysteminfo(void)
{
	u16 i = 0;
	DB_Info_t  *db[DRIV_BOARD_NUM];

	db[0] = (DB_Info_t *)DB0_INFO_ADDR;
	db[1] = (DB_Info_t *)DB1_INFO_ADDR;
	db[2] = (DB_Info_t *)DB2_INFO_ADDR;
	db[3] = (DB_Info_t *)DB3_INFO_ADDR;
	db[4] = (DB_Info_t *)DB4_INFO_ADDR;

	ReadMBinfo();
	ReadBKinfo();
	ReadDpsinfo();

	for(i = 0; i < DRIV_BOARD_NUM; i++)
	{
		ReadDBinfo(i);			//读取芯片信息
		memcpy(db[i]->DB_ERP_PN,   fpga_obj.Driver[i].Msg.PN, 32);
		memcpy(db[i]->DB_ERP_SN,   fpga_obj.Driver[i].Msg.SN, 32);
		memcpy(db[i]->DB_VSN_HW,  &fpga_obj.Driver[i].Msg.HWVersion, 2);
		memcpy(db[i]->DB_VSN_FW,   fpga_obj.Driver[i].Msg.Version, 16);
		memcpy(db[i]->DB_VSN_FWCT, fpga_obj.Driver[i].Msg.CompTime, 16);
		memcpy(db[i]->DB_OTR_BT,  &fpga_obj.Driver[i].Msg.PCBType, 4);
	}

	ReadDualSlotInfo();
}









