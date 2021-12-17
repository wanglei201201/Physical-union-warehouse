#include "smart_menu.h"
extern pthread_cond_t cond_uart_cmd;
extern pthread_mutex_t mutex_uart_cmd;
extern unsigned char dev_uart_mask;

void serial_init(int fd)
{
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
}

void *pthread_uart_cmd(void *args)//命令发送线程
{
	int fd;
	//int ret;

	//struct msg msgbuf;


	fd = open("/dev/ttyUSB0",O_RDWR);
	if(fd == -1){
		perror("OPEN_SOUND_SYSTEM error");
		exit(-1);
	}

    printf("cmd2 :%d\n",dev_uart_mask);
	serial_init(fd);
	while(1)
    {
        pthread_mutex_lock(&mutex_uart_cmd);
		if(dev_uart_mask == 0)
           pthread_cond_wait(&cond_uart_cmd,&mutex_uart_cmd);
		pthread_mutex_unlock(&mutex_uart_cmd);
        while(dev_uart_mask)
        {
            write(fd,&dev_uart_mask,sizeof(dev_uart_mask));
            dev_uart_mask = 0;
        }
	}
	close(fd);
}

