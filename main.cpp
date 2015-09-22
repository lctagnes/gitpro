#include <QCoreApplication>

#include "monitor.h"
#include  <stdio.h>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CMonitor monitor;
  //  printf("Hello word !");

    return a.exec();    //进入QCoreApplication事件循环
}
