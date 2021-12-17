#ifndef _SMART_MENU_H_
#define _SMART_MENU_H_


#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#define QUEUE_MSG_LEN		32

#define MSG_LED				1L
#define MSG_LED_TEM_ON		0x22
#define MSG_LED_TEM_OFF		0x20
#define MSG_LED_HUM_ON		0x44
#define MSG_LED_HUM_OFF		0x40

#define MSG_BEEP			2L
#define MSG_BEEP_OFF		0
#define MSG_BEEP_ON			1

#define MSG_M0				4L
#define MSG_M0_FAN_OFF		0x00
#define MSG_M0_FAN_ON1		0x01
#define MSG_M0_FAN_ON2		0x02
#define MSG_M0_FAN_ON3		0x03

#define MSG_M0_BEEP_OFF		0x10
#define MSG_M0_BEEP_ON		0x11
#define MSG_M0_BEEP_AU_OFF	0x12
#define MSG_M0_BEEP_AU_ON	0x13

#define MSG_M0_LED_OFF		0x20
#define MSG_M0_LED_ON		0x21

#define MSG_M0_SEG_ON		0x30
#define MSG_M0_SEG_OFF		0x3f

#define MIN_MAX             5L

void *pthread_sqlite(void *);			//数据库线程
void *pthread_analysis(void *);			//数据解析线程
void *pthread_transfer(void *);			//数据接收线程
void *pthread_uart_cmd(void *);			//命令发送线程
void *pthread_client_request(void *);	//接收CGI，QT请求
void *pthread_buzzer (void *);			//蜂鸣器控制线程
void *pthread_led(void *);				//LED控制线程
void *pthread_camera(void *);			//摄像头线程
void *pthread_sms(void *);				//发送短信线程
void *pthread_refresh(void *);       	//共享内存数据刷新线程




struct house_info//所有住宅信息
{
	unsigned char room_status; 
	unsigned char led_status;  //led状态值
	unsigned char buzzer_status; //蜂鸣器状态值
	unsigned char fan_status; //风扇状态值
	unsigned char dsp_status;//数码管状态
	float temperature;   //温度
	float temperatureMIN;
	float temperatureMAX;
	float humidity;     //湿度
	float humidityMIN;
	float humidityMAX;
	unsigned int illumination;
	unsigned int illuminationMIN;
	unsigned int illuminationMAX;
	char x;
	char y;
	char z;
	float adc;	
};

struct msg //命令码
{	
    long type;
    long msgtype;
    unsigned char text[QUEUE_MSG_LEN];
};


struct house_info all_info;

#endif
