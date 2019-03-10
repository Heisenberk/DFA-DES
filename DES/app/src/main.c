#include <stdio.h>
#include <stdlib.h>

#include "../inc/key_schedule.h"
#include "../inc/inner_function.h"
#include "../inc/manip_bits.h"
#include "../inc/feistel.h"
#include "../inc/attack.h"

int main(){

	/*DATA data = initialize_data();

	calcul_boite_s1_inv (&data);*/
	/*uint64_t init=0x133457799BBCDFF1;
	KEY key;
	key_schedule(&init, &key);*/
	/*uint64_t K;

	build_K(&K, 0xf0ccaaf, 0x556678f);

	printf(">");
	printf_uint64_t_binary(K);
	printf("!");
	printf_uint64_t_binary(0x133457799BBCDFF1);*/

	/*uint64_t K=0xFEFEFEFEFEFEFEFE;
	printf_uint64_t_binary(K);
	set_parity_bits(&K);
	printf_uint64_t_binary(K);*/

	//DATA data = initialize_data();
	
	DATA data;
	data.k16.bytes=0xcb3d8b0e17f5;
	data.chiffre_juste.output=0x85E813540F0AB405;
	data.message_clair=0x0123456789ABCDEF;

	brute_force_attack(&data);

	/*uint64_t m1=0x0123456789ABCDEF;
	uint64_t k1=0x133457799BBCDFF1;
	uint64_t m2=0x0123456789ABCDEF;
	uint64_t k2=0x123556789ABDDEF0;
	encryption_des(&m1, k1);
	printf_uint64_t_hexa(m1);
	printf("\n");
	encryption_des(&m2, k2);
	printf_uint64_t_hexa(m2);
	printf("\n");
	printf_uint64_t_hexa(k2);*/
	return EXIT_SUCCESS;
}
