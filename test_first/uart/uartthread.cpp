#include "uartthread.h"
#include "monitor.h"

CUartThread::CUartThread(QObject *parent)
    :QThread(parent)
{
    m_bRun = false;      //寮€濮嬬疆涓簍rue
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

//----------------------------------------------------------------
//涓插彛鍒濆鍖?
int CUartThread::Initial(char *strDev)
{
    m_nFD = openport(strDev);

    if(m_nFD < 0)
    {

         return -1;

    }

    return m_nFD;

}

void CUartThread::Stop()
{
    m_bRun = false;     //缃仠
    closeport(m_nFD);
}

void CUartThread::Start()
{
    m_bRun = true;     //寮€濮?
    // start thread recv data
    //this->start();
}

void CUartThread::ReadUart()
{
    int nLen = 0;
    char szBuf[RECV_DATA_LEN] = {0};    //512

    nLen = readport(m_nFD, szBuf, 5);
    if(nLen < 0)
    {
        CLOG::Log("Read sensor cmd error .");
        return;
    }
    if((szBuf[0] == 'O') && (szBuf[1] == 'K'))
    {

        int nDatalen = 0;
        if(nLen >= RECV_CMD_LEN)
        {
            nDatalen = szBuf[3]*256 + szBuf[4] - 5 + 2;
        }

        nLen = readport(m_nFD, szBuf, nDatalen);
        printf("Read OK success\r\n");
        if(nLen <= 0)
        {
            //printf("nLen <= 0,Read sensor data error .\r\n");
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

void CUartThread::wReadUart()
{
    int wLen = 0;
    char wszBuf[8] = {0};

    wLen = readport(m_nFD, wszBuf, 8);
    if(wLen < 0)
    {
        CLOG::Log("Read w cmd error .");
        return;
    }
    if((wszBuf[0] == 'O') && (wszBuf[1] == 'K'))
    {

        w_run = true;

        if(wLen <= 0)
        {
            //printf("nLen <= 0,Read sensor data error .\r\n");
            CLOG::Log("Read sensor data error .");
            return;
        }

        printf("Read uart data success  \r\n");
        CLOG::Log("Read uart data success .");

         //put a signals that recv data from serial port
    }

    if((wszBuf[0] == 'E') && (wszBuf[1] == 'R'))
    {
        //wLen = readport(wFD, wszBuf, 2);
        //printf("recv data error  \r\n");
    }
}

void CUartThread::WriteUart( char *strBuf, int nLen)
{

    writeport(m_nFD, strBuf, nLen);
}

void CUartThread::writeUartSerial(struCseSensor *wstrbuf)
{
    CLOG::Log("writeUartSerial .");

    wwriteport(m_nFD, wstrbuf, sizeof(struCseSensor));
}
void CUartThread::writeSerialSlot(struCseSensor *wstrBuf)
{
    CLOG::Log("writeUartSerial .");

    wReadUart();

    while (w_run)
    {
        sleep(10);
        writeUartSerial(wstrBuf);
    }
}
