/**
 * \file feistel.c 
 * \brief Représente les fonctions concernant le processus de Feistel du DES. 
 * \author Clément CAUMES
 * */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../inc/manip_bits.h"
#include "../inc/errors.h"
#include "../inc/inner_function.h"
#include "../inc/key_schedule.h"
#include "../inc/constants.h"

/*
 * Constante IP
 */
int IP[64] = { 58, 50, 42, 34, 26, 18, 10, 2,
               60, 52, 44, 36, 28, 20, 12, 4,
               62, 54, 46, 38, 30, 22, 14, 6,
               64, 56, 48, 40, 32, 24, 16, 8,
               57, 49, 41, 33, 25, 17, 9, 1,
               59, 51, 43, 35, 27, 19, 11, 3, 
               61, 53, 45, 37, 29, 21, 13, 5,
               63, 55, 47, 39, 31, 23, 15, 7 };

/*
 * Constante IP_inv
 */
int IP_inv[64] = { 40, 8, 48, 16, 56, 24, 64, 32,
                   39, 7, 47, 15, 55, 23, 63, 31,
                   38, 6, 46, 14, 54, 22, 62, 30,
                   37, 5, 45, 13, 53, 21, 61, 29,
                   36, 4, 44, 12, 52, 20, 60, 28,
                   35, 3, 43, 11, 51, 19, 59, 27,
                   34, 2, 42, 10, 50, 18, 58, 26,
                   33, 1, 41, 9, 49, 17, 57, 25 };

/**
 * \fn int process_permutation(uint64_t* data, int* IP)
 * \brief Fonction qui réalise la permutation initiale du DES. 
 * \param *data input du DES. 
 * \param *IP constante de permutation. 
 * \return renvoie 0 en cas de réussite et 1 en cas d'échec. 
 */
int process_permutation(uint64_t* data, int* IP){
	int i;
	uint64_t output=0x00;
	for(i=0;i<64;i++){
		uint8_t bit=get_bit_uint64_t_most(*data,IP[i]);
		if(set_bit_uint64_t(&output, bit, 64-i)) 
			return 1;
	}
	*data=output;
	return 0;
}

/**
 * \fn void build_L0_R0(uint64_t data, uint32_t* L0, uint32_t* R0)
 * \brief Fonction qui construit L0 et R0. 
 * \param data input qui se trouve juste après la permutation initiale. 
 * \param *L0 partie de gauche initialisée. 
 * \param *R0 partie de droite initialisée. 
 * \return renvoie 0 en cas de réussite et 1 en cas d'échec. 
 */
void build_L0_R0(uint64_t data, uint32_t* L0, uint32_t* R0){
	*L0=data>>32;
	data<<=32;
	*R0=data>>32;
}

/**
 * \fn void rebuild_R16_L16(uint64_t* data, uint32_t L16, uint32_t R16)
 * \brief Fonction qui construit L16 et R16. 
 * \param *data output qui se trouve juste avant la permutation initiale. 
 * \param L16 partie de gauche. 
 * \param R16 partie de droite. 
 * \return renvoie 0 en cas de réussite et 1 en cas d'échec. 
 */
void rebuild_R16_L16(uint64_t* data, uint32_t L16, uint32_t R16){
	*data=R16;
	*data<<=32;
	*data|=L16;
}

/**
 * \fn int process_round_1_15_encryption(uint32_t* Li, uint32_t* Ri, uint48_t Kiadd1)
 * \brief Fonction qui réalise les rounds du tour 1 au tour 15 de chiffrement. 
 * \param *Li partie de gauche à modifier. 
 * \param *Ri partie de droite à modifier. 
 * \param Kiadd1 Ki+1 sous clé entrantedu tour. 
 * \return renvoie 0 en cas de réussite et 1 en cas d'échec. 
 */
int process_round_1_15_encryption(uint32_t* Li, uint32_t* Ri, uint48_t Kiadd1){
	uint32_t Liadd1 = *Ri;
    if(inner_function(Kiadd1, Ri)) 
		return 1;
	*Ri= (*Li)^(*Ri); // R(i+1)=Li XOR F(K(i+1), Ri)
	*Li=Liadd1; // L(i+1)=Ri
	return 0;

}

/**
 * \fn int process_round_16_encryption(uint32_t* L15, uint32_t* R15, uint48_t K16)
 * \brief Fonction qui réalise le round du tour 16 de chiffrement. 
 * \param *L15 partie de gauche à modifier. 
 * \param *R15 partie de droite à modifier. 
 * \param K16 sous clé entrante du tour. 
 * \return renvoie 0 en cas de réussite et 1 en cas d'échec. 
 */
int process_round_16_encryption(uint32_t* L15, uint32_t* R15, uint48_t K16){ 
	uint32_t L16 = *R15;
	if(inner_function(K16, R15)) 
			return 1;
	*R15= (*L15)^(*R15); // R(16)=F(K16, R15) XOR L(15)
	*L15=L16; // L(16)=R(15)
	return 0;
}

/**
 * \fn int process_round_1_decryption(uint32_t* L0, uint32_t* R0, uint48_t K16)
 * \brief Fonction qui réalise le round du tour 1 de dechiffrement. 
 * \param *L0 partie de gauche à modifier. 
 * \param *R0 partie de droite à modifier. 
 * \param K16 sous clé entrante du tour. 
 * \return renvoie 0 en cas de réussite et 1 en cas d'échec. 
 */
int process_round_1_decryption(uint32_t* L0, uint32_t* R0, uint48_t K16){
	uint32_t R1=*R0;
	uint32_t L1=*L0;
	*R0=L1; 
	if(inner_function(K16, L0)) 
		return 1;
	*L0=(*L0)^(R1);
	return 0;
}

/**
 * \fn int process_round_2_15_decryption(uint32_t* Li, uint32_t* Ri, uint48_t Ki)
 * \brief Fonction qui réalise le round des tours 2 au tour 15 de dechiffrement. 
 * \param *Li partie de gauche à modifier. 
 * \param *Ri partie de droite à modifier. 
 * \param Ki sous clé entrante du tour. 
 * \return renvoie 0 en cas de réussite et 1 en cas d'échec. 
 */
int process_round_2_15_decryption(uint32_t* Li, uint32_t* Ri, uint48_t Ki){
	uint32_t RiMin1 = *Li;
	if(inner_function(Ki, Li)) //modification de Li
		return 1;
	*Li = (*Ri)^(*Li); //L(i+1)=R(i) XOR f(Li, Ki)
	*Ri = RiMin1; //R(i+1)=L(i)
	return 0;
}

/**
 * \fn int encryption_des(uint64_t* data, uint64_t key_64)
 * \brief Fonction de chiffrement DES. 
 * \param *data qui sera modifié à la sortie. 
 * \param key_64 clé 64 bits entrante du DES. 
 * \return renvoie 0 en cas de réussite et 1 en cas d'échec. 
 */
int encryption_des(uint64_t* data, uint64_t key_64){
	// permutation initiale
	if(process_permutation(data, IP)) 
		return des_errno=ERR_FEISTEL, 1;

	// division en L0 et R0
	uint32_t Li, Ri;
	build_L0_R0(*data, &Li, &Ri);

	// création des 16 sous clés
	KEY key;
	if(key_schedule (&key_64, &key))
		return des_errno=ERR_FEISTEL, 1;

	// Schema de Feistel
	int i=0;
	for(i=0;i<15;i++){
		if(process_round_1_15_encryption(&Li, &Ri, key.sub_key[i])) 
			return des_errno=ERR_FEISTEL, 1;
	}
	if(process_round_16_encryption(&Li, &Ri, key.sub_key[15])) 
		return des_errno=ERR_FEISTEL, 1;

	// permutation initiale inverse
	rebuild_R16_L16(data, Li, Ri);
	if(process_permutation(data, IP_inv)) 
		return des_errno=ERR_FEISTEL, 1;

	return 0;
}

