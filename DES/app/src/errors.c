/**
 * \file errors.c 
 * \brief Présente les erreurs possibles de l'application et gère les 
 * fonctions de jet d'exception. 
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
        /* ERR_BIT */                  "erreur d'identification de bit"
        /* ERR_OTHER */ 				"erreur inconnu"
    };

    /* Vérification de la valeur de "err". */
    err = (unsigned int)err <= ERR_OTHER ? err : ERR_OTHER;
    fprintf(stderr, "Erreur %d : %s.\n", err, err_desc[err]);
}
