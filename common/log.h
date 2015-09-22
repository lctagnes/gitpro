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
