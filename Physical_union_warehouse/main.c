#include "smart_menu.h"


void release_pthread_resource(int signo); 


pthread_mutex_t mutex_client_request,//定义互斥锁		
				mutex_refresh,	
				mutex_sqlite,	
				mutex_transfer,	
				mutex_analysis,
				mutex_led,		
				mutex_buzzer,	
				mutex_uart_cmd,		
				mutex_camera;	


pthread_cond_t  cond_client_request,//定义条件变量
				cond_refresh,
				cond_sqlite,
				cond_transfer,
				cond_analysis,
				cond_led,
				cond_buzzer,
				cond_uart_cmd,
				cond_camera;

extern struct house_info all_info;

int msgid;   //消息队列id
int shmid;   //共享内存id
int semid;   //信号灯集id


pthread_t id_client_request,//全局定义线程号
		  id_refresh,
		  id_sqlite,
		  id_transfer,
		  id_analysis,
		  id_led,
		  id_buzzer,
		  id_M0,
		  id_camera;



int main(int argc, const char *argv[])
{

	pthread_mutex_init(&mutex_uart_cmd,NULL);//初始化互斥锁
	pthread_mutex_init(&mutex_refresh,NULL);
	pthread_mutex_init(&mutex_analysis,NULL);

	signal(SIGINT,release_pthread_resource);//捕获信号

	pthread_cond_init(&cond_uart_cmd,NULL);//初始化条件变量
	pthread_cond_init(&cond_refresh,NULL);
	pthread_cond_init(&cond_analysis,NULL);


	pthread_create(&id_client_request,NULL,pthread_client_request,NULL);//接收CGI，QT请求
	pthread_create(&id_M0,NULL,pthread_uart_cmd,NULL);//命令发送线程
	pthread_create(&id_refresh,NULL,pthread_refresh,NULL);//共享内存数据刷新线程
	pthread_create(&id_transfer,NULL,pthread_transfer,NULL);//数据接收线程
	pthread_create(&id_analysis,NULL,pthread_analysis,NULL);//数据解析线程

	pthread_join(id_client_request,NULL);  printf("pthread1\n");//阻塞线程
	pthread_join(id_M0,NULL);  printf("pthread8\n");
	pthread_join(id_refresh,NULL);  printf("pthread2\n");
	pthread_join(id_transfer,NULL);  printf("pthread4\n");
	pthread_join(id_analysis,NULL);  printf("pthread5\n");
	return 0;
}

void release_pthread_resource(int signo)//回收资源
{

	pthread_mutex_destroy(&mutex_uart_cmd);
	pthread_mutex_destroy(&mutex_refresh);
	pthread_mutex_destroy(&mutex_analysis);

	pthread_cond_destroy(&cond_uart_cmd);
	pthread_cond_destroy(&cond_refresh);
	pthread_cond_destroy(&cond_analysis);

	pthread_detach(id_client_request);
	pthread_detach(id_M0);
	pthread_detach(id_refresh);
	pthread_detach(id_transfer);
	pthread_detach(id_analysis);
	printf("all pthread is detach\n");

	msgctl(msgid,IPC_RMID,NULL);
	shmctl(shmid,IPC_RMID,NULL);
	semctl(semid,1,IPC_RMID,NULL);

	exit(1);
}
