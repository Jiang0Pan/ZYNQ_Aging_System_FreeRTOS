/*
  ** @file           : tcp_server.h
  ** @brief          : TCP Server Header File
  **
  ** @attention
  **
  ** Copyright (c) 2020 ChengDuHangke.
  ** All rights reserved.
  **
  ** This software is licensed by ChengDuHangke under Ultimate Liberty license
  **
*/

#ifndef TCP_SERVER_H_
#define TCP_SERVER_H_


/*
  ** Include
*/
#include "netif/xadapter.h"
#include "lwip/sys.h"
#include "lwip/sockets.h"
#include "lwipopts.h"


/*
  ** Function Prototype
*/
void Network_Thread(void *p);
void Print_Ip(char *msg, ip_addr_t *ip);
void Print_Ip_Settings(ip_addr_t *ip, ip_addr_t *mask, ip_addr_t *gw);
void Print_Tcp_Server_Header(void);
int Tcp_Server_Send(const void * Data,u16 Length);

#endif /* TCP_SERVER_H_ */


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
