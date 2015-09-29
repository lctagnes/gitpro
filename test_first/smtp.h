
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

	int CreateSocket();//创建套接字准备与邮件服务器通信，这是第一步
	int ConnectHost(const char *hostname, int port);//连接邮件服务器，这是第二步
    int Login(char *username,char *password);//这里要看看命令HELO的发送，连接后需要发送自己的邮箱用户名和密码，这是第三步
	int SendMail(const char *from,const char *to,const char *date,const char *subject,const char *data);//发送邮件体

	bool End();//发送邮件结束标志，结束和SMTP的通话

private:	
	int m_nSocket;
	struct sockaddr_in remote;
	unsigned short port;
	
	int rt;//send的返回标志，看看是否正确
	char *send_data;
	char *recv_data;
	char *hostname;
    hostent *ht;//创建套接字的返回值
	char *usersrc,*userdes,*passsrc,*passdes;
	char *From,*To,*Date,*Subject;
	char *Filename;

private:    
	void Base64_Code(unsigned char *chsrc, unsigned char *chdes);//Base64编码是很常用的一种把二进制数据转换为字符串的算法；
	void Base64_Decode(unsigned char *chsrc, unsigned char *chdes);//但是只能发送文本文件
	std::string Encode(const char * szEncoding, int nSize);//这个编码好，可以发楚了bmp的所有文件
	bool GetResponse();//看看邮件服务器的回馈消息
	bool DisplayError(const char *rev);

//	fstream m_fstream;
	std::string m_RetStr;
	bool m_status;
};


#endif
