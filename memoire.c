#include "memoire.h"

void creerMemoire(){
	int shmid = shmget(1234, sizeof(int)*1000, IPC_CREAT|0660);
	shmat(shmid, NULL, 0);
	printf("shmid : %d\n", shmid);
}

void supprimerMemoire(){
	int shmid = shmget(1234, sizeof(int)*1000, 0660);
	shmctl(shmid, IPC_RMID, NULL);
}

int opererSurCompte(int id, int solde){
	int shmid = shmget(1234, sizeof(int)*1000, 0660);
	int* compte = shmat(shmid, NULL, 0);
	compte[id] += solde;
	return compte[id];
}

void consulterCompte(int id){
	int shmid = shmget(1234, sizeof(int)*1000, 0660);
	int* compte = shmat(shmid, NULL, 0);
	printf("Compte { ID : %d ; Solde : %d }\n", id, compte[id]);
}
