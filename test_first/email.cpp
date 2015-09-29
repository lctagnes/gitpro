
#include "email.h"

// SendEmail.cpp : implementation file
//
CSendEmail::CSendEmail()
{

}

bool CSendEmail::Initial(CCfgFile &file)
{
	m_strDate = "";
//	m_strTo	= to;

    m_strFrom = file.m_strSmtpFrom;
    m_strHost = file.m_strSmtpSrv;
    m_strUsr = file.m_strSmtpUsr;
    m_strPwd = file.m_strSmtpPwd;
    m_nPort = file.m_nSmtpPort;
	
//	m_strSubject = subject;
//	m_strData = data;

	m_pSmtp = NULL;
	m_pSmtp = new CSmtp();

	if(m_pSmtp->CreateSocket() == SOCKET_ERROR)
	{
        printf("Create smtp socket failed .");
        CLOG::Log("Create smtp socket failed .");
		return false;
	}

//    int ret = m_pSmtp->ConnectHost(m_strHost.toLatin1().data(), m_nPort);
//	if( (ret == CONNECT_HOST_ERR) | !ret )
//	{
//		CLOG::Log("Connect to smtp server failed .");
//		return false;
//	}

//    ret = m_pSmtp->Login(m_strUsr.toLatin1().data(), m_strPwd.toLatin1().data());
//	if( (ret==PWD_ERR) || (ret ==NAME_ERR) || (ret != SMTP_OK) )
//	{
//		return ret;
//	}

	return true;
}

CSendEmail::~CSendEmail()
{
	if(NULL != m_pSmtp)
	{
		delete m_pSmtp;
		m_pSmtp = NULL;
	}
}

void CSendEmail::SetTime()
{	
	struct tm *timenow;
	time_t now;
	time(&now);
	timenow = localtime(&now);
	char buf[20] = {0};

	sprintf(buf,"%d-%d-%d,%d:%d", timenow->tm_year+1900, timenow->tm_mon+1, timenow->tm_mday, timenow->tm_hour, timenow->tm_min );
	m_strDate = buf;

//	m_pSmtp = NULL;

//	return true;  // return true  unless you set the focus to a control
}

int CSendEmail::Send(const char * to , const char * subject, const char * data) 
{
	SetTime(); //设置时间
//	m_pSmtp = NULL;

	/*m_pSmtp = new CSmtp();

	if(m_pSmtp->CreateSocket()==SOCKET_ERR)
	{
		printf("create socket failure\n");
		delete m_pSmtp;
		return SOCKET_ERR;
	}
    */

    int ret = m_pSmtp->ConnectHost(m_strHost.toLatin1().data(), m_nPort);
    if( (ret == CONNECT_HOST_ERR) | (ret != SMTP_OK) )
    {
        return ret;
    }

    ret = m_pSmtp->Login(m_strUsr.toLatin1().data(), m_strPwd.toLatin1().data());
    if( (ret==PWD_ERR) || (ret == NAME_ERR) || (ret != SMTP_OK) )
    {
        return ret;
    }

    ret = m_pSmtp->SendMail(m_strFrom.toLatin1().data(), to, m_strDate.toLatin1().data(), subject, data);
	if( (ret == FROM_ERR) || (ret== TO_ERR )|| (ret != SMTP_OK))
	{	
		return ret;
	}

//	ret = m_pSmtp->End();
	if(false  == (m_pSmtp->End()))
	{
		return QUIT_ERR;
	}

	return SMTP_OK;
}

bool CSendEmail::AutoConn(int nErr)
{
	//for(int i=0; i<3; i++)
	//{
	//	if ( Send() == nErr )
	//	{
	//		Sleep(10);
	//	}
	//	else
	//	{
	//		return true;
	//	}
	//}
		
	return false;
}

void CSendEmail::SendEmail(const char * to , const char * subject, const char * data)
{
	string strErr;

	int nRS = Send(to, subject, data);

    switch (nRS)
	{
	case SOCKET_ERROR:		
		strErr = "Create socket failed .";
		break;
    case SMTP_OK:
		strErr = "Send email success .";
		break;
	case CONNECT_HOST_ERR:
		strErr = "Connect to smtp server failed .";
		break;
	case NAME_ERR:
		strErr = "Login usr error .";
		break;
	case PWD_ERR:
		strErr = "Login password error .";
		break;
	case FROM_ERR:
		strErr = "Send email address error .";
		break;
	case TO_ERR:
		strErr = "Recv email address error .";
		break;
	default:
		strErr = "Send email failed .";
		break;
	}
	
	CLOG::Log((char*)strErr.c_str());
}

