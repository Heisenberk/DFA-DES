#ifndef ATTACK__H
#define ATTACK__H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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
}; 
typedef struct data DATA;

DATA initialize_data();

#endif