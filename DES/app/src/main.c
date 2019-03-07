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
	uint64_t K;

	build_K(&K, 0xf0ccaaf, 0x556678f);

	printf(">");
	printf_uint64_t_binary(K);
	printf("!");
	printf_uint64_t_binary(0x133457799BBCDFF1);

	return EXIT_SUCCESS;
}
