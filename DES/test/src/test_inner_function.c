#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../app/inc/key_schedule.h"
#include "../../app/inc/inner_function.h"
#include "../../app/inc/errors.h"
#include "../inc/test_inner_function.h"
#include "../../app/inc/manip_bits.h"


void test_expand(){
	uint64_t expande;
	uint32_t R = 0xf0aaf0aa;
	CU_ASSERT_EQUAL(expand(&expande, R), 0);
	CU_ASSERT_EQUAL(expande, 0x7a15557a1555);

}

void test_get_6bits_uint64_t_most(){
	uint64_t bytes = 0x6117ba866527;
	SUB_KEY k;
	k.bytes=bytes;
	CU_ASSERT_EQUAL(get_6bits_uint64_t_most(k, 1),0x18);	
}

void test_process_S_box(){
	SUB_KEY k;
	k.bytes=0x6117ba866527;
	uint32_t g;
	CU_ASSERT_EQUAL(process_S_box(&g, k),0);	
	CU_ASSERT_EQUAL(g,0x5c82b597);
}

void test_permutation_inner_function(){
	uint32_t input = 0x5c82b597;
	uint32_t output;
	CU_ASSERT_EQUAL(permutation_inner_function(&output, input),0);
	CU_ASSERT_EQUAL(output, 0x234aa9bb);
}

void test_inner_function(){
	uint32_t R = 0xf0aaf0aa;
	SUB_KEY k;
	k.bytes = 0x1b02effc7072;
	CU_ASSERT_EQUAL(inner_function(k, &R),0);
	CU_ASSERT_EQUAL(R, 0x234aa9bb);

}