
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../../app/inc/errors.h"
#include "../inc/test_key_schedule.h"
#include "../inc/test_inner_function.h"
#include "../inc/test_manip_bits.h"
#include "../inc/test_feistel.h"

int setup(void)  { return 0; }
int teardown(void) { return 0; }

int main(){
	
	CU_initialize_registry();
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();	
	
	CU_pSuite suite0 = CU_add_suite("Tests Manipulation of bits", setup,teardown);
	CU_add_test(suite0, "test_get_bit_uint64_t", test_get_bit_uint64_t);
	CU_add_test(suite0, "test_get_bit_uint64_t_most", test_get_bit_uint64_t_most);
	CU_add_test(suite0, "test_set_bit_uint64_t", test_set_bit_uint64_t);
	CU_add_test(suite0, "test_get_6bits_uint64_t_most", test_get_6bits_uint64_t_most);

	CU_pSuite suite1 = CU_add_suite("Tests Key Schedule", setup,teardown);
	CU_add_test(suite1, "test_init_C0_D0", test_init_C0_D0);
	CU_add_test(suite1, "test_process_Ci_Di", test_process_Ci_Di);
	CU_add_test(suite1, "test_generate_sub_key", test_generate_sub_key);
	CU_add_test(suite1, "test_build_C16_D16", test_build_C16_D16);
	CU_add_test(suite1, "test_build_K56", test_build_K56);

	CU_pSuite suite2 = CU_add_suite("Test Inner Function", setup,teardown);
	CU_add_test(suite2, "test_expand", test_expand);
	CU_add_test(suite2, "test_process_S_box", test_process_S_box);
	CU_add_test(suite2, "test_process_S_box_particular", test_process_S_box_particular);
	CU_add_test(suite2, "test_permutation_inner_function", test_permutation_inner_function);
	CU_add_test(suite2, "test_permutation_inv_inner_function", test_permutation_inv_inner_function);
	CU_add_test(suite2, "test_inner_function", test_inner_function);
	CU_add_test(suite2, "test_get_input_sbox", test_get_input_sbox);

	CU_pSuite suite3 = CU_add_suite("Test Feistel", setup,teardown);
	CU_add_test(suite3, "test_process_permutation", test_process_permutation);
	CU_add_test(suite3, "test_build_L0_R0", test_build_L0_R0);
	CU_add_test(suite3, "test_process_round", test_process_round);
	CU_add_test(suite3, "test_encryption_des", test_encryption_des);
	CU_add_test(suite3, "test_encryption_decryption_K16", test_encryption_decryption_K16);
	CU_add_test(suite3, "test_encryption_decryption_K1", test_encryption_decryption_K1);
	CU_add_test(suite3, "test_get_R15_exemple", test_get_R15_exemple);
	CU_add_test(suite3, "test_get_R15_attaque", test_get_R15_attaque);
	CU_add_test(suite3, "test_get_R16_exemple", test_get_R16_exemple);

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();

    return 0;
}	
