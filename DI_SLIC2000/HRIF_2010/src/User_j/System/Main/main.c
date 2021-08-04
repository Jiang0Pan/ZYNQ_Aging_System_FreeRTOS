/*
  ** @file           : main.c
  ** @brief          : Main Source File
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
#include <system_emio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "lwip/sys.h"
#include "netif/xadapter.h"
#include "xil_cache.h"
#include "debug_config.h"
#include "return_val.h"
#include "tcp_server.h"
#include "system_state.h"
#include "ack.h"
#include "verify.h"
#include "main.h"
#include "xil_cache_l.h"
#include "spi.h"
#include "iic.h"
#include "interrupt.h"
#include "uart1.h"
#include "fpga.h"
#include "SystemInfo.h"
#include "Shell_port.h"
#include "semphr.h"
#include "isl28023.h"
#include "qspi_bus.h"
#include "dps_info.h"


/*
 ** Define
*/
#define 	THREAD_STACKSIZE    (1024)
#define 	GPIO_DEVICE_ID  	XPAR_XGPIOPS_0_DEVICE_ID


/*
  ** Static Global Variable
*/
static  TaskHandle_t System_stat;
static  TaskHandle_t System_Led;
static  TaskHandle_t Debug_t;

/*
  ** Extern
*/
extern void lwip_init(void);
s32 Peripherals_Init(void);
extern XGpioPs Emio;


/*
  ** Function Prototype
*/
int Main_Thread(void * P);
static void System_Led_Task(void * pvParameters);


/*
  ** Function Name        : main
  ** Function Description : Main Function
  ** Function Parameter   : Void
  ** Function Return      :  0 					: Normal
  **                        -1      			: Emio Config Failed
  **                        -2      			: Ack Init Failed
  ** Notes				  : None
*/
int main(void)
{
	/* Enable Cache */
//	Xil_ICacheEnable();
//	Xil_DCacheEnable();
    /* Disable L2 Cache */
//	Xil_L2CacheDisable();

	Xil_ICacheDisable();
	Xil_DCacheDisable();

	/* Crc32 Table Init */
	Crc32_Table_Init();

	/* Create Thread "Main Thread" */
	/* Thread Name */
	sys_thread_new("Main_Thread",\
			/* Thread Function */
			(void(*)(void*))Main_Thread,\
			/* Thread Parameter */
			NULL,\
			/* Thread Stack Size */
			THREAD_STACKSIZE,\
			/* Thread Priority */
			DEFAULT_THREAD_PRIO);

	/* Start Task Scheduler */
	vTaskStartScheduler();
	for ( ; ; )
	{

	}

	return 0;
}

/*
  ** Function Name        : Main_Thread
  ** Function Description : Main Thread
  ** Function Parameter   : p 					: Thread Parameter(IP Address)
  ** Function Return      : Void
  ** Notes				  : None
*/
int Main_Thread(void * P)
{
	Peripherals_Init();

	/* Get Ipaddr */
	u8 ip = Get_Ipaddr(&Emio);

	/* Ack Init */
	if (INIT_SUCCESS != Ack_Init(ip)) {
		debug("Ack Init Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		return -2;
	}

    lwip_init();

    /* any thread using lwIP should be created using sys_thread_new */
	/* Thread Name */
    sys_thread_new("Network_Thread",\
			/* Thread Function */
    		Network_Thread,
			/* Thread Parameter */
			&ip,
			/* Thread Stack Size */
			THREAD_STACKSIZE,
			/* Thread Priority */
            DEFAULT_THREAD_PRIO);

    /* System State (LED) */
	/* Task Function */
    xTaskCreate(System_Led_Task,\
    		/* Task Name */
    		( const char * )"System Led Control Thread",\
			/* Task Stack Size */
			THREAD_STACKSIZE,
			/* Task Parameter */
			NULL,\
			/* Task Priority */
			DEFAULT_THREAD_PRIO - 1,\
			/* Task Handler */
			&System_Led);

    /* Collection Data */
	/* Task Function */
    xTaskCreate(System_State_Process_Task,\
    		/* Task Name */
    		( const char * )"System Collection Data Thread",\
			/* Task Stack Size */
			THREAD_STACKSIZE * 4,		//max 4096
			/* Task Parameter */
			NULL,\
			/* Task Priority */
			DEFAULT_THREAD_PRIO,\
			/* Task Handler */
			&System_stat);

    xTaskCreate(shellTask,\
      		/* Task Name */
      		( const char * )"shellTask",\
  			/* Task Stack Size */
  			THREAD_STACKSIZE * 4,
  			/* Task Parameter */
  			(void *)&shell,\
  			/* Task Priority */
  			DEFAULT_THREAD_PRIO - 2,\
  			/* Task Handler */
  			&Debug_t);

    /* Delete Task Itself */
    vTaskDelete(NULL);

    return 0;
}

/*
  ** Function Name        : System_Led_Function
  ** Function Description : System Led Control Function
  ** Function Parameter   : pvParameters 		: Thread Parameter
  ** Function Return      : Void
  ** Notes				  : None
*/
static void System_Led_Task(void *pvParameters)
{
	const TickType_t x1second = pdMS_TO_TICKS(500);

	for( ;; )
	{
		/* Delay For 1 Second. */
		vTaskDelay(x1second);
		/* Cpu0 Led On */
		if (SET_SUCCESS != Set_System_Cpu0_Led(&Emio,ON)) {
			debug("Set Cpu0 Led On Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		}
		/* Delay For 1 Second. */
		vTaskDelay(x1second);
		/* Cpu0 Led Off */
		if (SET_SUCCESS != Set_System_Cpu0_Led(&Emio,OFF)) {
			debug("Set Cpu0 Led Off Failed In %s Files,%d Line\n",__FILE__,__LINE__);
		}
	}
}

//锟斤拷锟斤拷锟绞硷拷锟�
s32 Peripherals_Init(void)
{
	Uartps_Init();
	Interrupt_Init();
	Setup_Intr_Exception();
	AXIHP_Init();						//HP锟叫断筹拷始锟斤拷
	List_Init();						//LIST锟叫断筹拷始锟斤拷
	Emio_Config(GPIO_DEVICE_ID, &Emio);
	Spi_Init();
	Qspi_Init();
	IIC_Init();
	BoardHW_Init();
	ReadSysteminfo();
	Dps_Init(DPS_BD0);
	Dpm_Init(DPS_BD0);
	/* Debug version no board 1 */
	// Dps_Init(DPS_BD1);
	// Dpm_Init(DPS_BD1);

	return SUCCESS;
}



/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
