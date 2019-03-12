#include <stdio.h>
#include <stdlib.h>

#include "../inc/key_schedule.h"
#include "../inc/inner_function.h"
#include "../inc/manip_bits.h"
#include "../inc/feistel.h"
#include "../inc/attack.h"
#include "../inc/constants.h"

int main(){
	/*uint64_t cipher1, cipher2;
	cipher1=0x670994D1365D5EAD;
	cipher2=0x650C94D5365C5EB9;
	uint32_t r16_1=get_R16(cipher1);
	uint32_t r16_2=get_R16(cipher2);
	uint32_t out;
	permutation_inv_inner_function(&out, (r16_1^r16_2));
	printf_uint32_t_binary(out);*/
	DATA data = initialize_data();
	calcul_xor(&data);
	/*uint64_t d=0x650C94D5365C5EB9;
	printf("CORRECTION:\n");
	printf_uint64_t_binary(d);
	printf_uint32_t_binary(get_R16(d));
	printf("\nCE QUE JE TROUVE:\n");
	printf_uint64_t_binary((data.chiffre_faux[0].output));
	printf_uint32_t_binary((data.chiffre_faux[0].R16));
	printf("\n");*/

		//calcul_boite_s1_inv (&data);
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
	
	/*DATA data;
	data.k16.bytes=0x000210B12C4779;
	data.chiffre_juste.output=0x670994D1365D5EAD;
	data.message_clair=0xFBA2DC5EEAA7FEC2;


	find_K(&data);
	printf("\n!");
	printf_uint64_t_hexa(data.key);*/

	/*uint64_t expande;
	uint32_t R=0x8c91e251;
	expand(&expande, R);
	printf_uint32_t_binary(R);
	printf("\n");
	printf_uint64_t_binary(expande);
	printf("\n");*/

	return EXIT_SUCCESS;
}
