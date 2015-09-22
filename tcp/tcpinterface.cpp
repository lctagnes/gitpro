#include "tcpinterface.h"

CTCPInterface::CTCPInterface()
{
    m_nSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == m_nSocket)
    {
        return ;
    }

    memset(m_szBuff, 0, sizeof(m_szBuff));
}

CTCPInterface::~CTCPInterface()
{
    //close(m_nSocket);
}

int CTCPInterface::ConnectServer(char *strIP, int nPort)
{
    struct sockaddr_in s_add;

    bzero(&s_add,sizeof(struct sockaddr_in));

    s_add.sin_family = AF_INET;
    s_add.sin_addr.s_addr = inet_addr(strIP);
    s_add.sin_port = htons(nPort);


    if(-1 == connect(m_nSocket,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
    {
        return -1;
    }

    return 0;
}

int CTCPInterface::RecvMsg()
{
    int nbytes = recv(m_nSocket, m_szBuff, TCP_BUFFER_SIZE, 0);
    return nbytes;
}

int CTCPInterface::SendMsg(char * strMsg, int nSize)
{
    CseSerial seSerial;
    memset(&seSerial, 0, sizeof(CseSerial));
    int len = sizeof(CseSerial);

    seSerial.acc = 10;
    seSerial.env_hdy = 2;
    seSerial.env_tmp = 3;
    seSerial.fil = 8;
    seSerial.fld = 8;
    seSerial.inf = 8;
    seSerial.lcd = 8;
    seSerial.lcd_tmp = 8;
    seSerial.shk = 8;
    seSerial.smk = 8;

    seSerial.fan_speed01 = 88;
    seSerial.fan_speed02 = 88;
    seSerial.fan_speed03 = 88;
    seSerial.fan_speed04 = 88;
    seSerial.fan_speed05 = 88;
    seSerial.fan_speed06 = 88;
    seSerial.fan_speed07 = 88;
    seSerial.fan_speed08 = 88;
    seSerial.fan_speed09 = 88;
    seSerial.fan_speed10 = 88;
    seSerial.fan_speed11 = 88;
    seSerial.fan_speed12 = 88;
    seSerial.fan_speed13 = 88;
    seSerial.fan_speed14 = 88;
    seSerial.fan_speed15 = 88;
    seSerial.fan_speed16 = 88;

    char  *strTmp = "123456789";

    char szMsg[512];
    memset(szMsg, 0, sizeof(szMsg));

    memcpy(szMsg, &seSerial, len);

    char *addr = (char*)&seSerial;

    int nbytes = send(m_nSocket, addr, len, 0);

  //  int nbytes = send(m_nSocket, strMsg, nSize, 0);
    return nbytes;
}
