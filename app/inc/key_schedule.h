/**
 * \file key_schedule.h
 * \brief Représente les signatures des fonctions concernant la génération des sous
 * clés. 
 * \author Clément CAUMES
 * */

#ifndef KEY_SCHEDULE__H
#define KEY_SCHEDULE__H

#include <stdint.h>
#include "manip_bits.h"

struct key{
	uint32_t Ci;
	uint32_t Di;
	uint48_t sub_key[16];

}; typedef struct key KEY;

int init_C0_D0(KEY* k, uint64_t init);
int process_Ci_Di(KEY* key);
int key_schedule (uint64_t* init, KEY* key);


#endif
