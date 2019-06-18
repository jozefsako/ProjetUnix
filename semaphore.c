#include "semaphore.h"

void initSemaphore(){
	union semun{
		int val;
		struct semid_ds *buf;
		unsigned short *array;
		struct seminfo *__buf;
	}semunion;
	semunion.val = 1;
	int semid = semget(1000, 1, IPC_CREAT | 0660);
	if(semctl(semid, 0, SETVAL, semunion) < 0){
		printf("Echec init semaphore\n");
		exit(1);
	}
}

void supprimerSemaphore(){
	int semid = semget(1000, 1, 0660);
	semctl(semid, 0, IPC_RMID, NULL);
}

void downSemaphore(){
	struct sembuf down;
	down.sem_num = 0;
	down.sem_op = -1;
	down.sem_flg = 0;
	int semid = semget(1000, 1, 0660);
	if(semop(semid, &down, 1) < 0){
		printf("Echec down semaphore\n");
		exit(1);
	}
}

void upSemaphore(){
	struct sembuf up;
	up.sem_num = 0;
	up.sem_op = 1;
	up.sem_flg = 0;
	int semid = semget(1000, 1, 0660);
	if(semop(semid, &up, 1) < 0){
		printf("Echec up semaphore\n");
		exit(1);
	}
}