/* Authors : PAUWELS Sébastien - SAKO Jozef - Groupe : 11
 *  semaphore.h
 *  Header du fichier semaphore.c
*/
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <stdio.h>

#if !defined(_SEMAPHORE_H_)
#define _SEMAPHORE_H_

/*	Reserve une ressource à l'aide d'un semaphore
 *	PRE		:		/
 *	POST	:		void, retourne un code erreur en cas d'echec
*/
void downSemaphore();

/*	Libere une ressource à l'aide d'un semaphore
 *	PRE		:		/
 *	POST	:		void, retourne un code erreur en cas d'echec
*/
void upSemaphore();

/*	Initialise un semaphore
 *	PRE		:		/
 *	POST	:		void, retourne un code erreur en cas d'echec
*/
void initSemaphore();

/*	Supprime un semaphore
 *	PRE		:		/
 *	POST	:		/
*/
void supprimerSemaphore();

#endif
