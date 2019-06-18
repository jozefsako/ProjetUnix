#include "server.h"

int main(int argc, char** argv){
  int port, n;
  int nbVirementTraite = 0;
  if( argc != 2 ){
    fprintf(stderr,"Usage: %s port\n",argv[0]);
    exit(1);
  }
  port = atoi(argv[1]);
  initServer(&sck, port);

  //TraitementEnCours nous permet de savoir si le serveur est en train de traiter une
  //demande ou non. Si oui, on attends qu'il termine son execution lors du ctrl-c.
  //Sinon on le termine directement.
  traitementEnCours = 0;

  /*  Install the signal handler  */
  struct sigaction act;
  act.sa_handler = handler;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);

  //Charge le ctrl c
  sigaction(SIGINT, &act, NULL);

  while(1 && ! traitementEnCours){

    int sck2 = accepter(sck);
    traitementEnCours = 1;
    //Lit d'abord le nombre de virement à traiter.
    int nb;
    read(sck2, &nb, sizeof(int));
    //Boucle ensuite sur les virements à traiter (Si ce sont des virements récurrents,
    //on garde une connexion ouverte le temps d'envoyer les virements.)
    for(int i = 0; i < nb; i++){
      downSemaphore();
      nbVirementTraite++;
      //Crée le virement
      struct Virement virement = lecture(sck2);

      printf("\n----------------    %d    ----------------\n", nbVirementTraite);

      //Affiche les informations du virement
      printf("\nVirement { src : %d , dest : %d , somme : %d}\n",virement.src,
      virement.dest, virement.somme );

      printf("\nAvant : \n\n");
      consulterCompte(virement.src);
      consulterCompte(virement.dest);

      opererSurCompte(virement.src, -virement.somme);
      opererSurCompte(virement.dest, virement.somme);

      printf("\nApres : \n\n");
      consulterCompte(virement.src);
      consulterCompte(virement.dest);

      upSemaphore();
    }

    close(sck2);
    if(traitementEnCours == 1) traitementEnCours = 0;
    else break;
  }
  close(sck);
}

void initServer(int *sck, int port){

  if( (*sck = socket(AF_INET,SOCK_STREAM,0)) < 0 ){
    perror("server - socket");
    exit(1);
  }

  bzero((char*)&addr,sizeof(struct sockaddr_in));
  addr.sin_family      = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port        = htons(port);

  if( bind(*sck, (struct sockaddr *)&addr, sizeof(addr)) < 0 ){
    perror("server - bind");
    exit(1);
  }

  fprintf(stderr,"\nStart listening for connections..\n");
  listen(*sck,1);
}

int accepter(int sck){
  struct sockaddr_in addr2;
  u_int len2 = sizeof(addr2);
  return accept(sck,(struct sockaddr *)&addr2,&len2);
}

struct Virement lecture(int sck){
  struct Virement virement;
  int n = read(sck, &virement, sizeof(Virement));
  if(n<0){
    perror("Echec read socket ! \n");
  }
  return virement;
}

void handler(int sig){
  char msg[256];
  sprintf(msg, "\nSignal %d (%s) recu \n",sig,sys_siglist[sig]);
  int res = write(1, msg, strlen(msg));
  	if(sig == SIGINT && !ctrlc){
  		ctrlc++;
	  if(traitementEnCours){
	    traitementEnCours = 0;
	    printf("\nDes virements sont encore en cours de traitement, veuillez patienter.\n");
	  }else{
	    printf("Fermeture du serveur en cours.\n");
	    close(sck);
	    exit(0);
	  }
	}
}
