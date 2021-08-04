/*
  ** @file           : app_layer_protocol.c
  ** @brief          : Application Layer Protocol Source File
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
#include "GlobalRef.h"
#include <stdlib.h>
#include <string.h>
#include "debug_config.h"
#include "return_val.h"
#include "ack.h"
#include "verify.h"
#include "operation.h"
#include "app_layer_protocol.h"


/*
  ** Function Name        : App_Layer_Protocol_Process
  ** Function Description : Application Layer Protocol Process Function
  ** Function Parameter   : Data                : Point Tcp Server Receiver Data Poniter
  **                        Length              : The Length Of Tcp Server Receiver Data
  ** Function Return      : OPER_FAILED 		: Operation(Process) Failed
  **                        OPER_SUCCESS	    : Operation(Process) Success
  ** Notes				  : None
*/
u8 App_Layer_Protocol_Process(void *Data,u32 Length)
{
	/* Check Parameter (Data) */
	if (NULL == Data) {
		debug("Data Is NULL In Application Layer Protocol Process In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	/* Check Parameter (Length) */
	if (MINLENGTH >= Length) {
		debug("Data Length Error In Application Layer Protocol Process In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	/* Verify Data */
	if (VERIFY_SUCCESS != Verify_Data(Data,Length)) {
		/* Tcp Server Send Nack */
		if (SEND_SUCCESS != Tcp_Nack()) {
			debug("Send Nack Failed In \
					%s Files,%d Line\n",__FILE__,__LINE__);
			return OPER_FAILED;
		}
		debug("Data Verify Error In Application Layer Protocol Process In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	/* Network Data Process */
	if (OPER_SUCCESS != Data_Operation(Data)) {
		debug("Data Operation Failed In Application Layer Protocol Process In %s Files,%d Line\n",__FILE__,__LINE__);
		return OPER_FAILED;
	}

	return OPER_SUCCESS;
}


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
