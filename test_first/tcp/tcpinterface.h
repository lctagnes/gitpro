#ifndef TCPINTERFACE_H
#define TCPINTERFACE_H

#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include "common/define.h"

#define TCP_BUFFER_SIZE 1024

class CTCPInterface
{
public:
    CTCPInterface();
    ~CTCPInterface();

    int ConnectServer(char *strIP, int nPort);

    int RecvMsg();
    int SendMsg(char * strMsg, int nSize);

private:
    int m_nSocket;
    char m_szBuff[TCP_BUFFER_SIZE];

};

#endif // TCPINTERFACE_H
