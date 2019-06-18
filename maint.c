#include "maint.h"

int main(int argc, char** argv){
	if(argc < 2 || argc > 3){
		perror("Mauvaise utilisation du maint\n");
		exit(0);
	}
	if(argc == 2){
		if(atoi(argv[1]) == 1) creer();
		else supprimer();
	}else{
		reserver(atoi(argv[2]));
	}
}

void creer(){
	creerMemoire();
	initSemaphore();
}

void supprimer(){
	supprimerMemoire();
	supprimerSemaphore();
}

void reserver(int temps){
	downSemaphore();
	sleep(temps);
	upSemaphore();
}