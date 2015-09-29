/************************************************************
 *  函数名：isatty                                            *
 *  功能：检查设备类型，判断文件描述词是否为终端机。                 *
 *  用法：int isatty()int desc;                               *
 *  返回值：如果参数desc所代表的文件描述词为终端机则返回1，否则返回0。 *
 ************************************************************/

#include "uart.h"
#include "common/define.h"
 int openport(char *strDev)
{

     int fd = open( strDev, O_RDWR);//O_NDELAY  在读取不到数据或是写入缓冲区已满，会马上return，直到有数据或写入完成

    // if(fd == -1)
    // {
    //     printf("Open Serial Port Device %s error no %d\n",
     //           strDev , fd );
         //return 0;
    //     return fd;
   //  }
     /*测试是否为终端设备
     if(isatty(STDIN_FILENO) == 0)           // Standard input.
         printf("standard input is not a terminal device\n");
     else
         printf("isatty success!\n");
     printf("fd-open=%d\n",fd);
     return fd;
    */
     return fd;
}

 /*
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
    newtio.c_cflag &= ~CSIZE;

    switch (databits) //设置数据位数
    {
    case 7:
        newtio.c_cflag |= CS7; //7位数据位
        break;
    case 8:
        newtio.c_cflag |= CS8; //8位数据位
        break;
    default:
        newtio.c_cflag |= CS8;
        break;
    }

    switch (parity) //设置校验
    {
    case 'n':
    case 'N':
        newtio.c_cflag &= ~PARENB;   // Clear parity enable
        newtio.c_iflag &= ~INPCK;     // Enable parity checking
        break;
    case 'o':
    case 'O':
        newtio.c_cflag |= (PARODD | PARENB); // 设置为奇效验
        newtio.c_iflag |= INPCK;             // Disnable parity checking
        break;
    case 'e':
    case 'E':
        newtio.c_cflag |= PARENB;     // Enable parity
        newtio.c_cflag &= ~PARODD;   //转换为偶效验
        newtio.c_iflag |= INPCK;       //Disnable parity checking
        break;
    case 'S':
    case 's':  //as no parity
        newtio.c_cflag &= ~PARENB;
        newtio.c_cflag &= ~CSTOPB;
        break;
    default:
        newtio.c_iflag &= ~INPCK;     // Enable parity checking
        break;
    }

    switch (stopbits)//设置停止位
    {
    case 1:
        newtio.c_cflag &= ~CSTOPB;  //1
        break;  //请到HTTp://www.timihome.net访问
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
    tcflush(fd, TCIFLUSH);

    if (tcsetattr(fd,TCSANOW,&newtio) != 0)
    { 
        return -1;
    }

    return 0;
}
*/

 int readport(int fd, char *buf, int maxLen)//读数据，参数为串口，BUF，长度
{
    int nLen = read(fd, buf, maxLen);
    if(nLen<0)
    {
        return -1;
    }
    return nLen;
}

 int writeport(int fd, char *buf, int len)  //发送数据
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

 int wwriteport(int fd, struCseSensor *buf, int len)  //发送数据
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


