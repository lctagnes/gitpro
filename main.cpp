/************************************************************
 *Copyright (c) 2013-2015 ���������ش＼�����޹�˾�����з���
 *
 *FileName:		main.cpp        
 *Writer:		smart-skynet
 *create Date:  2015/01/01
 *Rewriter:		kason
 *Rewrite Date:	2015/09/28
 *Impact:
 *
 *Main Content(Function Name��parameters��returns)
 *
 ************************************************************/

#include <QCoreApplication>

#include "monitor.h"
#include  <stdio.h>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CMonitor monitor;

	//����QCoreApplication�¼�ѭ��
    return a.exec();			
}
