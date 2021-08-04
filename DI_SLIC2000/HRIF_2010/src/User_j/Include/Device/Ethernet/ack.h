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


#ifndef ACK_H_
#define ACK_H_


/*
  ** Include
*/


/*
  ** Define
*/
#define     ACK_SIZE            (10)

/*
  ** Function Prototype
*/
u8 Ack_Init(u8 Ip);
u8 Tcp_Ack(void);
u8 Tcp_Nack(void);
u8 * Read_Ack(void);
u8 * Read_Nack(void);


#endif /* ACK_H_ */


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
