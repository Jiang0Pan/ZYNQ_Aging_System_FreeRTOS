/*
  ** @file           : app_layer_protocol.h
  ** @brief          : Application Layer Protocol Header File
  **
  ** @attention
  **
  ** Copyright (c) 2020 ChengDuHangke.
  ** All rights reserved.
  **
  ** This software is licensed by ChengDuHangke under Ultimate Liberty license
  **
*/


#ifndef APP_LAYER_PROTOCOL_H_
#define APP_LAYER_PROTOCOL_H_


/*
  ** Include
*/
#include "xil_types.h"

/*
  ** Define
*/
/* Command */
/* Beep Control (Receive) */
#define     R_BEEP_CONTROL      (0x2003)
/* Time Synchronous (Receive) */
#define     R_TIME_SYNC         (0xA050)
/* DPS Channel Config (Receive) */
#define     R_DPS_CONFIG        (0xA151)
/* Pattern File (Receive) */
#define     R_PATTERN           (0xA152)
/* Control Command (Receive) */
#define     R_CONTROL_COM       (0xA153)
/* Time Work Restart (Receive) */
#define		R_TIME_RESTART      (0xA154)
/* Pin Channel Config (Receive) */
#define     R_PIN_CONFIG        (0xA155)
/* Beep Control (Receive) */
#define     R_ALARM             (0xA156)   // NOT USED
/* Sample Data (ADC) (Transmit) */
#define     S_SAMPLE_DATA       (0xA157)
/* Sample Data (MAX9979) (Transmit) */
#define     S_MAX9979_DATA      (0xA158)   // NOT USED
/* Control Dps Channel Status (Receive) */
#define     R_DPS_CH_STATUS     (0xA159)
/* Temperature Channel Sample Data (Receive) */
#define     R_TEMP_SAMPLE_DATA  (0xA15A)   // NOT USED
/* Real_time Information (Transmit) */
#define     S_REALTIME_STATUS   (0xA251)
/* Pattern Error Information (Transmit) */
#define     S_PATTERN_ERR_INFO  (0xA252)
/* Pattern Status (Receive) */
#define     R_SET_PATTERN_STS   (0xA253)
/* Driver Board Version (Transmit) */
#define     S_DB_VERSION        (0xA254)
/* Driver Board Status (Transmit) */
#define     S_DB_PATTERN_STS    (0xA255)   // NOT USED
/* Pattern Dubug Status (Receive) */
#define     R_PATTERN_DSTS      (0xA256)
/* Patern Status (Transmit) */
#define     S_PATTERN_STS       (0xA257)
/* Pin Channel Debug Config (Receive) */
#define     R_PIN_DCONFIG       (0xA258)
/* Pin Channel Mask */
#define     R_PIN_MASK          (0xA259)
/* Pin Channel Mask */
#define     R_CYCLE_INFO        (0xA261)
/* Pin Channel Mask */
#define     CONFIG_FILE         (0xA262)
/* Max9972 Channel Debug (Receive) */
#define     R_9972_DEBUG        (0xACC0)
/* Read ZYNQ (Management Board) PS DDR Data (Transmit) */
#define     R_PS_DDR            (0xACCC)
/* Management Board (ZYNQ) Firmware (Receive) */
#define     R_MB_FIRMWARE       (0xACCD)
/* Driver Board (FPGA) Firmware (Receive) */
#define     R_DB_FIRMWARE       (0xACCE)
/* Management Board (ZYNQ) Firmware Version (Receive) */
#define     S_VERSION           (0xACCF)   //NOT USED
/* DDR Test (Receive) */
#define     S_DDR_TEST          (0xAF00)
/* DDR Test Status (Transmit) */
#define     S_DDR_TEST_STS      (0xAF01)

/* Frame Size */
/* Frame Length Size */
#define     LENGTH_SIZE         (0x4)
/* Frame Head Size */
#define     HEAD_SIZE           (0x6)
/* Frame Command Size */
#define     COM_SIZE            (0x2)
/* Frame Max Data Size */
#define     DATA_SIZE           (0x100008)
//#define     DATA_SIZE           (0x200008)
/* Frame Check Sum Size */
#define     CHECK_SUM_SIZE      (0x4)
/* Min Frame Length */
#define     MIN_LENGTH          (0xC)
/* Frame Data Aid Command Size */
#define     DATA_AID_LENGTH     (0x8)


/*
  ** Typedef Struct
*/
#pragma pack(push)
#pragma pack(1)

typedef struct FRAME {
	/* The Length Of Frame */
	u32 length;
	/* The Head Of Frame */
	u8  head[HEAD_SIZE];
	/* The Command Of Frame */
	u16 com;
	/* The Content Of Frame */
	u8  data[DATA_SIZE + CHECK_SUM_SIZE];
}Frame;

#pragma pack(pop)


/*
  ** Const Static Data
*/
const static u8 HEAD[HEAD_SIZE] = {0xAA,0x55,0x08,0x00,0x00,0x00};


/*
  ** Function Prototype
*/
u8 App_Layer_Protocol_Process(void *data,u32 length);


#endif /* APP_LAYER_PROTOCOL_H_ */


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
