#include "smtp.h"
#include<stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <unistd.h>
#include <QDebug>
CSmtp::CSmtp()
{
	m_status = true;
	From      = new char[100];//100这个长度应该够密码用户名和邮件的地址了，不够再加
	To        = new char[100];
	Date      = new char[100];
	Subject   = new char[100];
	Filename  = new char[100];

	recv_data = new char[512];//用于接受smtp服务器的发挥信息
	userdes   = new char[100];
	passdes   = new char[100];

	//	m_fstream.open("log.txt" ,  ios::in|ios::out);
}
CSmtp::~CSmtp()
{
	if(!m_nSocket)//!=SOCKET_ERR
	{
        close(m_nSocket);
	}

	delete []From;//这里可千万别忘了释放new申请的空间
	delete []To;
	delete []Date;
	delete []Subject;
	delete []Filename;
	delete []recv_data;
	delete []userdes;
	delete []passdes;

	//	m_fstream.close();
}
void CSmtp::Base64_Code( unsigned char *chsrc, unsigned char *chdes)
{
	char chadd[3];
	unsigned char temp[4],t;

	int len,i;
	len = strlen((char *)chsrc);

	while(len>=3)
	{
		temp[0] = (*chsrc)>>2;
		t       = (*chsrc & 0x03)<<4;
		temp[1] = (*(chsrc+1)>>4)|t;
		t       = ((*(chsrc+1)<<2) & 0x3f);
		temp[2] = (*(chsrc+2)>>6)|t;
		temp[3] = (*(chsrc+2) & 0x3f);

		for(i=0;i<4;i++)
		{
			if(temp[i]>=0 && temp[i]<=25) 
				*(chdes+i) = temp[i]+65;
			if(temp[i]>=26 && temp[i]<=51) 
				*(chdes+i) = temp[i]+71;
			if(temp[i]>=52 && temp[i]<=61) 
				*(chdes+i) = temp[i]-4;
			if(temp[i]==62) 
				*(chdes+i) = 43;
			if(temp[i]==63) 
				*(chdes+i) = 47;
		}

		len -= 3;
		chsrc += 3;
		chdes += 4;
	}
	if(len!=0)
	{
		for(i=0;i<3;i++)
			chadd[i] = 0;
		memcpy(chadd,chsrc,len);

		temp[0] = chadd[0]>>2;
		t       = (chadd[0] & 0x03)<<4;
		temp[1] = (chadd[1]>>4)|t;
		t       = ((chadd[1]<<2) & 0x3f);
		temp[2] = (chadd[2]>>6)|t;
		temp[3] = chadd[2] & 0x3f;

		for(i=0;i<4;i++)
		{
			if(temp[i]>=0 && temp[i]<=25 && (i==0 || i==1)) *(chdes+i) = temp[i]+65;
			else  *(chdes+i) = 61;
			if(temp[i]>=26 && temp[i]<=51) *(chdes+i) = temp[i]+71;
			else if(temp[i]>=52 && temp[i]<=61) *(chdes+i) = temp[i]-4;
			else if(temp[i]==62) *(chdes+i) = 43;
			else if(temp[i]==63) *(chdes+i) = 47;
		}
		chdes += 4;
		*chdes = '\0';
		return;
	}
	*chdes = '\0';
}

void CSmtp::Base64_Decode( unsigned char *chsrc, unsigned char *chdes)
{
	unsigned char temp[4],t;
	int len,i;
	len = strlen((char *)chdes);
	while(len>=4)
	{
		for(i=0;i<4;i++)
		{
			if(*(chdes+i)>=65 && *(chdes+i)<=90) 
				temp[i] = *(chdes+i)-65;
			if(*(chdes+i)>=97 && *(chdes+i)<=122) 
				temp[i] = *(chdes+i)-71;
			if(*(chdes+i)>=48 && *(chdes+i)<=57) 
				temp[i] = *(chdes+i)+4;
			if(*(chdes+i)==43) 
				temp[i] = 62;
			if(*(chdes+i)==47) 
				temp[i] = 63;
			if(*(chdes+i)==61)
				temp[i] = 0;
		}
		t = (temp[1]>>4)&0x03;
		*chsrc = (temp[0]<<2)|t;
		t = (temp[2]>>2)&0x0f;
		*(chsrc+1) = (temp[1]<<4)|t;
		t = temp[3];
		*(chsrc+2) = (temp[2]<<6)|t;

		chsrc += 3;
		chdes += 4;
		len   -= 4;
	}
	chsrc -= 3;
	for(i=0;i<3;i++)
	{
		if(*(chsrc+i) == 0)
		{
			*(chsrc+i) = '\0';
			break;
		}
	}
	if(i>=2)
		*(chsrc+3) = '\0';
}

std::string CSmtp::Encode(const char * szEncoding, int nSize)//这个编码好，可以发楚了bmp的所有文件
{
	//Base64编码字符集：
	std::string m_sBase64Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	char *szOutput;
	//计算空间
	int size = (nSize + 2) / 57 * 2;
	size += nSize % 3 != 0 ? (nSize - nSize % 3 + 3) / 3 * 4 : nSize / 3 * 4;
	szOutput = new char[size + 1];
	memset(szOutput, 0, size + 1);

	const char * szInput = szEncoding;

	int nBitsRemaining = 0, nPerRowCount = 0;//换行计数
	register int nBitStorage = 0, nScratch = 0;
	int i, lp, endlCount;

	for(i=0, lp=0, endlCount = 0; lp < nSize; lp++)
	{
		nBitStorage = (nBitStorage << 8) | (szInput[lp] & 0xff);//1 byte//the lowest-byte to 0 not cycle
		nBitsRemaining += 8;//读了一个字节，加八位

		do//nBitStorage"剩下的位"记录变量
		{
			nScratch = nBitStorage >> (nBitsRemaining - 6) & 0x3f;//提出最前的六位
			szOutput[i++] = m_sBase64Alphabet[nScratch];
			nPerRowCount++;
			if(nPerRowCount >= 76)
			{
				szOutput[i++] = '\r';
				szOutput[i++] = '\n';
				endlCount += 2;
				nPerRowCount = 0;
			}
			nBitsRemaining -= 6;
		}while(nBitsRemaining >= 6);
	}

	if(nBitsRemaining != 0)
	{
		//原数据最后多一个两个字节时进入，编码未结束nBitsRemaining!=0
		nScratch = nBitStorage << (6-nBitsRemaining);//空位补0
		nScratch &= 0x3f;
		szOutput[i++] = m_sBase64Alphabet[nScratch];
		nPerRowCount++;
		if(nPerRowCount >= 76)
		{
			szOutput[i++] = '\r';
			szOutput[i++] = '\n';
			endlCount += 2;
			nPerRowCount = 0;
		}
	} 

	// Pad with '=' as per RFC 1521
	while((i - endlCount) % 4 != 0)
	{
		szOutput[i++] = '=';
		nPerRowCount++;
		if(nPerRowCount >= 76)
		{
			szOutput[i++] = '\r';
			szOutput[i++] = '\n';
			endlCount += 2;
			nPerRowCount = 0;
		}
	}

	std::string strResult = szOutput;
	delete[] szOutput;
	return strResult;
}

bool CSmtp::GetResponse()
{
    sleep(1);//这个时延很重要，花了我一天的时间才弄出来，要看你的网速，如果网速快，可以不要这个时延

	try{
		memset(recv_data, 0, sizeof(recv_data));
		rt = recv(m_nSocket,recv_data,512,0);

		if(rt == SOCKET_ERROR)
			return false;
		recv_data[rt]='\0';	//在最后一个字母后面加上结束符号,或者在接收之前zeromemory一下recv_data
		//	printf("recv : %s\n",recv_data);
		//	m_fstream<<recv_data<<endl;

		string str = recv_data;
		str = str.substr(0,3);

		if ( !DisplayError(str.c_str()))
			return false;
	}
	catch(...)
	{
		return false;
	}

	if(*recv_data == '5') //因为第一个返回数字是5一般都是错误代码，而正确代码一般都是2打头的
		return false;

	return true;
}

int CSmtp::CreateSocket()
{
//	WSADATA wsa;
//	rt = WSAStartup(MAKEWORD(2, 0), &wsa);

//	if(rt == SOCKET_ERROR)
//	{
//		//	closesocket(s);
//		return SOCKET_ERROR;
//	}

	m_nSocket = socket(AF_INET,SOCK_STREAM,0);
	if(m_nSocket == SOCKET_ERROR)
	{		
        close(m_nSocket);
		return SOCKET_ERROR;
	}

//    long time = 60000;	//获取或者发送套接字的超时时间（毫秒）
//    if(setsockopt(m_nSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&time, sizeof(long)) == SOCKET_ERROR )//setsockopt function sets a socket option.
//	{
//        close(m_nSocket);
//		return SOCKET_ERROR;
//	}

    return SMTP_OK;
}

int CSmtp::ConnectHost(const char *hostname, int port)
{
	if(hostname == NULL)
    {
		return CONNECT_HOST_ERR;
    }

    //ht = gethostbyname2(hostname,AF_INET);
    ht = gethostbyname2("smtp.163.com",AF_INET);
    //ht = gethostbyname("smtp.163.com");
   // ht = gethostname(hostname,sizeof(hostname));
    qDebug()<<"hostname="<<hostname;
        qDebug()<<"port="<<port;
        qDebug()<<"ht="<<ht;
    if(ht == NULL)
	{
		//	closesocket(m_nSocket);
		return CONNECT_HOST_ERR;
	}

	if(ht->h_addr_list[0] == NULL)
	{
		//	closesocket(m_nSocket);
		return CONNECT_HOST_ERR;
	}

	memset(&remote,0,sizeof(remote));
	remote.sin_family = AF_INET;
	remote.sin_port   = htons(port);
	remote.sin_addr = *(struct in_addr *)ht->h_addr;

	rt = connect(m_nSocket,(struct sockaddr *)&remote,sizeof(struct sockaddr));
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return CONNECT_HOST_ERR;
	}

	if(!GetResponse())
        return CONNECT_HOST_ERR;

    return SMTP_OK;
}

int CSmtp::Login(char *username,char *password)
{
	char ch[100]; //缓存
	memset(ch, 0, 100);
	if(username == NULL || password == NULL)//不能没有密码和用户名邮箱
	{
		return SMTP_ERR;
	}

	usersrc = username;
	passsrc = password;
	Base64_Code((unsigned char *)usersrc,(unsigned char *)userdes);//编码要用无符号的字符型
	Base64_Code((unsigned char *)passsrc,(unsigned char *)passdes);

	send_data = "HELO Localhost\r\n";//告诉服务器客户端程序要通话了
	rt = send(m_nSocket,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return SOCKET_ERROR;
	}
	if(!GetResponse())
		return SOCKET_ERROR;

	send_data = "AUTH LOGIN\r\n";//告诉服务器，我要登录
	rt = send(m_nSocket,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return SOCKET_ERROR;
	}
	if(!GetResponse())
		return SOCKET_ERROR;

	sprintf(ch,"%s\r\n",userdes);//用户名
	rt = send(m_nSocket,(char *)ch,strlen(ch),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return SOCKET_ERROR;
	}
	if(!GetResponse())
		return NAME_ERR;

	sprintf(ch,"%s\r\n",passdes);	//密码="MTI1c2YzX3pwX2Zz"
	rt = send(m_nSocket,(char *)ch,strlen(ch),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return SOCKET_ERROR;
	}

	if(!GetResponse())
		return PWD_ERR;

	return SMTP_OK;
}

int CSmtp::SendMail(const char *from,const char *to,const char *date,const char *subject,const char *data)
{
	if(from == NULL || to == NULL || date == NULL || subject == NULL)
	{
		//	closesocket(m_nSocket);
		return SMTP_ERR;
	}

	sprintf(From,"MAIL FROM: <%s>\r\n",from);//指定发信者的邮箱
	rt = send(m_nSocket,From,strlen(From),0);
	if(rt == SOCKET_ERROR)
	{
		//SD_BOTH disables both sends and receives as described above
		//	closesocket(m_nSocket);
		return SOCKET_ERROR;
	}

	if(!GetResponse())
		return FROM_ERR;

	sprintf(To,"RCPT TO: <%s>\r\n",to);//指定收信的信箱
	rt = send(m_nSocket,To,strlen(To),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return SOCKET_ERROR;
	}

	if(!GetResponse())
		return TO_ERR;

	//要发送邮件内容了，如果服务器响应码是354，则认为随后接受到的数据是邮件的内容，但是注意邮件内容有一定的格式
	//其实标准的字段有很多，发一些必须的就行了，够解析就行
	send_data = "DATA\r\n";
	rt = send(m_nSocket,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return SOCKET_ERROR;
	}

	if(!GetResponse())
		return SMTP_ERR;

	sprintf(From,"From: %s\r\n",from);//邮件固定格式，是发送者的姓名
	rt = send(m_nSocket,From,strlen(From),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return SOCKET_ERROR;
	}

	sprintf(To,"To: %s\r\n",to);//邮件的固定的格式，是接受者的姓名
	rt = send(m_nSocket,To,strlen(To),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return SOCKET_ERROR;
	}

	sprintf(Date,"Date: %s\r\n",date);//邮件的固定的格式，是发送时间
	rt = send(m_nSocket,Date,strlen(Date),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return SOCKET_ERROR;
	}

	sprintf(Subject,"Subject: %s\r\n",subject);//邮件的格式，是邮件的主题
	rt = send(m_nSocket,Subject,strlen(Subject),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return SOCKET_ERROR;
	}

	send_data = "\r\n";//邮件的最后一个空行
	rt = send(m_nSocket,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return SOCKET_ERROR;
	}

	rt = send(m_nSocket,(char *)data,strlen(data),0);//邮件正文
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return SOCKET_ERROR;
	}

	send_data = "\r\n";//邮件的最后一个空行，就是为了美观一下
	rt = send(m_nSocket,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return SOCKET_ERROR;
	}

	return SMTP_OK;
}

bool CSmtp::End()
{
	send_data = "\r\n";
	rt = send(m_nSocket,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return false;
	}

	send_data = "--The email from monitor center --\r\n"; //美观
	rt = send(m_nSocket,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return false;
	}

	send_data = "\r\n.\r\n"; //邮件真正的结束标志
	rt = send(m_nSocket,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return false;
	}

	if(!GetResponse())
		return false;

	send_data = "QUIT\r\n";//用来中止与SMTP的会话
	rt = send(m_nSocket,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return false;
	}

	if(!GetResponse())
		return false;

	return true;//正常结束了
}

bool CSmtp::DisplayError(const char *rev)
{
	int n = atoi(rev);

	switch (n)
	{
	case 220:
		m_RetStr = "服务就绪";
		break;
	case 221:
		m_RetStr = "服务关闭";
		break;
	case 250:
		m_RetStr = "要求的邮件操作完成";
		break;
	case 354:
		m_RetStr = "开始邮件正文";
		break;
	case 421:
		m_RetStr = "服务未就绪，关闭传输信道";
		m_status = false;
		break;
	case 450:
		m_RetStr = "要求的邮件操作未完成，邮箱不可用";
		m_status = false;
		break;
	case 451:
		m_RetStr = "放弃要求的操作；处理过程中出错";
		m_status = false;
		break;
	case 452:
		m_RetStr = "系统存储不足，要求的操作未执行";
		m_status = false;
		break;
	case 535:
		m_RetStr = "身份验证失败";
		m_status = false;
		break;
	case 550:
		m_RetStr = "要求的邮件操作未完成，邮箱不可用";
		m_status = false;
		break;
	case 551:
		m_RetStr = "用户非本地，请尝试";
		m_status = false;
		break;
	case 552:
		m_RetStr = "过量的存储，要求的操作未执行";
		m_status = false;
		break;
	case 553:
		m_RetStr = "邮箱名不可用，要求的操作未执行";
		m_status = false;
		break;
	case 554:
		m_RetStr = "操作失败";
		m_status = false;
		break;
	case 501:
		m_RetStr = "参数格式错误";
		m_status = false;
		break;
	case 502:
		m_RetStr = "命令不可实现";
		m_status = false;
		break;
	case 503:
		m_RetStr = "错误的命令序列";
		m_status = false;
		break;
	case 504:
		m_RetStr = "命令参数不可实现";
		m_status = false;
		break;
	}

	return m_status;
}
