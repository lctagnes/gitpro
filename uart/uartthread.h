/************************************************************
 *Copyright (c) 2013-2015 深圳市赛柏达技术有限公司技术研发部
 *
 *FileName:		uartthread.h        
 *Writer:		smart-skynet
 *create Date:  2015/01/01
 *Rewriter:		kason
 *Rewrite Date:	2015/09/28
 *Impact:
 *
 *Main Content(Function Name、parameters、returns)
 *
 ************************************************************/

#ifndef UARTRECVTHREAD_H
#define UARTRECVTHREAD_H

#include <QThread>
#include <QSocketNotifier>
#include <QTimer>
#include <QDebug>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>


#include "common/log.h"

extern "C"
{
#include "uart.h"
}


//const int RECV_DATA_LEN = 6;
const int RECV_DATA_LEN = 512;		// 数据位长度
const int RECV_CMD_LEN =5;			// 命令位长度
const int STRUCSESENSOR_LEN = 136;  // 传感器数据长度
using namespace std;

class CUartThread : public QThread
{
    Q_OBJECT

public:
    CUartThread(QObject *parent = 0);
    ~CUartThread();

    void Stop();                    // 停止线程
    void Start();					// 开始线程

	// 初始化串口，默认8位数据位，1位停止位，无校验位
	int Initial(char *strDev, int nBaud, int nByte = 8, int nStop = 1, char cCheck = 'N');

    void WriteUart( char *strBuf, int nLen);    // 写串口
    void ReadUart();                            // 读串口

protected:
    void run();

signals:
    void sSerialData(char *strBuf); 	// 发送串口数据的信号

private:
    bool m_bRun;                		// 线程开始停止标志位

	int m_nFD;							// 串口文件描述符
};

class CUartThread2 : public QThread
{
    Q_OBJECT

public:
    CUartThread2(QObject *parent = 0);
    ~CUartThread2();

    void Stop();                    // 停止线程
    void Start();					// 开始线程

    // 初始化串口，默认8位数据位，1位停止位，无校验位
    int Initial(char *strDev, int nBaud, int nByte = 8, int nStop = 1, char cCheck = 'N');

    void WriteUart( char *strBuf, int nLen);    // 写串口
    void ReadUart();                            // 读串口

public slots:
     void writeSerialSlot(char *wstrBuf);

protected:
    void run();

signals:
    void sSerialData(char *strBuf); 	// 发送串口数据的信号

private:
    bool m_bRun;                		// 线程开始停止标志位
    bool w_run;                         // 写串口开始停止标志位
    int m_nFD;							// 串口文件描述符
};

#endif // UARTRECVTHREAD_H
