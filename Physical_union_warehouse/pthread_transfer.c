#include "smart_menu.h"



typedef struct{
	unsigned char x[10];
	unsigned char y[10];
	unsigned char z[10];
	unsigned char temperature[10];
	unsigned char humidity[10];	
	unsigned char illumination[10];
	unsigned int  adc[10];
}INFO;


void transfer_data(INFO info, char *buf);
void Min_Max_data(void);

extern pthread_cond_t cond_analysis;
extern pthread_mutex_t mutex_analysis;
extern pthread_cond_t cond_refresh;
//extern pthread_mutex_t mutex_refresh;

extern void serial_init(int fd);
/*{
  struct termios options;
  tcgetattr(fd, &options);					//读取终端参数
  options.c_cflag |= ( CLOCAL | CREAD );	//忽略调制调解器线路状态,使用接收器
  options.c_cflag &= ~CSIZE;				//清目前字符长度
  options.c_cflag &= ~CRTSCTS;				//不实用RTS/CTS流控制
  options.c_cflag |= CS8;					//字符长度设置为8
  options.c_cflag &= ~CSTOPB; 				//设置1个标志位
  options.c_iflag |= IGNPAR;				//允许输入奇偶校验
  options.c_iflag &= ~(ICRNL | IXON);		//回车不转为换行,不允许输入时对XON/XOFF流进行控制
  options.c_oflag = 0;
  options.c_lflag = 0;

  cfsetispeed(&options, B115200);			//设置波特率为115200
  cfsetospeed(&options, B115200);
  tcsetattr(fd,TCSANOW,&options);			//设置终端参数
  }*/

void *pthread_transfer (void *arg)//数据接收线程
{
	//int i = 0, len;
	int fd;
	char buf[50] = "";
	char buf1[50];
	INFO info;
	if ((fd = open ("/dev/ttyUSB0", O_RDWR)) < 0)
	{
		perror ("open ttyUSB0");
		exit (-1);
	}
	serial_init(fd);
	printf ("pthread_transfer is ok\n");
	while(1)
	{

		memset(buf, 0, sizeof(buf));

		if(read(fd, buf, sizeof(buf)) > 0){

			sscanf(buf, "%s %s %s %s %s %s %ls", info.temperature,
					info.humidity,
					info.illumination,
					info.x,
					info.y,
					info.z,
					info.adc);

			transfer_data(info,buf1);
			pthread_mutex_lock(&mutex_analysis);
			pthread_cond_signal(&cond_analysis);
			pthread_mutex_unlock(&mutex_analysis);
			pthread_cond_signal(&cond_refresh);
		}

	}


}


void transfer_data(INFO info, char *buf)
{
	memset(buf, 0, strlen(buf));	
	sprintf(buf, "%s", info.temperature);
	all_info.temperature = atof(buf);
	printf("temp = %2.1f\n", all_info.temperature);

	memset(buf, 0, strlen(buf));	
	sprintf(buf, "%s", info.humidity);
	all_info.humidity = atof(buf);

	memset(buf, 0, strlen(buf));	
	sprintf(buf, "%s", info.illumination);
	all_info.illumination = atoi(buf);

	memset(buf, 0, strlen(buf));	
	sprintf(buf, "%s", info.x);
	all_info.x = atoi(buf);

	memset(buf, 0, strlen(buf));	
	sprintf(buf, "%s", info.y);
	all_info.y = atoi(buf);

	memset(buf, 0, strlen(buf));	
	sprintf(buf, "%s", info.z);
	all_info.z = atoi(buf);

	memset(buf, 0, strlen(buf));	
	sprintf(buf, "%ls", info.adc);
	all_info.adc = atof(buf);
}
