/*  Authors : PAUWELS Sébastien - SAKO Jozef - Groupe : 11
 *  client.h
 *  Header du fichier client.c
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <netinet/in.h>
#include "virement.h"

#if !defined (_CLIENT_H_)
#define _CLIENT_H_

#define BUF_SIZE 256

struct sockaddr_in addr;
struct hostent *host;

/*struct  hostent {
  char    *h_name;         official name of host
  char    **h_aliases;    alias list
  int     h_addrtype;     host address type
  int     h_length;       length of address
  char    **h_addr_list;  list of addresses from name server
};*/

/*  routine de traitement des signaux
 *  PRE   :   sig : entier qui reference un signal parmi la liste des signaux
 *  POST  :   void, ne retourne rien mais il kill les processus fils en
 *            fonction du signal recu via leurs pids
 */
void handler(int sig);

/*  initialise le socket pour la communication
 *  PRE   :   port : entier qui contient le numero du port
 *            socket : l'adresse d'un entier qui reference le Socket
 *            h : l'adresse d'une chaine de character qui reference l'adresse IP
 *  POST  :   type void, retourne un code d'erreur en cas d'un host
 *            inconnu sinon connect le Client - Serveur.
 */
void initSocket(int port, int* socket, char* h);

/*  communication du virement sur le socket
 *  PRE   :   socket : entier qui ref le socket
 *            virement : structure de type Virement
 *  POST  :   type void, retourne un code d'erreur en cas d'echec d'ecriture
 *            sur le socket
 */
void ecriture(int socket,struct Virement virement);

/*  communication au serveur le nombre de requetes qu'on veut faire avant de fermer la Connection
 *  PRE   :   socket : entier, ref le socket
 *            nb : entier, nombre de requetes
 *  POST  :   type void, retourne un code d'erreur en cas d'echec d'ecriture
 *            sur le socket
 */
void nbVirement(int socket, int nb);

#endif
