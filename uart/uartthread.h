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
const int RECV_DATA_LEN = 512;
const int RECV_CMD_LEN =5;

using namespace std;

class CUartThread : public QThread
{
    Q_OBJECT

public:
    CUartThread(QObject *parent = 0);
    ~CUartThread();
    
    int Initial(char *strDev, int nBaud, int nByte = 8, int nStop = 1, char cCheck = 'N');
    void Stop();
    void Start();
    void WriteUart( char *strBuf, int nLen);
    void ReadUart();                // 串口接收函数

protected:
    void run();

signals:
    void sSerialData(char *strBuf);            // serial port data singanls


private:
//    void ReadUart();                // 串口接收函数

private:
    bool m_bRun;                // 线程开始停止标志位
    int m_nFD;
    //int m_i;                   //for test
};

#endif // UARTRECVTHREAD_H
