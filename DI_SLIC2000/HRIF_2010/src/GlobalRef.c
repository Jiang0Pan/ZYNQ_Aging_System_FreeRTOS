





#include "GlobalRef.h"
#include "Hw_Ver.h"
#include "IIC.h"



//ϵͳʱ��
Time_Sync_t SysTime = {
	      // ��     ��     ʱ     ��     ��      ��      ����       ����
	.Data = {0, 0, 0, 1, 1, 20, 21, 0x5A}
};

//fpga���ƶ���
Driver_fpga	fpga_obj = {
	.EEPROM_SlaveAddr = 0x50,
	.PCA953_SlaveAddr = 0x20,
	.list      = (List *)LIST_ADDR,
	.Driver = {
		{//U0
			.BoardType = 0x1,
			.VecSize   = 0x0,
			.Msg = {
				.HWChan           = IIC02,
				.HWVersion        = 0,
				.PCBType          = 0,
				.PN               = {0},
				.SN               = {0},
				.Version          = {0},
				.CompTime         = {0},
				.ChipTemp  		  = 0.0,
				.ChipDAN          = {0},
				.Output_LVL = {
					.Reg_Addr    = PORT0WRITE,
					.Group0_Data = 0x0,
					.Group1_Data = 0xff,
				},
			},
			.Reg = {
				.Debug 	  = (volatile u32 *)(AXI_BASE_ADDR + 0x1000),
				.Opt_Addr = (volatile u32 *)(AXI_BASE_ADDR + 0x1004),
				.W_Data   = (volatile u32 *)(AXI_BASE_ADDR + 0x1008),
				.R_Data   = (volatile u32 *)(AXI_BASE_ADDR + 0x100C),
				.Data_Des = (volatile u32 *)(AXI_BASE_ADDR + 0x1010),
				.Opt_RW   = (volatile u32 *)(AXI_BASE_ADDR + 0x1014),
				.Trigger  = (volatile u32 *)(AXI_BASE_ADDR + 0x1018),
				.R_Update = (volatile u32 *)(AXI_BASE_ADDR + 0x101C),
				.WOpt_OK  = (volatile u32 *)(AXI_BASE_ADDR + 0x1020),
			},
		},
		{//U1
			.BoardType = 0x2,
			.VecSize   = 0x0,
			.Msg = {
				.HWChan           = IIC03,
				.HWVersion        = 0,
				.PCBType          = 0,
				.PN               = {0},
				.SN               = {0},
				.Version          = {0},
				.CompTime         = {0},
				.ChipTemp  		  = 0.0,
				.ChipDAN          = {0},
				.Output_LVL = {
					.Reg_Addr    = PORT0WRITE,
					.Group0_Data = 0x0,
					.Group1_Data = 0xff,
				},
			},
			.Reg = {
				.Debug 	  = (volatile u32 *)(AXI_BASE_ADDR + 0x2000),
				.Opt_Addr = (volatile u32 *)(AXI_BASE_ADDR + 0x2004),
				.W_Data   = (volatile u32 *)(AXI_BASE_ADDR + 0x2008),
				.R_Data   = (volatile u32 *)(AXI_BASE_ADDR + 0x200C),
				.Data_Des = (volatile u32 *)(AXI_BASE_ADDR + 0x2010),
				.Opt_RW   = (volatile u32 *)(AXI_BASE_ADDR + 0x2014),
				.Trigger  = (volatile u32 *)(AXI_BASE_ADDR + 0x2018),
				.R_Update = (volatile u32 *)(AXI_BASE_ADDR + 0x201C),
				.WOpt_OK  = (volatile u32 *)(AXI_BASE_ADDR + 0x2020),
			},
		},
		{//U2
			.BoardType = 0x3,
			.VecSize   = 0x0,
			.Msg = {
				.HWChan           = IIC04,
				.HWVersion        = 0,
				.PCBType          = 0,
				.PN               = {0},
				.SN               = {0},
				.Version          = {0},
				.CompTime         = {0},
				.ChipTemp  		  = 0.0,
				.ChipDAN          = {0},
				.Output_LVL = {
					.Reg_Addr    = PORT0WRITE,
					.Group0_Data = 0x0,
					.Group1_Data = 0xff,
				},
			},
			.Reg = {
				.Debug 	  = (volatile u32 *)(AXI_BASE_ADDR + 0x3000),
				.Opt_Addr = (volatile u32 *)(AXI_BASE_ADDR + 0x3004),
				.W_Data   = (volatile u32 *)(AXI_BASE_ADDR + 0x3008),
				.R_Data   = (volatile u32 *)(AXI_BASE_ADDR + 0x300C),
				.Data_Des = (volatile u32 *)(AXI_BASE_ADDR + 0x3010),
				.Opt_RW   = (volatile u32 *)(AXI_BASE_ADDR + 0x3014),
				.R_Update = (volatile u32 *)(AXI_BASE_ADDR + 0x3018),
				.Trigger  = (volatile u32 *)(AXI_BASE_ADDR + 0x3018),
				.R_Update = (volatile u32 *)(AXI_BASE_ADDR + 0x301C),
				.WOpt_OK  = (volatile u32 *)(AXI_BASE_ADDR + 0x3020),
			},
		},
		{//B0
			.BoardType = 0x4,
			.VecSize   = 0x0,
			.Msg = {
				.HWChan           = IIC00,
				.HWVersion        = 0,
				.PCBType          = 0,
				.PN               = {0},
				.SN               = {0},
				.Version          = {0},
				.CompTime         = {0},
				.ChipTemp  		  = 0.0,
				.ChipDAN          = {0},
				.Output_LVL = {
					.Reg_Addr    = PORT0WRITE,
					.Group0_Data = 0x0,
					.Group1_Data = 0xff,
				},
			},
			.Reg = {
				.Debug 	  = (volatile u32 *)(AXI_BASE_ADDR + 0x4000),
				.Opt_Addr = (volatile u32 *)(AXI_BASE_ADDR + 0x4004),
				.W_Data   = (volatile u32 *)(AXI_BASE_ADDR + 0x4008),
				.R_Data   = (volatile u32 *)(AXI_BASE_ADDR + 0x400C),
				.Data_Des = (volatile u32 *)(AXI_BASE_ADDR + 0x4010),
				.Opt_RW   = (volatile u32 *)(AXI_BASE_ADDR + 0x4014),
				.Trigger  = (volatile u32 *)(AXI_BASE_ADDR + 0x4018),
				.R_Update = (volatile u32 *)(AXI_BASE_ADDR + 0x401C),
				.WOpt_OK  = (volatile u32 *)(AXI_BASE_ADDR + 0x4020),
			},
		},
		{//B1
			.BoardType = 0x5,
			.VecSize   = 0x0,
			.Msg = {
				.HWChan           = IIC01,
				.HWVersion        = 0,
				.PCBType          = 0,
				.PN               = {0},
				.SN               = {0},
				.Version          = {0},
				.CompTime         = {0},
				.ChipTemp  		  = 0.0,
				.ChipDAN          = {0},
				.Output_LVL = {
					.Reg_Addr    = PORT0WRITE,
					.Group0_Data = 0x0,
					.Group1_Data = 0xff,
				},
			},
			.Reg = {
				.Debug 	  = (volatile u32 *)(AXI_BASE_ADDR + 0x5000),
				.Opt_Addr = (volatile u32 *)(AXI_BASE_ADDR + 0x5004),
				.W_Data   = (volatile u32 *)(AXI_BASE_ADDR + 0x5008),
				.R_Data   = (volatile u32 *)(AXI_BASE_ADDR + 0x500C),
				.Data_Des = (volatile u32 *)(AXI_BASE_ADDR + 0x5010),
				.Opt_RW   = (volatile u32 *)(AXI_BASE_ADDR + 0x5014),
				.Trigger  = (volatile u32 *)(AXI_BASE_ADDR + 0x5018),
				.R_Update = (volatile u32 *)(AXI_BASE_ADDR + 0x501C),
				.WOpt_OK  = (volatile u32 *)(AXI_BASE_ADDR + 0x5020),
			},
		},
	},
	.Vec_Addr = {
		.Debug    = (volatile u32 *)(AXI_BASE_ADDR + 0x6000),
		.S_Addr   = (volatile u32 *)(AXI_BASE_ADDR + 0x6004),
		.D_Len    = (volatile u32 *)(AXI_BASE_ADDR + 0x6008),
		.W_Trig   = (volatile u32 *)(AXI_BASE_ADDR + 0x600C),
		.Serial   = (volatile u32 *)(AXI_BASE_ADDR + 0x6010),
		.MV_Flag  = (volatile u32 *)(AXI_BASE_ADDR + 0x6014),
		.Clr_fifo = (volatile u32 *)(AXI_BASE_ADDR + 0x6018),
	},
	.ListReg = {
		.Debug 	    = (volatile u32 *)(AXI_BASE_ADDR + 0x7000),
		.List_L	    = (volatile u32 *)(AXI_BASE_ADDR + 0x7004),
		.List_M     = (volatile u32 *)(AXI_BASE_ADDR + 0x7008),
		.List_H     = (volatile u32 *)(AXI_BASE_ADDR + 0x700C),
		.WOpt_OK    = (volatile u32 *)(AXI_BASE_ADDR + 0x7010),
		.Fifo_Depth = (volatile u32 *)(AXI_BASE_ADDR + 0x7014),
		.IRQ_Mask   = (volatile u32 *)(AXI_BASE_ADDR + 0x7018),
		.Fifo_rst   = (volatile u32 *)(AXI_BASE_ADDR + 0x701C),
	},
};

DpsMsg	dps_info = {
	.GDQ54_SlaveAddr  = 0x5B,
	.EEPROM_SlaveAddr = 0x57,
	.PCA953_SlaveAddr = 0x20,
	.AD5694_SlaveAddr = 0x0C,
	.DBMsg = {
		{
			.Chan             = IIC06,
			.UseMask  		  = 0x0,
			.OpenMask 		  = 0x0,
			.CloseMask 		  = 0x0,
			.DpsAddr		  = {0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E},
			.Dpm              = {
									 {0x40, 0},{0x41, 0},{0x51, 0},{0x50, 0},{0x42, 0},
									 {0x43, 0},{0x44, 0},{0x45, 0},{0x54, 0},{0x55, 0},
								},
			.HWVersion        = 0,
			.PCBType          = 0,
			.PN               = {0},
			.SN               = {0},
			.Version          = {0},
			.CompTime         = {0},
			.ChipTemp  		  = 0.0,
			.ChipDAN          = {0},
			.Output_LVL = {
				.Reg_Addr    = PORT0WRITE,
				.Group0_Data = 0x0,
				.Group1_Data = 0xff,
			},
		},//DB0��Ϣ
		{
			.Chan             = IIC07,
			.UseMask  		  = 0x0,
			.OpenMask 		  = 0x0,
			.CloseMask 		  = 0x0,
			.DpsAddr		  = {0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E},
			.Dpm 			  = {
									 {0x40, 0},{0x41, 0},{0x51, 0},{0x50, 0},{0x42, 0},
									 {0x43, 0},{0x44, 0},{0x45, 0},{0x54, 0},{0x55, 0},
								},
			.HWVersion        = 0,
			.PCBType          = 0,
			.PN               = {0},
			.SN               = {0},
			.Version          = {0},
			.CompTime         = {0},
			.ChipTemp  		  = 0.0,
			.ChipDAN          = {0},
			.Output_LVL = {
				.Reg_Addr    = PORT0WRITE,
				.Group0_Data = 0x0,
				.Group1_Data = 0xff,
			},
		},//DB1��Ϣ
	},
};

BKBMsg	bbd_info = {
	.HWChan	          = IIC08,
	.EEPROM_SlaveAddr = 0x50,
	.PCA953_SlaveAddr = 0x20,
	.TOChan			  = IIC09,
	.IO_SlaveAddr     = 0x21,
	.HWVersion        = 0,
	.PCBType          = 0,
	.PN               = {0},
	.SN               = {0},

	.Output_LVL = {
		.Reg_Addr    = PORT0WRITE,
		.Group0_Data = 0x0,
		.Group1_Data = 0xff,
	},

	.Ctl_Output = {
		.Reg_Addr    = PORT0WRITE,
		.Group0_Data = 0xff,
		.Group1_Data = 0xff,
	},
};

MBMsg	mbd_info = {
	.HWChan 		  = PSIIC,
	.EEPROM_SlaveAddr = 0x50,
	.PCA953_SlaveAddr = 0x20,
	.DIPETH_SlaveAddr = 0x21,
	.DS1337_SlaveAddr = 0x68,
	.HWVersion 		  = 0,
	.PCBType 		  = 0,
	.PN 			  = {0},
	.SN 			  = {0},

	.Version0 		  = {0},
	.CompTime0 		  = {0},
	.Version1 		  = {0},
	.CompTime1 		  = {0},
	.ChipTemp 		  = 0.0,
	.ChipDAN 		  = {0},
	.Output_LVL = {
		.Reg_Addr    = PORT0WRITE,
		.Group0_Data = 0x0,
		.Group1_Data = 0xff,
	},
	.IP_ADDR    = {
		.Reg_Addr    = PORT0WRITE,
		.Group0_Data = 0x0,
		.Group1_Data = 0xff,
	}
};

Dual_Slot_Info dual_slot_info = {
	.Chan 		        = IIC10,
	.EEPROM_SlaveAddr   = 0x50,
	.PCA953_SlaveAddr_0 = 0x20,
	.PCA953_SlaveAddr_1 = 0x21,

	.HWVersion 		  = 0,
	.PCBType 		  = 0,
	.PN 			  = {0},
	.SN 			  = {0},

	.Output_LVL = {
		.Reg_Addr    = PORT0WRITE,
		.Group0_Data = 0x0,
		.Group1_Data = 0xFF,
	},

	.Slot_Insert = {
		.Reg_Addr    = PORT0WRITE,
		.Group0_Data = 0xFF,
		.Group1_Data = 0x0,
	}
};

BackIsp	All_isp;


