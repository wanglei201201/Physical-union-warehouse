#include "smart_menu.h"

#define N 1024 

extern pthread_mutex_t	mutex_client_request,	
	   mutex_refresh,			
	   mutex_sqlite,		
	   mutex_transfer,			
	   mutex_analysis,			
	   mutex_led,			
	   mutex_buzzer,			
	   mutex_fan,			
	   mutex_camera;			

extern pthread_cond_t 	cond_client_request,
	   cond_refresh,
	   cond_sqlite,
	   cond_transfer,
	   cond_analysis,
	   cond_led,
	   cond_buzzer,
	   cond_fan,
	   cond_camera;


extern	int msgid;	
extern	int shmid;	
extern	int semid;	



extern struct house_info all_info;	



struct shm_addr{
	char cgi_status;
	char qt_status;
	struct house_info rt_status;
};

struct house_info *shm_buf;

union semun{
	int 				val;		
	struct semid_ds 	*buf;		
	unsigned short 		*array;		
};

int init_sem(int sem_id, int init_val)
{
	union semun  sem_union;
	sem_union.val = init_val;

	if(semctl(sem_id, 0, SETVAL, sem_union) == -1){
		perror("fail to semctl");
		exit(1);
	}

	return 0;
}


int sem_p(int sem_id)
{
	struct sembuf sem_buf;
	sem_buf.sem_num = 0;	
	sem_buf.sem_op	= -1;	
	sem_buf.sem_flg = SEM_UNDO;		


	if(semop(sem_id, &sem_buf, 1) == -1){
		perror("fail to semop  P");
		exit(1);
	}
	return 0;
}


int sem_v(int sem_id)
{
	struct sembuf sem_buf;
	sem_buf.sem_num = 0;	
	sem_buf.sem_op	= 1;	
	sem_buf.sem_flg = SEM_UNDO;		


	if(semop(sem_id, &sem_buf, 1) == -1){
		perror("fail to semop V");
		exit(1);
	}
	return 0;
}


void *pthread_refresh(void * args)//共享内存数据刷新线程
{
	key_t sem_key,shm_key;
	sem_key = ftok("./", 'i');
	if(sem_key < 0){
		perror("fail to ftok");
		exit(-1);
	}


	semid = semget(sem_key, 1, IPC_CREAT | IPC_EXCL | 0666);
	if(semid == -1){
		if(errno == EEXIST){
			semid = semget(sem_key, 1, 0777);
		}
		else{
			perror("fail to semget");
			exit(1);
		}
	}
	else{
		init_sem(semid, 1);
	}

	shm_key = ftok("./", 'i');
	if(shm_key < 0){
		perror("shary fail to ftok");
		exit(-1);
	}



	shmid = shmget(shm_key, N, IPC_CREAT | IPC_EXCL | 0666);
	if(shmid == -1){
		if(errno == EEXIST){
			shmid = shmget(shm_key, N, 0777);
		}
		else{
			perror("fail to shmget");
			exit(1);
		}
	}


	shm_buf = (struct house_info *)shmat(shmid, NULL, 0);
	if(shm_buf == (void *)-1){
		perror("fail to shmat");
		exit(1);
	}

	printf("pthread_refresh is working >>>>>>>\n");

	while(1){

		pthread_mutex_lock(&mutex_refresh);
		pthread_cond_wait(&cond_refresh, &mutex_refresh);

		sem_p(semid);
		*shm_buf = all_info;
		printf("-----temp: %.2f\n",shm_buf->temperature);
		sleep(1);
		sem_v(semid);

		pthread_mutex_unlock(&mutex_refresh);

	}
}


