#ifndef FEISTEL__H
#define FEISTEL__H

#include "manip_bits.h"

int process_permutation(uint64_t* data, int* IP);
int build_L0_R0(uint64_t data, uint32_t* L0, uint32_t* R0);
int process_round_1_15_encryption(uint32_t* Li, uint32_t* Ri, SUB_KEY Kiadd1);
int process_round_16_encryption(uint32_t* L15, uint32_t* R15, SUB_KEY K16);
int process_round_1_decryption(uint32_t* L0, uint32_t* R0, SUB_KEY K16);
int process_round_2_15_decryption(uint32_t* Li, uint32_t* Ri, SUB_KEY Ki);
int encryption_des(uint64_t* data, uint64_t* key_64);
int decryption_des(uint64_t* data, uint64_t* key_64);

uint32_t get_R15(uint64_t cipher);
uint32_t get_R16(uint64_t cipher);

#endif
