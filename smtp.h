
#ifndef SMTP_H
#define SMTP_H

//#include <winsock2.h>
//#include <ws2tcpip.h>

#include <cstring>
#include <fstream>
#include <iostream>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <stdio.h>
#include <stdlib.h>
//#include <netinet/in.h>
//#include <sys/string.h>

using namespace std;

//#pragma comment(lib, "ws2_32.lib")


typedef enum SMTP_STATUS
{
    SOCKET_ERROR = -1,
    SMTP_OK = 1,
	SMTP_ERR,	
	CONNECT_HOST_ERR,
	NAME_ERR,
	PWD_ERR,
	FROM_ERR,
	TO_ERR,
	QUIT_ERR,
}SMTP_STATUS;

//#define CONNECT_HOST_ERR		-20
//#define NAME_ERR				-30
//#define PWD_ERR					-31
//#define FROM_ERR				-40
//#define TO_ERR					-41
//#define QUIT_ERR				-50
//#define SOCKET_ERR				-1
//#define SEND_OK 				0
//#define SEND_ERR 				0

class CSmtp
{
	
public:
	CSmtp();
	~CSmtp();

	int CreateSocket();//�����׽���׼�����ʼ�������ͨ�ţ����ǵ�һ��
	int ConnectHost(const char *hostname, int port);//�����ʼ������������ǵڶ���
    int Login(char *username,char *password);//����Ҫ��������HELO�ķ��ͣ����Ӻ���Ҫ�����Լ��������û��������룬���ǵ�����
	int SendMail(const char *from,const char *to,const char *date,const char *subject,const char *data);//�����ʼ���

	bool End();//�����ʼ�������־��������SMTP��ͨ��

private:	
	int m_nSocket;
	struct sockaddr_in remote;
	unsigned short port;
	
	int rt;//send�ķ��ر�־�������Ƿ���ȷ
	char *send_data;
	char *recv_data;
	char *hostname;
    hostent *ht;//�����׽��ֵķ���ֵ
	char *usersrc,*userdes,*passsrc,*passdes;
	char *From,*To,*Date,*Subject;
	char *Filename;

private:    
	void Base64_Code(unsigned char *chsrc, unsigned char *chdes);//Base64�����Ǻܳ��õ�һ�ְѶ���������ת��Ϊ�ַ������㷨��
	void Base64_Decode(unsigned char *chsrc, unsigned char *chdes);//����ֻ�ܷ����ı��ļ�
	std::string Encode(const char * szEncoding, int nSize);//�������ã����Է�����bmp�������ļ�
	bool GetResponse();//�����ʼ��������Ļ�����Ϣ
	bool DisplayError(const char *rev);

//	fstream m_fstream;
	std::string m_RetStr;
	bool m_status;
};


#endif
