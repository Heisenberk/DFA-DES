#include <stdio.h>
#include <stdlib.h>

#include "../inc/key_schedule.h"
#include "../inc/inner_function.h"
#include "../inc/manip_bits.h"
#include "../inc/feistel.h"
#include "../inc/attack.h"
#include "../inc/constants.h"

int main(){

	DATA data = initialize_data();
	if (find_K16(&data))
		return EXIT_FAILURE;
	if (find_K(&data))
		return EXIT_FAILURE;
	

	return EXIT_SUCCESS;
}
