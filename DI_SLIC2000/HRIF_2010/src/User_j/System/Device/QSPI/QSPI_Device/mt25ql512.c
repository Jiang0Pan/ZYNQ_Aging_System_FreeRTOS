/*
  ** @file           : mt25ql512.c
  ** @brief          : The Device Of MT25QL512(QSPI FLASH Device) Source File
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
/* QSPI device driver */
#include "xqspips.h"
#include "return_val.h"
#include "debug_config.h"
#include "qspi_bus.h"
#include "mt25ql512.h"


/*
  ** Define
*/
/*
 * The following constants define the offsets within a FlashBuffer data
 * type for each kind of data.  Note that the read data offset is not the
 * same as the write data because the QSPI driver is designed to allow full
 * duplex transfers such that the number of bytes received is the number
 * sent and received.
 */
/* Flash Instruction */
#define 	COMMAND_OFFSET		(0)
/* MSB byte of address to read or write */
#define 	ADDRESS_1_OFFSET	(1)
/* Middle byte of address to read or write */
#define 	ADDRESS_2_OFFSET	(2)
/* LSB byte of address to read or write */
#define 	ADDRESS_3_OFFSET	(3)
/* Start of Data for Read/Write */
#define 	DATA_OFFSET			(4)
/* Dummy byte offset for fast, dual and quad reads */
#define 	DUMMY_OFFSET		(4)
/* Number of dummy bytes for fast, dual and quad reads */
#define 	DUMMY_SIZE			(1)
/* Read ID command + 3 bytes ID response */
#define 	RD_ID_SIZE			(4)
/* Bulk Erase command size */
#define 	BULK_ERASE_SIZE		(1)
/* Sector Erase command + Sector address */
#define 	SEC_ERASE_SIZE		(4)
/* BRWR or EARWR command + 1 byte bank value */
#define 	BANK_SEL_SIZE		(2)
/* 1 byte Configuration register + RD CFG command*/
#define 	RD_CFG_SIZE			(2)
/* WRR command + 1 byte each Status and Config Reg*/
#define 	WR_CFG_SIZE			(3)
/* Die Erase command + Die address */
#define 	DIE_ERASE_SIZE		(4)

/*
  ** The following constants specify the extra bytes which are sent to the
  ** Flash on the QSPI interface, that are not data, but control information
  ** which includes the command and address
*/
#define 	OVERHEAD_SIZE		(4)
/* Base address of Flash1 */
#define 	FLASH1BASE 			(0x0000000)
/* Sixteen MB */
#define 	SIXTEENMB 			(0x1000000)


/* Mask for quad enable bit in Flash configuration register */
#define 	FLASH_QUAD_EN_MASK 	(0x02)
#define 	FLASH_SRWD_MASK 	(0x80)

/* Bank mask */
#define 	BANKMASK 			(0xF000000)

/*
  ** Identification of Flash
  ** Micron:
  ** Byte 0 is Manufacturer ID;
  ** Byte 1 is first byte of Device ID - 0xBB or 0xBA
  ** Byte 2 is second byte of Device ID describes flash size:
  ** 128Mbit : 0x18;
  ** 256Mbit : 0x19;
  ** 512Mbit : 0x20
  ** Spansion:
  ** Byte 0 is Manufacturer ID;
  ** Byte 1 is Device ID - Memory Interface type - 0x20 or 0x02
  ** Byte 2 is second byte of Device ID describes flash size:
  ** 128Mbit : 0x18;
  ** 256Mbit : 0x19;
  ** 512Mbit : 0x20
*/
#define 	MICRON_ID_BYTE0					(0x20)
#define 	MICRON_ID_BYTE2_128				(0x18)
#define 	MICRON_ID_BYTE2_256				(0x19)
#define 	MICRON_ID_BYTE2_512				(0x20)
#define 	MICRON_ID_BYTE2_1G				(0x21)
#define 	SPANSION_ID_BYTE0				(0x01)
#define 	SPANSION_ID_BYTE2_128			(0x18)
#define 	SPANSION_ID_BYTE2_256			(0x19)
#define 	SPANSION_ID_BYTE2_512			(0x20)
#define 	WINBOND_ID_BYTE0				(0xEF)
#define 	WINBOND_ID_BYTE2_128			(0x18)
#define 	MACRONIX_ID_BYTE0				(0xC2)
#define 	MACRONIX_ID_BYTE2_256			(0x19)
#define 	MACRONIX_ID_BYTE2_512			(0x1A)
#define 	MACRONIX_ID_BYTE2_1G			(0x1B)
/* The index for Flash config table */
/* Spansion*/
#define 	SPANSION_INDEX_START			(0)
#define 	FLASH_CFG_TBL_SINGLE_128_SP		(SPANSION_INDEX_START)
#define 	FLASH_CFG_TBL_STACKED_128_SP	(SPANSION_INDEX_START + 1)
#define 	FLASH_CFG_TBL_PARALLEL_128_SP	(SPANSION_INDEX_START + 2)
#define 	FLASH_CFG_TBL_SINGLE_256_SP		(SPANSION_INDEX_START + 3)
#define 	FLASH_CFG_TBL_STACKED_256_SP	(SPANSION_INDEX_START + 4)
#define 	FLASH_CFG_TBL_PARALLEL_256_SP	(SPANSION_INDEX_START + 5)
#define 	FLASH_CFG_TBL_SINGLE_512_SP		(SPANSION_INDEX_START + 6)
#define 	FLASH_CFG_TBL_STACKED_512_SP	(SPANSION_INDEX_START + 7)
#define 	FLASH_CFG_TBL_PARALLEL_512_SP	(SPANSION_INDEX_START + 8)
/* Micron */
#define 	MICRON_INDEX_START				(FLASH_CFG_TBL_PARALLEL_512_SP + 1)
#define 	FLASH_CFG_TBL_SINGLE_128_MC		(MICRON_INDEX_START)
#define 	FLASH_CFG_TBL_STACKED_128_MC	(MICRON_INDEX_START + 1)
#define 	FLASH_CFG_TBL_PARALLEL_128_MC	(MICRON_INDEX_START + 2)
#define 	FLASH_CFG_TBL_SINGLE_256_MC		(MICRON_INDEX_START + 3)
#define 	FLASH_CFG_TBL_STACKED_256_MC	(MICRON_INDEX_START + 4)
#define 	FLASH_CFG_TBL_PARALLEL_256_MC	(MICRON_INDEX_START + 5)
#define 	FLASH_CFG_TBL_SINGLE_512_MC		(MICRON_INDEX_START + 6)
#define 	FLASH_CFG_TBL_STACKED_512_MC	(MICRON_INDEX_START + 7)
#define 	FLASH_CFG_TBL_PARALLEL_512_MC	(MICRON_INDEX_START + 8)
#define 	FLASH_CFG_TBL_SINGLE_1GB_MC		(MICRON_INDEX_START + 9)
#define 	FLASH_CFG_TBL_STACKED_1GB_MC	(MICRON_INDEX_START + 10)
#define 	FLASH_CFG_TBL_PARALLEL_1GB_MC	(MICRON_INDEX_START + 11)
/* Winbond */
#define 	WINBOND_INDEX_START				(FLASH_CFG_TBL_PARALLEL_1GB_MC + 1)
#define 	FLASH_CFG_TBL_SINGLE_128_WB		(WINBOND_INDEX_START)
#define 	FLASH_CFG_TBL_STACKED_128_WB	(WINBOND_INDEX_START + 1)
#define 	FLASH_CFG_TBL_PARALLEL_128_WB	(WINBOND_INDEX_START + 2)
/* Macronix */
#define 	MACRONIX_INDEX_START			(FLASH_CFG_TBL_PARALLEL_128_WB + 1 - 3)
#define 	FLASH_CFG_TBL_SINGLE_256_MX		(MACRONIX_INDEX_START)
#define 	FLASH_CFG_TBL_STACKED_256_MX	(MACRONIX_INDEX_START + 1)
#define 	FLASH_CFG_TBL_PARALLEL_256_MX	(MACRONIX_INDEX_START + 2)
#define 	FLASH_CFG_TBL_SINGLE_512_MX		(MACRONIX_INDEX_START + 3)
#define 	FLASH_CFG_TBL_STACKED_512_MX	(MACRONIX_INDEX_START + 4)
#define 	FLASH_CFG_TBL_PARALLEL_512_MX	(MACRONIX_INDEX_START + 5)
#define 	FLASH_CFG_TBL_SINGLE_1G_MX		(MACRONIX_INDEX_START + 6)
#define 	FLASH_CFG_TBL_STACKED_1G_MX		(MACRONIX_INDEX_START + 7)
#define 	FLASH_CFG_TBL_PARALLEL_1G_MX	(MACRONIX_INDEX_START + 8)

/*
  ** The following constants map to the XPAR parameters created in the
  ** xparameters.h file. They are defined here such that a user can easily
  ** change all the needed parameters in one place.
*/
/* Number of flash pages to be written */
#define 	PAGE_COUNT						(1024)
/* Max page size to initialize write and read buffer */
#define 	MAX_PAGE_SIZE 					(1024)
/* Flash address to which data is to be written */
#define 	BIN_START_ADDRESS				(0x0000000)
#define 	UNIQUE_VALUE					(0x06)


/*
  ** Typedef
*/
typedef struct{
	/* Individual sector size or combined sector size in case of parallel config */
	u32 SectSize;
	/* Total no. of sectors in one/two flash devices */
	u32 NumSect;
	/* Individual page size or combined page size in case of parallel config*/
	u32 PageSize;
	/* Total no. of pages in one/two flash devices */
	u32 NumPage;
	/*
	  ** This is the size of one flash device
	  ** NOT the combination of both devices, if present
	*/
	u32 FlashDeviceSize;
	/* Manufacturer ID - used to identify make */
	u8 ManufacturerID;
	/* Byte of device ID indicating the memory size */
	u8 DeviceIDMemSize;
	/* Mask to get sector start address */
	u32 SectMask;
	/* No. of die forming a single flash */
	u8 NumDie;
}FlashInfo;

/*
  ** Macros (Inline Functions) Definitions
*/
static FlashInfo Flash_Config_Table[33] = {
		/* Spansion */
		{0x10000, 0x100, 256, 0x10000, 0x1000000,
				SPANSION_ID_BYTE0, SPANSION_ID_BYTE2_128, 0xFFFF0000, 1},
		{0x10000, 0x200, 256, 0x20000, 0x1000000,
				SPANSION_ID_BYTE0, SPANSION_ID_BYTE2_128, 0xFFFF0000, 1},
		{0x20000, 0x100, 512, 0x10000, 0x1000000,
				SPANSION_ID_BYTE0, SPANSION_ID_BYTE2_128, 0xFFFE0000, 1},
		{0x10000, 0x200, 256, 0x20000, 0x2000000,
				SPANSION_ID_BYTE0, SPANSION_ID_BYTE2_256, 0xFFFF0000, 1},
		{0x10000, 0x400, 256, 0x40000, 0x2000000,
				SPANSION_ID_BYTE0, SPANSION_ID_BYTE2_256, 0xFFFF0000, 1},
		{0x20000, 0x200, 512, 0x20000, 0x2000000,
				SPANSION_ID_BYTE0, SPANSION_ID_BYTE2_256, 0xFFFE0000, 1},
		{0x40000, 0x100, 512, 0x20000, 0x4000000,
				SPANSION_ID_BYTE0, SPANSION_ID_BYTE2_512, 0xFFFC0000, 1},
		{0x40000, 0x200, 512, 0x40000, 0x4000000,
				SPANSION_ID_BYTE0, SPANSION_ID_BYTE2_512, 0xFFFC0000, 1},
		{0x80000, 0x100, 1024, 0x20000, 0x4000000,
				SPANSION_ID_BYTE0, SPANSION_ID_BYTE2_512, 0xFFF80000, 1},
		/* Spansion 1Gbit is handled as 512Mbit stacked */
		/* Micron */
		{0x10000, 0x100, 256, 0x10000, 0x1000000,
				MICRON_ID_BYTE0, MICRON_ID_BYTE2_128, 0xFFFF0000, 1},
		{0x10000, 0x200, 256, 0x20000, 0x1000000,
				MICRON_ID_BYTE0, MICRON_ID_BYTE2_128, 0xFFFF0000, 1},
		{0x20000, 0x100, 512, 0x10000, 0x1000000,
				MICRON_ID_BYTE0, MICRON_ID_BYTE2_128, 0xFFFE0000, 1},
		{0x10000, 0x200, 256, 0x20000, 0x2000000,
				MICRON_ID_BYTE0, MICRON_ID_BYTE2_256, 0xFFFF0000, 1},
		{0x10000, 0x400, 256, 0x40000, 0x2000000,
				MICRON_ID_BYTE0, MICRON_ID_BYTE2_256, 0xFFFF0000, 1},
		{0x20000, 0x200, 512, 0x20000, 0x2000000,
				MICRON_ID_BYTE0, MICRON_ID_BYTE2_256, 0xFFFE0000, 1},
		{0x10000, 0x400, 256, 0x40000, 0x4000000,
				MICRON_ID_BYTE0, MICRON_ID_BYTE2_512, 0xFFFF0000, 2},
		{0x10000, 0x800, 256, 0x80000, 0x4000000,
				MICRON_ID_BYTE0, MICRON_ID_BYTE2_512, 0xFFFF0000, 2},
		{0x20000, 0x400, 512, 0x40000, 0x4000000,
				MICRON_ID_BYTE0, MICRON_ID_BYTE2_512, 0xFFFE0000, 2},
		{0x10000, 0x800, 256, 0x80000, 0x8000000,
				MICRON_ID_BYTE0, MICRON_ID_BYTE2_1G, 0xFFFF0000, 4},
		{0x10000, 0x1000, 256, 0x100000, 0x8000000,
				MICRON_ID_BYTE0, MICRON_ID_BYTE2_1G, 0xFFFF0000, 4},
		{0x20000, 0x800, 512, 0x80000, 0x8000000,
				MICRON_ID_BYTE0, MICRON_ID_BYTE2_1G, 0xFFFE0000, 4},
		/* Winbond */
		{0x10000, 0x100, 256, 0x10000, 0x1000000,
				WINBOND_ID_BYTE0, WINBOND_ID_BYTE2_128, 0xFFFF0000, 1},
		{0x10000, 0x200, 256, 0x20000, 0x1000000,
				WINBOND_ID_BYTE0, WINBOND_ID_BYTE2_128, 0xFFFF0000, 1},
		{0x20000, 0x100, 512, 0x10000, 0x1000000,
				WINBOND_ID_BYTE0, WINBOND_ID_BYTE2_128, 0xFFFE0000, 1},
		/* Macronix */
		{0x10000, 0x200, 256, 0x20000, 0x2000000,
				MACRONIX_ID_BYTE0, MACRONIX_ID_BYTE2_256, 0xFFFF0000, 1},
		{0x10000, 0x400, 256, 0x40000, 0x2000000,
				MACRONIX_ID_BYTE0, MACRONIX_ID_BYTE2_256, 0xFFFF0000, 1},
		{0x20000, 0x200, 512, 0x20000, 0x2000000,
				MACRONIX_ID_BYTE0, MACRONIX_ID_BYTE2_256, 0xFFFE0000, 1},
		{0x10000, 0x400, 256, 0x40000, 0x4000000,
				MACRONIX_ID_BYTE0, MACRONIX_ID_BYTE2_512, 0xFFFF0000, 1},
		{0x10000, 0x800, 256, 0x80000, 0x4000000,
				MACRONIX_ID_BYTE0, MACRONIX_ID_BYTE2_512, 0xFFFF0000, 1},
		{0x20000, 0x400, 512, 0x40000, 0x4000000,
				MACRONIX_ID_BYTE0, MACRONIX_ID_BYTE2_512, 0xFFFE0000, 1},
		{0x2000, 0x4000, 256, 0x80000, 0x8000000,
				MACRONIX_ID_BYTE0, MACRONIX_ID_BYTE2_1G, 0xFFFF0000, 1},
		{0x2000, 0x8000, 256, 0x100000, 0x8000000,
				MACRONIX_ID_BYTE0, MACRONIX_ID_BYTE2_1G, 0xFFFF0000, 1},
		{0x4000, 0x4000, 512, 0x80000, 0x8000000,
				MACRONIX_ID_BYTE0, MACRONIX_ID_BYTE2_1G, 0xFFFE0000, 1}
};/* Flash Config Table */

/*
  ** The following variables are used to read and write to the flash and they
  ** are global to avoid having large buffers on the stack
  ** The buffer size accounts for maximum page size and maximum banks -
  ** for each bank separate read will be performed leading to that many
  ** (overhead+dummy) bytes
*/
static u8 ReadBuffer[(PAGE_COUNT * MAX_PAGE_SIZE) + (DATA_OFFSET + DUMMY_SIZE)*8];
static u8 WriteBuffer[(PAGE_COUNT * MAX_PAGE_SIZE) + DATA_OFFSET];

/*
  ** The following constants specify the max amount of data and the size of the
  ** the buffer required to hold the data and overhead to transfer the data to
  ** and from the Flash. Initialized to single flash page size.
*/
//static u32 MaxData = PAGE_COUNT*256;

static u32 FlashMake;
/* Flash configuration table index */
static u32 FCTIndex;


/*
  ** Function Name        : Get_Write_Buffer
  ** Function Description : Get Pointer Write Buffer Point
  ** Function Parameter   : Void
  ** Function Return      : u8 *         		: Write Buffer Address
  ** Notes				  : None
*/
u8 * Get_Write_Buffer(void)
{
	return ReadBuffer;
}


/*
  ** Function Name        : Get_Read_Buffer
  ** Function Description : Get Pointer Read Buffer Point
  ** Function Parameter   : Void
  ** Function Return      : u8 *         		: Read Buffer Address
  ** Notes				  : None
*/
u8 * Get_Read_Buffer(void)
{
	return WriteBuffer;
}


/*
  ** Function Name        : Mt25ql512_Init
  ** Function Description : Mt25ql512 (Qspi FLash Device) Init
  ** Function Parameter   : Void
  ** Function Return      : u8 *         		: Read Buffer Address
  ** Notes				  : None
*/
u8 Mt25ql512_Init(void)
{
	if (READ_SUCCESS != \
			Read_Flash_ID(WriteBuffer, ReadBuffer)) {
		return INIT_FAILED;
	}
	return INIT_SUCCESS;
}


/*
  ** Function Name        : Get_Page_Count
  ** Function Description : Get Page Count
  ** Function Parameter   : Length              : Data Length
  ** Function Return      : u32         		: The Count Of Page
  ** Notes				  : None
*/
u32 Get_Page_Count(u32 Length)
{
	u32 Page_cnt = 0x00;

	/* calculate the page need to write */
	Page_cnt = Length / Flash_Config_Table[FCTIndex].PageSize;

	if (Length % Flash_Config_Table[FCTIndex].PageSize) {
		Page_cnt += 1;
	}

	return Page_cnt;
}


/*
  ** Function Name        : Get_Page_Count
  ** Function Description : Get Page Count
  ** Function Parameter   : Length              : Data Length
  ** Function Return      : u32         		: The Count Of Page
  ** Notes				  : None
*/
u32 Get_Page_Size(void)
{
	return Flash_Config_Table[FCTIndex].PageSize;
}


/*
  ** Function Name        : Write_Flash
  ** Function Description : Qspi Bus Interface (Write Flash)
  ** Function Parameter   : Address             : Write Data In Flash Address
  ** 						ByteCount           : Write Data Length
  ** 						Command             : Write Command
  ** 						WriteBfrPtr         : Write Data Address
  ** Function Return      : WRITE_FAILED 		: Write Failed
  **                        WRITE_SUCCESS	    : Write Success
  ** Notes				  : None
*/
u8 Write_Flash(u32 Address, u32 ByteCount, u8 Command, u8 * WriteBfrPtr)
{
	u8 WriteEnableCmd = { WRITE_ENABLE_CMD };
	/* Must send 2 bytes */
	u8 ReadStatusCmd[] = { READ_STATUS_CMD, 0 };
	u8 FlashStatus[2];
	u32 RealAddr;
	u32 BankSel;
	u8 ReadFlagSRCmd[] = {READ_FLAG_STS_CMD, 0};
	u8 FlagStatus[2];
	/*
	  ** Translate address based on type of connection
	  ** If stacked assert the slave select based on address
	*/
	RealAddr = GetRealAddr(Address);
	/* Bank Select */
	if (Flash_Config_Table[FCTIndex].FlashDeviceSize > SIXTEENMB) {
		/* Calculate bank */
		BankSel = RealAddr / SIXTEENMB;
		/* Select bank */
		SendBankSelect(WriteBfrPtr, BankSel);
	}
	/*
	 ** Send the write enable command to the Flash so that it can be
	 ** written to, this needs to be sent as a separate transfer before
	 ** the write
	*/
	if (WRITE_READ_SUCCESS != \
			Qspi_Write_Read(&WriteEnableCmd, NULL, sizeof(WriteEnableCmd))) {
		debug("Qspi Write And Read Failed In Write_Flash In %s Files,%d Line\n",\
				__FILE__,__LINE__);
		return WRITE_FAILED;
	}

	/*
	  ** Setup the write command with the specified address and data for the
	  ** Flash
	*/
	/*
	  ** This will ensure a 3B address is transferred even when address
	  ** is greater than 128Mb.
	*/
	WriteBfrPtr[COMMAND_OFFSET]   = Command;
	WriteBfrPtr[ADDRESS_1_OFFSET] = (u8)((RealAddr & 0xFF0000) >> 16);
	WriteBfrPtr[ADDRESS_2_OFFSET] = (u8)((RealAddr & 0xFF00) >> 8);
	WriteBfrPtr[ADDRESS_3_OFFSET] = (u8)(RealAddr & 0xFF);

	/*
	  ** Send the write command, address, and data to the Flash to be
	  ** written, no receive buffer is specified since there is nothing to
	  ** receive
	*/
	if (WRITE_READ_SUCCESS != \
			Qspi_Write_Read(WriteBfrPtr, NULL, ByteCount + OVERHEAD_SIZE)) {
		debug("Qspi Write And Read Failed In Write_Flash In %s Files,%d Line\n",\
				__FILE__,__LINE__);
		return WRITE_FAILED;
	}
	if ((Flash_Config_Table[FCTIndex].NumDie > 1) &&
			(FlashMake == MICRON_ID_BYTE0)) {
		if (WRITE_READ_SUCCESS != \
				Qspi_Write_Read(ReadFlagSRCmd, FlagStatus, sizeof(ReadFlagSRCmd))) {
			debug("Qspi Write And Read Failed In Write_Flash In %s Files,%d Line\n",\
					__FILE__,__LINE__);
			return WRITE_FAILED;
		}
	}
	/*
	  ** Wait for the write command to the Flash to be completed, it takes
	  ** some time for the data to be written
	*/
	while (1) {
		/*
		  ** Poll the status register of the Flash to determine when it
		  ** completes, by sending a read status command and receiving the
		  ** status byte
		*/
		if (WRITE_READ_SUCCESS != \
				Qspi_Write_Read(ReadStatusCmd, FlashStatus, sizeof(ReadStatusCmd))) {
			debug("Qspi Write And Read Failed In Write_Flash In %s Files,%d Line\n",\
					__FILE__,__LINE__);
			return WRITE_FAILED;
		}

		/*
		  ** If the status indicates the write is done, then stop waiting,
		  ** if a value of 0xFF in the status byte is read from the
		  ** device and this loop never exits, the device slave select is
		  ** possibly incorrect such that the device status is not being
		  ** read
		*/
		if ((FlashStatus[1] & 0x01) == 0) {
			break;
		}
	}
	if((Flash_Config_Table[FCTIndex].NumDie > 1) &&
			(FlashMake == MICRON_ID_BYTE0)) {
		if (WRITE_READ_SUCCESS != \
				Qspi_Write_Read(ReadFlagSRCmd, FlagStatus, sizeof(ReadFlagSRCmd))) {
			debug("Qspi Write And Read Failed In Write_Flash In %s Files,%d Line\n",\
					__FILE__,__LINE__);
			return WRITE_FAILED;
		}
	}
	return WRITE_SUCCESS;
}


/*
  ** Function Name        : Erase_Flash
  ** Function Description : Qspi Bus Interface (Erase Flash)
  ** Function Parameter   : Address             : Erase Flash Address
  ** 						ByteCount           : Erase Data Length
  ** 						WriteBfrPtr         : Erase Data Address
  ** Function Return      : WRITE_FAILED 		: Write Failed
  **                        WRITE_SUCCESS	    : Write Success
  ** Notes				  : None
*/
u8 Erase_Flash(u32 Address, u32 ByteCount, u8 *WriteBfrPtr)
{
	u8 WriteEnableCmd = { WRITE_ENABLE_CMD };
	/* Must send 2 bytes */
	u8 ReadStatusCmd[] = { READ_STATUS_CMD, 0 };
	u8 FlashStatus[2];
	int Sector;
	u32 RealAddr;
	u32 LqspiCr;
	u32 NumSect;
	u32 BankSel;
	u8 BankInitFlag = 1;
	u8 ReadFlagSRCmd[] = { READ_FLAG_STS_CMD, 0 };
	u8 FlagStatus[2];

	/*
	  ** If erase size is same as the total size of the flash, use bulk erase
	  ** command or die erase command multiple times as required
	*/
	if (ByteCount == ((Flash_Config_Table[FCTIndex]).NumSect *
			(Flash_Config_Table[FCTIndex]).SectSize) ) {
		XQspiPs * QspiPtr = Get_Qspi_Point();
		if (QspiPtr->Config.ConnectionMode == XQSPIPS_CONNECTION_MODE_STACKED) {
			/* Get the current LQSPI configuration register value */
			LqspiCr = XQspiPs_GetLqspiConfigReg(QspiPtr);
			/* Set selection to L_PAGE */
			XQspiPs_SetLqspiConfigReg(QspiPtr,
					LqspiCr & (~XQSPIPS_LQSPI_CR_U_PAGE_MASK));
			/* Assert the Flash chip select. */
			XQspiPs_SetSlaveSelect(QspiPtr);
		}
		if (Flash_Config_Table[FCTIndex].NumDie == 1) {
			/* Call Bulk erase */
			Erase_Bulk(WriteBfrPtr);
		}
		if (Flash_Config_Table[FCTIndex].NumDie > 1) {
			/* Call Die Erase */
			Erase_Die(WriteBfrPtr);
		}
		/* If stacked mode, bulk erase second flash */
		if (QspiPtr->Config.ConnectionMode == XQSPIPS_CONNECTION_MODE_STACKED) {
			/* Get the current LQSPI configuration register value */
			LqspiCr = XQspiPs_GetLqspiConfigReg(QspiPtr);
			/* Set selection to U_PAGE */
			XQspiPs_SetLqspiConfigReg(QspiPtr,
					LqspiCr | XQSPIPS_LQSPI_CR_U_PAGE_MASK);
			/* Assert the Flash chip select */
			XQspiPs_SetSlaveSelect(QspiPtr);
			if (Flash_Config_Table[FCTIndex].NumDie == 1) {
				/* Call Bulk erase */
				Erase_Bulk(WriteBfrPtr);
			}
			if (Flash_Config_Table[FCTIndex].NumDie > 1) {
				/* Call Die erase */
				Erase_Die(WriteBfrPtr);
			}
		}
		return ERASE_FAILED;
	}
	/*
	  ** If the erase size is less than the total size of the flash, use
	  ** sector erase command
	*/
	/* Calculate no. of sectors to erase based on byte count */
	NumSect = ByteCount/(Flash_Config_Table[FCTIndex].SectSize) + 1;

	/*
	  ** If ByteCount to k sectors,
	  ** but the address range spans from N to N+k+1 sectors, then
	  ** increment no. of sectors to be erased
	*/
	if (((Address + ByteCount) & Flash_Config_Table[FCTIndex].SectMask) ==
			((Address + (NumSect * Flash_Config_Table[FCTIndex].SectSize)) &
					Flash_Config_Table[FCTIndex].SectMask) ) {
		NumSect++;
	}

	for (Sector = 0; Sector < NumSect; Sector++) {
		/*
		  ** Translate address based on type of connection
		  ** If stacked assert the slave select based on address
		*/
		RealAddr = GetRealAddr(Address);
		/* Initial bank selection */
		if((BankInitFlag) &&
				(Flash_Config_Table[FCTIndex].FlashDeviceSize > SIXTEENMB)) {
			/* Reset initial bank select flag */
			BankInitFlag = 0;
			/* Calculate initial bank */
			BankSel = RealAddr/SIXTEENMB;
			/* Select bank */
			SendBankSelect(WriteBfrPtr, BankSel);
		}
		/* Check bank and send bank select if new bank */
		if((BankSel != RealAddr/SIXTEENMB) &&
				(Flash_Config_Table[FCTIndex].FlashDeviceSize > SIXTEENMB)) {
			/* Calculate initial bank */
			BankSel = RealAddr/SIXTEENMB;
			/* Select bank */
			SendBankSelect(WriteBfrPtr, BankSel);
		}
		/*
		  ** Send the write enable command to the SEEPOM so that it can be
		  ** written to, this needs to be sent as a separate transfer
		  ** before the write
		*/
		if (WRITE_READ_SUCCESS != \
				Qspi_Write_Read(&WriteEnableCmd, NULL, sizeof(WriteEnableCmd))) {
			return ERASE_FAILED;
		}
		/*
		  ** Setup the write command with the specified address and data
		  ** for the Flash
		*/
		/*
		  ** This ensures 3B address is sent to flash even with address
		  ** greater than 128Mb.
		*/
		WriteBfrPtr[COMMAND_OFFSET]   = SEC_ERASE_CMD;
		WriteBfrPtr[ADDRESS_1_OFFSET] = (u8)(RealAddr >> 16);
		WriteBfrPtr[ADDRESS_2_OFFSET] = (u8)(RealAddr >> 8);
		WriteBfrPtr[ADDRESS_3_OFFSET] = (u8)(RealAddr & 0xFF);
		/*
		  ** Send the sector erase command and address; no receive buffer
		  ** is specified since there is nothing to receive
		*/
		if (WRITE_READ_SUCCESS != \
				Qspi_Write_Read(WriteBfrPtr, NULL, SEC_ERASE_SIZE)) {
			return ERASE_FAILED;
		}

		if((Flash_Config_Table[FCTIndex].NumDie > 1) &&
				(FlashMake == MICRON_ID_BYTE0)) {
			if (WRITE_READ_SUCCESS != \
					Qspi_Write_Read(ReadFlagSRCmd, FlagStatus, sizeof(ReadFlagSRCmd))) {
				return ERASE_FAILED;
			}
		}
		/*
		  ** Wait for the sector erase command to the Flash to be completed
		*/
		while (1) {
			/*
			  ** Poll the status register of the device to determine
			  ** when it completes, by sending a read status command
			  ** and receiving the status byte
			*/
			if (WRITE_READ_SUCCESS != \
					Qspi_Write_Read(ReadStatusCmd, FlashStatus, sizeof(ReadStatusCmd))) {
				return ERASE_FAILED;
			}
			/*
			  ** If the status indicates the write is done, then stop
			  ** waiting, if a value of 0xFF in the status byte is
			  ** read from the device and this loop never exits, the
			  ** device slave select is possibly incorrect such that
			  ** the device status is not being read
			*/
			if ((FlashStatus[1] & 0x01) == 0) {
				break;
			}
		}

		if((Flash_Config_Table[FCTIndex].NumDie > 1) &&
				(FlashMake == MICRON_ID_BYTE0)) {
			if (WRITE_READ_SUCCESS != \
					Qspi_Write_Read(ReadFlagSRCmd, FlagStatus,sizeof(ReadFlagSRCmd))) {
				return ERASE_FAILED;
			}
		}

		Address += Flash_Config_Table[FCTIndex].SectSize;

	}
	return ERASE_SUCCESS;
}


/*
  ** Function Name        : Read_Flash_ID
  ** Function Description : Qspi Bus Interface (Read Flash ID)
  ** Function Parameter   : WriteBfrPtr         : Write Data Address
  ** 						ReadBfrPtr          : Read Data Address
  ** Function Return      : READ_FAILED 		: Write Failed
  **                        READ_SUCCESS	    : Write Success
  ** Notes				  : None
*/
u8 Read_Flash_ID(u8 *WriteBfrPtr, u8 *ReadBfrPtr)
{
	int Status;
	int StartIndex;

	/* Read ID in Auto mode. */
	WriteBfrPtr[COMMAND_OFFSET]   = READ_ID;
	/* 3 dummy bytes */
	WriteBfrPtr[ADDRESS_1_OFFSET] = 0x23;
	WriteBfrPtr[ADDRESS_2_OFFSET] = 0x08;
	WriteBfrPtr[ADDRESS_3_OFFSET] = 0x09;

	Status = Qspi_Write_Read(WriteBfrPtr, ReadBfrPtr, RD_ID_SIZE);
	if (WRITE_READ_SUCCESS != Status) {
		debug("Qspi Read Flash ID Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		return READ_FAILED;
	}

	/* Deduce flash make */
	if (ReadBfrPtr[1] == MICRON_ID_BYTE0) {
		FlashMake = MICRON_ID_BYTE0;
		StartIndex = MICRON_INDEX_START;
	}
	else if (ReadBfrPtr[1] == SPANSION_ID_BYTE0) {
		FlashMake = SPANSION_ID_BYTE0;
		StartIndex = SPANSION_INDEX_START;
	}
	else if (ReadBfrPtr[1] == WINBOND_ID_BYTE0) {
		FlashMake = WINBOND_ID_BYTE0;
		StartIndex = WINBOND_INDEX_START;
	}
	else if (ReadBfrPtr[1] == MACRONIX_ID_BYTE0) {
		FlashMake = MACRONIX_ID_BYTE0;
		StartIndex = MACRONIX_INDEX_START;
	}

	XQspiPs * QspiPtr = Get_Qspi_Point();
	/*
	  ** If valid flash ID, then check connection mode & size and
	  ** assign corresponding index in the Flash configuration table
	*/
	if(((FlashMake == MICRON_ID_BYTE0) || (FlashMake == SPANSION_ID_BYTE0)||
			(FlashMake == WINBOND_ID_BYTE0)) &&
			(ReadBfrPtr[3] == MICRON_ID_BYTE2_128)) {
		switch (QspiPtr->Config.ConnectionMode) {
			case XQSPIPS_CONNECTION_MODE_SINGLE:
				FCTIndex = FLASH_CFG_TBL_SINGLE_128_SP + StartIndex;
			break;
			case XQSPIPS_CONNECTION_MODE_PARALLEL:
				FCTIndex = FLASH_CFG_TBL_PARALLEL_128_SP + StartIndex;
			break;
			case XQSPIPS_CONNECTION_MODE_STACKED:
				FCTIndex = FLASH_CFG_TBL_STACKED_128_SP + StartIndex;
			break;
			default:
				FCTIndex = 0;
			break;
		}
	}
	/* 256 and 512Mbit supported only for Micron and Spansion, not Winbond */
	if(((FlashMake == MICRON_ID_BYTE0) || (FlashMake == SPANSION_ID_BYTE0)
			|| (FlashMake == MACRONIX_ID_BYTE0)) &&
			(ReadBfrPtr[3] == MICRON_ID_BYTE2_256)) {
		switch (QspiPtr->Config.ConnectionMode) {
			case XQSPIPS_CONNECTION_MODE_SINGLE:
				FCTIndex = FLASH_CFG_TBL_SINGLE_256_SP + StartIndex;
			break;
			case XQSPIPS_CONNECTION_MODE_PARALLEL:
				FCTIndex = FLASH_CFG_TBL_PARALLEL_256_SP + StartIndex;
			break;
			case XQSPIPS_CONNECTION_MODE_STACKED:
				FCTIndex = FLASH_CFG_TBL_STACKED_256_SP + StartIndex;
			break;
			default:
				FCTIndex = 0;
			break;
		}
	}
	if ((((FlashMake == MICRON_ID_BYTE0) || (FlashMake == SPANSION_ID_BYTE0)) &&
			(ReadBfrPtr[3] == MICRON_ID_BYTE2_512)) || ((FlashMake ==
			MACRONIX_ID_BYTE0) && (ReadBfrPtr[3] == MACRONIX_ID_BYTE2_512))) {
		switch (QspiPtr->Config.ConnectionMode) {
			case XQSPIPS_CONNECTION_MODE_SINGLE:
				FCTIndex = FLASH_CFG_TBL_SINGLE_512_SP + StartIndex;
			break;
			case XQSPIPS_CONNECTION_MODE_PARALLEL:
				FCTIndex = FLASH_CFG_TBL_PARALLEL_512_SP + StartIndex;
			break;
			case XQSPIPS_CONNECTION_MODE_STACKED:
				FCTIndex = FLASH_CFG_TBL_STACKED_512_SP + StartIndex;
			break;
			default:
				FCTIndex = 0;
			break;
		}
	}
	/*
	  ** 1Gbit Single connection supported for Spansion.
	  ** The ConnectionMode will indicate stacked as this part has 2 SS
	  ** The device ID will indicate 512Mbit.
	  ** This configuration is handled as the above 512Mbit stacked configuration
	*/
	/* 1Gbit single, parallel and stacked supported for Micron */
	if (((FlashMake == MICRON_ID_BYTE0) &&
		(ReadBfrPtr[3] == MICRON_ID_BYTE2_1G)) ||
		((FlashMake == MACRONIX_ID_BYTE0) &&
		 (ReadBfrPtr[3] == MACRONIX_ID_BYTE2_1G))) {

		switch (QspiPtr->Config.ConnectionMode) {
			case XQSPIPS_CONNECTION_MODE_SINGLE:
				FCTIndex = FLASH_CFG_TBL_SINGLE_1GB_MC;
			break;
			case XQSPIPS_CONNECTION_MODE_PARALLEL:
				FCTIndex = FLASH_CFG_TBL_PARALLEL_1GB_MC;
			break;
			case XQSPIPS_CONNECTION_MODE_STACKED:
				FCTIndex = FLASH_CFG_TBL_STACKED_1GB_MC;
			break;
			default:
				FCTIndex = 0;
			break;
		}
	}
	debug("FlashID=0x%x 0x%x 0x%x\n\r", ReadBfrPtr[1], ReadBfrPtr[2],
		   ReadBfrPtr[3]);

	return READ_SUCCESS;
}


/*
  ** Function Name        : Read_Flash
  ** Function Description : Qspi Bus Interface (Read Flash)
  ** Function Parameter   : Address             : Read Data In Flash Address
  ** 						ByteCount           : Write Data Length
  ** 						Command             : Write Command
  ** 						WriteBfrPtr         : Write Data Address
  ** 						ReadBfrPtr          : Read Data Address
  ** Function Return      : WRITE_FAILED 		: Write Failed
  **                        WRITE_SUCCESS	    : Write Success
  ** Notes				  : None
*/
u8 Read_Flash(u32 Address, u32 ByteCount, u8 Command,
				u8 *WriteBfrPtr, u8 *ReadBfrPtr)
{
	u32 RealAddr;
	u32 RealByteCnt;
	u32 BankSel;
	u32 BufferIndex;
	u32 TotalByteCnt;
	u8 ShiftSize;

	/* Retain the actual byte count */
	TotalByteCnt = ByteCount;

	while(((signed long)(ByteCount)) > 0) {
		/*
		  ** Translate address based on type of connection
		  ** If stacked assert the slave select based on address
		*/
		RealAddr = GetRealAddr(Address);
		/* Select bank */
		if (Flash_Config_Table[FCTIndex].FlashDeviceSize > SIXTEENMB) {
			BankSel = RealAddr/SIXTEENMB;
			SendBankSelect(WriteBfrPtr, BankSel);
		}
		/*
		  ** If data to be read spans beyond the current bank, then
		  ** calculate RealByteCnt in current bank. Else
		  ** RealByteCnt is the same as ByteCount
		*/
		if ((Address & BANKMASK) != ((Address+ByteCount) & BANKMASK)) {
			RealByteCnt = (Address & BANKMASK) + SIXTEENMB - Address;
		}
		else {
			RealByteCnt = ByteCount;
		}
		/*
		  ** Setup the write command with the specified address
		  ** and data for the Flash
		*/
		WriteBfrPtr[COMMAND_OFFSET]   = Command;
		WriteBfrPtr[ADDRESS_1_OFFSET] = (u8)((RealAddr & 0xFF0000) >> 16);
		WriteBfrPtr[ADDRESS_2_OFFSET] = (u8)((RealAddr & 0xFF00) >> 8);
		WriteBfrPtr[ADDRESS_3_OFFSET] = (u8)(RealAddr & 0xFF);

		if ((Command == FAST_READ_CMD) || (Command == DUAL_READ_CMD) ||
		    (Command == QUAD_READ_CMD)) {
			RealByteCnt += DUMMY_SIZE;
		}
		/*
		  ** Send the read command to the Flash to read the specified number
		  ** of bytes from the Flash, send the read command and address and
		  ** receive the specified number of bytes of data in the data buffer
		*/
		if (WRITE_READ_SUCCESS != Qspi_Write_Read(WriteBfrPtr,
				&(ReadBfrPtr[TotalByteCnt - ByteCount]),
				RealByteCnt + OVERHEAD_SIZE)) {
			return READ_FAILED;
		}

		/* To discard the first 5 dummy bytes, shift the data in read buffer */
		if ((Command == FAST_READ_CMD) || (Command == DUAL_READ_CMD) ||
			    (Command == QUAD_READ_CMD)) {
			ShiftSize = OVERHEAD_SIZE + DUMMY_SIZE;
		}
		else {
			ShiftSize =  OVERHEAD_SIZE;
		}

		for (BufferIndex = (TotalByteCnt - ByteCount);
				BufferIndex < (TotalByteCnt - ByteCount) + RealByteCnt;
				BufferIndex++) {
			ReadBfrPtr[BufferIndex] = ReadBfrPtr[BufferIndex + ShiftSize];
		}
		/* Increase address to next bank */
		Address = (Address & BANKMASK) + SIXTEENMB;
		/* Decrease byte count by bytes already read */
		if ((Command == FAST_READ_CMD) || (Command == DUAL_READ_CMD) ||
		    (Command == QUAD_READ_CMD)) {
			ByteCount = ByteCount - (RealByteCnt - DUMMY_SIZE);
		}
		else {
			ByteCount = ByteCount - RealByteCnt;
		}

	}
	return READ_SUCCESS;
}


/*
  ** Function Name        : SendBankSelect
  ** Function Description : Qspi Bus Interface (Send Bank Select Flash)
  ** Function Parameter   : WriteBfrPtr         : Write Data Address
  ** 						BankSel             : Which Flash Bulk
  ** Function Return      : SEND_FAILED 		: Send Failed
  **                        SEND_SUCCESS	    : Send Success
  ** Notes				  : None
*/
u8 SendBankSelect(u8 *WriteBfrPtr, u32 BankSel)
{
	u8 WriteEnableCmd = { WRITE_ENABLE_CMD };

	/* Bank select commands for Micron and Spansion are different */
	if(FlashMake == MICRON_ID_BYTE0) {
		/*
		  ** For Micron command WREN should be sent first
		  ** except for some specific feature set
		*/
		Qspi_Write_Read(&WriteEnableCmd, NULL, sizeof(WriteEnableCmd));
		WriteBfrPtr[COMMAND_OFFSET]   = EXTADD_REG_WR;
		WriteBfrPtr[ADDRESS_1_OFFSET] = BankSel;
		/*
		  ** Send the Extended address register write command
		  ** written, no receive buffer required
		*/
		if (WRITE_READ_SUCCESS != \
				Qspi_Write_Read(WriteBfrPtr, NULL, BANK_SEL_SIZE)) {
			return SEND_FAILED;
		}

	}
	if (FlashMake == SPANSION_ID_BYTE0) {
		WriteBfrPtr[COMMAND_OFFSET]   = BANK_REG_WR;
		WriteBfrPtr[ADDRESS_1_OFFSET] = BankSel;
		/*
		  ** Send the Extended address register write command
		  ** written, no receive buffer required
		*/
		if (WRITE_READ_SUCCESS != \
				Qspi_Write_Read(WriteBfrPtr, NULL, BANK_SEL_SIZE)) {
			return SEND_FAILED;
		}
	}
	/* Winbond can be added here */
	return SEND_SUCCESS;
}


/*
  ** Function Name        : Erase_Bulk
  ** Function Description : Qspi Bus Interface (Erase Flash Bulk)
  ** Function Parameter   : WriteBfrPtr         : Write Data Address
  ** Function Return      : ERASE_FAILED 		: Write Failed
  **                        ERASE_SUCCESS	    : Write Success
  ** Notes				  : None
*/
u8 Erase_Bulk(u8 *WriteBfrPtr)
{
	u8 WriteEnableCmd = { WRITE_ENABLE_CMD };
	u8 ReadStatusCmd[] = { READ_STATUS_CMD, 0 };  /* Must send 2 bytes */
	u8 FlashStatus[2];

	/*
	  ** Send the write enable command to the Flash so that it can be
	  ** written to, this needs to be sent as a separate transfer
	  ** before the erase
    */
	if (WRITE_READ_SUCCESS != \
			Qspi_Write_Read( &WriteEnableCmd, NULL, sizeof(WriteEnableCmd))) {
		return ERASE_FAILED;
	}

	/*
	  ** Setup the bulk erase command
	*/
	WriteBfrPtr[COMMAND_OFFSET]   = BULK_ERASE_CMD;

	/*
	  ** Send the bulk erase command; no receive buffer is specified
	  ** since there is nothing to receive
	*/
	if (WRITE_READ_SUCCESS != \
			Qspi_Write_Read(WriteBfrPtr, NULL, BULK_ERASE_SIZE)) {
		return ERASE_FAILED;
	}

	/*
	  ** Wait for the erase command to the Flash to be completed
	*/
	while (1) {
		/*
		  ** Poll the status register of the device to determine
		  ** when it completes, by sending a read status command
		  ** and receiving the status byte
		*/
		if (WRITE_READ_SUCCESS != \
				Qspi_Write_Read(ReadStatusCmd, FlashStatus, sizeof(ReadStatusCmd))) {
			return ERASE_FAILED;
		}

		/*
		  ** If the status indicates the write is done, then stop
		  ** waiting; if a value of 0xFF in the status byte is
		  ** read from the device and this loop never exits, the
		  ** device slave select is possibly incorrect such that
		  ** the device status is not being read
		*/
		if ((FlashStatus[1] & 0x01) == 0) {
			break;
		}
	}
	return ERASE_SUCCESS;
}


/*
  ** Function Name        : Erase_Die
  ** Function Description : Qspi Bus Interface (Erase Flash Die)
  ** Function Parameter   : WriteBfrPtr         : Write Data Address
  ** Function Return      : ERASE_FAILED 		: Write Failed
  **                        ERASE_SUCCESS	    : Write Success
  ** Notes				  : None
*/
u8 Erase_Die(u8 *WriteBfrPtr)
{
	u8 WriteEnableCmd = { WRITE_ENABLE_CMD };
	u8 DieCnt;
	u8 ReadFlagSRCmd[] = {READ_FLAG_STS_CMD, 0 };
	u8 FlagStatus[2];

	for (DieCnt = 0; DieCnt < Flash_Config_Table[FCTIndex].NumDie; DieCnt++) {
		/*
		  ** Select bank - the lower of the 2 banks in each die
		  ** This is specific to Micron flash
		*/
		SendBankSelect(WriteBfrPtr, DieCnt*2);

		/*
		  ** Send the write enable command to the SEEPOM so that it can be
		  ** written to, this needs to be sent as a separate transfer
		  ** before the write
		*/
		if (WRITE_READ_SUCCESS != \
				Qspi_Write_Read(&WriteEnableCmd, NULL,sizeof(WriteEnableCmd))) {
			return ERASE_FAILED;
		}

		/*
		  ** Setup the write command with the specified address and data
		  ** for the Flash
		*/
		/*
		  ** This ensures 3B address is sent to flash even with address
		  ** greater than 128Mb.
		  ** The address is the start address of die - MSB bits will be
		  ** derived from bank select by the flash
		*/
		WriteBfrPtr[COMMAND_OFFSET]   = DIE_ERASE_CMD;
		WriteBfrPtr[ADDRESS_1_OFFSET] = 0x00;
		WriteBfrPtr[ADDRESS_2_OFFSET] = 0x00;
		WriteBfrPtr[ADDRESS_3_OFFSET] = 0x00;

		/*
		  ** Send the sector erase command and address; no receive buffer
		  ** is specified since there is nothing to receive
		*/
		if (WRITE_READ_SUCCESS != \
				Qspi_Write_Read(WriteBfrPtr, NULL, DIE_ERASE_SIZE)) {
			return ERASE_FAILED;
		}

		/* Wait for the sector erase command to the Flash to be completed */
		while (1) {
			/*
			  ** Poll the status register of the device to determine
			  ** when it completes, by sending a read status command
			  ** and receiving the status byte
			*/
			if (WRITE_READ_SUCCESS != \
					Qspi_Write_Read(ReadFlagSRCmd, FlagStatus, sizeof(ReadFlagSRCmd))) {
				return ERASE_FAILED;
			}

			/*
			  ** If the status indicates the write is done, then stop
			  ** waiting, if a value of 0xFF in the status byte is
			  ** read from the device and this loop never exits, the
			  ** device slave select is possibly incorrect such that
			  ** the device status is not being read
			*/
			if ((FlagStatus[1] & 0x80) == 0x80) {
				break;
			}
		}

	}
	return ERASE_SUCCESS;
}


/*
  ** Function Name        : GetRealAddr
  ** Function Description : Qspi Bus Interface (Get Flash Really Address)
  ** Function Parameter   : Address             : Address Which Is To Be Accessed
  ** Function Return      : u32         		: Flash Really Address
  ** Notes				  : None
*/
u32 GetRealAddr(u32 Address)
{
	u32 LqspiCr;
	u32 RealAddr;
	XQspiPs * QspiPtr = Get_Qspi_Point();

	switch (QspiPtr->Config.ConnectionMode) {
		case XQSPIPS_CONNECTION_MODE_SINGLE:
			RealAddr = Address;
		break;
		case XQSPIPS_CONNECTION_MODE_STACKED:
			/* Get the current LQSPI Config reg value */
			LqspiCr = XQspiPs_GetLqspiConfigReg(QspiPtr);
			/* Select lower or upper Flash based on sector address */
			if (Address & Flash_Config_Table[FCTIndex].FlashDeviceSize) {
				/* Set selection to U_PAGE */
				XQspiPs_SetLqspiConfigReg(QspiPtr,
						LqspiCr | XQSPIPS_LQSPI_CR_U_PAGE_MASK);
				/* Subtract first flash size when accessing second flash */
				RealAddr = Address &
						(~Flash_Config_Table[FCTIndex].FlashDeviceSize);
			}
			else {
				/* Set selection to L_PAGE */
				XQspiPs_SetLqspiConfigReg(QspiPtr,
						LqspiCr & (~XQSPIPS_LQSPI_CR_U_PAGE_MASK));
				RealAddr = Address;
			}
			/* Assert the Flash chip select. */
			XQspiPs_SetSlaveSelect(QspiPtr);
		break;
		case XQSPIPS_CONNECTION_MODE_PARALLEL:
			/*
			  ** The effective address in each flash is the actual
			  ** address / 2
			*/
			RealAddr = Address / 2;
		break;
		default:
			/* RealAddr wont be assigned in this case; */
		break;
	}
	return(RealAddr);
}


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/

