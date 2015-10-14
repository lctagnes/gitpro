/************************************************************
 *Copyright (c) 2013-2015 深圳市赛柏达技术有限公司技术研发部
 *
 *FileName:		monitor.cpp
 *Writer:		smart-skynet
 *create Date:  2015/01/01
 *Rewriter:		kason
 *Rewrite Date:	2015/10/08
 *Impact:
 *
 *Main Content(Function Name、parameters、returns)
 *
 ************************************************************/

#include "monitor.h"

CMonitor::CMonitor(QObject *parent)
    : QObject(parent)
{
    CLOG::Log("Application start .");
    //    QString strSub = "Door opened . \r\n";
    //    SendGSM(strSub);
    m_nGSMTimerID = startTimer(m_cfgFile.m_nSendGSM_TIME *60* 1000);    // 60 * 1000 ms

    m_pThread = new CUartThread;                            // uart1与采集板通信串口线程
    CommunicateToUpperComputerpThread = new CUartThread2;   // uart2与上位机通信串口线程

    m_nLCDLast = 0;
    m_bConnected1 = false;
    m_bConnected2 = false;

    //    GetLocalIP();

    char * strDev = m_cfgFile.m_strDev.toLatin1().data();
    int nBaud = m_cfgFile.m_nBaud;

    char * strDev_for_upper_computer = m_cfgFile.strDev_2.toLatin1().data();
    int baud_2 = m_cfgFile.baud_2;

    int nFD = m_pThread->Initial(strDev, nBaud);
    int uart2_fd = CommunicateToUpperComputerpThread->Initial(strDev_for_upper_computer, baud_2);

    //if(nFD>0)
   // {
        printf("Open Serial Port Success .\r\n");
        CLOG::Log("Open Serial Port Success .");

        // start recv thread
        m_pThread->Start();
       // m_nTimerID = startTimer(m_cfgFile.m_nSampTime * 1000);
    //}
    if(-1 == nFD)
    {
        printf("Open Serial Port Failed , Exit .\r\n");
        CLOG::Log("Open Serial Port Failed , Exit .");
        //  exit(0);
    }
    else
    {
        printf("Set Serial Port Failed , Exit .\r\n");
        CLOG::Log("Set Serial Port Failed , Exit .");
        // exit(0);
    }

    if(uart2_fd > 0)
    {
        printf("Open Serial 2 Port Success .\r\n");
        CLOG::Log("Open Serial 2 Port Success .");

        // start recv thread

        m_pThread->Start();
    }
    if(-1 == uart2_fd)
    {
        printf("Open Serial 2 Port Failed , Exit .\r\n");
        CLOG::Log("Open Serial 2 Port Failed , Exit .");
        //  exit(0);
    }
    else
    {
        printf("Set Serial 2 Port Failed , Exit .\r\n");
        CLOG::Log("Set Serial 2 Port Failed , Exit .");
        // exit(0);
    }

    m_pTcpClient1 = new CTCPClient;
    m_pTcpClient2 = new CTCPClient;

    m_nTcpTimerID = startTimer(60*30* 1000);
    m_pTcpClient1->ConnectToServer1(m_cfgFile.m_strIP, m_cfgFile.m_nPort);

    // initial all connect
    connect(m_pTcpClient1, SIGNAL(sConnected()), this, SLOT(GetServerIP()));
    connect(m_pTcpClient2, SIGNAL(sConnected()), this, SLOT(ConnectedSlot()));

    connect(m_pThread, SIGNAL(sSerialData(char *)), this, SLOT(SerialDataSlot(char *)));
    connect(this, SIGNAL(writeSerial(char *)), CommunicateToUpperComputerpThread, SLOT(writeSerialSlot(char *)));
}

CMonitor::~CMonitor()
{
    if(0 != m_nTimerID)
    {
        killTimer(m_nTimerID);
    }

    if(0 != m_nTcpTimerID)
    {
        killTimer(m_nTcpTimerID);
    }
    if(0 != m_nGSMTimerID)
    {
        killTimer(m_nGSMTimerID);
    }

    if(NULL != m_pTcpClient1)
    {
        m_pTcpClient1->disconnect();
        delete m_pTcpClient1;
        m_pTcpClient1 = NULL;
    }

    if(NULL != m_pTcpClient2)
    {
        m_pTcpClient2->disconnect();
        delete m_pTcpClient2;
        m_pTcpClient2 = NULL;
    }

    if(NULL != m_pThread)
    {
        m_pThread->Stop();

        // sleep 50 ms, wait thread exit
        sleep(50);

        delete m_pThread;
        m_pThread = NULL;
    }

    if(NULL != CommunicateToUpperComputerpThread)
    {
        CommunicateToUpperComputerpThread->Stop();

        // sleep 50 ms, wait thread exit
        sleep(50);

        delete CommunicateToUpperComputerpThread;
        CommunicateToUpperComputerpThread = NULL;
    }
}


void CMonitor::SerialDataSlot(char *strBuf)
{
    CLOG::Log("SerialDataSlot .");
    DecodeSerialData(strBuf);
}

char* CMonitor::GetSystemTime()          // 获取系统当前的时间，以"yyyy-MM-dd hh:mm:ss"格式保存到m_strTime
{
    QDateTime time = QDateTime::currentDateTime();
    m_strTime = time.toString("yyyy-MM-dd hh:mm:ss");
    return m_strTime.toLatin1().data();
}

void CMonitor::GetLocalIP()
{
    QList<QHostAddress> AddressList = QNetworkInterface::allAddresses();

    foreach(QHostAddress address, AddressList)
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress::Null && address != QHostAddress::LocalHost)
        {
            if (address.toString().contains("127.0."))
            {
                continue;
            }

            m_strIP = address.toString();
            break;
        }
    }
}

void CMonitor::timerEvent(QTimerEvent *event)   // 事件处理器，接受定时器事件
{
    if(m_nTimerID == event->timerId())          // 构造一个定时器标识符为timerID的定时器事件对象
    {
        SendCMDToSerialPort();
    }
    if(m_nGSMTimerID == event->timerId())       // 构造一个定时器标识符为imerID的定时器事件对象
    {
        IsAlarm(seSerial, strAlarm);
        if(CSE_ALARM_STATUS == seSerial.status)
        {

            //            QString strSub = "Door opened . \r\n";
            //            SendGSM(strSub);
            CLOG::Log("Send alarm info to messages .");
            SendGSM(strAlarm);

        }
    }
    if(m_nTcpTimerID == event->timerId())
    {

        // reconnect to server every 60 s
        qDebug()<<"reconnect to server 1979";

                if(NULL != m_pTcpClient2)
                {
                    m_pTcpClient2->disconnect();
                    delete m_pTcpClient2;
                    m_pTcpClient2 = NULL;
                }
                m_pTcpClient2 = new CTCPClient;

                m_pTcpClient2->ConnectToServer2(m_cfgFile.m_strIP, 1979);


    }

}

void CMonitor::SendCMDToSerialPort()        // 向串口发指令
{
    char szCMD[CMD_LEN] = {0x1B, 0X10, 0X04, 0X00, 0X00, 0X06, 0X1F, 0X8F};

    m_pThread->WriteUart(szCMD, 8);

    CLOG::Log("send read cmd .");
    QString strCMD("%1:%2:%3:%4:%5:%6:%7:%8\r\n");
    strCMD = strCMD.arg(szCMD[0]).arg(szCMD[1]).arg(szCMD[2]).arg(szCMD[3]).arg(szCMD[4]).arg(szCMD[5]).arg(szCMD[6]).arg(szCMD[7]);
    CLOG::Log(strCMD);

    char szlog[64];
    memset(szlog, 0, 64);

    printf("send read uart cmd %x:%x:%x:%x:%x \r\n", szCMD[0], szCMD[1], szCMD[2], szCMD[6], szCMD[7]);
    CLOG::Log("Send read cmd .");

    sleep(10);
    m_pThread->ReadUart();
}

void CMonitor::DecodeSerialData(char *strBuf)
{

    CLOG::Log("DecodeSerialData.");

    Sensor_Data sensor;
    memset(&sensor, 0, sizeof(Sensor_Data));
    memcpy(&sensor, strBuf, sizeof(Sensor_Data));

    //struCseSensor seSerial;
    memset(&seSerial, 0, sizeof(struCseSensor));

    seSerial.acc = sensor.Door0_status;

    seSerial.env_hdy_val = (unsigned int)sensor.am2301[0]-1.0;
    if(seSerial.env_hdy_val > m_cfgFile.m_nMaxENVHDY)
    {
        seSerial.env_hdy = 0;
    }
    else
    {
        seSerial.env_hdy = 1;
    }

    seSerial.env_tmp_val = (float)sensor.am2301[1] - 1.0;
    if(seSerial.env_tmp_val > m_cfgFile.m_nMaxENVTMP)
    {
        seSerial.env_tmp = 0;
    }
    else
    {
        seSerial.env_tmp = 1;
    }

    //  printf("fil_val = %d \r\n", (unsigned int)(sensor.light_intensity[1]));
    seSerial.fil_val = (unsigned int)(((float)sensor.light_intensity[1]/MAX_FIL)*m_cfgFile.m_nMaxfil*100);

    if(seSerial.fil_val > 100)
    {
        seSerial.fil_val = 100;
    }
    else
    {
        seSerial.fil_val = 100 - seSerial.fil_val;
    }

    if(seSerial.fil_val > m_cfgFile.m_nMaxFIL)
    {
        seSerial.fil = 0;
    }
    else
    {
        seSerial.fil = 1;
    }

    seSerial.fld = sensor.Liquid_status;
    seSerial.inf = sensor.Ir_status;

    //  printf("lcd_val = %d \r\n", (unsigned int)(sensor.light_intensity[0]));
    seSerial.lcd_val = (unsigned int)(((float)sensor.light_intensity[0]/MAX_IDL)*m_cfgFile.m_nMaxIDL*100);
    if(seSerial.lcd_val > 100)
    {
        seSerial.lcd_val = 100;
    }

    if(seSerial.lcd_val < m_cfgFile.m_nMinLCDIDL)
    {
        seSerial.lcd = 0;
        seSerial.bck = 0;
    }
    else
    {
        seSerial.lcd = 1;
        seSerial.bck = 1;
    }

    // 0: lcd temp value
    seSerial.lcd_tmp_val = sensor.temp[0];
    if(seSerial.lcd_tmp_val > m_cfgFile.m_nMaxLCDTMP)
    {
        seSerial.lcd_tmp = 0;
    }
    else
    {
        seSerial.lcd_tmp = 1;
    }

    seSerial.shk = sensor.Shock_status;
    seSerial.smk = sensor.Smoke_status;


    // 屏幕变化，若两次变化小于1、认为没变化
    int lcd = abs(int (seSerial.lcd_val - m_nLCDLast));
    if((lcd < 1) && (0 != m_nLCDLast))
    {
        seSerial.scr = 1;
    }
    else
    {
        seSerial.scr = 0;
    }

    m_nLCDLast = seSerial.lcd_val;

    //  int fan1 = GetFanSpeed(sensor.fan_speed[15]);

    seSerial.fan_speed01 = GetFanSpeed(sensor.fan_speed[0]);
    seSerial.fan_speed02 = GetFanSpeed(sensor.fan_speed[1]);
    seSerial.fan_speed03 = GetFanSpeed(sensor.fan_speed[2]);
    seSerial.fan_speed04 = GetFanSpeed(sensor.fan_speed[3]);
    seSerial.fan_speed05 = GetFanSpeed(sensor.fan_speed[4]);
    seSerial.fan_speed06 = GetFanSpeed(sensor.fan_speed[5]);
    seSerial.fan_speed07 = GetFanSpeed(sensor.fan_speed[6]);
    seSerial.fan_speed08 = GetFanSpeed(sensor.fan_speed[7]);
    seSerial.fan_speed09 = GetFanSpeed(sensor.fan_speed[8]);
    seSerial.fan_speed10 = GetFanSpeed(sensor.fan_speed[9]);
    seSerial.fan_speed11 = GetFanSpeed(sensor.fan_speed[10]);
    seSerial.fan_speed12 = GetFanSpeed(sensor.fan_speed[11]);
    seSerial.fan_speed13 = GetFanSpeed(sensor.fan_speed[12]);
    seSerial.fan_speed14 = GetFanSpeed(sensor.fan_speed[13]);
    seSerial.fan_speed15 = GetFanSpeed(sensor.fan_speed[14]);
    seSerial.fan_speed16 = GetFanSpeed(sensor.fan_speed[15]);

    seSerial.status = CSE_NORMAL_STATUS;

    //向与安卓设备通信串口发信号
    //TestSendMsg ();
    char wSerial[STRUCSESENSOR_LEN] = {0};
    memset(&wSerial, 0, sizeof(wSerial));
    memcpy(&wSerial, &seSerial, sizeof(wSerial));

    emit writeSerial(wSerial);

    // QString strAlarm;
    // IsAlarm(seSerial, strAlarm);

    // GetSystemTime();

    // QString strLog;
    // strLog.arg(1).arg(2) = seSerial.acc

    // send sensor to monitor center
    if(true == m_bConnected2)
    {
        // cse msg
        struCseMsg seMsg;
        memset(&seMsg, 0, sizeof(struCseMsg));

        seMsg.msg_id = CSE_SENSOR_REQ_MESSAGE;
        seMsg.msg_len = sizeof(struCseSensor);
        strcpy(seMsg.prd_id, m_cfgFile.m_strPrdID.toLatin1().data());

        qDebug()<<"m_cfgFile.m_strPrdID="<< m_cfgFile.m_strPrdID.toLatin1().data();
        qDebug()<<"seMsg.prd_id="<< seMsg.prd_id;
        memcpy(seMsg.msg, &seSerial, seMsg.msg_len);

        // send msg
        QByteArray msg;
        msg.resize(sizeof(struCseMsg));
        memcpy(msg.data(), &seMsg, sizeof(struCseMsg));

        m_pTcpClient2->SendToServer(msg.data(), sizeof(struCseMsg));
        QString strLg = QString( "%1,%2,%3,%4")
                .arg(seMsg.msg_id).arg(seMsg.msg_len).arg(seMsg.prd_id).
                arg(seMsg.msg);
        CLOG::Log(strLg.toLatin1().data());
        QString strLog = QString( "%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16,%17")
                .arg(seSerial.acc).arg(seSerial.bck).arg(seSerial.env_hdy).
                arg(seSerial.env_hdy_val).arg(seSerial.env_tmp).arg(seSerial.env_tmp_val).
                arg(seSerial.fil).arg(seSerial.fil_val).arg(seSerial.fld).
                arg(seSerial.inf).arg(seSerial.lcd).arg(seSerial.lcd_val).
                arg(seSerial.lcd_tmp).arg(seSerial.lcd_tmp_val).arg(seSerial.scr).
                arg(seSerial.shk).arg(seSerial.smk);
        CLOG::Log(strLog.toLatin1().data());

        qDebug()<<"send acc="<<seSerial.acc;
        qDebug()<<"send bck="<<seSerial.bck;
        qDebug()<<"send env_hdy="<<seSerial.env_hdy;
        qDebug()<<"send env_hdy_val="<< seSerial.env_hdy_val;
        qDebug()<<"send env_tmp="<< seSerial.env_tmp;
        qDebug()<<"send env_tmp_val="<<seSerial.env_tmp_val;
        qDebug()<<"send fil="<<seSerial.fil;
        qDebug()<<"send fil_val="<<seSerial.fil_val;
        qDebug()<<"send fld="<< seSerial.fld;
        qDebug()<<"send inf="<<seSerial.inf;
        qDebug()<<"send lcd="<<seSerial.lcd;
        qDebug()<<"send lcd_val="<<seSerial.lcd_val;
        qDebug()<<"send lcd_tmp="<<seSerial.lcd_tmp;
        qDebug()<<"send lcd_tmp_val="<<seSerial.lcd_tmp_val;
        qDebug()<<"send scr="<<seSerial.scr;
        qDebug()<<"send shk="<<seSerial.shk;
        qDebug()<<"send smk="<<seSerial.smk;
        qDebug()<<"send fan_speed01="<<seSerial.fan_speed01;
        qDebug()<<"send fan_speed02="<<seSerial.fan_speed02;
        qDebug()<<"send fan_speed03="<<seSerial.fan_speed03;
        qDebug()<<"send fan_speed04="<<seSerial.fan_speed04;
        qDebug()<<"send fan_speed05="<< seSerial.fan_speed05;
        qDebug()<<"send fan_speed06="<<seSerial.fan_speed06;
        qDebug()<<"send fan_speed07="<<seSerial.fan_speed07;
        qDebug()<<"send fan_speed08="<<seSerial.fan_speed08;
        qDebug()<<"send fan_speed09="<<seSerial.fan_speed09;
        qDebug()<<"send fan_speed10="<<seSerial.fan_speed10;
        qDebug()<<"send fan_speed11="<<seSerial.fan_speed11;
        qDebug()<<"send fan_speed12="<< seSerial.fan_speed12;
        qDebug()<<"send fan_speed13="<< seSerial.fan_speed13;
        qDebug()<<"send fan_speed14="<<seSerial.fan_speed14;
        qDebug()<<"send fan_speed15="<<seSerial.fan_speed15;
        qDebug()<<"send fan_speed16="<< seSerial.fan_speed16;
        qDebug()<<"send status="<<seSerial.status;
    }


}

void CMonitor::SendGSM(QString & strMsg)
{
    if(1 == m_cfgFile.m_nGsm)
    {
        char * strDev = m_cfgFile.m_strGsmDev.toLatin1().data();
        int nFD = m_gsmThread.Initial(strDev, m_cfgFile.m_nGsmBaud);

        if(nFD > 0)
        {
            printf("Open Gsm Port Success .\n");
            CLOG::Log("Open Gsm Port Success .");

            char *strStart = "at\r";
            char *strCsCs = "at+cscs=gsm\x0D";
            char *strCmgf = "at+cmgf=1\x0D";
            char strCmgs[30];
            QString strMegs = "at+cmgs=";
            m_cfgFile.m_strTel+= "\x0D";
            strMegs += m_cfgFile.m_strTel;
            strcpy(strCmgs, strMegs.toLatin1().data());
            strMsg += " \x1A";
            char *strData = strMsg.toLatin1().data();
            m_gsmThread.WriteUart(strStart, strlen(strStart));
            sleep(1);

            m_gsmThread.WriteUart(strCsCs, strlen(strCsCs));
            sleep(1);

            m_gsmThread.WriteUart(strCmgf, strlen(strCmgf));
            sleep(1);

            m_gsmThread.WriteUart(strCmgs, strlen(strCmgs));
            sleep(1);

            m_gsmThread.WriteUart(strData, strlen(strData));
            sleep(1);
        }

        else if(-1 == nFD)
        {
            printf("Open Gsm Port Failed .\n");
            CLOG::Log("Open Gsm Port Failed .");
        }
        else
        {
            printf("Set Gsm Port Failed .\n");
            CLOG::Log("Set Gsm Port Failed .");
        }
    }

}

int CMonitor::GetFanSpeed(uint16 nFan)
{
    int nSpeed = 0;

    //  printf("nFan = %d\r\n", nFan);

    QString str = QString::number(nFan, 10);
    bool ok;
    nSpeed = str.toInt(&ok, 16);       // hex == 255, ok == true

    //qDebug()<<"before nSpeed ="<< nSpeed;

    if((2 == m_cfgFile.m_nFanType) || (3 == m_cfgFile.m_nFanType))
    {
        nSpeed = nSpeed * 30;
    }
    else
    {
        nSpeed = nSpeed * 20;
    }

    //qDebug()<<"after nSpeed ="<< nSpeed;

    if(nSpeed > m_cfgFile.m_nFanSpeed)
    {
        nSpeed = m_cfgFile.m_nFanSpeed;
    }

    return nSpeed;
}

void CMonitor::IsAlarm(struCseSensor &seSerial, QString &strAlarm)
{
    QString strTemp("%1\r\n");

    if(seSerial.acc == 1)
    {
        seSerial.status = CSE_ALARM_STATUS;
        strAlarm += "Door opened . \r\n";
    }

    if(seSerial.bck == 0)
    {
        seSerial.status = CSE_ALARM_STATUS;
        strAlarm += "Backlight fault . \r\n";
    }

    if(seSerial.env_hdy == 0)
    {
        seSerial.status = CSE_ALARM_STATUS;
        strAlarm += "Env HDY Alarm: ";
        strTemp.arg(seSerial.env_hdy_val);
        strAlarm += strTemp;
    }

    if(seSerial.env_tmp == 0)
    {
        seSerial.status = CSE_ALARM_STATUS;
        strAlarm += "Env TEMP Alarm: ";
        strTemp.arg(seSerial.env_tmp_val);
        strAlarm += strTemp;
    }

    if(seSerial.fil == 0)
    {
        seSerial.status = CSE_ALARM_STATUS;
        strAlarm += "Dirty Alarm: ";
        strTemp.arg(seSerial.fil_val);
        strAlarm += strTemp;
    }

    if(seSerial.fld == 0)
    {
        seSerial.status = CSE_ALARM_STATUS;
        strAlarm += "Level Alarm . \r\n";
    }

    if(seSerial.inf == 1)
    {
        seSerial.status = CSE_ALARM_STATUS;
        strAlarm += "Infrared trigger . \r\n";
    }

    if(seSerial.lcd == 0)
    {
        seSerial.status = CSE_ALARM_STATUS;
        strAlarm += "Lcd Alarm: ";
        strTemp.arg(seSerial.lcd_val);
        strAlarm += strTemp;
    }

    if(seSerial.lcd_tmp == 0)
    {
        seSerial.status = CSE_ALARM_STATUS;
        strAlarm += "LCD TEMP Alarm: ";
        strTemp.arg(seSerial.lcd_tmp_val);
        strAlarm += strTemp;
    }

    if(seSerial.scr == 0)
    {
        seSerial.status = CSE_ALARM_STATUS;
        strAlarm += "Screen anomaly . \r\n";
    }

    if(seSerial.shk == 0)
    {
        seSerial.status = CSE_ALARM_STATUS;
        strAlarm += "Shake Alarm . \r\n";
    }

    if(seSerial.smk == 0)
    {
        seSerial.status = CSE_ALARM_STATUS;
        strAlarm += "Smoke Alarm . \r\n";
    }
}

void CMonitor::RecvTCPMsgSlot(QByteArray &recvData)
{
    DecodeMsg(recvData);
}

void CMonitor::GetServerIP()
{
    m_bConnected1 =true;
    CLOG::Log("603:GetServerIP()");
    // send headmsg to monitor center
    struCseMsg seMsg;
    memset(&seMsg, 0, sizeof(struCseMsg));

    seMsg.msg_id = CSE_MONITOR_CHECKSERVICEIP_MESSAGE;//8
    qDebug()<<"seMsg.msg_id="<< seMsg.msg_id;
    seMsg.msg_len = 2;
    strcpy(seMsg.prd_id, m_cfgFile.m_strPrdID.toLatin1().data());
    qDebug()<<"**seMsg.prd_id="<< seMsg.prd_id;
    strcpy(seMsg.msg, "OK");

    // send msg
    QByteArray msg;
    msg.resize(sizeof(struCseMsg));
    memcpy(msg.data(), &seMsg, sizeof(struCseMsg));
    CLOG::Log("619:SendToServer");
    m_pTcpClient1->SendToServer(msg.data(), sizeof(struCseMsg));
    QString strLog = QString( "%1,%2,%3,%4")
            .arg(seMsg.msg_id).arg(seMsg.msg_len).arg(seMsg.prd_id).
            arg(seMsg.msg);
    CLOG::Log(strLog.toLatin1().data());

    connect(m_pTcpClient1, SIGNAL(sRecvTCPMsg(QByteArray &)), this, SLOT(RecvTCPMsgSlot(QByteArray &)));
}
void CMonitor::ConnectedSlot()
{
    CLOG::Log("626:Connect to server success .");
    m_bConnected2 = true;
    struCseMonitor cseMonitor;
    memset(&cseMonitor, 0, sizeof(struCseMonitor));

    //strcpy(cseMonitor.ip, m_strIP.toLatin1().data());
    strcpy(cseMonitor.monitor, m_cfgFile.m_strMonitor.toLatin1().data());

    cseMonitor.vol = m_cfgFile.m_nVol;
    cseMonitor.samptime = m_cfgFile.m_nSampTime;
    cseMonitor.max_env_hdy = m_cfgFile.m_nMaxENVHDY;
    cseMonitor.max_env_tmp = m_cfgFile.m_nMaxENVTMP;
    cseMonitor.max_lcd_tmp = m_cfgFile.m_nMaxLCDTMP;
    cseMonitor.min_lcd_idl = m_cfgFile.m_nMinLCDIDL;
    cseMonitor.max_fil = m_cfgFile.m_nMaxFIL;
    cseMonitor.fan_type = m_cfgFile.m_nFanType;
    cseMonitor.fan_speed = m_cfgFile.m_nFanSpeed;

    strcpy(cseMonitor.email, m_cfgFile.m_strEmail.toLatin1().data());
    strcpy(cseMonitor.tel, m_cfgFile.m_strTel.toLatin1().data());

    //  GetSystemTime();

    // cse msg
    struCseMsg seMsg;
    memset(&seMsg, 0, sizeof(struCseMsg));

    seMsg.msg_id = CSE_MONITOR_REQ_MESSAGE;
    qDebug()<<"seMsg.msg_id="<<seMsg.msg_id;
    seMsg.msg_len = sizeof(struCseMonitor);
    strcpy(seMsg.prd_id, m_cfgFile.m_strPrdID.toLatin1().data());
    memcpy(seMsg.msg, &cseMonitor, seMsg.msg_len);
    qDebug()<<"*seMsg.prd_id="<<seMsg.prd_id;
    // send msg
    QByteArray msg;
    msg.resize(sizeof(struCseMsg));
    memcpy(msg.data(), &seMsg, sizeof(struCseMsg));

    m_pTcpClient2->SendToServer(msg.data(), sizeof(struCseMsg));
    QString strLg = QString( "%1,%2,%3,%4")
            .arg(seMsg.msg_id).arg(seMsg.msg_len).arg(seMsg.prd_id).
            arg(seMsg.msg);
    CLOG::Log(strLg.toLatin1().data());
    QString strLog = QString( "%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12")
            .arg(cseMonitor.monitor).arg(cseMonitor.vol).arg(cseMonitor.samptime).
            arg(cseMonitor.max_env_hdy).arg(cseMonitor.max_env_tmp).arg(cseMonitor.max_lcd_tmp).
            arg(cseMonitor.min_lcd_idl).arg(cseMonitor.max_fil).arg(cseMonitor.fan_type).
            arg(cseMonitor.fan_speed).arg(cseMonitor.email).arg(cseMonitor.tel);
    CLOG::Log(strLog.toLatin1().data());
    qDebug()<<"send monitor="<<cseMonitor.monitor;
    qDebug()<<"send vol="<<cseMonitor.vol;
    qDebug()<<"send samptime="<<cseMonitor.samptime;
    qDebug()<<"send max_env_hdy="<<cseMonitor.max_env_hdy;
    qDebug()<<"send max_env_tmp="<<cseMonitor.max_env_tmp;
    qDebug()<<"send max_lcd_tmp="<<cseMonitor.max_lcd_tmp;
    qDebug()<<"send min_lcd_idl="<<cseMonitor.min_lcd_idl;
    qDebug()<<"send max_fil="<<cseMonitor.max_fil;
    qDebug()<<"send fan_type="<<cseMonitor.fan_type;
    qDebug()<<"send fan_speed="<<cseMonitor.fan_speed;
    qDebug()<<"send email="<<cseMonitor.email;
    qDebug()<<"send tel="<<cseMonitor.tel;


    connect(m_pTcpClient2, SIGNAL(sRecvTCPMsg(QByteArray &)), this, SLOT(RecvTCPMsgSlot(QByteArray &)));
}

void CMonitor::TestSendMsg()
{

    m_bConnected2 = true;

    struCseSensor seSerial;
    memset(&seSerial, 0, sizeof(struCseSensor));

    seSerial.acc = 1;
    seSerial.bck = 2;
    seSerial.env_hdy = 3;
    seSerial.env_hdy_val = 4;
    seSerial.env_tmp = 5;
    seSerial.env_tmp_val = 6;
    seSerial.fil = 7;
    seSerial.fil_val = 8;
    seSerial.fld = 9;
    seSerial.inf =10;
    seSerial.lcd =11;
    seSerial.lcd_val =12;
    seSerial.lcd_tmp =13;
    seSerial.lcd_tmp_val =14;
    seSerial.scr =15;
    seSerial.shk =16;
    seSerial.smk =17;
    seSerial.fan_speed01 =18;
    seSerial.fan_speed02 =19;
    seSerial.fan_speed03 =20;
    seSerial.fan_speed04 =21;
    seSerial.fan_speed05 =22;
    seSerial.fan_speed06 =23;
    seSerial.fan_speed07 =24;
    seSerial.fan_speed08 =25;
    seSerial.fan_speed09 =26;
    seSerial.fan_speed10 =27;
    seSerial.fan_speed11 =28;
    seSerial.fan_speed12 =29;
    seSerial.fan_speed13 =30;
    seSerial.fan_speed14 =31;
    seSerial.fan_speed15 =32;
    seSerial.fan_speed16 =33;
    seSerial.status =1;

    QString strLog = QString( "%1,%2")
            .arg(seSerial.acc).arg(seSerial.bck);
    CLOG::Log(strLog.toLatin1().data());
    // send sensor to monitor center
    if(true == m_bConnected2)
    {
        // cse msg
        struCseMsg seMsg;
        memset(&seMsg, 0, sizeof(struCseMsg));

        seMsg.msg_id = CSE_SENSOR_REQ_MESSAGE;
        qDebug()<<"seMsg.msg_id="<< seMsg.msg_id;
        seMsg.msg_len = sizeof(struCseSensor);
        strcpy(seMsg.prd_id, m_cfgFile.m_strPrdID.toLatin1().data());
        qDebug()<<"**seMsg.prd_id="<< seMsg.prd_id;
        memcpy(seMsg.msg, &seSerial, seMsg.msg_len);

        // send msg
        QByteArray msg;
        msg.resize(sizeof(struCseMsg));
        memcpy(msg.data(), &seMsg, sizeof(struCseMsg));

        // m_bSend2=m_pTcpClient2->SendToServer(msg.data(), sizeof(struCseMsg));


        CLOG::Log(msg.data());

        qDebug()<<"send acc="<<seSerial.acc;
        qDebug()<<"send bck="<<seSerial.bck;
        qDebug()<<"send env_hdy="<<seSerial.env_hdy;
        qDebug()<<"send env_hdy_val="<< seSerial.env_hdy_val;
        qDebug()<<"send env_tmp="<< seSerial.env_tmp;
        qDebug()<<"send env_tmp_val="<<seSerial.env_tmp_val;
        qDebug()<<"send fil="<<seSerial.fil;
        qDebug()<<"send fil_val="<<seSerial.fil_val;
        qDebug()<<"send fld="<< seSerial.fld;
        qDebug()<<"send inf="<<seSerial.inf;
        qDebug()<<"send lcd="<<seSerial.lcd;
        qDebug()<<"send lcd_val="<<seSerial.lcd_val;
        qDebug()<<"send lcd_tmp="<<seSerial.lcd_tmp;
        qDebug()<<"send lcd_tmp_val="<<seSerial.lcd_tmp_val;
        qDebug()<<"send src="<<seSerial.scr;
        qDebug()<<"send shk="<<seSerial.shk;
        qDebug()<<"send smk="<<seSerial.smk;

    }
}


void CMonitor::DecodeMonitor(struCseMsg * cseMsg)
{
    struCseMonitor *cseMonitor = (struCseMonitor*)cseMsg->msg;
    m_cfgFile.SaveMonitor(cseMonitor);
}

void CMonitor::DecodeMsg(QByteArray &recvData)
{
    cseMsg = (struCseMsg *)recvData.data();
    qstr=QString(QLatin1String(cseMsg->msg));

    qDebug()<<"msg_id"<< cseMsg->msg_id;
    qDebug()<<"msg_len"<< cseMsg->msg_len;
    qDebug()<< "prd_id"<<cseMsg->prd_id;
    qDebug()<<"msg"<< cseMsg->msg;
    switch(cseMsg->msg_id)
    {
    //            case CSE_MONITOR_RSP_MESSAGE://6
    //            if(0 == strcmp(cseMsg->msg, "OK"))
    //                  {
    //                      TestSendMsg();
    //                  }
    //                  break;

    case CSE_MONITOR_REQ_MESSAGE://5
        DecodeMonitor(cseMsg);

        cseMsg->msg_id = CSE_MONITOR_RECVCALLBACK_MESSAGE;//7
        cseMsg->msg_len = 2;
        strcpy(cseMsg->prd_id, m_cfgFile.m_strPrdID.toLatin1().data());
        strcpy(cseMsg->msg, "OK");

        msg.resize(sizeof(struCseMsg));
        memcpy(msg.data(), cseMsg, sizeof(struCseMsg));
        m_pTcpClient2->SendToServer(msg.data(), sizeof(struCseMsg));
        break;
    case CSE_MONITOR_CHECKSERVICEIP_MESSAGE://8

        m_pTcpClient2->ConnectToServer2(qstr, 1979);

        break;

    default:
        break;
    }

}
