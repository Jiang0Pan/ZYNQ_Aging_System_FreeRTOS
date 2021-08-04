/**
 * @file shell_port.c
 * @author Letter (NevermindZZT@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-22
 * 
 * @Copyright (c) 2019 Unicook
 * 
 */

#include "shell.h"
#include "uart1.h"
#include "stdarg.h"
#include "stdio.h"
#include "shell_port.h"



Shell shell;
char shellBuffer[512];


/**
 * @brief 閻€劍鍩泂hell閸愶拷
 * 
 * @param data 閺佺増宓�
 */
void UserShellWrite(char data)
{
	outbyte(data);
}


/**
 * @brief 閻€劍鍩泂hell鐠囷拷
 * 
 * @param data 閺佺増宓�
 * @return char 閻樿埖锟斤拷
 */
signed char UserShellRead(char *data)
{
	*data = XUartPs_RecvByte(STDIN_BASEADDRESS);
	return 0;
}

/**
 * @brief 閻€劍鍩泂hell閸掓繂顫愰崠锟�
 * 
 */
void UserShellInit(void)
{
    shell.write = UserShellWrite;
    shell.read  = UserShellRead;
    shellInit(&shell, shellBuffer, 512);
}


int shellprintf(const char *fmt, ...)
{
	int len;
	static char OutBuf[1024];

	va_list args;

	va_start(args, fmt);

	len = vsprintf(OutBuf, fmt, args);

	va_end(args);

	shellWriteEndLine(&shell, OutBuf, len);

	return len;

}

int shellprintln(const char *fmt, ...)
{
	int len;
	static char OutBuf[1024];

	va_list args;

	va_start(args, fmt);

	len = vsprintf(OutBuf, fmt, args);

	va_end(args);

	OutBuf[len] = '\r';
	OutBuf[len+1] = '\n';
	OutBuf[len+2] = '\0';
	shellWriteEndLine(&shell, OutBuf, len+2);

	return len;

}




