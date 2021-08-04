/*
  ** @file           : debug_config.h
  ** @brief          : Debug Config Header File
  **
  ** @attention
  **
  ** Copyright (c) 2020 ChengDuHangke.
  ** All rights reserved.
  **
  ** This software is licensed by ChengDuHangke under Ultimate Liberty license
  **
*/


#ifndef DEBUG_CONFIG_H_
#define DEBUG_CONFIG_H_


/*
  ** include
*/
#include "xil_printf.h"


/*
  ** define
*/

/* (DEBUG_CONFIG == 1) DEBUG OPEN , (DEBUG_CONFIG == 0) DEBUG CLOSE */
#define     DEBUG_CONFIG        (1)

#if DEBUG_CONFIG
	#define 	debug                xil_printf
#else
	#define debug
#endif


#endif /* DEBUG_CONFIG_H_ */


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
