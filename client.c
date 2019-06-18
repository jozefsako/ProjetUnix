#include "client.h"
#include "virement.h"

#define READ 0
#define WRITE 1

#define SYS1(call) (call == -1 ? perror("SYSCALL ERROR"), exit(5) : 0);
#define SYS2(call, msg) (call == -1 ? perror("SYSCALL ERROR : "msg"\n"), exit(5) : 0);
#define SYS3(call, msg, code) (call == -1 ? perror("SYSCALL ERROR: "msg"\n"), exit(code) : 0);

pid_t CHILD_G = 0;
pid_t CHILD_D = 0;

int main(int argc, char **argv){

  if(argc!=5){
    fprintf(stderr, "Usage : %s 'adr' 'port' 'num' 'delay' \n",argv[0]);
    exit(EXIT_FAILURE);
  }

  int res;

  /*  Install the signal handler  */
  struct sigaction act;
  act.sa_handler = handler;
  act.sa_flags = 0;
  res = sigemptyset(&act.sa_mask);
  SYS1(res);

  int taille_logique_pid = 0;

  int sock;
  signal(SIGPIPE, handler);

  int r;
  char buffer[BUF_SIZE];
  char msg[BUF_SIZE];
  char msg_recu[BUF_SIZE];

  char *adr = argv[1];
  int port = atoi(argv[2]);

  r = sprintf(buffer,"\n\n-#-\t\t\tMENU\t\t\t-#-\n\n");
  write(1,buffer,r);

  int num = atoi(argv[3]);
  r = sprintf(buffer,"1)\tVotre n°compte : '%d' \n",num);
  write(1,buffer,r);

  int delay = atoi(argv[4]);
  r = sprintf(buffer,"2)\tVirement Récurrent : '%d' sec \n\n",delay);
  write(1,buffer,r);

  int fd[2];
  SYS1(pipe(fd));

  SYS1((CHILD_D = fork()));

  if(CHILD_D != 0){

    SYS1((CHILD_G = fork()));

    if(CHILD_G != 0){
      /*  pere  */

      SYS1(close(fd[READ]));

      while(1){
      	system("clear");
        r = sprintf(buffer,"\n**************************\n*          MENU          *\n**************************\n\n\t+ n2 somme\n\t* n2 somme \n\tq (Quit)\n\nEntrez votre choix : ");
        write(1,buffer,r);

        if(sigaction(SIGINT, &act, NULL) != 0){
          sprintf(msg,"Signal %d (%s) non capturé\n", SIGINT, sys_siglist[SIGINT]);
          int res = write(2, msg, strlen(msg));
          SYS1(res);
          SYS1(close(fd[READ]));
          SYS1(close(fd[WRITE]));
        }


        while(fgets(buffer, BUF_SIZE, stdin)){

          char action = buffer[0];
          struct Virement virement;

          if(action =='q'){

            r = sprintf(buffer, "\nExit\n");
            write(1, buffer, r);

            virement.src = -1;
            virement.dest = -1;
            virement.somme = -1;
            write(fd[WRITE], &virement, sizeof(Virement));

            SYS1(close(fd[WRITE]));
            raise(SIGINT);
            exit(0);

          }else{

            char *n2;
            n2 = strtok(buffer,"+* ");
            int dest = atoi(n2);

            char *somme;
            somme = strtok(NULL, " ");
            double montant = atoi(somme);

            virement.src = num;
            virement.dest = dest;
            virement.somme = montant;

            if(action=='+'){
              /*  VIR DIRECT  */
              initSocket(port, &sock, adr);
              //Dit au serveur qu'il y a que un seul virement à traiter
              nbVirement(sock, 1);
              //Envoie le virement au serveur
              ecriture(sock,virement);
              close(sock);

            }else{
              /*  code virement Récurrent */
              write(fd[WRITE], &virement, sizeof(Virement));
            }
          }
          system("clear");
          r = sprintf(buffer,"\n**************************\n*          MENU          *\n**************************\n\n\t+ n2 somme\n\t* n2 somme \n\tq (Quit)\n\nEntrez votre choix : ");
          write(1,buffer,r);
        }
      }

      wait(NULL);

    }else{
      /* fils gauche : MINUTERIE */
      close(fd[READ]);
      struct Virement vir_bidon;

      vir_bidon.src = -1;
      vir_bidon.dest = -1;
      vir_bidon.somme = -1;

      while(1){
        sleep(delay);
        if((write(fd[WRITE], &vir_bidon, sizeof(vir_bidon)))<0){
          SYS1(close(WRITE));
          break;
        }
      }
      SYS1(close(WRITE));
    }

  }else{
    /*  fils droit : VIR RECURRENT  */
    close(fd[WRITE]);

    struct Virement *tab_virements;

    int taille_logique = 0;
    int taille_physique = 5;

    tab_virements = malloc(taille_physique * sizeof(Virement));
    if(tab_virements == NULL){
      perror("Erreur 01 : Malloc tab_virements KO !\n");
      exit(1);
    }
    struct Virement vir_recu;


    while(read(fd[READ], &vir_recu, sizeof(Virement)) != EOF) {

      if(vir_recu.src != -1){

        if(taille_logique == taille_physique){
          taille_physique *= 2;

          tab_virements = realloc(tab_virements, taille_physique * sizeof(Virement));
          if(tab_virements == NULL){
            perror("Erreur 02 : Realloc tab_virements KO !\n");
            exit(1);
          }
        }

        tab_virements[taille_logique] = vir_recu;
        taille_logique += 1;

        /*  si on veut afficher le virement chez le client */
        //sprintf(buffer,"\nVirement { Src : %d, Dest : %d, Somme : %d } \n\n",vir_recu.src, vir_recu.dest, vir_recu.somme);
        //write(1,buffer,strlen(buffer));

      }else{
        /*  Execution des virements RECURRENT */
        if(taille_logique!=0){
          //Crée le socket avec le serveur
          initSocket(port, &sock, adr);
          //Lui envoie le nombre de virements à traiter
          nbVirement(sock, taille_logique);

          //Lui écrit tout les virements via le socket
          for(int i=0; i<taille_logique; i++){
            ecriture(sock,tab_virements[i]);
          }
          //Ferme le socket
          close(sock);
        }
      }
    }
    SYS1(close(fd[READ]));
  }
}

void handler(int sig){
  char msg[256];
  sprintf(msg, "\nSignal %d (%s) recu \n",sig,sys_siglist[sig]);
  int res = write(1, msg, strlen(msg));

  if(CHILD_D!=0){
    kill(CHILD_D, SIGKILL);
    CHILD_D = 0;
  }
  if(CHILD_G!=0){
    kill(CHILD_G, SIGKILL);
    CHILD_G = 0;
  }
  SYS1(res);
}

void initSocket(int port, int* sock, char* h){
  if( (*sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    perror("Serveur - Socket");
    exit(1);
  }

  /*DOC:	Takes such a name as an argument and returns a pointer to a hostent containing
  information about that host. The field char *h_addr contains the IP address. If this
  structure is NULL, the system could not locate a host with this name. */
  host = gethostbyname(h);

  if(host == NULL){
    fprintf(stderr, "Unknown host\n");
    exit(1);
  }

  /*DOC :	This code sets the fields in serv_addr. Much of it is the same as in the server.
  However, because the field server->h_addr is a character string, we use the function:
  void bcopy(char *s1, char *s2, int length) which copies length bytes from s1 to s2.*/
  bzero((char*)&addr, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  bcopy(host->h_addr, (char*)&addr.sin_addr.s_addr, host->h_length);
  addr.sin_port = htons(port);

  /*DOC :	The connect function is called by the client to establish a connection to the
  server. It takes three arguments, the socket file descriptor, the address of the host to
  which it wants to connect (including the port number), and the size of this address.
  This function returns 0 on success and -1 if it fails.*/
  if(connect(*sock, (struct sockaddr*)&addr, sizeof(addr)) < 0){
    perror("Client - Serveur");
    exit(1);
  }
}

void nbVirement(int sock, int nb){
  int n;
  n = write(sock, &nb, sizeof(int));
  if (n < 0) perror("ERROR writing to socket\n");
}

void ecriture(int sock, struct Virement virement){
  int n;
  n = write(sock, &virement, sizeof(Virement));
  if (n < 0) perror("ERROR writing to socket\n");
}
