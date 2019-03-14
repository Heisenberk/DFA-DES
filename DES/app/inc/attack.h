/**
 * \file attack.h
 * \brief Représente les signatures des fonctions concernant l'attaque DFA sur DES. 
 * \author Clément CAUMES
 * */

#ifndef ATTACK__H
#define ATTACK__H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "key_schedule.h"
#include "manip_bits.h"

/**
 * \struct cipher_Ri
 * \brief structure représentant un chiffré associé à son R15 et son R16.
 */
struct cipher_Ri {
	uint64_t output; /*!< Chiffré. */
	uint32_t R15; /*!< R15 du chiffré. */
	uint32_t R16; /*!< R16 du chiffré. */
}; 
typedef struct cipher_Ri CIPHER_RI;

/**
 * \struct data
 * \brief structure représentant toutes les données utilisées pour 
 * réaliser correctement l'attaque DFA sur le DES.
 */
struct data {
	uint64_t message_clair; /*!< Message clair. */
	CIPHER_RI chiffre_juste; /*!< Chiffré juste. */
	CIPHER_RI chiffre_faux[32]; /*!< 32 chiffrés faux. */
	uint48_t k16; /*!< Sous clé k16 à trouver. */
	uint64_t key; /*!< clé finale à trouver. */
}; 
typedef struct data DATA;


DATA initialize_data();
uint32_t get_R15(uint64_t cipher);
uint32_t get_R16(uint64_t cipher);

int attack_sbox(DATA* data, uint8_t* uint48_t_part, int num_sbox);
int find_K16(DATA* data);

int set_parity_bits(uint64_t* K);
int build_C16_D16(uint48_t k16, uint32_t* C16, uint32_t* D16);
uint64_t build_K56(uint32_t C0, uint32_t D0);
int build_K(uint64_t* K, uint32_t C16, uint32_t D16);
int find_K(DATA* data);


#endif