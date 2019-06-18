/* Authors : PAUWELS Sébastien - SAKO Jozef - Groupe : 11
 *  maint.h
 *  Header du fichier maint.c
*/
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "semaphore.h"
#include "memoire.h"

#if !defined(_MAINT_H_)
#define _MAINT_H_

/*	Fait appel à 2 methodes creerMemoire() et initSemaphore()
 *	qui permet de creer la zone de mémoire et d'initialiser un semaphore
 *	PRE		:		/
 *	POST	:		/
*/
void creer();

/*	Fait appel à 2 methodes supprimerMemoire() et supprimerSemaphore();
 *	qui permet de supprimer la zone de memoire precedement créee, idem pour le semaphore
 *	PRE		:		/
 *	POST	:		/
*/
void supprimer();

/*	Fait appel à 3 methodes downSemaphore(), sleep() et upSemaphore();
 *	qui reserve la zone memoire avec un semaphore ce qui empeche l'accès aux autres programmes
 *	durant une periode determinée ensuite il libere la ressource.
 *	PRE		:		temps : entier, rep le nombre de secondes
 *	POST	:		/
*/
void reserver(int temps);

#endif
