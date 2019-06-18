#include "pdr.h"
#include "semaphore.h"
#include "memoire.h"

int main(int argc, char** argv){
	if(argc != 3){
		perror("Mauvaise utilisation du programme pdr.c\n");
		exit(1);
	}
	int compte = atoi(argv[1]);
	int montant = atof(argv[2]);
	depotEtRetrait(compte, montant);
}

void depotEtRetrait(int compte, int montant){
	downSemaphore();
	printf("Début dépot - retrait : \n");
	consulterCompte(compte);
	int res = opererSurCompte(compte, montant);
	printf("Fin dépot - retrait : \n");
	consulterCompte(compte);
	upSemaphore();
}