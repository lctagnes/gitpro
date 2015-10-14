/************************************************************
 *Copyright (c) 2013-2015 深圳市赛柏达技术有限公司技术研发部
 *
 *FileName:		uart.h        
 *Writer:		smart-skynet
 *create Date:  2015/01/01
 *Rewriter:		kason
 *Rewrite Date:	2015/09/28
 *Impact:
 *
 *Main Content(Function Name、parameters、returns)
 *
 ************************************************************/

#ifndef UART_H
#define UART_H

# include <stdio.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <errno.h>
# include <fcntl.h>
# include <unistd.h>
# include <termios.h>
# include <stdlib.h>

//串口定义
//extern char *dev;      //串口号 /dev/ttymxc0  对应于串口1  全局变量声明

extern int openport(char *strDev); 												//打开串口
extern int setport(int fd, int baud, int databits, int stopbits, int parity);	//设置串口，波特率，数据位，停止位，校验
//extern int readport(int fd, char *buf, int maxLen, int maxwaittime);			//读数据，参数为串口，BUF，长度，超时时间
extern int readport(int fd, char *buf, int maxLen);								//读数据，参数为串口，BUF，长度
extern int writeport(int fd, char *buf, int len);  								//发送数据 防采集
extern void closeport(int fd);                                                  //关闭串口

#endif // UART_H
