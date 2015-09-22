#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include <QThread>

#include "tcpinterface.h"
#include "common/log.h"


class CTCPThread : public QThread
{
    Q_OBJECT
public:
    explicit CTCPThread(QObject *parent = 0);
    ~CTCPThread();

    void Start(char *strIP, int nPort);
    void Stop();
    void sendMsg(char * strMsg, int nSize);

protected:
    void run();

signals:

public slots:

private:
    bool m_bRun;
    CTCPInterface m_tcpClient;
};

#endif // TCPTHREAD_H
