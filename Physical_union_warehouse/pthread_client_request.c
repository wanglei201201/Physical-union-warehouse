#include "smart_menu.h"

unsigned char dev_led_mask;
unsigned char dev_buzzer_mask;
unsigned char dev_uart_mask;

extern  pthread_mutex_t mutex_client_request,		
		mutex_refresh,	
		mutex_sqlite,	
		mutex_transfer,	
		mutex_analysis,
		mutex_led,		
		mutex_buzzer,	
		mutex_uart_cmd,		
		mutex_camera;	


extern pthread_cond_t cond_client_request,
	   cond_refresh,
	   cond_sqlite,
	   cond_transfer,
	   cond_analysis,
	   cond_led,
	   cond_buzzer,
	   cond_uart_cmd,
	   cond_camera;
extern int msgid;
int Min_Max_msg(struct msg msg_buf);
void *pthread_client_request(void*arg)//接收CGI，QT请求
{
	key_t key;
	struct msg msg_buf;
	if((key = ftok("./", 'g')) < 0)
	{
		perror("ftok");
		exit(1);
	}
	if((msgid = msgget(key, IPC_CREAT|0666)) < 0)
	{
		perror("msgget");
		exit(1);
	}
	while(1)
	{
		msgrcv(msgid, &msg_buf,sizeof(msg_buf)-sizeof(long),1L,0);
		switch (msg_buf.msgtype)
		{
		case MSG_M0:
			pthread_mutex_lock (&mutex_uart_cmd);	
			dev_uart_mask = msg_buf.text[0];
			pthread_mutex_unlock (&mutex_uart_cmd);
			printf("cmd :%d\n",dev_uart_mask);
			pthread_cond_signal (&cond_uart_cmd);     
			break;
		case MIN_MAX:
			Min_Max_msg(msg_buf);
			break;     
		default:
			break;
		}
	}
}


int Min_Max_msg(struct msg msg_buf)
{
	memset(&all_info, 0, sizeof(all_info));
	all_info.temperatureMAX 	= *((unsigned int *)&msg_buf.text[1]);
	all_info.temperatureMIN 	= *((unsigned int *)&msg_buf.text[5]);
	all_info.humidityMAX 	 	= *((unsigned int *)&msg_buf.text[9]);
	all_info.humidityMIN 	 	= *((unsigned int *)&msg_buf.text[13]);
	all_info.illuminationMAX 	= *((unsigned int *)&msg_buf.text[17]);
	all_info.illuminationMIN	= *((unsigned int *)&msg_buf.text[21]);

	printf("set env data\n");

	printf("temMAX: %d\n", *((int *)&msg_buf.text[1]));
	printf("temperatureMAX: %f \n", all_info.temperatureMAX);

	printf("temMIN: %d\n", *((int *)&msg_buf.text[5]));	
	printf("temperatureMIN: %f \n", all_info.temperatureMIN);

	printf("humMAX: %d\n", *((int *)&msg_buf.text[9]));
	printf("humidityMAX: %f \n", all_info.humidityMAX);

	printf("humMIN: %d\n", *((int *)&msg_buf.text[13]));
	printf("humidityMIN: %f \n", all_info.humidityMIN);

	printf("illMAX: %d\n", *((int *)&msg_buf.text[17]));
	printf("illuminationMAX: %d \n", all_info.illuminationMAX);

	printf("illMIN: %d\n", *((int *)&msg_buf.text[21]));
	printf("illuminationMIN: %d \n", all_info.illuminationMIN);

	return 0;
}
