/*
  ** @file           : return_val.h
  ** @brief          : Return Val Header File
  **
  ** @attention
  **
  ** Copyright (c) 2020 ChengDuHangke.
  ** All rights reserved.
  **
  ** This software is licensed by ChengDuHangke under Ultimate Liberty license
  **
*/


#ifndef RETURN_VAL_H_
#define RETURN_VAL_H_


/*
  ** Typedef Enum
*/
/* Init Status */
typedef enum INIT_Val {
	INIT_SUCCESS 		= 0x00,
	INIT_FAILED 		= 0x01,
}Init_val;
/* Config Status */
typedef enum CONFIG_Val {
	CONFIG_SUCCESS 		= 0x00,
	CONFIG_FAILED 		= 0x01,
}Config_val;
/* Set Status */
typedef enum SET_Val {
	SET_SUCCESS 		= 0x00,
	SET_FAILED 			= 0x01,
}Set_val;
/* Get Status */
typedef enum GET_Val {
	GET_SUCCESS 		= 0x00,
	GET_FAILED 			= 0x01,
}Get_val;
/* Create Status */
typedef enum CREATE_Val {
	CREATE_SUCCESS 		= 0x00,
	CREATE_FAILED 		= 0x01,
}Create_val;
/* Destroy Status */
typedef enum DESTROY_Val {
	DESTROY_SUCCESS     = 0X00,
	DESTROY_FAILED      = 0x01,
}Destroy_val;
/* Send Status */
typedef enum SEND_Val {
	SEND_SUCCESS        = 0x00,
	SEND_FAILED         = 0x01,
}Send_val;
/* Recv Status */
typedef enum RECV_Val {
	RECV_SUCCESS        = 0x00,
	RECV_FAILED         = 0x01,
}Recv_val;
/* Write Status */
typedef enum WRITE_Val {
	WRITE_SUCCESS       = 0x00,
	WRITE_FAILED        = 0x01,
}Write_val;
/* Write And Read Status */
typedef enum RW_Val {
	WRITE_READ_SUCCESS  = 0x00,
	WRITE_READ_FAILED   = 0x01,
}Rw_Val;
/* Read Status */
typedef enum READ_Val {
	READ_FAILED         = 0x00,
	READ_SUCCESS        = 0x01,
}Read_val;
/* Insert Status */
typedef enum INSERT_Val {
	INSERT_SUCCESS      = 0x00,
	INSERT_FAILED       = 0x01,
}Insert_val;
/* Delete Status */
typedef enum DELETE_Val {
	DELETE_SUCCESS      = 0x00,
	DELETE_FAILED       = 0x01,
}Delete_val;
/* Verify Status */
typedef enum VERITF_Val {
	VERIFY_SUCCESS      = 0x00,
	VERIFY_FAILED       = 0x01,
}Verify_val;
/* Operation Status */
typedef enum OPER_Val {
	OPER_SUCCESS        = 0x00,
	OPER_FAILED         = 0x01,
}Oper_val;
/* Clear Status */
typedef enum CLEAR_Val {
    CLEAR_SUCCESS        = 0x00,
	CLEAR_FAILED         = 0x01,
}Clear_val;
/* ERASE Status */
typedef enum ERASE_Val {
    ERASE_SUCCESS        = 0x00,
	ERASE_FAILED         = 0x01,
}Erase_val;
/* Wait Status */
typedef enum WAIT_Val {
    WAIT_SUCCESS        = 0x00,
	WAIT_FAILED         = 0x01,
}Wait_val;
/* Enable Status */
typedef enum ENABLE_Val {
    ENABLE_SUCCESS        = 0x00,
	ENABLE_FAILED         = 0x01,
}Enable_val;

#endif /* RETURN_VAL_H_ */


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
