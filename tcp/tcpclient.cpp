#include "tcpclient.h"

CTCPClient::CTCPClient(QObject *parent)
    : QObject(parent)
{
    m_pTcpSocket = NULL;
    m_pTcpSocket = new QTcpSocket(this);
}

CTCPClient::~CTCPClient()
{
    if(NULL != m_pTcpSocket)
    {
        m_pTcpSocket->disconnect();
        m_pTcpSocket->close();

        delete m_pTcpSocket;
        m_pTcpSocket = NULL;
    }
}
void CTCPClient::ConnectToServer1(QString &strIP, unsigned int nPort)
{
    if(NULL != m_pTcpSocket)
    {
        // connect to server
        QHostAddress host;
        host.setAddress(strIP);

        m_pTcpSocket->connectToHost(host, nPort);

        connect(m_pTcpSocket, SIGNAL(connected()), this, SLOT(GetServerIP()));


    }
}
void CTCPClient::ConnectToServer2(QString &strIP, unsigned int nPort)
{
    if(NULL != m_pTcpSocket)
    {
        // connect to server
        QHostAddress host;
        host.setAddress(strIP);

        m_pTcpSocket->connectToHost(host, nPort);

        connect(m_pTcpSocket, SIGNAL(connected()), this, SLOT(ConnectedSlot()));


    }
}

//bool CTCPClient::ConnectToServer1(QString &strIP, unsigned int nPort)
//{

//    if(NULL != m_pTcpSocket)
//    {
//    // connect to server
//    QHostAddress host;
//    host.setAddress(strIP);
//    m_pTcpSocket->connectToHost(host, nPort);
//    qDebug("State1:%dn",m_pTcpSocket->state());
//    const int Timeout=3*1000;
//    if(m_pTcpSocket->waitForConnected(Timeout))
//    {
//        qDebug("Connected!");


//    connect(m_pTcpSocket, SIGNAL(connected()), this, SLOT(GetServerIP()));
//     qDebug("State2:%dn",m_pTcpSocket->state());
//     return(true);
//     }
//   else
//     if (m_pTcpSocket->state() == QAbstractSocket::UnconnectedState ||
//             m_pTcpSocket->waitForDisconnected(Timeout))
//    {
//             qDebug("Disconnected!");
//    return (false);
//    }

//    }

//}

//bool CTCPClient::ConnectToServer2(QString &strIP, unsigned int nPort)
//{
//    if(NULL != m_pTcpSocket)
//    {

//        QHostAddress host;
//    host.setAddress(strIP);

//    m_pTcpSocket->connectToHost(host, nPort);
//    qDebug("State1:%dn",m_pTcpSocket->state());
//    const int Timeout=3*1000;
//    if(m_pTcpSocket->waitForConnected(Timeout))
//    {
//        qDebug("Connected!");


//    connect(m_pTcpSocket, SIGNAL(connected()), this, SLOT(ConnectedSlot()));
//     qDebug("State2:%dn",m_pTcpSocket->state());
//     return(true);
//     }
//    else
//     if (m_pTcpSocket->state() == QAbstractSocket::UnconnectedState ||
//             m_pTcpSocket->waitForDisconnected(Timeout))
//    {
//             qDebug("Disconnected!");
//    return (false);
//    }
//    }

//}

void CTCPClient::ConnectedSlot()
{
    connect(m_pTcpSocket, SIGNAL(readyRead()), this, SLOT(ReadMsgSlot()));

    emit sConnected();
}
void CTCPClient::GetServerIP()
{
    connect(m_pTcpSocket, SIGNAL(readyRead()), this, SLOT(ReadMsgSlot()));
    qDebug()<<"CTCPClient::GetServerIP()";
    emit sConnected();
}

void CTCPClient::ReadMsgSlot()
{
    QByteArray recvData = m_pTcpSocket->readAll();
  //  QString strMsg = QVariant(tcpData).toString();

    emit sRecvTCPMsg(recvData);
}

void CTCPClient::SendToServer(QString &strMsg)
{
    int nSize = m_pTcpSocket->write(strMsg.toLatin1().data(), strMsg.length());

    m_pTcpSocket->waitForBytesWritten(300);
    m_pTcpSocket->flush();

    if(nSize <= 0)
    {
        CLOG::Log("send data to server failed .");
    }
    else
    {
        CLOG::Log("send data to server success .");
    }
}
void CTCPClient::SendToServer(char * strMsg, int nLen)

{
    CLOG::Log("fghhhhhhhhhh111");
    char str[560];
    QString string = QString(QLatin1String(strMsg));
    strcpy(str, string.toLatin1().data());

    int nSize = m_pTcpSocket->write(strMsg, nLen);
    m_pTcpSocket->waitForBytesWritten(300);
    m_pTcpSocket->flush();
    QString s = QString("%1,%2").arg(str).arg(nLen);
    CLOG::Log(s);
    CLOG::Log("fghhhhhhhhhh");
QString a = QString("%1").arg(nSize);
    CLOG::Log(a);


    if(nSize <= 0)
    {        
        qDebug()<<"send data to server failed 222222.";
        CLOG::Log("send data to server failed222222 .");

    }
    else
    {

        qDebug()<<"send data to server success 2222222222.";
        CLOG::Log("103:send data to server success 222222222.");

    }
}
