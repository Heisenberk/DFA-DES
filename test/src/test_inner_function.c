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

int S1_test[4][16] = { 14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
			 	   0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
			       4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
                  15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13};

int S2_test[4][16] = { 15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
			       3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
			       0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
			      13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9};

int S3_test[4][16] = { 10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
			      13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
			      13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
			       1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12};

int S4_test[4][16] = { 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
			     13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
			     10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
			      3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14};

int S5_test[4][16] = { 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
			     14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
			      4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
			     11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3};

int S6_test[4][16] = { 12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
			      10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
			       9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
			       4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13};

int S7_test[4][16] = { 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
			     13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
			      1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
			      6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12};

int S8_test[4][16] = { 13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
			       1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
			       7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
                   2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11};


void test_expand(){
	uint64_t expande;
	uint32_t R = 0xf0aaf0aa;
	CU_ASSERT_EQUAL(expand(&expande, R), 0);
	CU_ASSERT_EQUAL(expande, 0x7a15557a1555);

}

void test_get_6bits_uint64_t_most(){
	uint64_t bytes = 0x6117ba866527;
	uint48_t k;
	k.bytes=bytes;
	CU_ASSERT_EQUAL(get_6bits_uint64_t_most(k, 1),0x18);	
}

void test_process_S_box(){
	uint48_t k;
	k.bytes=0x6117ba866527;
	uint32_t g;
	CU_ASSERT_EQUAL(process_S_box(&g, k),0);	
	CU_ASSERT_EQUAL(g,0x5c82b597);
}

void test_process_S_box_particular(){
	uint8_t input =0x1B;
	CU_ASSERT_EQUAL(process_S_box_particular(input, S5_test), 9);
}

void test_permutation_inner_function(){
	uint32_t input = 0x5c82b597;
	uint32_t output;
	CU_ASSERT_EQUAL(permutation_inner_function(&output, input),0);
	CU_ASSERT_EQUAL(output, 0x234aa9bb);
}

void test_permutation_inv_inner_function(){
	uint32_t input = 0x5c82b597;
	uint32_t input_find;
	uint32_t output, output_find;
	output_find=0x234aa9bb;
	CU_ASSERT_EQUAL(permutation_inner_function(&output, input),0);
	CU_ASSERT_EQUAL(output, output_find);
	CU_ASSERT_EQUAL(permutation_inv_inner_function(&input_find, output),0);
	CU_ASSERT_EQUAL(input_find, input);
}

void test_inner_function(){
	uint32_t R = 0xf0aaf0aa;
	uint48_t k;
	k.bytes = 0x1b02effc7072;
	CU_ASSERT_EQUAL(inner_function(k, &R),0);
	CU_ASSERT_EQUAL(R, 0x234aa9bb);
}

void test_get_input_sbox(){
	uint8_t output=9;
	uint8_t input1, input2, input3, input4;
	CU_ASSERT_EQUAL(get_input_sbox(output, S5_test, &input1, &input2, &input3, &input4), 0);
	CU_ASSERT_EQUAL(input1, 30);
	CU_ASSERT_EQUAL(input2, 27);
	CU_ASSERT_EQUAL(input3, 50);
	CU_ASSERT_EQUAL(input4, 55);
}
