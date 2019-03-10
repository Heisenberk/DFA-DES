#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../app/inc/key_schedule.h"
#include "../../app/inc/inner_function.h"
#include "../../app/inc/errors.h"
#include "../../app/inc/manip_bits.h"
#include "../../app/inc/feistel.h"

int PC1_test[] = { 57, 49,  41, 33,  25,  17,  9,
			  1, 58,  50, 42,  34,  26, 18,
			  10,  2,  59, 51,  43,  35, 27,
			  19, 11,   3, 60,  52,  44, 36,
			  63, 55,  47, 39,  31,  23, 15,
			  7, 62,  54, 46,  38,  30, 22,
			  14,  6,  61, 53,  45,  37, 29,
			  21, 13, 5, 28, 20, 12, 4};


void test_init_C0_D0(){
	uint64_t init=0x133457799BBCDFF1;
	uint64_t nouv=0;
	int i;
	for(i=0;i<56;i++){
		uint8_t t=get_bit_uint64_t_most(init,PC1_test[i]);
		set_bit_uint64_t(&nouv, t, 56-i);
	}
	init=nouv;

	KEY k;
	CU_ASSERT_EQUAL(init_C0_D0(&k,init),0);
	CU_ASSERT_EQUAL(k.Ci, 252496559);
	CU_ASSERT_EQUAL(k.Di, 89548687)
}

void test_process_Ci_Di(){
	uint64_t init=0x133457799BBCDFF1;
	uint64_t nouv=0;
	int i;
	for(i=0;i<56;i++){
		uint8_t t=get_bit_uint64_t_most(init,PC1_test[i]);
		set_bit_uint64_t(&nouv, t, 56-i);
	}
	init=nouv;

	KEY k;
	CU_ASSERT_EQUAL(init_C0_D0(&k, init), 0);
	CU_ASSERT_EQUAL(process_Ci_Di(&k),0); 
	CU_ASSERT_EQUAL(k.Ci, 252496559);
	CU_ASSERT_EQUAL(k.Di, 89548687);
}

void test_generate_sub_key(){
	uint64_t init=0x133457799BBCDFF1;
	KEY key;
	CU_ASSERT_EQUAL(key_schedule(&init, &key), 0);
	CU_ASSERT_EQUAL(key.sub_key[0].bytes, 29699430183026); 
	CU_ASSERT_EQUAL(key.sub_key[1].bytes, 133791886330341); 
	CU_ASSERT_EQUAL(key.sub_key[2].bytes, 94543139753881); 
	CU_ASSERT_EQUAL(key.sub_key[3].bytes, 126090959598877); 
	CU_ASSERT_EQUAL(key.sub_key[4].bytes, 137353186988968); 
	CU_ASSERT_EQUAL(key.sub_key[5].bytes, 109561366215471); 
	CU_ASSERT_EQUAL(key.sub_key[6].bytes, 260054766196924); 
	CU_ASSERT_EQUAL(key.sub_key[7].bytes, 272173063289851); 
	CU_ASSERT_EQUAL(key.sub_key[8].bytes, 247235160696705); 
	CU_ASSERT_EQUAL(key.sub_key[9].bytes, 195658438559311); 
	CU_ASSERT_EQUAL(key.sub_key[10].bytes, 36695460205446); 
	CU_ASSERT_EQUAL(key.sub_key[11].bytes, 129132311898089); 
	CU_ASSERT_EQUAL(key.sub_key[12].bytes, 166875887221313); 
	CU_ASSERT_EQUAL(key.sub_key[13].bytes, 104744453596986); 
	CU_ASSERT_EQUAL(key.sub_key[14].bytes, 210631860764426); 
	CU_ASSERT_EQUAL(key.sub_key[15].bytes, 223465186400245); 
}

void test_build_C16_D16(){
	SUB_KEY k16;
	k16.bytes=0xcb3d8b0e17f5;
	uint32_t C16, D16;
	CU_ASSERT_EQUAL(build_C16_D16(k16, &C16, &D16), 0);
	CU_ASSERT_EQUAL(C16, 0xf04caa7); //les bits inconnus sont a 0
	CU_ASSERT_EQUAL(D16, 0x552478b); //les bits inconnus sont a 0
	
}

void test_build_K56(){
	CU_ASSERT_EQUAL(build_K56(0xf0ccaaf, 0x556678f),0xf0ccaaf556678f);
}

void test_build_K(){
	uint64_t K;
	CU_ASSERT_EQUAL(build_K(&K, 0xf0ccaaf, 0x556678f),0);
	uint64_t m=0x0123456789ABCDEF;
    CU_ASSERT_EQUAL(encryption_des(&m, K), 0);
    CU_ASSERT_EQUAL(m, 0x85E813540F0AB405);
}

void test_set_parity_bits(){
	uint64_t K=0xC0C0C0C0C0C0C0C0;
	CU_ASSERT_EQUAL(set_parity_bits(&K) ,0);
	CU_ASSERT_EQUAL(K, 0xC1C1C1C1C1C1C1C1);
}