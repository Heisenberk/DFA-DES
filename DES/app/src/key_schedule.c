/**
 * \file key_schedule.c 
 * \brief Représente les fonctions concernant la génération des sous
 * clés. 
 * \author Clément CAUMES
 * */
 
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "../inc/key_schedule.h"
#include "../inc/errors.h"
#include "../inc/manip_bits.h"
#include "../inc/constants.h"

int PC1[] = { 57, 49,  41, 33,  25,  17,  9,
			  1, 58,  50, 42,  34,  26, 18,
			  10,  2,  59, 51,  43,  35, 27,
			  19, 11,   3, 60,  52,  44, 36,
			  63, 55,  47, 39,  31,  23, 15,
			  7, 62,  54, 46,  38,  30, 22,
			  14,  6,  61, 53,  45,  37, 29,
			  21, 13, 5, 28, 20, 12, 4};

int PC2[] = { 14, 17, 11, 24, 1, 5,
              3, 28, 15, 6, 21, 10,
              23, 19, 12, 4, 26, 8,
              16, 7, 27, 20, 13, 2,
              41, 52, 31, 37, 47, 55,
              30, 40, 51, 45, 33, 48,
              44, 49, 39, 56, 34, 53,
              46, 42, 50, 36, 29, 32};


/**
 * \fn int init_C0_D0(KEY* k, uint64_t init)
 * \brief Fonction qui initialise Ci et Di de la structure KEY. 
 * \param *key clef qui possède les champs Ci et Di qui seront initialisés. 
 * \param init état de la clef sur 56 bits après la permutation initiale. 
 * \return renvoie 0 en cas de réussite et 1 en cas d'échec 
 * (change la valeur de l'erreur). 
 */
int init_C0_D0(KEY* key, uint64_t init){
	key->Ci=key->Di=0;
	int i;
	uint8_t bit;
	for(i=1;i<=28;i++){
		bit=get_bit_uint64_t(init, i);
		if(set_bit_uint32_t(&(key->Di), bit, i)) 
				return des_errno=ERR_BIT, 1;
	}

	for(i=29;i<=56;i++){
		bit=get_bit_uint64_t(init, i);
		if(set_bit_uint32_t(&(key->Ci), bit, i-28))
				return des_errno=ERR_BIT, 1;
	}
	return 0;
}

int shift_Ci_Di(uint32_t* val, int times){
	int i;
	for(i=0;i<times;i++){
		uint8_t bit=get_bit_uint64_t(*val, 28);
		(*val)<<=1;
		set_bit_uint32_t(val, 0, 29);
		set_bit_uint32_t(val, bit, 1);
	}
	return 0;
}

int generate_sub_key(SUB_KEY* sub_key, uint32_t Ci, uint32_t Di){
	int i; uint8_t bit;
	sub_key->bytes=0;
	for(i=0;i<48;i++){
		if(PC2[i]<=28){ //Ci
			bit=get_bit_uint32_t_most(Ci, PC2[i]+4);
		}
		else { //Di
			bit=get_bit_uint32_t_most(Di, PC2[i]-28+4);
		}
		if(set_bit_uint64_t(&sub_key->bytes, bit, 48-i)) 
				return des_errno=ERR_BIT, 1;
	}
	return 0;
}

int process_Ci_Di(KEY* key){
	// initialisation de Vi
	int Vi[16]; int i;
	for(i=0;i<16;i++){
		if((i==0)||(i==1)||(i==8)||(i==15)){
			Vi[i]=1;
		}
		else Vi[i]=2;
	}
	for(i=0;i<16;i++){
		if(shift_Ci_Di(&(key->Ci), Vi[i]))
			return des_errno=ERR_BIT, 1;
		if(shift_Ci_Di(&(key->Di), Vi[i]))
			return des_errno=ERR_BIT, 1;
		uint32_t* Ci, *Di;
		Ci=&(key->Ci);
		Di=&(key->Di);
		/*printf("Ci=");
		printf_uint32_t_binary(*Ci);
		printf("\n");
		printf("Di=");
		printf_uint32_t_binary(*Di);
		printf("\n");*/
		if(generate_sub_key(&(key->sub_key[i]), *Ci, *Di))
			return des_errno=ERR_BIT, 1;
		/*printf("K%d :", i+1);
		printf_uint64_t_binary(key->sub_key[i].bytes);
		printf("\n\n");*/
	}
	return 0;
}

/**
 * \fn int key_schedule (uint64_t* init, KEY* k)
 * \brief Fonction qui crée les sous clés du DES.
 *
 * \param *init pointeur sur la clé initiale de 64 bits.
 * \param *key structure représentant les sous clés DES. 
 * \return renvoie 0 en cas de réussite et 1 en cas d'échec
 * (change la valeur de l'erreur).
 */
int key_schedule (uint64_t* init, KEY* key){
	uint64_t nouv=0;
	int i;
	for(i=0;i<56;i++){
		uint8_t t=get_bit_uint64_t_most((*init),PC1[i]);
		if(set_bit_uint64_t(&nouv, t, 56-i)) 
			return des_errno=ERR_BIT, 1;
	}
	(*init)=nouv;

	if(init_C0_D0(key, *init)) 
		return des_errno=ERR_BIT, 1;

	if(process_Ci_Di(key)) 
		return des_errno=ERR_BIT, 1;

	return 0;
}




