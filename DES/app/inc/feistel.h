#ifndef FEISTEL__H
#define FEISTEL__H

#include "manip_bits.h"

int process_permutation(uint64_t* data, int* IP);
int build_L0_R0(uint64_t data, uint32_t* L0, uint32_t* R0);
int process_round_1_15_encryption(uint32_t* Li, uint32_t* Ri, uint48_t Kiadd1);
int process_round_16_encryption(uint32_t* L15, uint32_t* R15, uint48_t K16);
int process_round_1_decryption(uint32_t* L0, uint32_t* R0, uint48_t K16);
int process_round_2_15_decryption(uint32_t* Li, uint32_t* Ri, uint48_t Ki);
int encryption_des(uint64_t* data, uint64_t key_64);


#endif
