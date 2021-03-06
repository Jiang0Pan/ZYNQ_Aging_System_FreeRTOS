




#include "Debug.h"
#include "stdlib.h"
#include "stdio.h"
#include "Systeminfo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "shell_port.h"
#include "max528.h"
#include "SMBus.h"
#include "max9972.h"
#include "unistd.h"
#include "operation.h"
#include "ad5694.h"
#include "fpga_ctrl.h"
#include "isl28023.h"
#include "isl8272.h"
#include "system_state.h"
#include "insert_status.h"




void Debug_float(const char *name, float value)
{
	char fdat[20] = "0";
	sprintf(fdat, "%.2f", value);
	shellprintf("%s:%s\r\n", name, fdat);
}

//Debugִ�к���
s32 Debug_MV_Vector(int argc, char * argv[])
{
	u16 timeout = 20;
	u32 frm = atoi(argv[3]);
	u32 Serial = strtoul(argv[1], NULL, 10);

	//����DDR�Ƿ�OK
	if(Debug_DDR(&(fpga_obj.Driver[Serial-1].Reg)))
		return FAILURE;

	//����ַ
	if(DDR_WOpt_Arg(&(fpga_obj.Driver[Serial-1].Reg), 0, frm))
		return FAILURE;

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
	Reg_Opt(fpga_obj.Vec_Addr.S_Addr  ) = strtoul(argv[2], NULL, 16);	//������ݵ�ַ
	Reg_Opt(fpga_obj.Vec_Addr.D_Len   ) = frm * 32;			//������ݳ���
	Reg_Opt(fpga_obj.Vec_Addr.Serial  ) = Serial; //Ŀ��忨��,PL��Ŵ�1��ʼ
	Reg_Opt(fpga_obj.Vec_Addr.W_Trig  ) = 0x1;				//����PL

	shellprintf("    Move reg set ok\r\n");


	return SUCCESS;
}

//read   boardid  eeprom  addr   length
s32 ReadEEPROM(int argc, char * argv[])
{
	//EEPROM ����
	u8 eeprom_buffer[32] = {0};

	u16 Bid  = atoi(argv[1]);
	u16 Addr = strtoul(argv[3], NULL, 16);
	u16 len  = (32 > atoi (argv[4])) ? atoi (argv[4]) : 31;
	u8 crc8  = 0;

	if(0 == strcmp("eeprom", argv[2]))
	{
		memset(eeprom_buffer, 0, 32);
		shellprintf("\r\n");
		shellprintf("eeprom read ....\r\n");
		if(0 == Bid)									//���ذ�
		{
			if(EEPROM_ReadBuff(Addr, eeprom_buffer, 32, mbd_info.EEPROM_SlaveAddr, mbd_info.HWChan))
				shellprintf("eeprom read error!\r\n");
			else
			{
				crc8 = CRC8(eeprom_buffer, len);		//У��
				if(crc8 != eeprom_buffer[31])
					shellprintf("Crc8:%x, Read crc8:%x\r\n", crc8, eeprom_buffer[31]);
				else
					shellprintf("ReadData:%s\r\n", eeprom_buffer);
			}
		}
		else if(Bid >= 1 && Bid < 6)					//������
		{
			if(EEPROM_ReadBuff(Addr, eeprom_buffer, 32,  fpga_obj.EEPROM_SlaveAddr,  fpga_obj.Driver[Bid-1].Msg.HWChan))
				shellprintf("eeprom read error!\r\n");
			else
			{
				crc8 = CRC8(eeprom_buffer, len);		//У��
				if(crc8 != eeprom_buffer[31])
					shellprintf("Read crc8:%x, crc8:%x\r\n", crc8, eeprom_buffer[31]);
				else
					shellprintf("ReadData:%s\r\n", eeprom_buffer);
			}
		}
		else if(Bid >= 6 && Bid < 8)					//��Դ��
		{
			if(EEPROM_ReadBuff(Addr, eeprom_buffer, 32, dps_info.EEPROM_SlaveAddr, dps_info.DBMsg[Bid-6].Chan))
				shellprintf("eeprom read error!\r\n");
			else
			{
				crc8 = CRC8(eeprom_buffer, len);		//У��
				if(crc8 != eeprom_buffer[31])
					shellprintf("Read crc8:%x, crc8:%x\r\n", crc8, eeprom_buffer[31]);
				else
					shellprintf("ReadData:%s\r\n", eeprom_buffer);
			}
		}
		else if(8 == Bid)								//����
		{
			if(EEPROM_ReadBuff(Addr, eeprom_buffer, 32, bbd_info.EEPROM_SlaveAddr, bbd_info.HWChan))
				shellprintf("eeprom read error!\r\n");
			else
			{
				crc8 = CRC8(eeprom_buffer, len);		//У��
				if(crc8 != eeprom_buffer[31])
					shellprintf("Read crc8:%x, crc8:%x\r\n", crc8, eeprom_buffer[31]);
				else
					shellprintf("ReadData:%s\r\n", eeprom_buffer);
			}
		}
		else if (9 == Bid) {
			if(EEPROM_ReadBuff(Addr, eeprom_buffer, 32, dual_slot_info.EEPROM_SlaveAddr, dual_slot_info.Chan))
				shellprintf("eeprom read error!\r\n");
			else
			{
				crc8 = CRC8(eeprom_buffer, len);		//У��
				if(crc8 != eeprom_buffer[31])
					shellprintf("Read crc8:%x, crc8:%x\r\n", crc8, eeprom_buffer[31]);
				else
					shellprintf("ReadData:%s\r\n", eeprom_buffer);
			}
		}
		else {
			shellprintf("Board%d No exist!\r\n", Bid);
		}
	}

	return SUCCESS;
}

s32 WriteEEPROM(int argc, char * argv[])
{
	u8 eeprom_buffer[32] = {0};
	u16 Bid  = atoi(argv[1]);
	u16 Addr = strtoul(argv[3], NULL, 16);
	u16 len  = (32 > strlen(argv[4])) ? strlen(argv[4]) : 31;
	u8 crc8  = 0;

	if(0 == strcmp("eeprom", argv[2]))
	{
		memset(eeprom_buffer, 0, 32);
		memcpy(eeprom_buffer, argv[4], len);
		crc8 = CRC8(eeprom_buffer, len);
		eeprom_buffer[31] = crc8;

		shellprintf("\r\n");
		shellprintf("Crc8:%x\n", crc8);
		shellprintf("eeprom Erase ....\r\n");
		if(0 == Bid)							//���ذ�
		{
			CLEAR_WP(&(mbd_info.Output_LVL), mbd_info.PCA953_SlaveAddr, mbd_info.HWChan);
			if(EEPROM_EraseBuff(Addr, 32, mbd_info.EEPROM_SlaveAddr, mbd_info.HWChan))
				shellprintf("eeprom Erase error!\r\n");
			else
			{
				shellprintf("eeprom Write ....\r\n");
				if(EEPROM_WriteBuff(Addr, eeprom_buffer, 32, mbd_info.EEPROM_SlaveAddr, mbd_info.HWChan))
					shellprintf("eeprom write error!\r\n");
				else
					shellprintf("eeprom write OK!\r\n");
			}
			ADD_WP(&(mbd_info.Output_LVL), mbd_info.PCA953_SlaveAddr, mbd_info.HWChan);
		}
		else if(Bid >= 1 && Bid < 6)			//������
		{
			CLEAR_WP(&(fpga_obj.Driver[Bid-1].Msg.Output_LVL), fpga_obj.PCA953_SlaveAddr, fpga_obj.Driver[Bid-1].Msg.HWChan);
			if(EEPROM_EraseBuff(Addr, 32, fpga_obj.EEPROM_SlaveAddr, fpga_obj.Driver[Bid-1].Msg.HWChan))
				shellprintf("eeprom Erase error!\r\n");
			else
			{
				shellprintf("eeprom Write ....\r\n");
				if(EEPROM_WriteBuff(Addr, eeprom_buffer, 32, fpga_obj.EEPROM_SlaveAddr, fpga_obj.Driver[Bid-1].Msg.HWChan))
					shellprintf("eeprom write error!\r\n");
				else
					shellprintf("eeprom write OK!\r\n");
			}
			ADD_WP  (&(fpga_obj.Driver[Bid-1].Msg.Output_LVL), fpga_obj.PCA953_SlaveAddr, fpga_obj.Driver[Bid-1].Msg.HWChan);
		}
		else if(Bid >= 6 && Bid < 8)			//��Դ��
		{
			CLEAR_WP(&(dps_info.DBMsg[Bid-6].Output_LVL), dps_info.PCA953_SlaveAddr, dps_info.DBMsg[Bid-6].Chan);

			if(EEPROM_EraseBuff(Addr, 32, dps_info.EEPROM_SlaveAddr, dps_info.DBMsg[Bid-6].Chan))
				shellprintf("eeprom Erase error!\r\n");
			else
			{
				shellprintf("eeprom Write ....\r\n");
				if(EEPROM_WriteBuff(Addr, eeprom_buffer, 32, dps_info.EEPROM_SlaveAddr, dps_info.DBMsg[Bid-6].Chan))
					shellprintf("eeprom write error!\r\n");
				else
					shellprintf("eeprom write OK!\r\n");
			}
			ADD_WP  (&(dps_info.DBMsg[Bid-6].Output_LVL), dps_info.PCA953_SlaveAddr, dps_info.DBMsg[Bid-6].Chan);
		}
		else if(Bid == 8)						//����
		{
			shellprintf("back board\r\n");
			CLEAR_WP(&(bbd_info.Output_LVL), bbd_info.PCA953_SlaveAddr, bbd_info.HWChan);
			if(EEPROM_EraseBuff(Addr, 32, bbd_info.EEPROM_SlaveAddr, bbd_info.HWChan))
				shellprintf("eeprom Erase error!\r\n");
			else
			{
				shellprintf("eeprom Write ....\r\n");
				if(EEPROM_WriteBuff(Addr, eeprom_buffer, 32, bbd_info.EEPROM_SlaveAddr, bbd_info.HWChan))
					shellprintf("eeprom write error!\r\n");
				else
					shellprintf("eeprom write OK!\r\n");
			}
			ADD_WP  (&(bbd_info.Output_LVL), bbd_info.PCA953_SlaveAddr, bbd_info.HWChan);
		}
		else
			shellprintf("Board%d No exist!\r\n", Bid);
	}

	return SUCCESS;
}

s32 WriteReg(int argc, char * argv[])
{
	u16 Bid  = atoi(argv[1]);
	u32 Addr = strtoul(argv[3], NULL, 16);
	u32 Data = strtoul(argv[4], NULL, 16);

	if(0 == strcmp("reg", argv[2]))
	{
		shellprintf("\r\n");
		if(Fpga_WOpt(&(fpga_obj.Driver[Bid-1].Reg), Addr, Data))
			shellprintf("  Write [0x%x] error....\r\n", Addr);
		else
			shellprintf("  Write [0x%x] ok....\r\n", Addr);
	}

	return SUCCESS;
}

s32 ReadReg(int argc, char * argv[])
{
	u16 Bid  = atoi(argv[1]);
	u32 Addr = strtoul(argv[3], NULL, 16);
	u32 Data = strtoul(argv[4], NULL, 16);

	if(0 == strcmp("reg", argv[2]))
	{
		shellprintf("\r\n");
		if(Fpga_ROpt(&(fpga_obj.Driver[Bid-1].Reg), Addr, &Data))
			shellprintf("  Read [0x%x] error....\r\n", Addr);
		else
			shellprintf("  Read [0x%x]:%x\r\n", Addr, Data);
	}
	return SUCCESS;
}

s32 Setready(int argc, char * argv[])
{
	if(argc == 4)
	{
		u16 Bid  = atoi(argv[1]);
		u32 Addr = strtoul(argv[2], NULL, 16);			//ת�޷���
		u32 frm  = atoi(argv[3]);

		DDR_ROpt_Arg(&(fpga_obj.Driver[Bid-1].Reg), Addr, frm);
		shellprintf("  Read [%x]  %d fram ...\r\n", Addr, frm);
	}

	return SUCCESS;
}

s32 Trigger(int argc, char * argv[])
{
	if(argc == 1)
	{
		Set_pin_value(SYS_TRIG, 1);
		usleep(50000);
		Set_pin_value(SYS_TRIG, 0);
		shellprintf("  trigger ok ...\r\n");
	}
	else if(argc == 2)
	{
		u16 flag = atoi(argv[1]);
		Set_pin_value(SYS_TRIG, flag ? 1 : 0);
		shellprintf("  trigger %d ok ...\r\n", flag);
	}

	return SUCCESS;
}


s32 WriteDDR(int argc, char * argv[])
{
	u32 Addr = strtoul(argv[3], NULL, 16);
	u32 Data = strtoul(argv[4], NULL, 16);

	if(0 == strcmp("ddr", argv[2]))
	{
		if(Addr > 0x40000000)
			return FAILURE;
		shellprintf("\r\n");
		RegValue(Addr) = Data;
		shellprintf("write DDR ok!\r\n");
	}
	return SUCCESS;
}

s32 WritePL(int argc, char * argv[])
{
	u32 Addr = strtoul(argv[2], NULL, 16);
	u32 Data = strtoul(argv[3], NULL, 16);

	if(0 == strcmp("pl", argv[1]))
	{
		if(Addr > 0x43c0ffff || Addr < 0x43c00000)
			return FAILURE;
		shellprintf("\r\n");
		RegValue(Addr) = Data;
		shellprintf("write pl ok!\r\n");
	}
	return SUCCESS;
}

s32 ReadDDR(int argc, char * argv[])
{
	u32 Addr = strtoul(argv[3], NULL, 16);

	if(0 == strcmp("ddr", argv[2]))
	{
		if(Addr > 0x40000000)
			return FAILURE;
		shellprintf("\r\n");
		shellprintf("Read:%x\r\n", RegValue(Addr));
	}

	return SUCCESS;
}

s32 ReadPL(int argc, char * argv[])
{
	u32 Addr = strtoul(argv[2], NULL, 16);

	if(0 == strcmp("pl", argv[1]))
	{
		if(Addr > 0x43c0ffff || Addr < 0x43c00000)
			return FAILURE;
		shellprintf("\r\n");
		shellprintf("Read:%x\r\n", RegValue(Addr));
	}

	return SUCCESS;
}

s32 ReadSystem(int argc, char * argv[])
{
	u16 Bid  = atoi(argv[1]);

	if(0 == strcmp("system", argv[2]))
	{
		if((0 == Bid))
		{
			ReadMBinfo();
			shellprintf("\r\n");
			shellprintf("       PN:%s\r\n", mbd_info.PN);
			shellprintf("       SN:%s\r\n", mbd_info.SN);
			shellprintf("  PCBType:%x\r\n", mbd_info.PCBType);
			shellprintf("    HWVer:%x\r\n", mbd_info.HWVersion);
			Debug_float("ChipTemp",         mbd_info.ChipTemp);
			shellprintf("  ChipDAN:%x%x\r\n", mbd_info.ChipDAN.DANH, mbd_info.ChipDAN.DANL);
			shellprintf("Firmware0:%s  Compile time:%s\r\n", mbd_info.Version0, mbd_info.CompTime0);
			shellprintf("Firmware1:%s  Compile time:%s\r\n", mbd_info.Version1, mbd_info.CompTime1);
		}

		else if((Bid >= 1) && (Bid < 6))  //������1-5
		{
			ReadDBinfo(Bid-1);
			shellprintf("\r\n");
			shellprintf("       PN:%s\r\n",  fpga_obj.Driver[Bid-1].Msg.PN);
			shellprintf("       SN:%s\r\n",  fpga_obj.Driver[Bid-1].Msg.SN);
			shellprintf("  PCBType:%x\r\n",  fpga_obj.Driver[Bid-1].Msg.PCBType);
			shellprintf("    HWVer:%x\r\n",  fpga_obj.Driver[Bid-1].Msg.HWVersion);
			Debug_float("ChipTemp",          fpga_obj.Driver[Bid-1].Msg.ChipTemp);
			shellprintf("  ChipDAN:%x%x\r\n",  fpga_obj.Driver[Bid-1].Msg.ChipDAN.DANH, fpga_obj.Driver[Bid-1].Msg.ChipDAN.DANL);
			shellprintf(" Firmware:%s  Compile time:%s\r\n", fpga_obj.Driver[Bid-1].Msg.Version, fpga_obj.Driver[Bid-1].Msg.CompTime);
		}

		else if((Bid >= 6) && (Bid < 8))
		{
			ReadDpsinfo();
			shellprintf("\r\n");
			shellprintf("       PN:%s\r\n",  dps_info.DBMsg[Bid-6].PN);
			shellprintf("       SN:%s\r\n",  dps_info.DBMsg[Bid-6].SN);
			shellprintf("  PCBType:%x\r\n",  dps_info.DBMsg[Bid-6].PCBType);
			shellprintf("    HWVer:%x\r\n",  dps_info.DBMsg[Bid-6].HWVersion);
			Debug_float("ChipTemp",          dps_info.DBMsg[Bid-6].ChipTemp);
			shellprintf("  ChipDAN:%x%x\r\n",  dps_info.DBMsg[Bid-6].ChipDAN.DANH, dps_info.DBMsg[Bid-6].ChipDAN.DANL);
			shellprintf(" Firmware:%s  Compile time:%s\r\n", dps_info.DBMsg[Bid-6].Version, dps_info.DBMsg[Bid-6].CompTime);
		}
		else if(8 == Bid)
		{
			ReadBKinfo();
			shellprintf("\r\n");
			shellprintf("       PN:%s\r\n",  bbd_info.PN);
			shellprintf("       SN:%s\r\n",  bbd_info.SN);
			shellprintf("  PCBType:%x\r\n",  bbd_info.PCBType);
			shellprintf("    HWVer:%x\r\n",  bbd_info.HWVersion);
		}
		else if (9 == Bid) {
			if (READ_SUCCESS != ReadDualSlotInfo()) {
				shellprintf("read failed\r\n");
			}
			else {
				shellprintf("\r\n");
				shellprintf("       PN:%s\r\n",  dual_slot_info.PN);
				shellprintf("       SN:%s\r\n",  dual_slot_info.SN);
				shellprintf("  PCBType:%x\r\n",  dual_slot_info.PCBType);
				shellprintf("    HWVer:%x\r\n",  dual_slot_info.HWVersion);
			}
		}
		else {
			shellprintf("Board%d No exist!\r\n", Bid);
		}
	}

	return SUCCESS;
}

int WriteIO(int argc, char * argv[])
{
	u16 Bid  = atoi(argv[1]);
	u32 pin  = atoi(argv[3]);
	u32 Data = atoi(argv[4]);
	if(0 == strcmp("io", argv[2]))
	{
		if(Bid == 0)
		{
			if(IO_Level_Set(&(mbd_info.Output_LVL), Group1, pin, Data, mbd_info.PCA953_SlaveAddr, mbd_info.HWChan))
				shellprintf("  Set io error!\r\n");
			else
				shellprintf("  Set io ok!\r\n");
		}
		else if((Bid >= 1) && (Bid < 6))
		{
			if(IO_Level_Set(&(fpga_obj.Driver[Bid-1].Msg.Output_LVL), Group1, pin, Data, fpga_obj.PCA953_SlaveAddr, fpga_obj.Driver[Bid-1].Msg.HWChan))
				shellprintf("  Set io error!\r\n");
			else
				shellprintf("  Set io ok!\r\n");
		}
		else if((Bid >= 6) && (Bid < 8))
		{
			if(IO_Level_Set(&(dps_info.DBMsg[Bid-6].Output_LVL), Group1, pin, Data, dps_info.PCA953_SlaveAddr, dps_info.DBMsg[Bid-6].Chan))
				shellprintf("  Set io error!\r\n");
			else
				shellprintf("  Set io ok!\r\n");
		}
		else if(8 == Bid)
		{
			if(ALL_IO_Level_Set(&(bbd_info.Ctl_Output), pin, Data, bbd_info.PCA953_SlaveAddr, bbd_info.HWChan))
				shellprintf("  Set io error!\r\n");
			else
				shellprintf("  Set io ok!\r\n");
		}
	}
	return SUCCESS;
}

int ReadDebug(int argc, char * argv[])
{
	ReadEEPROM(argc, argv);
    ReadReg(argc, argv);
    ReadDDR(argc, argv);
    ReadPL(argc, argv);
    ReadSystem(argc, argv);

	return 0;
}

int WriteDebug(int argc, char * argv[])
{
	WriteEEPROM(argc, argv);
	WriteReg(argc, argv);
	WriteDDR(argc, argv);
	WritePL(argc, argv);
	WriteIO(argc, argv);

	return 0;
}

int DebugEcho(int argc, char * argv[])
{
	int i = 1;
    for (i = 1; i < argc; i++)
    	shellprintf("%s\r\n", argv[i]);

    return SUCCESS;
}

const u32 patten[8] = {
	0x11111111, 0x11111111, 0x11111111, 0x11111111,
	0x11111111, 0x11111111, 0x11111111, 0x11111111,
};

//const u32 patten[8] = {
//	0, 0, 0, 0,
//	0, 0, 0, 0,
//};

int Debug_SetDDR(int argc, char * argv[])
{
	u32 con = 0x0, j = 0;
	u32 Addr  = strtoul(argv[1], NULL, 16);
	u32 flag  = strtoul(argv[3], NULL, 16);
	u32 Frm   =  atoi(argv[2]);
	if(4 == argc)
	{
		for(con = 0; con < Frm; con++)
		{
			for(j = 0; j < 8; j++)
			{
				RegValue(Addr) = patten[j];
				Addr += 4;
			}
		}

		RegValue(Addr) = flag;

		shellprintf("    Write addr %x ok   End:%x\r\n", strtoul(argv[1], NULL, 16), flag);
	}
	return 0;
}

int Debug_FillDDR(int argc, char * argv[])
{
	u32 con = 0x0;
	u32 Addr  = strtoul(argv[1], NULL, 16);
	u32 flag  = strtoul(argv[2], NULL, 16);
	u32 len   =  atoi(argv[3]);

	for(con = 0; con < len; con++)
	{
		RegValue(Addr) = flag;
		Addr += 4;
	}

	shellprintf("    Fill DDR  Start:%x Len:%x OK ...\r\n", Addr, len);

	return 0;
}

int Debug_Switch(int argc, char * argv[])
{
	u8 on_off =  0;
	u32 io = 0;
	if(0 == strcmp("12v", argv[1]) && 3 == argc)
	{
		if(0 == strcmp("on", argv[2]))
			on_off = 1;
		else if(0 == strcmp("off", argv[2]))
			on_off = 0;
		if(SUCCESS == Switch_12V((SW)on_off))
			shellprintln("    %s 12v", on_off ? "Open" : "Close");
		else
			shellprintln("    switch error ...");
	}
	else if(0 == strcmp("io", argv[1]) && 4 == argc)
	{
			io = atoi(argv[2]);
		on_off = atoi(argv[3]);
		if(SUCCESS == Set_IO_Level((SW)on_off, io))
			shellprintln("    IO(%d) Set %c", io, on_off ? 'H' : 'L');
		else
			shellprintln("    switch error ...");
	}

	return SUCCESS;
}

int Debug_Max528(int argc, char * argv[])
{
	u16 Bid   = atoi(argv[1]);
	u16 dacid = atoi(argv[2]);
	u16 chan  = atoi(argv[3]);
	float vo  = atof(argv[4]);

	if(5 == argc)
	{
		Max528_Set_DAC(&(fpga_obj.Driver[Bid-1].Reg), (short)(vo*1000), chan, dacid);
		shellprintln(" Max528(%d) Chan:%d Set:%.2f", dacid, chan, vo);
	}

	return SUCCESS;
}

int Debug_DriverLevel(int argc, char * argv[])
{
	u16 Bid   = atoi(argv[1]);
	float vo1 = atof(argv[3]);
	float vo2 = atof(argv[4]);

	if(5 == argc)
	{
		if(0 == strcmp("dhv", argv[2]))
		{
			Set_DHV32(&(fpga_obj.Driver[Bid-1].Reg),  (short)(vo1*1000), (short)(vo2*1000));
			shellprintln(" Set dhv1:%.2f dhv2:%.2f", vo1, vo2);
		}
		else if(0 == strcmp("dlv", argv[2]))
		{
			Set_DLV32(&(fpga_obj.Driver[Bid-1].Reg),  (short)(vo1*1000), (short)(vo2*1000));
			shellprintln(" Set dlv1:%.2f dlv2:%.2f", vo1, vo2);
		}
		else if(0 == strcmp("dtv", argv[2]))
		{
			Set_DTV32(&(fpga_obj.Driver[Bid-1].Reg),  (short)(vo1*1000), (short)(vo2*1000));
			shellprintln(" Set dtv1:%.2f dtv2:%.2f", vo1, vo2);
		}
		else if(0 == strcmp("cphv", argv[2]))
		{
			Set_CPHV32(&(fpga_obj.Driver[Bid-1].Reg), (short)(vo1*1000), (short)(vo2*1000));
			shellprintln(" Set cphv1:%.2f cphv2:%.2f", vo1, vo2);
		}
		else if(0 == strcmp("cplv", argv[2]))
		{
			Set_CPLV32(&(fpga_obj.Driver[Bid-1].Reg), (short)(vo1*1000), (short)(vo2*1000));
			shellprintln(" Set cplv1:%.2f cplv2:%.2f", vo1, vo2);
		}
	}

	return SUCCESS;
}

//����SMBus
int Debug_SMbus(int argc, char * argv[])
{
	u16 SlaveAddr, id;
	u8 Code;
	u16 RSize;
	u8 i = 0;
	u8 Buf[10] = {0};
	if(5 == argc)
	{
		if(0 == strcmp("r", argv[1]))
		{
			SlaveAddr = strtoul(argv[2], NULL, 16);
			Code = strtoul(argv[3], NULL, 16);
			RSize = atoi(argv[4]);

			if(SMBus_Read (Buf, RSize, SlaveAddr, Code, IIC06))
			{
				shellprintln(" Read error ...");
				return 1;
			}
			for(i = 0; i < RSize; i++)
				shellprintf(" Rdata:%x", Buf[i]);
			shellprintf("\r\n");
		}
	}
	else if(6 == argc)
	{
		if(0 == strcmp("w", argv[1]))
		{
			int i = 0;
			SlaveAddr = strtoul(argv[2], NULL, 16);
			Code = strtoul(argv[3], NULL, 16);
			RSize = atoi(argv[4]);

			RSize  = RSize > 4 ? 4 :RSize;

			u32 Data = strtoul(argv[5], NULL, 16);	//����

			Data <<= ((4-RSize)*8);					//���ݷŵ���λ
			shellprintln(" Shift data:%x", Data);
			for(i = 0; i < RSize; i++)
			{
				Buf[i] = (((Data << (i*8))&0xff000000) >> 24) & 0xff;
				shellprintf(" Wdata:%x", Buf[i]);
			}
			shellprintf("\r\n");
			if(SMBus_Write (Buf, RSize, SlaveAddr, Code, IIC06))
			{
				shellprintln(" write error ...");
				return 1;
			}
			shellprintln(" write ok ...");

		}
	}
	else if(3 == argc)
	{
		if(0 == strcmp("info", argv[1]))
		{
			u32 data = 0x0;
			float temp = 0.0;
			id = atoi(argv[2]);
			id = (id > 4) ? 3 : id;

			SMBus_Read(Buf, 9, dps_info.DBMsg[0].Dpm[6+id].DpmAddr, DPM_IC_DEVICE_ID, dps_info.DBMsg[0].Chan);
			Buf[10] = '\0';
			shellprintln(" ID:%s", Buf+1);

			memset(Buf, 0, 10);

			SMBus_Read(Buf, 4, dps_info.DBMsg[0].Dpm[6+id].DpmAddr, DPM_IC_DEVICE_REV, dps_info.DBMsg[0].Chan);
			data |= Buf[1] << 16;
			data |= Buf[2] << 8;
			data |= Buf[3] << 0;
			shellprintln("   IC DEVICE REF: %x", data);
			if(0x3 == Buf[0])
			{
				shellprintln(" Silicon Version:%s", ((Buf[2] >> 3)&0x1) ? "12V" : "60V");
				temp =  Dpm_TEMP(&(dps_info.DBMsg[0].Dpm[6+id]), dps_info.DBMsg[0].Chan);
				shellprintln("   Temp:%.2f ��", temp);
			}
		}
		else if(0 == strcmp("version", argv[1]))
		{
			u32 data = 0x0;
			id = atoi(argv[2]);
			id = (id > 4) ? 3 : id;

			SMBus_Read(Buf, 4, dps_info.DBMsg[0].Dpm[6+id].DpmAddr, DPM_IC_DEVICE_REV, dps_info.DBMsg[0].Chan);
			data |= Buf[1] << 16;
			data |= Buf[2] << 8;
			data |= Buf[3] << 0;
			shellprintln("   IC DEVICE REF: %x", data);
			if(0x3 == Buf[0])
				shellprintln(" Silicon Version:%s", ((Buf[2] >> 3)&0x1) ? "12V" : "60V");
		}
		else if(0 == strcmp("init", argv[1]))
		{
			id = atoi(argv[2]);
			id = (id > 4) ? 3 : id;
			Buf[0] = 0xF;
			if(SMBus_Write(Buf, 1, dps_info.DBMsg[0].Dpm[6+id].DpmAddr, DPM_SET_DPM_MODE, dps_info.DBMsg[0].Chan))
			{
				shellprintln("    init %x error ...", id);
			}
			else
				shellprintln("    init %x ok ...", id);
		}

	}

	return SUCCESS;
}

int Debug_DAC(int argc, char * argv[])
{
	u16 ndac = atoi(argv[2]);
	float Vo = 0.0;

	if((4 == argc) && (0 == strcmp("w", argv[1])))
	{
		Vo = atof(argv[3]);
		ndac = ndac > 7 ? 7 : ndac;
		if(SUCCESS == Set_AD5694(Vo, ndac, dps_info.AD5694_SlaveAddr, dps_info.DBMsg[ndac/4].Chan));
			shellprintln("    Set Dps%d Vout:%.2f ok ...",ndac/4, Vo);
	}
	else if((3 == argc) && (0 == strcmp("r", argv[1])))
	{
		ndac = ndac > 7 ? 7 : ndac;
//		Vo = Get_AD5694(ndac, dps_info.AD5694_SlaveAddr, dps_info.DBMsg[ndac/4].Chan);
		shellprintln("    Dps%d Vout:%.2f",ndac/4, Vo);
	}

	return SUCCESS;
}

int Debug_Max9972(int argc, char * argv[])
{
	u16 bid,cs;
	u32 Rdata;
	Max9972 tx;

	if((5 == argc) && (0 == strcmp("w", argv[2])))
	{
		bid = atoi(argv[1]);
		cs  = atoi(argv[3]);
		tx.Data = (u16)strtoul(argv[4], NULL, 16);
		if(SUCCESS == Max9972_WOpt(&(fpga_obj.Driver[bid-1].Reg), &tx, cs))
			shellprintln("    write ok...");
	}
	else if((4 == argc) && (0 == strcmp("r", argv[2])))
	{
		bid = atoi(argv[1]);
		cs  = atoi(argv[3]);
		if(SUCCESS == Max9972_ROpt(&(fpga_obj.Driver[bid-1].Reg), &Rdata, cs))
			shellprintln("    Read data:%x", Rdata);
	}

	return SUCCESS;
}

int SetIp(int argc, char * argv[])
{
	s32 ip = -1, status = FAILURE;
	u16 setip;
	if(2 == argc && 0 == strcmp("get", argv[1]))
	{
		ip = Read_IPAddr(mbd_info.EEPROM_SlaveAddr, mbd_info.HWChan);
		shellprintln("    ipaddr:192.168.1.%d", ip);
	}
	else if(3 == argc && 0 == strcmp("set", argv[1]))
	{
		setip = atoi(argv[2]);
		CLEAR_WP(&(mbd_info.Output_LVL), mbd_info.PCA953_SlaveAddr, mbd_info.HWChan);
		status = Set_IPAddr((u8)setip, mbd_info.EEPROM_SlaveAddr, mbd_info.HWChan);
		ADD_WP(&(mbd_info.Output_LVL), mbd_info.PCA953_SlaveAddr, mbd_info.HWChan);
		if(status)
			shellprintln("    set ipaddr error ...");
		else
		{
			ip = Read_IPAddr(mbd_info.EEPROM_SlaveAddr, mbd_info.HWChan);
			shellprintln("    ipaddr:192.168.1.%d", ip);
		}
	}
	return SUCCESS;
}

int Debug_brebootfpga(int argc, char * argv[])
{
	u16 bid = atoi(argv[1]);
	if(2 == argc)
	{
		StopFpga (&(fpga_obj.Driver[bid-1].Reg));
		ResetFpga(&(fpga_obj.Driver[bid-1].Reg), 0);
		usleep(10000);
		ResetFpga(&(fpga_obj.Driver[bid-1].Reg), 1);
	}

	return SUCCESS;
}

void ListData_WOpt(u32 Addr, List *list)
{
	RegValue(Addr+0) = list->Saddr;
	RegValue(Addr+4) = list->Slength;
	RegValue(Addr+8) = list->Repeat;
}

int Debug_List(int argc, char * argv[])
{
	if(6 == argc)
	{
		u32 i = 0;
		u32 Size = 0;
		List data = {
			.Saddr	 = strtoul(argv[3], NULL, 16),
			.Slength = strtoul(argv[4], NULL, 16),
			.Repeat  = strtoul(argv[5], NULL, 16),
		};
		if(0 == strcmp("set", argv[1]))
		{
			for(i = 0; i < atoi(argv[2]); i++)
			{
				if(i == ((0x10000000/sizeof(List))-1))
					break;
				ListData_WOpt(LIST_ADDR + (i * sizeof(List)), &data);
			}
			Size = i*12;
			Set_Cycle_Size(Size);
			shellprintln("  List num:%d", i);
		}
		shellprintln("  (%d)List set ok ...", atoi(argv[2]));
	}
	else if(3 == argc)
	{
		List * ptr = (List *)LIST_ADDR;
		u32 i = 0;

		if(0 == strcmp("w", argv[1]))
		{
			Reg_Opt(fpga_obj.ListReg.IRQ_Mask) = 1;		//�����ж��ź�
			fpga_obj.list = 0;

			u32 num = atoi(argv[2]);

			num = num <= (FIFO_LEN - Reg_Opt(fpga_obj.ListReg.Fifo_Depth)) ? num : (FIFO_LEN - Reg_Opt(fpga_obj.ListReg.Fifo_Depth));

			for(i = 0; i < num; i++)
				Write_List(&(fpga_obj.ListReg), ptr+i);

			shellprintln("  (%d)List write ok ...", atoi(argv[2]));
		}
	}
	else if(2 == argc)
	{
		if(0 == strcmp("start", argv[1]))
		{
			Reg_Opt(fpga_obj.ListReg.IRQ_Mask) = 1;		//�����ж��ź�
			StopList_WOpt();
			ResetListFifo();
			StartList_WOpt();
			fpga_obj.list  = (List *)LIST_ADDR,
			shellprintln("  List Start ...");
			Reg_Opt(fpga_obj.ListReg.IRQ_Mask) = 0;		//�����ж��ź�
		}
		else if(0 == strcmp("stop", argv[1]))
		{
			Reg_Opt(fpga_obj.ListReg.IRQ_Mask) = 1;		//�����ж��ź�
			StopList_WOpt();
			ResetListFifo();
			fpga_obj.list  = (List *)LIST_ADDR,
			shellprintln("  List Stop ...");
			Reg_Opt(fpga_obj.ListReg.IRQ_Mask) = 0;		//�����ж��ź�
		}
	}

	return SUCCESS;
}

int Debug_fpga(int argc, char * argv[])
{
	if(2 == argc)
	{
		if(0 == strcmp("start", argv[1]))
		{
			StartFpgaMask(0x1f);
			shellprintln("  fpga start ...");
		}
		else if(0 == strcmp("stop", argv[1]))
		{
			StopFpgaMask(0x1f);
			shellprintln("  fpga stop ...");
		}
		else if(0 == strcmp("pause", argv[1]))
		{
			PauseFpgaMask(0x1f);
			shellprintln("  fpga pause ...");
		}
		else if(0 == strcmp("reset", argv[1]))
		{
			RebootFpgaMask(0x1f);
			shellprintln("  fpga reset ...");
		}
	}
	return SUCCESS;
}


int Debug_Dpm(int argc, char * argv[])
{
	if(3 == argc)
	{

		u16 nadc = atoi(argv[2]);
		nadc = nadc > 9 ? 9 : nadc;
		if(0 == strcmp("adc", argv[1]))
		{
			float VBus   = Dpm_VBus(&dps_info.DBMsg[0].Dpm[nadc], dps_info.DBMsg[0].Chan);
			float VShunt = Dpm_VShunt(&dps_info.DBMsg[0].Dpm[nadc], dps_info.DBMsg[0].Chan);
			shellprintln("    VBus%d: %.2f V", nadc,   VBus);
			shellprintln("  VShunt%d: %.2f V", nadc, VShunt);
		}
	}
	return 0;
}

int reg_test(int argc, char * argv[])
{
	if(argc == 4)
	{
		u32 rdata = 0, i = 0;
		u16 bid = atoi(argv[1]);
		u32 reg = strtoul(argv[2], NULL, 16);
		u32 time = strtoul(argv[3], NULL, 10);
		for(i = 1; i <= time; i++)
		{
			if(Fpga_ROpt(&(fpga_obj.Driver[bid-1].Reg), reg, &rdata))
			{
				shellprintln("  test[0x%x] error(time:%d) ...", reg, i);
				return 1;
			}
		}
		shellprintln("  test[0x%x] success ...", reg);
	}
	return 0;
}



int printdps(int argc, char * argv[])
{
	u16 i = 0, j = 0, id = 0x0;
	Dps_Arg * Dps_ptr = (Dps_Arg * )DPS_CFG_DATA_ADDR;
	for(i = 0; i < 2; i++)	//�����ϵ�
	{
		for(j = 0; j < (DPS_NUM/2); j++, id++)
		{
			shellprintf("\r\n");
			shellprintf("==================Dps(%02d)================\r\n", id+1);
			shellprintf("      InputMaxV: %d\r\n", Dps_ptr->Dsp[id].InputMaxV);
			shellprintf("      InputMinV: %d\r\n", Dps_ptr->Dsp[id].InputMinV);
			shellprintf("        DpsMaxV: %d\r\n", Dps_ptr->Dsp[id].DpsMaxV);
			shellprintf("        DpsMinV: %d\r\n", Dps_ptr->Dsp[id].DpsMinV);
			shellprintf("      Protect_I: %d\r\n", Dps_ptr->Dsp[id].Protect_I);
			shellprintf("       SetValue: %d\r\n", Dps_ptr->Dsp[id].SetValue);
			shellprintf("-----------------------------------------\r\n");
			shellprintf("      Open.Mode: %d\r\n", Dps_ptr->Dsp[id].Open.Mode);
			shellprintf("     Open.STime: %d\r\n", Dps_ptr->Dsp[id].Open.STime);
			shellprintf("      Open.Keep: %d\r\n", Dps_ptr->Dsp[id].Open.Keep);
			shellprintf("      Open.Step: %d\r\n", Dps_ptr->Dsp[id].Open.Step);
			shellprintf("-----------------------------------------\r\n");
			shellprintf("     Close.Mode: %d\r\n", Dps_ptr->Dsp[id].Close.Mode);
			shellprintf("    Close.STime: %d\r\n", Dps_ptr->Dsp[id].Close.STime);
			shellprintf("     Close.Keep: %d\r\n", Dps_ptr->Dsp[id].Close.Keep);
			shellprintf("     Close.Step: %d\r\n", Dps_ptr->Dsp[id].Close.Step);
			shellprintf("=========================================\r\n");
			shellprintf("\r\n");
		}
	}
	return 0;
}

int reboot(int argc, char * argv[])
{
	Xil_Out32(0xF8000008, 0xDF0D); //дʹ��
	Xil_Out32(0xF8000200, 0x01); //��λ
	return 0;
}

int Insert_Status(int argc, char * argv[])
{
	//u8 reg = strtoul(argv[1], NULL, 16);

	Sample_Data   * Sample    = (Sample_Data   *)SAMPLE_DATA_ADD;		//��������

	ReadDualSlotStatus(Sample);
	u8 i = 0;
	shellprintf("Insert status :\r\n");
	for (i = 0; i <4 ;i++) {
		shellprintf("%d", (Sample->BoardStatus >> (3 - i)) & 0x01);
	}
	shellprintf("\r\n");

	return 0;
}

int Dps_Fpga(int argc, char * argv[])
{
	u8 bid = strtoul(argv[2], NULL, 16);
	u8 reg = strtoul(argv[3], NULL, 16);
	u16 t_data = strtoul(argv[4], NULL, 16);
	u16 r_data = 0x00;

	if (0 == strcmp("read", argv[1])) {
		if ((0 == bid) || (1 == bid)) {
			Read_Dps_Fpga_Reg(reg , &r_data, bid);
			shellprintf("read data:%x%x\r\n", reg, r_data);
		}
		else {
			shellprintf("Board not exist\r\n");
		}
	}
	else if (0 == strcmp("write", argv[1])) {
		if ((0 == bid) || (1 == bid)) {
			Write_Dps_Fpga_Reg(reg , t_data, bid);
		}
		else {
			shellprintf("Board not exist\r\n");
		}
	}
	else {

	}

	return 0;
}

int ADC_Set_Voltage(int argc, char * argv[])
{
	u8 bid = atoi(argv[1]);
	u8 nadc = atoi(argv[2]);

	float volage = atof(argv[3]);

	if (SET_SUCCESS != Set_AD5694(volage, nadc, \
			dps_info.AD5694_SlaveAddr, dps_info.DBMsg[bid].Chan)) {
		shellprintf("IIC Communication Failed\r\n");
	}


	return SUCCESS;
}


int Isl28023_Set_Voltage(int argc, char * argv[])
{

	u8 bid = atoi(argv[1]);
	u8 nadc = atoi(argv[2]) - 1;

	float volage = atof(argv[3]);
	shellprintf("Channel:%d\r\n", nadc);

	if ((nadc > 0) && (nadc < 9)) {
		if (SUCCESS != Set_dac_out(volage,
				dps_info.DBMsg[bid].Dpm[nadc].DpmAddr, dps_info.DBMsg[bid].Chan)) {
			shellprintf("IIC Communication Failed\r\n");
		}
	}
		
	return SUCCESS;

}

int Isl8272_Read_Reg(int argc, char * argv[])
{
	u8 bid = atoi(argv[1]);
	u8 reg = strtoul(argv[3], NULL, 16);
	u8 ch = strtoul(argv[2], NULL, 16);
	u8 length = atoi(argv[4]);

	u8 r_data[2] = {0x00};

	if (READ_SUCCESS != Read_Isl8272_Reg(reg, r_data, length, dps_info.DBMsg[bid].DpsAddr[ch],\
			dps_info.DBMsg[bid].Chan)) {
		shellprintf("Read Failed\r\n");
	}
	else {
		shellprintf("Read Data:%x%x\r\n", r_data[0], r_data[1]);
	}

	return SUCCESS;
}


int Dps_Disbale(int argc, char * argv[])
{
	/* Sample data */
	Sample_Data   * Sample    = (Sample_Data   *)SAMPLE_DATA_ADD;		
    /* Dps parameter */
	Dps_Arg       * dps_cfg   = (Dps_Arg       *)DPS_CFG_DATA_ADDR;	
	u32 flag = 0x00;
	if (SET_SUCCESS != Dps_Disable(dps_cfg , Sample, DPS_BD0, &flag)) {
		shellprintf("Disable Failed\r\n");
	}
	else {
		shellprintf("Disable Success\r\n");
	}

	if (flag) {
		Sample->AgeStatus = AGING_STOP;
	}

	return SUCCESS;	
}

int Get_Dps_Current(int argc, char * argv[])
{
	u8 bid = atoi(argv[1]);
	u8 ch = strtoul(argv[2], NULL, 16);

	float current = 0.0f;
	
	current = Dpm_VShunt(&dps_info.DBMsg[bid].Dpm[ch], dps_info.DBMsg[bid].Chan);
	/* Calculate the current value */
	switch (ch) {
		case 0: {
			current = current  / 0.001;
		}
			break;
		case 1: {
			current = current / 0.002;
		}
			break;
		case 2 ... 3: {
			current = current / 0.004;
		}
			break;		
		case 4 ... 7 : {
			current = current / 0.002;
		}
			break;		
		default:
			break;
	}
	shellprintf("Board %d channel %d current %f\r\n", bid, ch, current);

	return SUCCESS;
}


int Get_Dps_Voltage(int argc, char * argv[])
{
	u8 bid = atoi(argv[1]);
	u8 ch = strtoul(argv[2], NULL, 16);	

	float voltage;

	voltage = Dpm_VBus(&dps_info.DBMsg[bid].Dpm[ch], dps_info.DBMsg[bid].Chan);
	shellprintf("Board %d channel %d voltage %f\r\n", bid, ch, voltage);

	return SUCCESS;
}


int IIC_Address_Test(int argc, char * argv[])
{
	u8 bid = atoi(argv[1]);
	u8 addr = strtoul(argv[2], NULL, 16);	
	u8 Status;

	u8 buf[2] = {0x00};

	Status = SMBus_Read(buf, 2, addr, DPM_READ_VOUT, dps_info.DBMsg[bid].Chan);
	if (Status == FAILURE) {
		shellprintf("Test Failed\r\n");
	}
	else {
		shellprintf("Test Success\r\n");
	}

	return SUCCESS;
}


int DPSA_Enable_Disable(int argc, char * argv[])
{
	if (0 == strcmp("enable", argv[1])) {
		dpsa_disable = 0x00;
		if (SET_SUCCESS != Set_All_Dps_L1_Enable(0x3F, DPS_BD0)) {
			shellprintf("disable failure\r\n");
			return INIT_FAILED;
		}			
	}
	else if (0 == strcmp("disable", argv[1])) {
		dpsa_disable = 0x01;
		if (SET_SUCCESS != Set_All_Dps_L1_Enable(0x3E, DPS_BD0)) {
			shellprintf("disable failure\r\n");
			return INIT_FAILED;
		}	
	}
	else {
		shellprintf("Command Error:dpsa enable/disable\r\n");		
	}

	return SUCCESS;
}
