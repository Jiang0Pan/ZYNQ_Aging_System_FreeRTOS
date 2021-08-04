/*
  ** @file           : tcp_server.c
  ** @brief          : TCP Server Source File
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
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "lwip/tcp_impl.h"
#include "xil_cache.h"
#include "main.h"
#include "debug_config.h"
#include "return_val.h"
#include "app_layer_protocol.h"
#include "tcp_server.h"
#include "fpga.h"


/*
  ** Define
*/
/* Tcp Server Port */
#define 	LOCAL_PORT 			(5000)
/* Tcp Server Boardcast Ip Address */
#define     BOARDCAST_ADDR  	(0xFF)
/* Tcp Server Invaild Ip Address */
#define     INVAILD_ADDR  	    (0x00)
/* */
#define 	THREAD_STACKSIZE    (1024)
#define     MAX_READ            (1460)
#define     MAX_LISTEN_COUNT    (2000)
#define     SET_SOCKET_TIMEOUT  (1000)
#define     BIND_TIMEOUT        (20)
#define     LISTEN_TIMEOUT      (20)
#define     ACCEPT_TIMEOUT      (20)

#define     FRAME_ADDRESS       (0x20000000)


/*
  ** Extern Function Declaration
*/
extern void lwip_init(void);


/*
  ** Static Global Variable
*/
volatile static  struct netif 		Server_Netif;
volatile static  u16_t 				Servet_Port = LOCAL_PORT;
/* Frame In DDR */
volatile static 	Frame 	*recv_buf = (volatile Frame *)FRAME_ADDRESS;
/* Socket */
static int listen_sock , connect_sock;
TaskHandle_t ack_t;

/*
  ** Static Function Declaration
*/
void Tcp_Server_Thread(void * P);
void Data_Process_Request(void * P);

/*
  ** Function Name        : Network_Thread
  ** Function Description : Ethernet Network Thread
  ** Function Parameter   : P                   : Thread Parameter(IP Address)
  ** Function Return      : void
  ** Notes				  : None
*/
void Network_Thread(void * P)
{
	/* Get IP Address */
	u8 ip = *(u8 *)P;

    struct netif *netif;

    /* IP Address Is 0xFF , Return */
    if ((BOARDCAST_ADDR == ip) || (INVAILD_ADDR == ip)) {
    	debug("IP Address Config Error In %s Files,%d Line\n",__FILE__,__LINE__);
    	return;
    }

    /* The MAC Address Of Board. This Should Be Unique Per Board */
    unsigned char mac_ethernet_address[] = { 0x00, 0x0a, 0x35, 0x00, 0x01, ip };

    ip_addr_t ipaddr, netmask, gw;

    netif = (struct netif *)&Server_Netif;
    /* Print All Application Headers */
    debug("\r\n\r\n");
    debug("-----LwIP Socket Mode Tcp Server Application ------\r\n");

    /* Initliaze IP Addresses To Be Used */
    IP4_ADDR(&ipaddr,  192, 168, 1, ip);
    IP4_ADDR(&netmask, 255, 255, 255,  0);
    IP4_ADDR(&gw,      192, 168, 1, 1);

    /* Print Out IP Settings Of The Board */
    Print_Ip_Settings(&ipaddr, &netmask, &gw);

    /* Add Network Interface To The Netif_list , And Set It As Default */
    if (!xemac_add(netif, &ipaddr, &netmask, &gw, \
    		mac_ethernet_address, XPAR_XEMACPS_0_BASEADDR)) {
		debug("Error adding N/W interface In %s Files,%d Line\n",__FILE__,__LINE__);
		return;
    }

    /* Set Netif Default */
    netif_set_default(netif);

    /* Specify That The Network If Is Up */
    netif_set_up(netif);

    /* Start Packet Receive Thread - Required For LwIP Operation */
	/* Thread Name */
    sys_thread_new("Xemacif_Input_Thread",\
			/* Thread Function */
    		(void(*)(void*))xemacif_input_thread,\
			/* Thread Parameter */
			netif,\
			/* Thread Stack Size */
            THREAD_STACKSIZE,
			/* Thread Priority */
            DEFAULT_THREAD_PRIO + 1);

    debug("\r\n");
    debug("%20s %6s %s\r\n", "Server", "Port", "Connect With..");
    debug("%20s %6s %s\r\n", "--------------------", "------", "--------------------");

    Print_Tcp_Server_Header();
    debug("\r\n");

	/* Thread Name */
    sys_thread_new("Tcp_Server_Thread", \
			/* Thread Function */
    		Tcp_Server_Thread,\
			/* Thread Parameter */
			NULL,\
			/* Thread Stack Size */
			THREAD_STACKSIZE*4,\
			/* Thread Priority */
			DEFAULT_THREAD_PRIO);

    xTaskCreate(Patten_ACK_Task,\
			/* Task Name */
			"Ack task",\
			/* Task Stack Size */
			THREAD_STACKSIZE,
			/* Task Parameter */
			NULL,\
			/* Task Priority */
			DEFAULT_THREAD_PRIO,\
			/* Task Handler */
			&ack_t);

    /* Delete Task Itself */
    vTaskDelete(NULL);

    return;
}


/*
  ** Function Name        : Print_Ip
  ** Function Description : Print Ip/NetMask/Gateway
  ** Function Parameter   : Msg                 : Message Head
  **                        Ip                  : The Value Of Ip/NetMask/Gateway
  ** Function Return      : Void
  ** Notes				  : Static
*/
void Print_Ip(char *msg, ip_addr_t *ip)
{
	debug(msg);
	debug("%d.%d.%d.%d\n\r", ip4_addr1(ip), ip4_addr2(ip),
			ip4_addr3(ip), ip4_addr4(ip));
}


/*
  ** Function Name        : Print_Ip_Settings
  ** Function Description : Print Ip/NetMask/Gateway Setting
  ** Function Parameter   : Ip                  : Ip Addr
  **                        Mask                : Netmask
  **                        Gateway             : Gateway
  ** Function Return      : Void
  ** Notes				  : Static
*/
void Print_Ip_Settings(ip_addr_t *ip, ip_addr_t *mask, ip_addr_t *gw)
{
	Print_Ip("Board IP: ", ip);
	Print_Ip("Netmask : ", mask);
	Print_Ip("Gateway : ", gw);
}


/*
  ** Function Name        : Print_Tcp_Server_Header
  ** Function Description : Print Tcp Server Header Information
  ** Function Parameter   : Void
  ** Function Return      : Void
  ** Notes				  : None
*/
void Print_Tcp_Server_Header(void)
{
    debug("%20s %6d %s\r\n", "Tcp Server",
    					Servet_Port,
						"$ Telnet <Board_IP> 5000");
}


/*
  ** Function Name        : Tcp_Server_Thread
  ** Function Description : Get Tcp Netif
  ** Function Parameter   : p 					: Thread Parameter
  ** Function Return      : Void
  ** Notes				  : None
*/
void Tcp_Server_Thread(void * P)
{
	/* Address Size */
	int size;
	/* Socket State */
	int listen_state, connect_state;
	/* Circulating Count */
	int times_count1, times_count2;
	/* Socket address */
	struct sockaddr_in address, remote;
	/* Continuance Flag */
	u8 flag = 0;

	while (1) {
		/* Lwip Interrelated Init */
		pbuf_init();
		netif_init();
		tcp_init();
		lwip_socket_init();

		/* Clear Address Data */
		memset(&address, 0, sizeof(address));

		/* Create Socket */
		do {
			vTaskDelay(1);
			listen_sock = lwip_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		} while ( -1 == listen_sock );

		/* Set Socket Attribute */
		address.sin_family = AF_INET;
		address.sin_port = htons(Servet_Port);
		address.sin_addr.s_addr = INADDR_ANY;

		/* Socket Bind */
		times_count1 = 0;
		do {
			vTaskDelay(1);
			times_count1++;
			if (BIND_TIMEOUT <= times_count1) {
				times_count2 = 0;
				debug("Listen Socket Bind Address Failed In %s Files,%d Line\n",__FILE__,__LINE__);
				do {
					vTaskDelay(1);
					flag = 1;
					times_count2++;
					if (BIND_TIMEOUT <= times_count2) {
						debug("Listen Socket Close In Bind Address Failed In %s Files,%d Line\n",__FILE__,__LINE__);
						break;
					}
					listen_state = close(listen_sock);
				} while ( 0 != listen_state );
				break;
			}
			listen_state = lwip_bind(listen_sock, (struct sockaddr *)&address, sizeof (address));
		} while ( 0 != listen_state );

		/* Restart Circulating */
		if (flag) {
			flag = 0;
			continue;
		}

		/* Socket Listen */
		times_count1 = 0;
		do {
			times_count1++;
			if (LISTEN_TIMEOUT <= times_count1) {
				times_count2 = 0;
				debug("Listen Socket Listen Failed In %s Files,%d Line\n",__FILE__,__LINE__);
				do {
					vTaskDelay(1);
					flag = 1;
					times_count2++;
					if (BIND_TIMEOUT <= times_count2) {
						debug("Listen Socket Close In Listen Failed In %s Files,%d Line\n",__FILE__,__LINE__);
						break;
					}
					listen_state = close(listen_sock);
				} while ( 0 != listen_state );
				break;
			}
			listen_state = lwip_listen(listen_sock, 1);
			vTaskDelay(1);
		} while ( 0 != listen_state );
		size = sizeof(remote);

		while (1) {
			/* Socket Accept */
			if ((connect_sock = lwip_accept(listen_sock, (struct sockaddr *)&remote, \
					(socklen_t *)&size)) > 0) {
				/* Thread Name */
				sys_thread_new("Data_Process_Request",
						/* Thread Function */
						Data_Process_Request,
						/* Thread Parameter */
						(void*)connect_sock,
						/* Thread Stack Size */
						THREAD_STACKSIZE,
						/* Thread Priority */
						DEFAULT_THREAD_PRIO + 1);
			}
			else if (-1 == connect_sock) {
				times_count2 = 0;
				times_count1 = 0;
				/* Close Listen Socket */
				do {
					times_count1++;
					if (ACCEPT_TIMEOUT <= times_count1) {
						debug("Listen Socket Close Failed In %s Files,%d Line\n",__FILE__,__LINE__);
						break;
					}
					listen_state = close(listen_sock);
					vTaskDelay(1);
				} while ( 0 != listen_state );
				/* Close Connect Socket */
				do {
					vTaskDelay(1);
					times_count2++;
					if (ACCEPT_TIMEOUT <= times_count2) {
						debug("Connect Socket Close Failed In %s Files,%d Line\n",__FILE__,__LINE__);
						break;
					}
					connect_state = close(connect_sock);
				} while ( 0 != connect_state );
				break;
			}
			else {

			}

		}
	}
}


/*·
  ** Function Name        : Data_Process_Request·
  ** Function Description : Data Process Request Thread·
  ** Function Parameter   : p 					: Thread Parameter·
  ** Function Return      : Void·
  ** Notes				  : None·
*/
void Data_Process_Request(void * p)
{
	int sd = (int)p;
	volatile u32 count = 0;
	int temp = 0;

	while (1) {
		count = 0;
		/* Read LENGTH_SIZE From Socket */
		temp = recv(sd, (char8 *)&recv_buf->length, LENGTH_SIZE, 0);
		/* Client Disconnection */
		if (0 >= temp) {
			break;
		}
		/* Read Other Data */
		if ((0  < temp) && (50000 > temp)) {
			while (count < recv_buf->length) {
				temp = recv(sd, (char8 *)(recv_buf->head) + count, MAX_READ ,0);
				if (0 >= temp) {
					break;
				}
			    count += temp;
			}
		}
		else {
			count = 0;
		}
		/* Data Valid */
		if (0 < count) {
			/* Application Layer Protocol Process */
			if (OPER_SUCCESS != App_Layer_Protocol_Process((void *)recv_buf,count)) {
				debug("Application Layer Protocol Process Failed In %s Files,%d Line\n",__FILE__,__LINE__);
			}
		}
		else {

		}
	}

	/* Close Connection */
	close(sd);
    /* Delete Task Itself */
	vTaskDelete(NULL);
}


/*
  ** Function Name        : Get_Socket
  ** Function Description : Get Socket Handle
  ** Function Parameter   : Void
  ** Function Return      : int                 : Socket Handle
  ** Notes				  : None
*/
int Tcp_Server_Send(const void * Data,u16 Length)
{
	if (Length == lwip_send(connect_sock,Data,Length,0)) {
		return SEND_SUCCESS;
	}
	else {
		return SEND_FAILED;
	}
}


/*
  ** (C) COPYRIGHT ChengDuHangke END OF FILE
*/
