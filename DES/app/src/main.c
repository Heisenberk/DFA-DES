/**
 * \file main.c
 * \brief Représente les fonctions concernant l'attaque DFA sur DES. 
 * \author Clément CAUMES
 * */

#include <stdio.h>
#include <stdlib.h>

#include "../inc/attack.h"
#include "../inc/constants.h"
#include "../inc/errors.h"

/**
 * \fn int main
 * \brief Fonction principale du programme qui réalise l'attaque.  
 * \return renvoie EXIT_FAILURE en cas d'erreur et EXIT_SUCCESS sinon. 
 */
int main(){

	// Initialisation des données avec un clair, un chiffré vrai et 32 chiffrés fautés. 
	DATA data = initialize_data();

	// Recherche de K16 (48 bits).
	if (find_K16(&data))
		return err_print(des_errno), EXIT_FAILURE;

	// Recherche de K (64 bits).
	if (find_K(&data))
		return err_print(des_errno), EXIT_FAILURE;

	return EXIT_SUCCESS;
}
