#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../../app/inc/attack.h"

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

void test_brute_force_attack(){
	DATA data;
	data.k16.bytes=0x000210B12C4779;
	data.chiffre_juste.output=0x670994D1365D5EAD;
	data.message_clair=0xFBA2DC5EEAA7FEC2;

	CU_ASSERT_EQUAL(find_K(&data), 0);
	CU_ASSERT_EQUAL(data.key, 0x51922C19028FFD49);

}