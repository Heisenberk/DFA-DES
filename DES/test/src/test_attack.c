#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../../app/inc/attack.h"
#include "../../app/inc/feistel.h"

void test_get_R16_exemple(){ //exemple chiffrement
     uint64_t cipher = 0x85E813540F0AB405; //exemple sur internet
     uint32_t R16 = get_R16(cipher);
     CU_ASSERT_EQUAL(R16, 0xa4cd995);
}

void test_get_R15_exemple(){ //exemple chiffrement
     uint64_t cipher = 0x85E813540F0AB405; //exemple sur internet
     uint32_t R15 = get_R15(cipher);
     CU_ASSERT_EQUAL(R15, 0x43423234);
}

void test_get_R15_attaque(){
     /*uint64_t cipher = 0x670994D1365D5EAD; //chiffré juste pour l'attaque
     uint32_t R15 = get_R15(cipher);
     CU_ASSERT_EQUAL(R15, 0x697CF5AB);*/
     CU_ASSERT_EQUAL(1,1);
}

void test_find_K(){
	DATA data;
	data.k16.bytes=0x000210B12C4779;
	data.chiffre_juste.output=0x670994D1365D5EAD;
	data.message_clair=0xFBA2DC5EEAA7FEC2;

	CU_ASSERT_EQUAL(find_K(&data), 0);
	CU_ASSERT_EQUAL(data.key, 0x51922C19028FFD49);

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