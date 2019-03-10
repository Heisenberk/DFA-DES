#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../../app/inc/attack.h"

void test_brute_force_attack(){
	DATA data;
	data.k16.bytes=0x000210B12C4779;
	data.chiffre_juste.output=0x670994D1365D5EAD;
	data.message_clair=0xFBA2DC5EEAA7FEC2;

	CU_ASSERT_EQUAL(brute_force_attack(&data), 0);
	CU_ASSERT_EQUAL(data.key, 0x51922C19028FFD49);

}