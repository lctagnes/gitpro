#include "writethread.h"
#include "monitor.h"

WriteThread::WriteThread(QObject *parent)
    :QThread(parent)
{
    stopped = false;
}

WriteThread::~WriteThread()
{
    closeport(wFD);
}

void WriteThread::run()
{
    while (!stopped)
    {
        ReadUart();
    }

}

int WriteThread::Initial(char *strDev)
{
    wFD = openport(strDev);

    if(wFD < 0)
    {
         return -1;
    }
    else
    {
        return wFD;
    }
}

void WriteThread::Stop()
{
    stopped = false;
    closeport(wFD);
}

void WriteThread::ReadUart()
{
    int wLen = 0;
    char szBuf[8] = {0};

    wLen = readport(wFD, szBuf, 8);
    if(wLen < 0)
    {
        CLOG::Log("Read write_serial cmd error .");
        return;
    }
    else if((szBuf[0] == 'O') && (szBuf[1] == 'K'))
    {
        //设置写串口数据标志位为true
        writeFlag = true;

        printf("Read OK  \r\n");
        CLOG::Log("Read OK .");
    }
}

void WriteThread::writeSerial(struCseSensor *wstrbuf)
{
    CLOG::Log("writeDataToSerial .");

    wwriteport(wFD, wstrbuf, sizeof(struCseSensor));
}
void WriteThread::writeSerialSlot(struCseSensor *wstrBuf)
{
    CLOG::Log("writeDataToSerial .");

    ReadUart();

    if(writeFlag)
    {
        writeSerial(wstrBuf);
    }
}
