/*  Authors : PAUWELS Sébastien - SAKO Jozef - Groupe : 11
 *  memoire.h
 *  Header du fichier memoire.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>

#if !defined(_MEMOIRE_H_)

/*  cree la zone de memoire pour 1000 comptes
 *  PRE   :   /
 *  POST  :   void, ne retourne rien mais creee la zone memoire et print le shmid
 */
void creerMemoire();

/*  supprime la zone de memoire pour 1000 comptes
 *  PRE   :   /
 *  POST  :   /
 */
void supprimerMemoire();

/*  met à jour le solde du compte
 *  PRE   :   compte : entier, represente un compte bancaire
 *						solde : entier, represente la somme à ajouter au compte
 *	POST  :		retourne le nouveau solde du compte
 */
int opererSurCompte(int compte, int solde);

/*  affiche le solde d'un compte
 *  PRE   :   id : entier, represente le numero de compte
 *  POST  :   void, ne retourne rien mais affiche le solde du compte
 */
void consulterCompte(int id);

#endif
