#ifndef DEFINE_H
#define DEFINE_H

#pragma pack(1)


#define CSE_PRD_ID_LEN 20

#define CSE_MSG_LEN 512
#define CSE_MONITOR_LEN 256

#define CSE_EMAIL_LEN 32
#define CSE_TEL_LEN 21
#define CSE_IP_LEN 20

typedef enum _CSE_MESSAGE
{
    CSE_INVALID_MESSAGE = 0,
    CSE_HEART_REQ_MESSAGE,
    CSE_HEART_RSP_MESSAGE,
    CSE_SENSOR_REQ_MESSAGE,
    CSE_SENSOR_RSP_MESSAGE,
    CSE_MONITOR_REQ_MESSAGE,
    CSE_MONITOR_RSP_MESSAGE,
    CSE_MONITOR_RECVCALLBACK_MESSAGE,
    CSE_MONITOR_CHECKSERVICEIP_MESSAGE,
}CSE_MESSAGE;

typedef enum _CSE_STATUS
{
    CSE_INVALID_STATUS = 0,
    CSE_NORMAL_STATUS,
    CSE_ALARM_STATUS,
}CSE_STATUS;

//typedef struct _struCseHead
//{
//unsigned int msg_id;
//unsigned int msg_len;
//char prd_id[CSE_PRD_ID_LEN];
//}struCseHead;

typedef struct _struCseMsg
{
    //struCseHead head;
    unsigned int msg_id;
    unsigned int msg_len;
    char prd_id[CSE_PRD_ID_LEN];
    char msg[CSE_MSG_LEN];
}struCseMsg;


typedef struct _struCseSensor
{
    unsigned int acc;           // 门禁
    unsigned int bck;           // 背光是否正常
    unsigned int env_hdy;       // 环境湿度
    unsigned int env_hdy_val;   // 环境湿度值
    unsigned int env_tmp;       // 环境温度
    float env_tmp_val;          // 环境温度值
    unsigned int fil;           // 脏污检测
    unsigned int fil_val;       // 脏污检测值
    unsigned int fld;           // 水浸
    unsigned int inf;           // 红外感应
    unsigned int lcd;           // LCD强度
    unsigned int lcd_val;       // LCD强度值
    unsigned int lcd_tmp;       // 面板温度
    float lcd_tmp_val;          // 面板温度值
    unsigned int scr;           // 屏幕是否运行
    unsigned int shk;           // 震动
    unsigned int smk;           // 烟雾

    // Inside fan speed
    unsigned int fan_speed01;  // 风扇速度
    unsigned int fan_speed02;  // 风扇速度
    unsigned int fan_speed03;  // 风扇速度
    unsigned int fan_speed04;  // 风扇速度
    unsigned int fan_speed05;  // 风扇速度
    unsigned int fan_speed06;  // 风扇速度
    unsigned int fan_speed07;  // 风扇速度
    unsigned int fan_speed08;  // 风扇速度

    // Outside fan speed
    unsigned int fan_speed09;  // 风扇速度
    unsigned int fan_speed10;  // 风扇速度
    unsigned int fan_speed11;  // 风扇速度
    unsigned int fan_speed12;  // 风扇速度
    unsigned int fan_speed13;  // 风扇速度
    unsigned int fan_speed14;  // 风扇速度
    unsigned int fan_speed15;  // 风扇速度
    unsigned int fan_speed16;  // 风扇速度
    unsigned int status;	   // 运行状态
}struCseSensor;



typedef struct _struCseMonitor
{
    //char ip[CSE_IP_LEN];            //
    char monitor[CSE_MONITOR_LEN];  //
    unsigned int vol;               // 工作电压
    unsigned int samptime;          // 采样时间
    float max_env_tmp;              // 最大环境温度
    unsigned int max_env_hdy;       // 最大环境湿度
    float max_lcd_tmp;              // 最大面板温度
    //float max_idl;
    unsigned int min_lcd_idl;       // 最小LCD亮度
    unsigned int max_fil;           // 最大脏污程度
    unsigned int fan_type;          //
    unsigned int fan_speed;         //
    char email[CSE_EMAIL_LEN];
    char tel[CSE_TEL_LEN];
}struCseMonitor;


#endif // DEFINE_H
