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
	data.k16.bytes=0x000210B12C4779;
	data.chiffre_juste.output=0x670994D1365D5EAD;
	data.message_clair=0xFBA2DC5EEAA7FEC2;


	find_K(&data);
	printf("\n!");
	printf_uint64_t_hexa(data.key);
	
	return EXIT_SUCCESS;
}
