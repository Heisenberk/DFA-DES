#include <stdio.h>
#include <stdlib.h>

#include "../inc/key_schedule.h"
#include "../inc/inner_function.h"
#include "../inc/manip_bits.h"
#include "../inc/feistel.h"
#include "../inc/attack.h"

int main(){

	DATA data = initialize_data();

	int i; uint32_t R15_chiffre_faux;
	for(i=0;i<32;i++){
		printf_uint64_t_hexa(data.chiffre_faux[i].output);
		printf(": 0x");
		printf_uint32_t_hexa(data.chiffre_faux[i].R15);
		printf("\n");
	}
	
	return EXIT_SUCCESS;
}
