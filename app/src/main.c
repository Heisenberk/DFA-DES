/**
 * \file main.c
 * \brief Represente les fonctions concernant l'attaque DFA sur DES. 
 * \author Clement CAUMES
 * */

#include <stdio.h>
#include <stdlib.h>

#include "../inc/attack.h"
#include "../inc/constants.h"
#include "../inc/errors.h"

/**
 * \fn int main
 * \brief Fonction principale du programme qui realise l'attaque.  
 * \return renvoie EXIT_FAILURE en cas d'erreur et EXIT_SUCCESS sinon. 
 */
int main(){

	// Initialisation des donnees avec un clair, un chiffre vrai et 32 chiffres fautes. 
	DATA data = initialize_data();

	// Recherche de K16 (48 bits).
	if (find_K16(&data))
		return err_print(des_errno), EXIT_FAILURE;

	// Recherche de K (64 bits).
	if (find_K(&data))
		return err_print(des_errno), EXIT_FAILURE;

	return EXIT_SUCCESS;
}
