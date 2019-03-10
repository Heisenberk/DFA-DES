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
void calcul_boite_s1 (DATA d);
int brute_force_attack(DATA* data);

#endif