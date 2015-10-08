#ifndef WRITETHREAD_H
#define WRITETHREAD_H

#include <QThread>
#include <QSocketNotifier>
#include <QTimer>
#include <QDebug>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>


#include "common/log.h"
#include "common/sensor.h"
#include "common/define.h"


extern "C"
{
#include "uart.h"
}

using namespace std;

class WriteThread : public QThread
{
    Q_OBJECT

public:
    WriteThread(QObject *parent = 0);
    ~WriteThread();
    
    int Initial(char *strDev);      //初始化串口

    void Stop();

    void writeSerial(struCseSensor *wstrbuf);
    void ReadUart();               //安卓通信串口接受函数

public slots:
     void writeSerialSlot(struCseSensor *wstrBuf);

protected:
    void run();

private:
    volatile bool stopped;      //线程停止标志位

    int wFD;                    //串口设备号

    bool writeFlag;             //写串口标志位

};

#endif // UARTRECVTHREAD_H
