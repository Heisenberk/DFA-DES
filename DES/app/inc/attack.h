#ifndef ATTACK__H
#define ATTACK__H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "key_schedule.h"

struct cipher_Ri {
	uint64_t output;
	uint32_t R15;
	uint32_t R16;
}; 
typedef struct cipher_Ri CIPHER_RI;

struct data {
	uint64_t message_clair;
	CIPHER_RI chiffre_juste;
	CIPHER_RI chiffre_faux[32];
	SUB_KEY k16;
	uint64_t key;
}; 
typedef struct data DATA;

/*struct sbox {
	uint8_t num;
}*/

DATA initialize_data();


uint32_t get_R15(uint64_t cipher);
uint32_t get_R16(uint64_t cipher);
void calcul_boite_s1 (DATA d);
void attack_sbox(DATA* data, int num_sbox);


int build_C16_D16(SUB_KEY k16, uint32_t* C16, uint32_t* D16);
uint64_t build_K56(uint32_t C0, uint32_t D0);
int build_K(uint64_t* K, uint32_t C16, uint32_t D16);
int set_parity_bits(uint64_t* K);

int find_K(DATA* data);

#endif