#include "smtp.h"
#include<stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <unistd.h>
#include <QDebug>
CSmtp::CSmtp()
{
	m_status = true;
	From      = new char[100];//100�������Ӧ�ù������û������ʼ��ĵ�ַ�ˣ������ټ�
	To        = new char[100];
	Date      = new char[100];
	Subject   = new char[100];
	Filename  = new char[100];

	recv_data = new char[512];//���ڽ���smtp�������ķ�����Ϣ
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

	delete []From;//�����ǧ��������ͷ�new����Ŀռ�
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

std::string CSmtp::Encode(const char * szEncoding, int nSize)//�������ã����Է�����bmp�������ļ�
{
	//Base64�����ַ�����
	std::string m_sBase64Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	char *szOutput;
	//����ռ�
	int size = (nSize + 2) / 57 * 2;
	size += nSize % 3 != 0 ? (nSize - nSize % 3 + 3) / 3 * 4 : nSize / 3 * 4;
	szOutput = new char[size + 1];
	memset(szOutput, 0, size + 1);

	const char * szInput = szEncoding;

	int nBitsRemaining = 0, nPerRowCount = 0;//���м���
	register int nBitStorage = 0, nScratch = 0;
	int i, lp, endlCount;

	for(i=0, lp=0, endlCount = 0; lp < nSize; lp++)
	{
		nBitStorage = (nBitStorage << 8) | (szInput[lp] & 0xff);//1 byte//the lowest-byte to 0 not cycle
		nBitsRemaining += 8;//����һ���ֽڣ��Ӱ�λ

		do//nBitStorage"ʣ�µ�λ"��¼����
		{
			nScratch = nBitStorage >> (nBitsRemaining - 6) & 0x3f;//�����ǰ����λ
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
		//ԭ��������һ�������ֽ�ʱ���룬����δ����nBitsRemaining!=0
		nScratch = nBitStorage << (6-nBitsRemaining);//��λ��0
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
    sleep(1);//���ʱ�Ӻ���Ҫ��������һ���ʱ���Ū������Ҫ��������٣�������ٿ죬���Բ�Ҫ���ʱ��

	try{
		memset(recv_data, 0, sizeof(recv_data));
		rt = recv(m_nSocket,recv_data,512,0);

		if(rt == SOCKET_ERROR)
			return false;
		recv_data[rt]='\0';	//�����һ����ĸ������Ͻ�������,�����ڽ���֮ǰzeromemoryһ��recv_data
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

	if(*recv_data == '5') //��Ϊ��һ������������5һ�㶼�Ǵ�����룬����ȷ����һ�㶼��2��ͷ��
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

//    long time = 60000;	//��ȡ���߷����׽��ֵĳ�ʱʱ�䣨���룩
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
	char ch[100]; //����
	memset(ch, 0, 100);
	if(username == NULL || password == NULL)//����û��������û�������
	{
		return SMTP_ERR;
	}

	usersrc = username;
	passsrc = password;
	Base64_Code((unsigned char *)usersrc,(unsigned char *)userdes);//����Ҫ���޷��ŵ��ַ���
	Base64_Code((unsigned char *)passsrc,(unsigned char *)passdes);

	send_data = "HELO Localhost\r\n";//���߷������ͻ��˳���Ҫͨ����
	rt = send(m_nSocket,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return SOCKET_ERROR;
	}
	if(!GetResponse())
		return SOCKET_ERROR;

	send_data = "AUTH LOGIN\r\n";//���߷���������Ҫ��¼
	rt = send(m_nSocket,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return SOCKET_ERROR;
	}
	if(!GetResponse())
		return SOCKET_ERROR;

	sprintf(ch,"%s\r\n",userdes);//�û���
	rt = send(m_nSocket,(char *)ch,strlen(ch),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return SOCKET_ERROR;
	}
	if(!GetResponse())
		return NAME_ERR;

	sprintf(ch,"%s\r\n",passdes);	//����="MTI1c2YzX3pwX2Zz"
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

	sprintf(From,"MAIL FROM: <%s>\r\n",from);//ָ�������ߵ�����
	rt = send(m_nSocket,From,strlen(From),0);
	if(rt == SOCKET_ERROR)
	{
		//SD_BOTH disables both sends and receives as described above
		//	closesocket(m_nSocket);
		return SOCKET_ERROR;
	}

	if(!GetResponse())
		return FROM_ERR;

	sprintf(To,"RCPT TO: <%s>\r\n",to);//ָ�����ŵ�����
	rt = send(m_nSocket,To,strlen(To),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return SOCKET_ERROR;
	}

	if(!GetResponse())
		return TO_ERR;

	//Ҫ�����ʼ������ˣ������������Ӧ����354������Ϊ�����ܵ����������ʼ������ݣ�����ע���ʼ�������һ���ĸ�ʽ
	//��ʵ��׼���ֶ��кܶ࣬��һЩ����ľ����ˣ�����������
	send_data = "DATA\r\n";
	rt = send(m_nSocket,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return SOCKET_ERROR;
	}

	if(!GetResponse())
		return SMTP_ERR;

	sprintf(From,"From: %s\r\n",from);//�ʼ��̶���ʽ���Ƿ����ߵ�����
	rt = send(m_nSocket,From,strlen(From),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return SOCKET_ERROR;
	}

	sprintf(To,"To: %s\r\n",to);//�ʼ��Ĺ̶��ĸ�ʽ���ǽ����ߵ�����
	rt = send(m_nSocket,To,strlen(To),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return SOCKET_ERROR;
	}

	sprintf(Date,"Date: %s\r\n",date);//�ʼ��Ĺ̶��ĸ�ʽ���Ƿ���ʱ��
	rt = send(m_nSocket,Date,strlen(Date),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return SOCKET_ERROR;
	}

	sprintf(Subject,"Subject: %s\r\n",subject);//�ʼ��ĸ�ʽ�����ʼ�������
	rt = send(m_nSocket,Subject,strlen(Subject),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return SOCKET_ERROR;
	}

	send_data = "\r\n";//�ʼ������һ������
	rt = send(m_nSocket,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return SOCKET_ERROR;
	}

	rt = send(m_nSocket,(char *)data,strlen(data),0);//�ʼ�����
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return SOCKET_ERROR;
	}

	send_data = "\r\n";//�ʼ������һ�����У�����Ϊ������һ��
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

	send_data = "--The email from monitor center --\r\n"; //����
	rt = send(m_nSocket,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return false;
	}

	send_data = "\r\n.\r\n"; //�ʼ������Ľ�����־
	rt = send(m_nSocket,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return false;
	}

	if(!GetResponse())
		return false;

	send_data = "QUIT\r\n";//������ֹ��SMTP�ĻỰ
	rt = send(m_nSocket,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		//	closesocket(m_nSocket);
		return false;
	}

	if(!GetResponse())
		return false;

	return true;//����������
}

bool CSmtp::DisplayError(const char *rev)
{
	int n = atoi(rev);

	switch (n)
	{
	case 220:
		m_RetStr = "�������";
		break;
	case 221:
		m_RetStr = "����ر�";
		break;
	case 250:
		m_RetStr = "Ҫ����ʼ��������";
		break;
	case 354:
		m_RetStr = "��ʼ�ʼ�����";
		break;
	case 421:
		m_RetStr = "����δ�������رմ����ŵ�";
		m_status = false;
		break;
	case 450:
		m_RetStr = "Ҫ����ʼ�����δ��ɣ����䲻����";
		m_status = false;
		break;
	case 451:
		m_RetStr = "����Ҫ��Ĳ�������������г���";
		m_status = false;
		break;
	case 452:
		m_RetStr = "ϵͳ�洢���㣬Ҫ��Ĳ���δִ��";
		m_status = false;
		break;
	case 535:
		m_RetStr = "�����֤ʧ��";
		m_status = false;
		break;
	case 550:
		m_RetStr = "Ҫ����ʼ�����δ��ɣ����䲻����";
		m_status = false;
		break;
	case 551:
		m_RetStr = "�û��Ǳ��أ��볢��";
		m_status = false;
		break;
	case 552:
		m_RetStr = "�����Ĵ洢��Ҫ��Ĳ���δִ��";
		m_status = false;
		break;
	case 553:
		m_RetStr = "�����������ã�Ҫ��Ĳ���δִ��";
		m_status = false;
		break;
	case 554:
		m_RetStr = "����ʧ��";
		m_status = false;
		break;
	case 501:
		m_RetStr = "������ʽ����";
		m_status = false;
		break;
	case 502:
		m_RetStr = "�����ʵ��";
		m_status = false;
		break;
	case 503:
		m_RetStr = "�������������";
		m_status = false;
		break;
	case 504:
		m_RetStr = "�����������ʵ��";
		m_status = false;
		break;
	}

	return m_status;
}
