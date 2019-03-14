/**
 * \file errors.c 
 * \brief Représente les fonctions concernant la gestion des erreurs. 
 * \author Clément CAUMES
 * */

#include <stdio.h>
#include <stdlib.h>

#include "../inc/errors.h"

/* Initialisation. */
enum err_code des_errno = ERR_NONE;

/**
 * \fn void err_print(enum err_code err)
 * \brief Fonction d'affichage de l'erreur lancée.
 *
 * \param err numéro de l'erreur lancée.
 */
void err_print(enum err_code err)
{
    static const char *err_desc[] = {
        /* ERR_NONE */ 					"aucune erreur",
        /* ERR_KEY_SCHEDULE */			"erreur génération des sous clés"
        /* ERR_INNER_FUNCTION */ 		"erreur de la fonction intérieure"
        /* ERR_FEISTEL */ 				"erreur de feistel"
        /* ERR_ATTACK */		        "erreur lors de l'attaque DFA"
        /* ERR_OTHER */ 				"erreur inconnu"
    };

    /* Vérification de la valeur de "err". */
    err = (unsigned int)err <= ERR_OTHER ? err : ERR_OTHER;
    fprintf(stderr, "Erreur %d : %s.\n", err, err_desc[err]);
}
