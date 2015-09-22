#include "tcpthread.h"

CTCPThread::CTCPThread(QObject *parent) :
    QThread(parent)
{
    m_bRun = false;
}

CTCPThread::~CTCPThread()
{

}

void CTCPThread::Start(char *strIP, int nPort)
{
    m_bRun = true;

    if(-1 == m_tcpClient.ConnectServer(strIP, nPort))
    {
        CLOG::Log("Connect server failed .");
    }
    else
    {
        CLOG::Log("Connect server success, start tcp thread .");
        this->start();
    }
}

void CTCPThread::Stop()
{
    m_bRun = false;
}

void CTCPThread::run()
{
    while (m_bRun)
    {
       // RecvDataFromPort();
    }
}

void CTCPThread::sendMsg(char * strMsg, int nSize)
{
    if(-1 == m_tcpClient.SendMsg(strMsg, nSize))
    {
        CLOG::Log("Send msg failed .");
    }
}
