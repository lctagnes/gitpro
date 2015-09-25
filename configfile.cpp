#include "configfile.h"
#include <QDebug>
CCfgFile::CCfgFile()
{
    QString strPath;
    QDir dir;
    strPath = dir.currentPath();

    m_setting = new QSettings(strPath + CONFIG_FILE_NAME, QSettings::IniFormat);
    ReadFile();
    VerifyData();
}

CCfgFile::~CCfgFile()
{
    if(NULL != m_setting)
    {
        delete m_setting;
        m_setting = NULL;
    }
}

void CCfgFile::ReadFile()
{
    m_strDev = m_setting->value("serialport/DEV").toString().trimmed();
    m_nBaud = m_setting->value("serialport/BAUD").toInt();

    w_strDev = m_setting->value("serialport/DEV2").toString().trimmed();
    w_nBaud = m_setting->value("serialport/BAUD2").toInt();

    m_strIP = m_setting->value("server/IP").toString().trimmed();
    m_nPort = m_setting->value("server/PORT").toInt();

    m_nMaxIDL = m_setting->value("sys/MAXIDL").toInt();
    m_nMaxfil = m_setting->value("sys/MAXfil").toInt();
    m_nSendGSM_TIME = m_setting->value("sys/SENDGSM_TIME").toInt();
//    m_strPrdID = m_setting->value("sys/PRDID").toString().trimmed();

    m_strPrdID = m_setting->value("initial/PRDID").toString().trimmed();
    m_strMonitor = m_setting->value("initial/MONITOR").toString().trimmed();
    m_nVol = m_setting->value("initial/VOL").toInt();
    m_nSampTime = m_setting->value("initial/SAMPTIME").toInt();
    m_nMaxENVTMP = m_setting->value("initial/MAXENVTMP").toFloat();
    m_nMaxENVHDY = m_setting->value("initial/MAXENVHDY").toInt();
    m_nMaxLCDTMP = m_setting->value("initial/MAXLCDTMP").toFloat();
    m_nMinLCDIDL = m_setting->value("initial/MINLCDIDL").toInt();
    m_nMaxFIL = m_setting->value("initial/MAXFIL").toInt();
    m_nFanType = m_setting->value("initial/FANTYPE").toInt();
    m_nFanSpeed = m_setting->value("initial/FANSPEED").toInt();
    m_strEmail = m_setting->value("initial/EMAIL").toString().trimmed();
    m_strTel = m_setting->value("initial/TEL").toString().trimmed();

    m_strGsmDev = m_setting->value("gsm/DEV").toString().trimmed();
    m_nGsmBaud = m_setting->value("gsm/BAUD").toInt();
    m_nGsm = m_setting->value("gsm/GSM").toInt();

    //m_nGsm = m_setting->value("gsm/GSM").toInt();

    m_strSmtpSrv = m_setting->value("smtp/SMTPSERVER").toString().trimmed();
    m_strSmtpFrom = m_setting->value("smtp/SMTPFROM").toString().trimmed();
    m_strSmtpUsr = m_setting->value("smtp/SMTPUSR").toString().trimmed();
    m_strSmtpPwd = m_setting->value("smtp/SMTPPWD").toString().trimmed();
    m_nSmtpPort = m_setting->value("smtp/SMTPPORT").toInt();
    m_nSmtpFlag = m_setting->value("smtp/SMTPFLAG").toInt();
}

void CCfgFile::VerifyData()
{
    if(m_strGsmDev.isEmpty())
    {
        m_strDev = "/dev/ttyS1";
    }

    if(0 == m_nBaud)
    {
        m_nBaud = 115200;
    }

    if(0 == w_nBaud)
    {
        m_nBaud = 9600;
    }

    if(0 == m_nSampTime)
    {
        m_nSampTime = 20000;
    }

    if(m_strIP.isEmpty())
    {
        m_strIP = "127.0.0.1";
    }

    if(0 == m_nPort)
    {
        m_nPort = 5566;
    }

    if(0 == m_nMaxIDL)
    {
        m_nMaxIDL = 500;
    }
    if(0 == m_nMaxfil)
    {
        m_nMaxfil = 500;
    }
    if(0 == m_nSendGSM_TIME)
    {
        m_nSendGSM_TIME = 30;
    }

    if(m_strMonitor.isEmpty())
    {
        m_strMonitor = "Monitor";
    }

    if(0 == m_nVol)
    {
        m_nVol = 12;
    }

    if(0 == m_nMaxENVTMP)
    {
        m_nMaxENVTMP = 60.0;
    }

    if(0 == m_nMaxENVHDY)
    {
        m_nMaxENVHDY = 80;
    }

    if(0 == m_nMaxLCDTMP)
    {
        m_nMaxLCDTMP = 60;
    }    

    if(0 == m_nMinLCDIDL)
    {
        m_nMinLCDIDL = 20;
    }

    if(0 == m_nMaxFIL)
    {
        m_nMaxFIL = 80;
    }

    if(0 == m_nFanType)
    {
        m_nFanType = 30;
    }
}

void CCfgFile::SaveFile()
{

    m_setting->setValue("serialport/DEV", m_strDev);
    m_setting->setValue("serialport/BAUD", m_nBaud);

    m_setting->setValue("serialport/DEV2", w_strDev);
    m_setting->setValue("serialport/BAUD2", w_nBaud);

    m_setting->setValue("server/IP", m_strIP);
    m_setting->setValue("server/PORT", m_nPort);

    m_setting->setValue("sys/MAXIDL", m_nMaxIDL);
    m_setting->setValue("sys/MAXfil", m_nMaxfil);
    m_setting->setValue("sys/SENDGSM_TIME",m_nSendGSM_TIME);

    m_setting->setValue("initial/PRDID", m_strPrdID);
    m_setting->setValue("initial/MONITOR", m_strMonitor);
    m_setting->setValue("initial/VOL", m_nVol);
    m_setting->setValue("initial/SAMPTIME", m_nSampTime);
    m_setting->setValue("initial/MAXENVTMP", m_nMaxENVTMP);
    m_setting->setValue("initial/MAXENVHDY", m_nMaxENVHDY);
    m_setting->setValue("initial/MAXLCDTMP", m_nMaxLCDTMP);

    m_setting->setValue("initial/MINLCDIDL", m_nMinLCDIDL);
    m_setting->setValue("initial/MAXFIL", m_nMaxFIL);
    m_setting->setValue("initial/FANTYPE", m_nFanType);
    m_setting->setValue("initial/FANSPEED", m_nFanSpeed);
    m_setting->setValue("initial/EMAIL", m_strEmail);
    m_setting->setValue("initial/TEL", m_strTel);

    m_setting->setValue("gsm/DEV", m_strGsmDev);
    m_setting->setValue("gsm/BAUD", m_nGsmBaud);
    m_setting->setValue("gsm/GSM", m_nGsm);
    //m_setting->setValue("gsm/GSM", m_nGsm);

    m_setting->setValue("smtp/SMTPSERVER", m_strSmtpSrv);
    m_setting->setValue("smtp/SMTPFROM", m_strSmtpFrom);
    m_setting->setValue("smtp/SMTPUSR", m_strSmtpUsr);
    m_setting->setValue("smtp/SMTPPWD", m_strSmtpPwd);
    m_setting->setValue("smtp/SMTPPORT", m_nSmtpPort);
    m_setting->setValue("smtp/SMTPFLAG", m_nSmtpFlag);
}

void CCfgFile::SaveMonitor(struCseMonitor * cseMonitor)
{
    m_strMonitor = cseMonitor->monitor;
    m_nVol = cseMonitor->vol;
    m_nSampTime = cseMonitor->samptime;
    m_nMaxENVTMP = cseMonitor->max_env_tmp;
    m_nMaxENVHDY = cseMonitor->max_env_hdy;
    m_nMaxLCDTMP = cseMonitor->max_lcd_tmp;
    m_nMinLCDIDL = cseMonitor->min_lcd_idl;
    m_nMaxFIL = cseMonitor->max_fil;
    m_nFanType = cseMonitor->fan_type;
    m_nFanSpeed = cseMonitor->fan_speed;
    m_strEmail = cseMonitor->email;
    m_strTel = cseMonitor->tel;

    m_setting->setValue("initial/MONITOR", m_strMonitor);
    m_setting->setValue("initial/VOL", m_nVol);
    m_setting->setValue("initial/SAMPTIME", m_nSampTime);
    m_setting->setValue("initial/MAXENVTMP", m_nMaxENVTMP);
    m_setting->setValue("initial/MAXENVHDY", m_nMaxENVHDY);
    m_setting->setValue("initial/MAXLCDTMP", m_nMaxLCDTMP);
    m_setting->setValue("initial/MINLCDIDL", m_nMinLCDIDL);
    m_setting->setValue("initial/MAXFIL", m_nMaxFIL);
    m_setting->setValue("initial/FANTYPE", m_nFanType);
    m_setting->setValue("initial/FANSPEED", m_nFanSpeed);
    m_setting->setValue("initial/EMAIL", m_strEmail);
    m_setting->setValue("initial/TEL", m_strTel);
}

void CCfgFile::SavePrdID(QString strPrdID)
{
    m_strPrdID = strPrdID;
    m_setting->setValue("initial/PRDID", m_strPrdID);
}
