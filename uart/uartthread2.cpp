/************************************************************
 *Copyright (c) 2013-2015 深圳市赛柏达技术有限公司技术研发部
 *
 *FileName:		uartthread2.cpp
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

CUartThread2::CUartThread2(QObject *parent)
    :QThread(parent)
{
    m_bRun = false;      //线程运行标志位
}

CUartThread2::~CUartThread2()
{
    closeport(m_nFD);
}

void CUartThread2::run()
{
    while (m_bRun)
    {
        ReadUart();
    }
}

int CUartThread2::Initial(char *strDev, int nBaud, int nByte, int nStop, char cCheck)
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

void CUartThread2::Stop()
{
    m_bRun = false;     //停止
    closeport(m_nFD);
}

void CUartThread2::Start()
{
    m_bRun = true;     //开始
    // start thread recv data
    //this->start();
}

void CUartThread2::ReadUart()
{
    int Len = 0;
    char szBuf[8] = {0};

    Len = readport(m_nFD, szBuf, 8);
    if(Len < 0)
    {
        CLOG::Log("Read upper computer cmd error .");
        return;
    }
    else
        if((szBuf[0] == 'O') && (szBuf[1] == 'K'))
        {
            w_run = true;
            printf("Read uart data success  \r\n");
            CLOG::Log("Read uart data success .");
        }else
        {
             printf("recv upper computer cmd error  \r\n");
        }
}

void CUartThread2::WriteUart( char *strBuf, int nLen)
{

    writeport(m_nFD, strBuf, nLen);
}

void CUartThread2::writeSerialSlot(char *wstrBuf)
{
    CLOG::Log("writeUartSerial .");

    ReadUart();

    if (w_run)
    {
        sleep(10);
        WriteUart(wstrBuf, 136);
    }
}
