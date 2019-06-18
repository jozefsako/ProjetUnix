/*  Authors : PAUWELS Sébastien - SAKO Jozef - Groupe : 11
 *  pdr.h
 *  Header du fichier pdr.c
*/
#if !defined(_PDR_H_)
#define _PDR_H_

/*  ajoute / retire un montant d'un compte bancaire
 *  PRE   :   compte :  entier, represente le compte bancaire
 *            montant : entier, represente la somme d'argent à retirer
 *  POST  :   void, reserve la ressource, consulte l'état du compte
 *            avant et apres le depot/retrait ensuite libere la ressource
 */
void depotEtRetrait(int compte, int montant);

#endif
