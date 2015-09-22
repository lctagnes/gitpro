#include "newtcplient.h"

NTCPClient::NTCPClient(QObject *parent)
    : QObject(parent)
{
    m_pTcpSocket0 = NULL;
    m_TcpSocket0 = new QTcpSocket(this);
}

NTCPClient::~NTCPClient()
{
    if(NULL != m_pTcpSocket0)
    {
        m_pTcpSocket0->disconnect();
        m_pTcpSocket0->close();

        delete m_pTcpSocket0;
        m_pTcpSocket0 = NULL;
    }
}

void NTCPClient::ConnectToServer(QString &strIP, unsigned int nPort)
{
    if(NULL != m_pTcpSocket0)
    {
        // connect to server
        QHostAddress host;
        host.setAddress(strIP);

        m_pTcpSocket0->connectToHost(host, nPort);

        connect(m_pTcpSocket0, SIGNAL(connected()), this, SLOT(ConnectedSlot1()));
        //connect(m_pTcpSocket, SIGNAL(Connected()), this, SLOT(ConnectedSlot1()));
    }
}


void NTCPClient::ConnectedSlot1()
{
    connect(m_pTcpSocket0, SIGNAL(readyRead()), this, SLOT(ReadMsgSlot()));

    emit sConnected();
}

void NTCPClient::ReadMsgSlot()
{
    QByteArray recvData = m_pTcpSocket0->readAll();
  //  QString strMsg = QVariant(tcpData).toString();

    emit sRecvTCPMsg(recvData);
}

void NTCPClient::SendToServer(QString &strMsg)
{
    int nSize = m_pTcpSocket0->write(strMsg.toLatin1().data(), strMsg.length());
    m_pTcpSocket0->flush();

    if(nSize <= 0)
    {
        printf("send data to server failed .");
        CLOG::Log("send data to server failed .");
    }
    else
    {
        printf("send data to server success .");
        CLOG::Log("send data to server success .");
    }
}

void NTCPClient::SendToServer(char * strMsg, int nLen)
{
    int nSize = m_pTcpSocket0->write(strMsg, nLen);
    m_pTcpSocket0->flush();

    if(nSize <= 0)
    {
        printf("send data to server failed .");
        CLOG::Log("send data to server failed .");
    }
    else
    {
        printf("send data to server success .");
        CLOG::Log("send data to server success .");
    }
}

