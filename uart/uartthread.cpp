#include "uartthread.h"

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
int CUartThread::Initial(char *strDev, int nBaud, int nByte, int nStop, char cCheck)
{
    int ret = 0;

    m_nFD = openport(strDev);

    if(m_nFD > 0)
    {
        ret = setport(m_nFD, nBaud, nByte, nStop, cCheck);  //璁剧疆涓插彛锛屾尝鐗圭巼锛屾暟鎹綅锛屽仠姝綅锛屾牎楠?
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
    m_bRun = false;     //缃仠
    closeport(m_nFD);
}

void CUartThread::Start()
{
    m_bRun = true;     //寮€濮?
    // start thread recv data
    //this->start();
}

//void CUartThread::ReadUart()
//{
//    int nLen = 0;
//    char szBuf[RECV_DATA_LEN] = {0};

//   // int time = 1000;
//    nLen = readport(m_nFD, szBuf, RECV_CMD_LEN);

//    if(nLen <= 0)
//    {
//        printf("recv data return \r\n");
//        return;
//    }

//    if((szBuf[0] == 'O') && (szBuf[1] == 'K'))
//    {
//        int nDatalen = 0;
//        if(nLen >= RECV_CMD_LEN)
//        {
//            nDatalen = szBuf[3]*256 + szBuf[4] - 5 + 2;
//            //printf("nDatalen=%d  \r\n", nDatalen);
//        }

//        nLen = readport(m_nFD, szBuf, nDatalen);

//       // printf("recv data len=%d  \r\n", nLen);

//        if(nLen <= 0)
//        {
//            return;
//        }

//        printf("remit sSerialData(szBuf)  \r\n");

//        // put a signals that recv data from serial port
//        emit sSerialData(szBuf);
//    }

//    if((szBuf[0] == 'E') && (szBuf[1] == 'R'))
//    {
//        nLen = readport(m_nFD, szBuf, 2);
//        printf("recv data error  \r\n");
//    }
//}
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

    wLen = readport(wFD, wszBuf, 8);
    if(wLen < 0)
    {
        CLOG::Log("Read w cmd error .");
        return;
    }
    if((wszBuf[0] == 'O') && (wszBuf[1] == 'K'))
    {

        connect(CMonitor, SIGNAL(writeSerial(struCseSensor *)), this, SLOT(writeSerialSlot(char *)));

        if(wLen <= 0)
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
    }

    if((wszBuf[0] == 'E') && (wszBuf[1] == 'R'))
    {
        wLen = readport(wFD, wszBuf, 2);
        //printf("recv data error  \r\n");
    }
}

void CUartThread::WriteUart( char *strBuf, int nLen)
{

    writeport(m_nFD, strBuf, nLen);
}

void CUartThread::writeSerial(struCseSensor *wstrbuf)
{
    CLOG::Log("writeSerial .");

    wwriteport(wFD, wstrbuf, sizeof(struCseSensor));
}
void CUartThread::writeSerialSlot(struCseSensor *wstrBuf)
{
    CLOG::Log("writeSerial .");

    writeSerial(wstrBuf);
}
