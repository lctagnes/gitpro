#include "log.h"
#include <QTextStream>
#include <QDebug>

CLOG::CLOG()
{

}

void CLOG::Log(QString strLog)
{
    // If log file size > 1MB , delete log file

    QFile file("log/monitor.log");
    if (false == (file.open(QIODevice::WriteOnly | QIODevice::Text| QIODevice::Append)))
    {
        QTextStream out(&file);
           out << "This file is generated by Qt\n";
           file.close();
        } else {
          qDebug() << "Error";
        }


//    if(file.size() > MAX_LOG_FILE_SIZE)
//    {
//        file.remove();
//    }

    QDateTime time = QDateTime::currentDateTime();  //返回当前日期事件，来自于系统时钟
    QString strTime = time.toString("yyyy-MM-dd hh:mm:ss   ");

    strLog = strTime + strLog + "\r\n";

    file.write(strLog.toLatin1().data(), strLog.length());  //toLatin1: ISO-8859-1编码

    file.close();
}


