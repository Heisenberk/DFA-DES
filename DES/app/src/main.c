#include <stdio.h>
#include <stdlib.h>

#include "../inc/key_schedule.h"
#include "../inc/inner_function.h"
#include "../inc/manip_bits.h"
#include "../inc/feistel.h"

int main(){


	/*uint64_t data=0x0123456789ABCDEF;
	uint64_t key=0x133457799BBCDFF1;
	encryption_des(&data, &key);

	printf("\n\n");

	if(decryption_des(&data, &key))
		printf("erreur");

	printf("\n");*/

	//printf_uint64_t_hexa(data);*/

	// etape 1 : trouver R15 à partir du chiffré juste
	uint64_t chiffre_juste = 0x670994D1365D5EAD; //mon chiffre juste
	uint32_t R15_chiffre_juste = get_R15(chiffre_juste);

	// etape 2 : trouver les R15 à partir des chiffrés faux
	uint64_t chiffre_faux[32] = {
		0x650C94D5365C5EB9,
		0x671B94D5365C5EAD,
		0x67099695365D5EAD,
		0x66599097365C5EAD,
		0x665994D5345D5EAD,
		0x664990D1265F5EAD,
		0x660990D1365D5CAD,
		0x664990D0364D5EAF,
		0x6F4990D076595EAD,
		0x670194D0364D5EAD,
		0x67099CD036595EAD,
		0x270984D8765D5EAD,
		0x270994D03E195EAD,
		0x670984D136555EAC,
		0x270994D1365D56AC,
		0x270984D1325D5EE4,
		0x470994D1361D4EEC,
		0x672994D1325D4EED,
		0x6709B4D1325D4FAD,
		0x6709D5F1325D4FED,
		0x730994D1165D4EED,
		0x7309D4D1367D5EAD,
		0x630995D1365D7EAD,
		0x6309D5C1365D5A8D,
		0xE30995C1365D1EAD,
		0x678994C1365D1AAD,
		0x670914C1375D5EAD,
		0x670D9451365D1ABD,
	    0x670D94D1B75D5AB9,
		0x670D94D136DD5EA9,
		0x670C94D1365DDEB9,
		0x671D9491365C5E3D,
	};

	int i; uint32_t R15_chiffre_faux;
	for(i=0;i<32;i++){
		printf_uint64_t_hexa(chiffre_faux[i]);
		R15_chiffre_faux = 0;
		R15_chiffre_faux = get_R15(chiffre_faux[i]);
		printf(": 0x");
		printf_uint32_t_hexa(R15_chiffre_faux);
		printf(": ");
		printf_uint32_t_binary(R15_chiffre_juste ^ R15_chiffre_faux);
		printf("\n");
	}

	
	return EXIT_SUCCESS;
}
