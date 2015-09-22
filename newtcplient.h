#ifndef TCPLIENT_H
#define TCPLIENT_H

#include <QObject>
#include <QtNetwork/QtNetwork>

#include "common/log.h"
#include "configfile.h"

#include "common/define.h"

//using namespace std;

#define CONNECT_TIME_OUT 3000

typedef unsigned char BYTE;


class NTCPClient: public QObject
{
    Q_OBJECT

public:
    explicit NTCPClient(QObject *parent = 0);
    ~NTCPClient();

    // connect to server
    void ConnectToServer(QString &strIP, unsigned int nPort);

    // send msg to server
    void SendToServer(QString &strMsg);
    void SendToServer(char * strMsg, int nLen);

signals:
    void sConnected();
    void sRecvTCPMsg(QByteArray &recvData);

private slots:
    void ReadMsgSlot();
    //void ConnectedSlot();
    void ConnectedSlot1();

private:
    QTcpSocket *m_pTcpSocket0;
};

#endif // TCPCLIENT_H

