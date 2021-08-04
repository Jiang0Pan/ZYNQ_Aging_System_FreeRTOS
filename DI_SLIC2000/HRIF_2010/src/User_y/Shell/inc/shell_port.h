/**
 * @file shell_port.h
 * @author Letter (NevermindZZT@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-22
 * 
 * @Copyright (c) 2019 Unicook
 * 
 */

#ifndef __SHELL_PORT_H__
#define __SHELL_PORT_H__


#include "shell.h"

extern Shell shell;

void UserShellInit(void);

int shellprintf(const char *fmt, ...);

int shellprintln(const char *fmt, ...);



#endif
