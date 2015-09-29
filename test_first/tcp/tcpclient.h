#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QtNetwork/QtNetwork>

#include "common/log.h"
#include "configfile.h"

#include "common/define.h"
#include <netinet/tcp.h>

//using namespace std;

#define CONNECT_TIME_OUT 3000

typedef unsigned char BYTE;

class CTCPClient: public QObject
{
    Q_OBJECT

public:
    explicit CTCPClient(QObject *parent = 0);
    ~CTCPClient();

public://public variables

    // connect to server
    void ConnectToServer1(QString &strIP, unsigned int nPort);
    void ConnectToServer2(QString &strIP, unsigned int nPort);

    // send msg to server
    void SendToServer(QString &strMsg);
    void SendToServer(char * strMsg, int nLen);

signals:
    void sConnected();
    void sRecvTCPMsg(QByteArray &recvData);
private slots:
    void ReadMsgSlot();
    void ConnectedSlot();
    void GetServerIP();
private:
    QTcpSocket* m_pTcpSocket;

};


#endif // TCPCLIENT_H
