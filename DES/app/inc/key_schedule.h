#ifndef KEY_SCHEDULE__H
#define KEY_SCHEDULE__H

#include <stdint.h>

struct sub_key {
	uint64_t bytes;
};
typedef struct sub_key SUB_KEY;

struct key{
	uint32_t Ci;
	uint32_t Di;
	SUB_KEY sub_key[16];

}; typedef struct key KEY;

int init_C0_D0(KEY* k, uint64_t init);
int process_Ci_Di(KEY* key);
int key_schedule (uint64_t* init, KEY* key);
int build_C16_D16(SUB_KEY k16, uint32_t* C16, uint32_t* D16);
uint64_t build_K56(uint32_t C0, uint32_t D0);
int build_K(uint64_t* K, uint32_t C16, uint32_t D16);

#endif
