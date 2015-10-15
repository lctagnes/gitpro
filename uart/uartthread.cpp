/************************************************************
 *Copyright (c) 2013-2015 深圳市赛柏达技术有限公司技术研发部
 *
 *FileName:		uartthread.cpp
 *Writer:		smart-skynet
 *create Date:  2015/01/01
 *Rewriter:		kason
 *Rewrite Date:	2015/10/08
 *Impact:
 *
 *Main Content(Function Name、parameters、returns)
 *
 ************************************************************/

#include "uartthread.h"

CUartThread::CUartThread(QObject *parent)
    :QThread(parent)
{
    m_bRun = false;      //线程运行标志位
}

CUartThread::~CUartThread()
{
    closeport(m_nFD);
}

void CUartThread::run()
{
    while (m_bRun)
    {
        ReadUart();
    }
}


int CUartThread::Initial(char *strDev, int nBaud, int nByte, int nStop, char cCheck)
{
    int ret = 0;

    m_nFD = openport(strDev);

    if(m_nFD > 0)
    {
        ret = setport(m_nFD, nBaud, nByte, nStop, cCheck);  //设置端口参数，波特率、数据位、停止位、校验位
        if(ret < 0)
        {
            return 0;
        }

        // start thread recv data
        //this->Start();

    }
    else
    {
        return -1;
    }

    return m_nFD;
}

void CUartThread::Stop()
{
    m_bRun = false;     //停止
    closeport(m_nFD);
}

void CUartThread::Start()
{
    m_bRun = true;     //开始
    // start thread recv data
    //this->start();
}

/*void CUartThread::ReadUart()
{
    int nLen = 0;
    char szBuf[RECV_DATA_LEN] = {0};

    int time = 1000;
    nLen = readport(m_nFD, szBuf, RECV_CMD_LEN);

    if(nLen <= 0)
    {
        printf("recv data return \r\n");
        return;
    }

    if((szBuf[0] == 'O') && (szBuf[1] == 'K'))
    {
        int nDatalen = 0;
        if(nLen >= RECV_CMD_LEN)
        {
            nDatalen = szBuf[3]*256 + szBuf[4] - 5 + 2;
            //printf("nDatalen=%d  \r\n", nDatalen);
        }
        nLen = readport(m_nFD, szBuf, nDatalen);

       // printf("recv data len=%d  \r\n", nLen);
        if(nLen <= 0)
        {
            return;
        }

        printf("remit sSerialData(szBuf)  \r\n");

        // put a signals that recv data from serial port
        emit sSerialData(szBuf);
    }

    if((szBuf[0] == 'E') && (szBuf[1] == 'R'))
    {
        nLen = readport(m_nFD, szBuf, 2);
        printf("recv data error  \r\n");
    }
}*/
void CUartThread::ReadUart()
{
    printf("readuart");
    int nLen = 0;
    char szBuf[RECV_DATA_LEN] = {0};

    nLen = readport(m_nFD, szBuf, 5);
    if(nLen < 0)
    {
        printf("readuart < 0");
        CLOG::Log("Read sensor cmd error .");
        return;
    }
    if((szBuf[0] == 'O') && (szBuf[1] == 'K'))
    {
        printf("readuart ok");
        int nDatalen = 0;
        if(nLen >= RECV_CMD_LEN)
        {
            nDatalen = szBuf[3]*256 + szBuf[4] - 5 + 2;
        }

        nLen = readport(m_nFD, szBuf, nDatalen);

        if(nLen <= 0)
        {
            printf("nLen <= 0,Read sensor data error .\r\n");
            CLOG::Log("Read sensor data error .");
            return;
        }

//        for(int i = 0; i < nLen; i++)
//        {
//            printf("%x\r\n", *(szBuf + i));
//        }

        printf("Read uart data success  \r\n");
        CLOG::Log("Read uart data success .");

         //put a signals that recv data from serial port
        emit sSerialData(szBuf);
    }

    if((szBuf[0] == 'E') && (szBuf[1] == 'R'))
    {
        nLen = readport(m_nFD, szBuf, 2);
        //printf("recv data error  \r\n");
    }
}

void CUartThread::WriteUart( char *strBuf, int nLen)
{
    printf("dev=%s",strBuf);
    writeport(m_nFD, strBuf, nLen);
}



