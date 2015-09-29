
#ifndef SENDEMAIL_H
#define SENDEMAIL_H

#include "smtp.h"
#include "common/log.h"
#include "configfile.h"

class CSendEmail
{
public:
	CSendEmail();	
	~CSendEmail();

    bool Initial(CCfgFile &file);
	void SetTime();
	int Send(const char * to , const char * subject, const char * data);

	void SendEmail(const char * to , const char * subject, const char * data);

//protected:
	// 自动重连接
	bool AutoConn(int nErr);
private:
    QString	m_strDate;
    QString	m_strFrom;
    QString	m_strTo;
    QString	m_strHost;
    QString	m_strSubject;
    QString	m_strData;
    QString	m_strPwd;
    QString	m_strUsr;

	CSmtp *m_pSmtp;
//	int m_nCount;
	int	m_nPort;
};

#endif
