/************************************************************
 *Copyright (c) 2013-2015 深圳市赛柏达技术有限公司技术研发部
 *
 *FileName:		log.h        
 *Writer:		smart-skynet
 *create Date:  2015/01/01
 *Rewriter:		kason
 *Rewrite Date:	2015/09/28
 *Impact:
 *
 *Main Content(Function Name、parameters、returns)
 *
 ************************************************************/

#ifndef LOG_H
#define LOG_H

#include <QDateTime>
#include <QFile>

const QString LOG_FILE = "log/monitor.log";
const int MAX_LOG_FILE_SIZE = 1024000;

class CLOG
{
public:
    CLOG();

    static void Log(QString strLog);


};

#endif // LOG_H
