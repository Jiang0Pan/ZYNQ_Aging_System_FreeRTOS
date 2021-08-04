/*
  ** @file           : insert_status.h
  ** @brief          : Dual Slots board insert status information source file
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
#include "return_val.h"
#include "debug_config.h"
#include "GlobalRef.h"
#include "io.h"
#include "eeprom.h"
#include "shell_port.h"
#include "insert_status.h"


u8 ReadDualSlotInfo(void)
{
	u8 tp = 0x0;

	if (SUCCESS != IO_Group_Get(Group0, &tp, dual_slot_info.PCA953_SlaveAddr_0, \
			dual_slot_info.Chan)) {
		return READ_FAILED;
	}
	dual_slot_info.HWVersion = (tp >> 4) & 0xf;
	dual_slot_info.PCBType   = (tp >> 0) & 0xf;

	if (SUCCESS != EEPROM_ReadBuff( 0, dual_slot_info.PN, 32, dual_slot_info.EEPROM_SlaveAddr,\
			dual_slot_info.Chan)) {
		return READ_FAILED;
	}
	CRC8(dual_slot_info.PN, 31);
	if (SUCCESS != EEPROM_ReadBuff(32, dual_slot_info.SN, 32, dual_slot_info.EEPROM_SlaveAddr, \
			dual_slot_info.Chan)) {
		return READ_FAILED;
	}
	CRC8(dual_slot_info.SN, 31);

	return READ_SUCCESS;
}

u8 ReadDualSlotStatus(Sample_Data * Sample)
{
	u8 tp = 0x0;

	if (SUCCESS != IO_Group_Get(Group0, &tp, dual_slot_info.PCA953_SlaveAddr_1, \
			dual_slot_info.Chan)) {
		return READ_FAILED;
	}
	else {
		Sample->BoardStatus = ~(tp & 0x0F);
	}

	return READ_SUCCESS;
}

/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/


