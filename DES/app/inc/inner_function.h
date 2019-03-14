#ifndef INNER_FUNCTION__H
#define INNER_FUNCTION__H

#include <stdio.h>
#include <stdlib.h>
#include "attack.h"
#include "manip_bits.h"

int expand(uint64_t* expand, uint32_t R);
int process_S_box(uint32_t* result, uint48_t elem);
uint8_t process_S_box_particular(uint8_t input, int S[4][16]);
int permutation_inner_function(uint32_t* output, uint32_t input);
int permutation_inv_inner_function(uint32_t* output, uint32_t input);
int inner_function(uint48_t sub_key, uint32_t* R);
int compte_possibilites_S1(uint8_t sortie);
int get_input_sbox(uint8_t output, int S[4][16], uint8_t* input1, uint8_t* input2, uint8_t* input3, uint8_t* input4);
void calcul_boite_s1_inv (DATA *data);
#endif
