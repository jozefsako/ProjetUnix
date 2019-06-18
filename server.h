/* Authors : PAUWELS Sébastien - SAKO Jozef - Groupe : 11
 *  server.h
 *  Header du fichier server.c
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "virement.h"
#include "memoire.h"
#include "semaphore.h"

#if !defined (_SERVER_H_)
#define _SERVER_H_

#define BUFSIZE 80

/* DOC : struct sockaddr_in {
 * short   sin_family;
 * u_short sin_port;
 * struct  in_addr sin_addr;
 * char    sin_zero[8]; };
*/
struct sockaddr_in addr;

/*  def de variable globales  */
int traitementEnCours;
int ctrlc;
int sck;

/*	Initialise le serveur
 *	PRE		:		sck : adresse d'un entier, rep l'adresse du socket
 *            port : entier, rep le numero de port
 *	POST	:		void, retourne un code erreur en cas d'echec
 *            sinon affiche un message lorsque serveur commence l'écoute
*/
void initServer(int* sck, int port);

/*	Accepter une connexion entrante sur une socket d'écoute
 *	PRE		:		sck : entier, rep le socket
 *	POST	:		renvoie le descripteur de socket nouvellement connecté,
 *            -1 en cas d'erreur, avec errno.
*/
int accepter(int sck);

/*  Lit sur le socket et renvoie le virement recu
 *	PRE		:		sck : entier, rep le socket
 *	POST	:		renvoie une structure de type Virement,
 *            sinon un message d'erreur
*/
struct Virement lecture(int sck);

/*  Routine de traitement des signaux
 *	PRE		:		sig : entier, rep un signal
 *	POST	:		void, un message avec le signal recu, modifie
 *            une variable globale, sinon il procede à la
 *            fermeture du serveur en cours et exit(0)
*/
void handler(int sig);

#endif
