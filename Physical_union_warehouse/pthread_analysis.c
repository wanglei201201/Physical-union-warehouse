#include "smart_menu.h"

extern  pthread_mutex_t mutex_client_request,		
						mutex_refresh,
						mutex_sqlite,	
						mutex_transfer,	
						mutex_analysis,	
						mutex_led,		
						mutex_buzzer,	
						mutex_M0,	
						mutex_camera;	


extern pthread_cond_t cond_client_request,
	   cond_refresh,
	   cond_sqlite,
	   cond_transfer,
	   cond_analysis,
	   cond_led,
	   cond_buzzer,
	   cond_M0,
	   cond_camera;
extern unsigned char flag;
extern int msgid;
int send_msg_queue(long type, unsigned char text)
{
	struct msg msgbuf;
	msgbuf.type = 1L;
	msgbuf.msgtype = type;
	msgbuf.text[0] = text;
	if(msgsnd(msgid, &msgbuf, sizeof(msgbuf) - sizeof(long), 0) == -1){
		perror("fail to msgsnd type");
		exit(1);
	}

	return 0;
}

void *pthread_analysis(void * args)//数据解析线程
{
	while(1)
	{
		sleep(1);
		pthread_mutex_lock(&mutex_analysis);
		pthread_cond_wait(&cond_analysis, &mutex_analysis);
		printf("pthread_analysis is working ......\n");
		pthread_mutex_unlock(&mutex_analysis);
	}	
}
