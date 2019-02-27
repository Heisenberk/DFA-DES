#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../../app/inc/manip_bits.h"
#include "../../app/inc/errors.h"
#include "../inc/test_manip_bits.h"


void test_get_bit_uint64_t(){
	char* chain; int i; uint8_t bit;
	char* chain2;
	chain=malloc(65*sizeof(char));
	chain2=malloc(65*sizeof(char));
	uint64_t test=252496559;
	for(i=0;i<64;i++){
		bit=get_bit_uint64_t (test, 64-i);
		if(bit==0) chain[i]='0';
		else chain[i]='1';
	}
	chain[64]='\0';
	strcpy(chain2, "0000000000000000000000000000000000001111000011001100101010101111");
	CU_ASSERT_EQUAL(strcmp(chain, chain2), 0);
	free(chain);
	free(chain2);
}

void test_get_bit_uint64_t_most(){
	char* chain; int i; uint8_t bit;
	char* chain2;
	chain=malloc(65*sizeof(char));
	chain2=malloc(65*sizeof(char));
	uint64_t test=252496559;
	for(i=0;i<64;i++){
		bit=get_bit_uint64_t_most (test, i+1);
		if(bit==0) chain[i]='0';
		else chain[i]='1';
	}
	chain[64]='\0';
	strcpy(chain2, "0000000000000000000000000000000000001111000011001100101010101111");
	CU_ASSERT_EQUAL(strcmp(chain, chain2), 0);
	free(chain);
	free(chain2);
}

void test_set_bit_uint64_t(){
	uint64_t val=1;
	uint8_t bit1=1; uint8_t bit2=0;
	uint8_t pos1=2; uint8_t pos2=1;
	CU_ASSERT_EQUAL(set_bit_uint64_t(&val, bit1, pos1), 0);
	CU_ASSERT_EQUAL(val, 3);
	CU_ASSERT_EQUAL(set_bit_uint64_t(&val, bit2, pos2), 0);
	CU_ASSERT_EQUAL(val, 2);
}


