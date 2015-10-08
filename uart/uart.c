/************************************************************
 *Copyright (c) 2013-2015 深圳市赛柏达技术有限公司技术研发部
 *
 *FileName:		uart.c		 
 *Writer:		smart-skynet
 *create Date: 	2015/01/01
 *Rewriter:		kason
 *Rewrite Date: 2015/09/28
 *Impact:
 *
 *函数名: isatty																							
 *功  能: 检查设备类型 ， 判断文件描述词是否是为终端机。													
 *用  法: int isatty(int desc);																			
 *返回值：如果参数desc所代表的文件描述词为一终端机则返回1，否则返回0。									
 *																										
 *函数名: tcgetattr																						
 *功  能: 获取与终端相关的参数。参数fd为终端的文件描述符，返回的结果保存在termios 结构体中。				
 *用  法: int tcgetattr(int fd, struct termios *termios_p);																																						*
 *返回值: 成功返回零；失败返回非零，发生失败接口将设置errno错误标识。										
 *	
 *参数:
 *c_iflag：输入模式标志，控制终端输入方式。																
 *c_oflag：输出模式标志，控制终端输出方式。																
 *c_cflag：控制模式标志，指定终端硬件控制信息。															
 *c_lflag: 本地模式标志，控制终端编辑功能。																
 *																											
 *	tcsetattr	设置终端参数																				
 *
 *	cfsetospeed & cfsetispeed		设置波特率
 *
 *	tcflush							作用：清空终端未完成的输入/输出请求及数据。
 *********************************************************************/


#include "uart.h"
 int openport(char *strDev)
{
	//O_NDELAY  在读不到数据或是写入缓冲区已满，会马上return，直到有数据或写入完成
    int fd = open( strDev, O_RDWR | O_NONBLOCK);

    if(fd == -1)
    {
        printf("Open Serial Port Device %s error no %d\n",
                strDev , fd );
        return 0;
    }
	 
    //测试是否为终端设备
    if(isatty(STDIN_FILENO) == 0)
        printf("standard input is not a terminal device\n");
    else
        printf("isatty success!\n");
    printf("fd-open=%d\n",fd);
    return fd;
}

 int setport(int fd, int baud, int databits, int stopbits, int parity)
{
	int baudrate;
    struct termios newtio;

    switch(baud)
    {
    case 300:
        baudrate=B300;
        break;
    case 600:
        baudrate=B600;
        break;
    case 1200:
        baudrate=B1200;
        break;
    case 2400:
        baudrate=B2400;
        break;
    case 4800:
        baudrate=B4800;
        break;
    case 9600:
        baudrate=B9600;
        break;
    case 19200:
        baudrate=B19200;
        break;
    case 38400:
        baudrate=B38400;
    case 57600:
        baudrate=B57600;
        break;
    case 115200:
        baudrate=B115200;
        break;
    default :
        baudrate=B9600;
   		break;
    }

    tcgetattr(fd,&newtio);
    bzero(&newtio,sizeof(newtio));

	//字符长度掩码（传送或接收字元时用的位数）。 取值为CS5（传送或接收字元时用5bits）, CS6, CS7, 或 CS8。
    newtio.c_cflag &= ~CSIZE;  

    switch (databits) 			//设置数据位数
    {
    case 7:
        newtio.c_cflag |= CS7; 	//7位数据位
        break;
    case 8:
        newtio.c_cflag |= CS8; 	//8位数据位
        break;
    default:
        newtio.c_cflag |= CS8;
        break;
    }

    switch (parity) 			//设置校验
    {
    case 'n':
    case 'N':
        newtio.c_cflag &= ~PARENB;   	// Clear parity enable 
        newtio.c_iflag &= ~INPCK;     	// Enable parity checking 
        break;
    case 'o':
    case 'O':
        newtio.c_cflag |= (PARODD | PARENB); // 设置为奇效验
        newtio.c_iflag |= INPCK;             // Disnable parity checking 
        break;
    case 'e':
    case 'E':
        newtio.c_cflag |= PARENB;     	// Enable parity 
        newtio.c_cflag &= ~PARODD;  	// 转换为偶效验
        newtio.c_iflag |= INPCK;      	// Disnable parity checking 
        break;
    case 'S':
    case 's':  /*as no parity*/
        newtio.c_cflag &= ~PARENB;
        newtio.c_cflag &= ~CSTOPB;
		break;
    default:
        newtio.c_iflag &= ~INPCK;     // Enable parity checking
        break;
    }

    switch (stopbits)					//设置停止位
    {
    case 1:
        newtio.c_cflag &= ~CSTOPB;  //1
        break;  
    case 2:
        newtio.c_cflag |= CSTOPB;  //2
        break;
    default:
        newtio.c_cflag &= ~CSTOPB;
        break;
    }

    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 13;
    newtio.c_cflag   |=   (CLOCAL|CREAD);
    newtio.c_oflag|=OPOST;
    newtio.c_iflag   &=~(IXON|IXOFF|IXANY);
    cfsetispeed(&newtio,baudrate);
    cfsetospeed(&newtio,baudrate);
    tcflush(fd, TCIFLUSH);		// 清除正收到的数据，且不会读取出来。

    if (tcsetattr(fd,TCSANOW,&newtio) != 0)
    { 
        return -1;
    }

    return 0;
}


 int readport(int fd, char *buf, int maxLen)	// 读数据，参数为串口，BUF，长度
{
    int nLen = read(fd, buf, maxLen);
    if(nLen<0)
    {
        return -1;
    }
    return nLen;
}

 int writeport(int fd, char *buf, int len)  	// 发送数据
{
    int wrnum = 0;

    wrnum = write(fd, buf, len);

    if(wrnum == len)
    {
        return wrnum;

    }
    else
    {
        return -1;
    }

}

// 关闭串口
 void closeport(int fd)
{
    close(fd);
}


