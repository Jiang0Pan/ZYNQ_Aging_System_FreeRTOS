/*
  ** @file           : verify.c
  ** @brief          : Verify Data Source File
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
#include "app_layer_protocol.h"
#include "verify.h"


/*
  ** Define
*/


/*
  ** Static Global Variable
*/
static u32 crc_table[256] = {0x00};
static u32 check_data = 0;


/*
  ** Function Name        : Verify_Data
  ** Function Description : Verify Tcp Server Receive Data
  ** Function Parameter   : Data                : Point Receive Data Pointer
  **                        Length				: Receive Data Length
  ** Function Return      : VERIFY_SUCCESS 		: Verify Success
  **                        VERIFY_FAILED	    : Verify Failed
  ** Notes				  : None
*/
u8 Verify_Data(void *Data,u32 Length)
{
	if (NULL == Data) {
		return VERIFY_FAILED;
	}

	if (MIN_LENGTH > Length) {
		return VERIFY_FAILED;
	}

	/* Type Transform */
	Frame * Temp_Data = (Frame *)Data;

	/* Cheak Data Length */
	if ((Temp_Data->length) != Length) {
		debug("Verify Length Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		return VERIFY_FAILED;
	}

	/* Check Data Head */
	u8 count = 0;
	for (count = 0 ; count < HEAD_SIZE ; count++) {
		if (Temp_Data->head[count] != HEAD[count]) {
			debug("Verify Head Failed In %s Files,%d Line\n",__FILE__,__LINE__);
			return VERIFY_FAILED;
		}
	}

	if(R_PATTERN != Temp_Data->com) {
		/* Check Sum */
		u32 check_sum = 0;
		u32 check_temp = 0;

		check_sum = *(u32 *)&(Temp_Data->data[Length - LENGTH_SIZE - HEAD_SIZE -\
											  COM_SIZE]);

		if (VERIFY_SUCCESS != Check_Sum(Data,Length,&check_temp)) {
			debug("Verify Check Sum Failed In %s Files,%d Line\n",__FILE__,__LINE__);
			return VERIFY_FAILED;
		}

		if (check_sum != check_temp) {
			debug("Verify Check Sum Error In %s Files,%d Line\n",__FILE__,__LINE__);
			return VERIFY_FAILED;
		}
	}
	else {

	}

	return VERIFY_SUCCESS;
}


/*
  ** Function Name        : Check_Sum
  ** Function Description : Check Sun Tcp Server Receive Data
  ** Function Parameter   : Data                : Point Receive Data Pointer
  **                        Length				: Receive Data Length
  ** Function Return      : u32                 : The Val Of Check Sum
  ** Notes				  : None
*/
u8 Check_Sum(void *Data,u32 Length,u32 *CheckSum)
{
	if (NULL == Data) {
		return VERIFY_FAILED;
	}

	if (0 == Length) {
		return VERIFY_FAILED;
	}

	u32 check_sum = 0x00;
	*CheckSum = 0x00;
	u32 count = 0x00;
	u8 * temp_data = (u8 *)Data;

	/* Check Sum */
	for (count = 0; count < Length ; count++) {
		check_sum += temp_data[count];
	}
	/* Get Check Sum */
	*CheckSum = check_sum;

	return VERIFY_SUCCESS;
}


/*
  ** Function Name        : Crc32_Table_Init
  ** Function Description : Init Crc32 Table
  ** Function Parameter   : void
  ** Function Return      : Void
  ** Notes				  : None
*/
void Crc32_Table_Init(void)
{
	u32 c;
	u16 i, j;
	/* Inie Check Crc Table (Search Table Way) */
	for (i = 0 ;i < 256; i++) {
		c = (u32)i;
		for (j = 0 ;j < 8 ;j++) {
			if ((c & 1) == 1) {
				c = 0xEDB88320 ^ (c >> 1);
			}
			else {
				c = c >> 1;
			}
		}
		crc_table[i] = c;
	}
}


/*
  ** Function Name        : Get_Crc32_Val
  ** Function Description : Get Crc32 Value
  ** Function Parameter   : Crc_Val             : CRC32 Value (Last Value)
  ** 						Data                : Need Check Data Address
  **                        Length				: Need Check Data Length
  ** Function Return      : u32                 : CRC32 Value (Current)
  ** Notes				  : None
*/
u32 Get_Crc32_Val(u32 Crc_Val, u8 * Data, u32 Length)
{
	u32 i = 0;
	/* Check Crc (Search Table Way) */
	for (i = 0; i < Length; i++) {
		Crc_Val = crc_table[(Crc_Val ^ Data[i]) & 0xFF] \
				^ (Crc_Val >> 8);
	}

	return Crc_Val;
}


/*
  ** Function Name        : Get_Data_Crc32_Val
  ** Function Description : Get Fram Data Crc32 Value
  ** Function Parameter   : Viod
  ** Function Return      : u32                 : Frame Data CRC32 Value (Current)
  ** Notes				  : None
*/
u32 Get_Data_Crc32_Val(void)
{
	return check_data;
}


/*
  ** Function Name        : Get_Data_Crc32_Val
  ** Function Description : Get Fram Data Crc32 Value
  ** Function Parameter   : Viod
  ** Function Return      : u32                 : Frame Data CRC32 Value (Current)
  ** Notes				  : None
*/



/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
