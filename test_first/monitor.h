#ifndef MONITOR_H
#define MONITOR_H

#include <cstring>
#include <math.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "configfile.h"
#include "common/log.h"
#include "uart/uartthread.h"
#include "tcp/tcpclient.h"
#include "common/define.h"
#include "common/sensor.h"
#include "email.h"



const unsigned int CMD_LEN = 20;
const unsigned int AUTO_CONNECT_TIME = 60*10;
//const unsigned int SENDGSM_TIME = 30;

class CMonitor: public QObject
{
    Q_OBJECT

public:
    CMonitor(QObject *parent = 0);
    ~CMonitor();

protected:
    void timerEvent(QTimerEvent *event);

signals:
    void writeSerial(struCseSensor *wstrBuf);

private slots:
    void SerialDataSlot(char *strBuf);
    void ConnectedSlot();
    void GetServerIP();
    void RecvTCPMsgSlot(QByteArray &recvData);

private:
    // decode serial data
    void DecodeSerialData(char *strBuf);
    int GetFanSpeed(uint16 nFan);
    void IsAlarm(struCseSensor &seSerial, QString &strAlarm);

    void SendCMDToSerialPort();

    void SendTowSerialPort();

    void SendGSM(QString & strMsg);

    // decode server msg
    void DecodeMsg(QByteArray &recvData);

 //   void DecodeHeader(struCseHead * cseHead, QString &strMsg);
    void DecodeMonitor(struCseMsg * cseMsg);

    char *GetSystemTime();
    void GetLocalIP();   
    void TestSendMsg();

private:
    CCfgFile m_cfgFile;
    CUartThread *m_pThread;
    CUartThread *m_wpThread;

    CUartThread m_gsmThread;
    CTCPClient *m_pTcpClient1;
    CTCPClient *m_pTcpClient2;
    QString m_strTime;
    QString m_strTime1;
    QString m_strIP;
    QString strAlarm;
    QString qstr;
    struCseMsg * cseMsg;
    struCseSensor seSerial;
    QByteArray msg;

    unsigned int m_nLCDLast;
    int m_nTimerID;
    int wTimerID;

    int m_nTcpTimerID;
    int m_nGSMTimerID;
    bool m_bConnected1;
    bool m_bConnected2;

    unsigned int m_nAlarm;

    CSendEmail m_email;
};

#endif // MONITOR_H
