/************************************************************
 *Copyright (c) 2013-2015 深圳市赛柏达技术有限公司技术研发部
 *
 *FileName:		configfile.h       
 *Writer:		smart-skynet
 *create Date:  2015/01/01
 *Rewriter:		kason
 *Rewrite Date:	2015/09/28
 *Impact:
 *
 *Main Content(Function Name、parameters、returns)
 *
 ************************************************************/

#ifndef FILE_H
#define FILE_H

#include <QString>
#include <QSettings>
#include <stdlib.h>
#include <QDir>

#include "common/define.h"


const QString CONFIG_FILE_NAME = "/monitor.cfg";

class  CCfgFile
{
public:
    CCfgFile();
    ~CCfgFile();
    // set initial value
    void SaveMonitor(struCseMonitor * cseMonitor);
    void SavePrdID(QString strPrdID);

private:
    void ReadFile();        // 读文件
    void VerifyData();      // 数据校验
    void SaveFile();        // 写文件

private:
    QSettings *m_setting;

public:
    // uart config
    QString m_strDev;
    unsigned int m_nBaud;

    QString strDev_for_upper_computer;
    unsigned int baud_2;

    // server config
    QString m_strIP;
    unsigned int m_nPort;

    unsigned int m_nMaxIDL;
    unsigned int m_nMaxfil;
    unsigned int m_nSendGSM_TIME;

    // system config
    unsigned int m_nSampTime;

    // config max val
 //   QString m_strPrdID;
    QString m_strPrdID;
    QString m_strMonitor;

    unsigned int m_nVol;
    float m_nMaxENVTMP;
    unsigned int m_nMaxENVHDY;
    float m_nMaxLCDTMP;
    unsigned int m_nMinLCDIDL;
    unsigned int m_nMaxFIL;
    unsigned int m_nFanType;
    unsigned int m_nFanSpeed;
    QString m_strEmail;
    QString m_strTel;

    // gsm config
    QString m_strGsmDev;
    unsigned int m_nGsmBaud;
    unsigned int m_nGsm;

    // smtp config
    QString m_strSmtpSrv;
    QString m_strSmtpFrom;
    QString m_strSmtpUsr;
    QString m_strSmtpPwd;
    unsigned int m_nSmtpPort;
    unsigned int m_nSmtpFlag;
};

#endif // FILE_H
