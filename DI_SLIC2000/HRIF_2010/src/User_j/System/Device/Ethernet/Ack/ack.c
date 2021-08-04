/*
  ** @file           : ack.c
  ** @brief          : Tcp Server Receive Data Ack Source File
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
#include "tcp_server.h"
#include "verify.h"
#include "ack.h"


/*
  ** Define
*/
#define     IP_INVALID_0        (0x00)
#define     IP_INVALID_1        (0xFF)
#define     ACK_IP_0            (0)
#define     ACK_IP_1            (1)
#define     ACK_IP_2            (2)
#define     ACK_IP_3            (3)
#define     ACK_POS_0           (4)
#define     ACK_POS_1           (5)
#define     DATA_LENGTH         (6)
#define     IP_0                (0xC0)
#define     IP_1                (0xA8)
#define     IP_2                (0x00)
#define     ACK_DATA_0          (0xAA)
#define     ACK_DATA_1          (0x55)
#define     NACK_DATA_0         (0x00)
#define     NACK_DATA_1         (0x00)


/*
  ** Const Static Data
*/
static u8 ack[ACK_SIZE] = {0x00};
static u8 nack[ACK_SIZE] = {0x00};


/*
  ** Function Name        : Ack_Init
  ** Function Description : Tcp Server Receive Data Ack Init
  ** Function Parameter   : Ip                  : IP Address
  ** Function Return      : INIT_FAILED 		: Init Failed
  **                        INIT_SUCCESS	    : Init Success
  ** Notes				  : None
*/
u8 Ack_Init(u8 Ip)
{
	/* Check Ip Address */
	if ((IP_INVALID_0 == Ip) || (IP_INVALID_1 == Ip)) {
		debug("Ack Init Failed (IP Address Error)\n");
		return INIT_FAILED;
	}

	/* According To Ip Addr Set Ack And NAck */
	u8 count = 0;
	u32 check_sum_ack = 0;
	u32 check_sum_nack = 0;

	ack[ACK_IP_0] = IP_0;
	nack[ACK_IP_0] = IP_0;

	ack[ACK_IP_1] = IP_1;
	nack[ACK_IP_1] = IP_1;

	ack[ACK_IP_2] = IP_2;
	nack[ACK_IP_2] = IP_2;

	ack[ACK_IP_3] = Ip;
	nack[ACK_IP_3] = Ip;

	ack[ACK_IP_0] = 0xC0;
	nack[ACK_IP_0] = 0xC0;

	ack[ACK_POS_0] = ACK_DATA_0;
	ack[ACK_POS_1] = ACK_DATA_1;
	nack[ACK_POS_0] = NACK_DATA_0;
	nack[ACK_POS_1] = NACK_DATA_1;

	if (VERIFY_SUCCESS != Check_Sum(ack,DATA_LENGTH,&check_sum_ack)) {
		return INIT_FAILED;
	}
	if (VERIFY_SUCCESS != Check_Sum(nack,DATA_LENGTH,&check_sum_nack)) {
		return INIT_FAILED;
	}

	for (count = DATA_LENGTH ; count < ACK_SIZE ; count++) {
		ack[count] = (u8)(check_sum_ack >> ((count - DATA_LENGTH) * 8));
		nack[count] = (u8)(check_sum_nack >> ((count - DATA_LENGTH) * 8));
	}

	return INIT_SUCCESS;
}


/*
  ** Function Name        : Tcp_Ack
  ** Function Description : Tcp Server Tharsmit Ack
  ** Function Parameter   : Void
  ** Function Return      : SEND_FAILED 		: Send Failed
  **                        SEND_SUCCESS	    : Send Success
  ** Notes				  : None
*/
u8 Tcp_Ack(void)
{
	return Tcp_Server_Send(ack,ACK_SIZE);
}


/*
  ** Function Name        : Tcp_Nack
  ** Function Description : Tcp Server Tharsmit Nack
  ** Function Parameter   : Void
  ** Function Return      : SEND_FAILED 		: Send Failed
  **                        SEND_SUCCESS	    : Send Success
  ** Notes				  : None
*/
u8 Tcp_Nack(void)
{
	return Tcp_Server_Send(nack,ACK_SIZE);
}


/*
  ** Function Name        : Read_Ack
  ** Function Description : Read Ack Data
  ** Function Parameter   : Void
  ** Function Return      : u8 *                : Ack Data Address
  ** Notes				  : None
*/
u8 * Read_Ack(void)
{
	return ack;
}


/*
  ** Function Name        : Read_Nack
  ** Function Description : Read Nack Data
  ** Function Parameter   : Void
  ** Function Return      : u8 *                : Nack Data Address
  ** Notes				  : None
*/
u8 * Read_Nack(void)
{
	return nack;
}


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
