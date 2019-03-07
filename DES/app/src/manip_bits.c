#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../inc/errors.h"
#include "../inc/key_schedule.h"


/**
 * \fn uint8_t get_bit_uint64_t (uint64_t elem, uint8_t i)
 * \brief Fonction qui permet d'obtenir le i ème bit de elem en sachant
 * que le bit de poids faible est à la position i=1.
 *
 * \param i numéro du bit à obtenir.
 * \param elem uint64_t dont l'on cherche le i ème bit. 
 * \return renvoie 0, 1, (ou 2 en cas d'erreur) en fonction du 
 * i ème bit de elem. 
 */
uint8_t get_bit_uint64_t (uint64_t elem, uint8_t i){
	if(i<0) return 2;
	if(i>64) return 2;
	return (elem>>(i-1))%2;
}

/**
 * \fn uint8_t get_bit_uint32_t (uint32_t elem, uint8_t i)
 * \brief Fonction qui permet d'obtenir le i ème bit de elem en sachant
 * que le bit de poids faible est à la position i=1.
 *
 * \param i numéro du bit à obtenir.
 * \param elem uint32_t dont l'on cherche le i ème bit. 
 * \return renvoie 0, 1, (ou 2 en cas d'erreur) en fonction du 
 * i ème bit de elem. 
 */
uint8_t get_bit_uint32_t (uint32_t elem, uint8_t i){
	if(i<0) return 2;
	if(i>32) return 2;
	return (elem>>(i-1))%2;
}

/**
 * \fn uint8_t get_bit_uint32_t (uint8_t elem, uint8_t i)
 * \brief Fonction qui permet d'obtenir le i ème bit de elem en sachant
 * que le bit de poids faible est à la position i=1.
 *
 * \param i numéro du bit à obtenir.
 * \param elem uint8_t dont l'on cherche le i ème bit. 
 * \return renvoie 0, 1, (ou 2 en cas d'erreur) en fonction du 
 * i ème bit de elem. 
 */
uint8_t get_bit_uint8_t (uint8_t elem, uint8_t i){
	if(i<0) return 2;
	if(i>8) return 2;
	return (elem>>(i-1))%2;
}

/**
 * \fn uint8_t get_bit_uint64_t_most (uint64_t elem, uint8_t i)
 * \brief Fonction qui permet d'obtenir le i ème bit de elem en sachant
 * que le bit de poids fort est à la position i=1.
 *
 * \param i numéro du bit à obtenir.
 * \param elem uint64_t dont l'on cherche le i ème bit. 
 * \return renvoie 0, 1, (ou 2 en cas d'erreur) en fonction du 
 * i ème bit de elem. 
 */
uint8_t get_bit_uint64_t_most (uint64_t elem, uint8_t i){
	if(i<0) return 2;
	if(i>64) return 2;
	return (elem>>(64-i))%2;
}

/**
 * \fn uint8_t get_bit_uint32_t_most (uint32_t elem, uint8_t i)
 * \brief Fonction qui permet d'obtenir le i ème bit de elem en sachant
 * que le bit de poids fort est à la position i=1.
 *
 * \param i numéro du bit à obtenir.
 * \param elem uint32_t dont l'on cherche le i ème bit. 
 * \return renvoie 0, 1, (ou 2 en cas d'erreur) en fonction du 
 * i ème bit de elem. 
 */
uint8_t get_bit_uint32_t_most (uint32_t elem, uint8_t i){
	if(i<0) return 2;
	if(i>32) return 2;
	return (elem>>(32-i))%2;
}

/**
 * \fn uint8_t get_6bits_uint64_t_most (SUB_KEY elem, uint8_t i)
 * \brief Fonction qui permet d'obtenir le i ème bloc de 6 bits de elem en sachant
 * que le groupe de 6 bits de poids fort est à la position i=1.
 *
 * \param i numéro de du groupe de 6 octets à obtenir.
 * \param elem uint64_t dont l'on cherche le i ème octet. 
 * \return renvoie 0, 1, (ou 2 en cas d'erreur) en fonction du 
 * i ème bit de elem. 
 */
uint8_t get_6bits_uint64_t_most (SUB_KEY elem, uint8_t i){
	if(i<0) return 2;
	if(i>8) return 2;
	else{
		uint8_t num_bloc = 9-i;
		uint8_t result=0x00;
		uint8_t bit = num_bloc*6-5;
		uint8_t bit2;
		int j;
		for(j=0;j<6;j++){
			bit2=get_bit_uint64_t(elem.bytes, bit);
			bit2<<=j;
			result|=bit2;
			bit++;
		}
		return result;
	}
}

/**
 * \fn void printf_uint64_t_binary(uint64_t key);
 * \brief Fonction qui affiche en binaire un uint64_t. 
 *
 * \param key uint64_t à afficher. 
 */
void printf_uint64_t_binary(uint64_t key){
	char* chain; int i; uint8_t bit;
	chain=malloc(65*sizeof(char));
	
	for(i=0;i<64;i++){
		bit=key%2;
		if(bit==0) chain[63-i]='0';
		else chain[63-i]='1';
		key>>=1;
	}
	chain[64]='\0';
	printf("%s\n", chain);
	free(chain);
}


/**
 * \fn void printf_uint32_t_binary(uint32_t key)
 * \brief Fonction qui affiche en binaire un uint32_t. 
 *
 * \param key uint32_t à afficher. 
 */
void printf_uint32_t_binary(uint32_t key){
	char* chain; int i; uint8_t bit;
	chain=malloc(33*sizeof(char));
	for(i=0;i<32;i++){
		bit=key%2;
		if(bit==0) chain[31-i]='0';
		else chain[31-i]='1';
		key>>=1;
	}
	chain[32]='\0';
	printf("%s", chain);
	free(chain);
}

/**
 * \fn void printf_uint8_t_binary(uint8_t key)
 * \brief Fonction qui affiche en binaire un uint8_t. 
 *
 * \param key uint8_t à afficher. 
 */
void printf_uint8_t_binary(uint8_t key){
	char* chain; int i; uint8_t bit;
	chain=malloc(9*sizeof(char));
	for(i=0;i<8;i++){
		bit=key%2;
		if(bit==0) chain[7-i]='0';
		else chain[7-i]='1';
		key>>=1;
	}
	chain[8]='\0';
	printf("%s", chain);
	free(chain);
}

/**
 * \fn void printf_uint64_t_hexa(uint64_t key)
 * \brief Fonction qui affiche en hexa un uint64_t. 
 *
 * \param key uint64_t à afficher. 
 */
void printf_uint64_t_hexa(uint64_t key){
	printf("%016llX", key);
}

/**
 * \fn void printf_uint32_t_hexa(uint32_t key)
 * \brief Fonction qui affiche en hexa un uint32_t. 
 *
 * \param key uint32_t à afficher. 
 */
void printf_uint32_t_hexa(uint32_t key){
	printf("%16lX", key);
}

/**
 * \fn int set_bit_uint64_t (uint64_t* elem, uint8_t bit, uint8_t pos)
 * \brief Fonction qui initialise le bit à la position pos de elem. 
 * Le bit de poids faible a la position pos=1. 
 * \param elem élément à modifier
 * \param bit valeur du futur bit à changer. 
 * \param pos position du bit à changer. 
 * \return renvoie 0 en cas de réussite et 1 en cas d'échec
 * (change la valeur de l'erreur).
 */
int set_bit_uint64_t (uint64_t* elem, uint8_t bit, uint8_t pos){
	if(bit==1){
		uint64_t mask=1ULL;
		mask <<= (pos-1);
		(*elem)|=mask;
	}
	else if(bit==0){
		uint64_t mask1=0xFFFFFFFFFFFFFFFF;
		uint64_t mask2=1ULL;
		mask2 <<= (pos-1);
		mask1 ^= mask2;
		(*elem)&=mask1;
	}
	else {
		return des_errno=ERR_BIT, 1;
	}
	return 0;
}

/**
 * \fn int set_bit_uint32_t (uint32_t* elem, uint8_t bit, uint8_t pos)
 * \brief Fonction qui initialise le bit à la position pos de elem. 
 * Le bit de poids faible a la position pos=1. 
 * \param elem élément à modifier
 * \param bit valeur du futur bit à changer. 
 * \param pos position du bit à changer. 
 * \return renvoie 0 en cas de réussite et 1 en cas d'échec
 * (change la valeur de l'erreur).
 */
int set_bit_uint32_t (uint32_t* elem, uint8_t bit, uint8_t pos){
	if(bit==1){
		uint32_t mask=1ULL;
		mask <<= (pos-1);
		(*elem)|=mask;
	}
	else if(bit==0){
		uint32_t mask1=0xFFFFFFFF;
		uint32_t mask2=1ULL;
		mask2 <<= (pos-1);
		mask1 ^= mask2;
		(*elem)&=mask1;
	}
	else {
		return des_errno=ERR_BIT, 1;
	}
	return 0;
}

/**
 * \fn int set_bit_uint8_t (uint8_t* elem, uint8_t bit, uint8_t pos)
 * \brief Fonction qui initialise le bit à la position pos de elem. 
 * Le bit de poids faible a la position pos=1. 
 * \param elem élément à modifier
 * \param bit valeur du futur bit à changer. 
 * \param pos position du bit à changer. 
 * \return renvoie 0 en cas de réussite et 1 en cas d'échec
 * (change la valeur de l'erreur).
 */
int set_bit_uint8_t (uint8_t* elem, uint8_t bit, uint8_t pos){
	if(bit==1){
		uint32_t mask=1ULL;
		mask <<= (pos-1);
		(*elem)|=mask;
	}
	else if(bit==0){
		uint32_t mask1=0xFF;
		uint32_t mask2=1ULL;
		mask2 <<= (pos-1);
		mask1 ^= mask2;
		(*elem)&=mask1;
	}
	else {
		return des_errno=ERR_BIT, 1;
	}
	return 0;
}

