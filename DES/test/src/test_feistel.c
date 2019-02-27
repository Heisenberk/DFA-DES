#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../../app/inc/feistel.h"


int IP_test[64] = { 58, 50, 42, 34, 26, 18, 10, 2,
               60, 52, 44, 36, 28, 20, 12, 4,
               62, 54, 46, 38, 30, 22, 14, 6,
               64, 56, 48, 40, 32, 24, 16, 8,
               57, 49, 41, 33, 25, 17, 9, 1,
               59, 51, 43, 35, 27, 19, 11, 3, 
               61, 53, 45, 37, 29, 21, 13, 5,
               63, 55, 47, 39, 31, 23, 15, 7 };

void test_process_permutation(){
	uint64_t data = 0x0123456789ABCDEF;
	CU_ASSERT_EQUAL(process_permutation(&data, IP_test), 0);
	CU_ASSERT_EQUAL(data, 0xCC00CCFFF0AAF0AA);
}

void test_build_L0_R0(){
     uint64_t data=0xCC00CCFFF0AAF0AA;
     uint32_t L0, R0;
     CU_ASSERT_EQUAL(build_L0_R0(data, &L0, &R0), 0);
     CU_ASSERT_EQUAL(L0, 0xcc00ccff);
     CU_ASSERT_EQUAL(R0, 0xf0aaf0aa);
}

void test_process_round(){
     //int process_round(uint32_t* Li, uint32_t* Ri, SUB_KEY Kiadd1)
     uint32_t Li, Ri; 
     Li = 0xcc00ccff;
     Ri = 0xf0aaf0aa;
     SUB_KEY k;
     k.bytes=0x1b02effc7072;

     CU_ASSERT_EQUAL(process_round_1_15_encryption(&Li, &Ri, k), 0);
     CU_ASSERT_EQUAL(Li, 0xf0aaf0aa);
     CU_ASSERT_EQUAL(Ri, 0xef4a6544);
}

void test_encryption_des(){
     uint64_t m=0x0123456789ABCDEF;
     uint64_t k=0x133457799BBCDFF1;
     CU_ASSERT_EQUAL(encryption_des(&m, &k), 0);
     CU_ASSERT_EQUAL(m, 0x85E813540F0AB405);
}

void test_encryption_decryption_K16(){
     uint32_t L15 = 0xc28c960d; 
     uint32_t L15_cpy=L15;
     uint32_t R15 = 0x43423234; 
     uint32_t R15_cpy=R15;
     SUB_KEY K16; 
     K16.bytes = 0xBF918D3D3F0A;
     process_round_16_encryption(&L15, &R15, K16);
     process_round_1_decryption(&L15, &R15, K16);

     CU_ASSERT_EQUAL(L15_cpy, L15);
     CU_ASSERT_EQUAL(R15_cpy, R15);
}

void test_encryption_decryption_K1(){
     uint32_t L0 = 0xcc00ccff; uint32_t L0_cpy=L0;
     uint32_t R0 = 0xf0aaf0aa; uint32_t R0_cpy=R0;
     SUB_KEY K1; K1.bytes = 0x1B02EFFC7072;
     process_round_1_15_encryption(&L0, &R0, K1);
     process_round_2_15_decryption(&L0, &R0, K1);

     CU_ASSERT_EQUAL(L0_cpy, L0);
     CU_ASSERT_EQUAL(R0_cpy, R0);
}

void test_get_R15_exemple(){ //exemple chiffrement
     uint64_t cipher = 0x85E813540F0AB405; //exemple sur internet
     uint32_t R15 = get_R15(cipher);
     CU_ASSERT_EQUAL(R15, 0xa4cd995);
}

void test_get_R15_attaque(){
     uint64_t cipher = 0x670994D1365D5EAD; //chiffré juste pour l'attaque
     uint32_t R15 = get_R15(cipher);
     CU_ASSERT_EQUAL(R15, 0x697CF5AB);
}

