#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../inc/manip_bits.h"
#include "../inc/errors.h"
#include "../inc/inner_function.h"
#include "../inc/key_schedule.h"

int IP[64] = { 58, 50, 42, 34, 26, 18, 10, 2,
               60, 52, 44, 36, 28, 20, 12, 4,
               62, 54, 46, 38, 30, 22, 14, 6,
               64, 56, 48, 40, 32, 24, 16, 8,
               57, 49, 41, 33, 25, 17, 9, 1,
               59, 51, 43, 35, 27, 19, 11, 3, 
               61, 53, 45, 37, 29, 21, 13, 5,
               63, 55, 47, 39, 31, 23, 15, 7 };

int IP_inv[64] = { 40, 8, 48, 16, 56, 24, 64, 32,
                   39, 7, 47, 15, 55, 23, 63, 31,
                   38, 6, 46, 14, 54, 22, 62, 30,
                   37, 5, 45, 13, 53, 21, 61, 29,
                   36, 4, 44, 12, 52, 20, 60, 28,
                   35, 3, 43, 11, 51, 19, 59, 27,
                   34, 2, 42, 10, 50, 18, 58, 26,
                   33, 1, 41, 9, 49, 17, 57, 25 };


int process_permutation(uint64_t* data, int* IP){
	int i;
	uint64_t output=0x00;
	for(i=0;i<64;i++){
		uint8_t bit=get_bit_uint64_t_most(*data,IP[i]);
		if(set_bit_uint64_t(&output, bit, 64-i)) 
			return des_errno=ERR_BIT, 1;
	}
	*data=output;
	return 0;
}

int build_L0_R0(uint64_t data, uint32_t* L0, uint32_t* R0){
	*L0=data>>32;
	data<<=32;
	*R0=data>>32;
	return 0;
}

int rebuild_R16_L16(uint64_t* data, uint32_t L16, uint32_t R16){
	*data=R16;
	*data<<=32;
	*data|=L16;
	return 0;
}

int process_round_1_15_encryption(uint32_t* Li, uint32_t* Ri, SUB_KEY Kiadd1){

	uint32_t Liadd1 = *Ri;
    if(inner_function(Kiadd1, Ri)) 
			return des_errno=ERR_BIT, 1;
	*Ri= (*Li)^(*Ri); // R(i+1)=Li XOR F(K(i+1), Ri)
	*Li=Liadd1; // L(i+1)=Ri

	/*printf_uint32_t_binary(*Li);
	printf(" ");
	printf_uint32_t_binary(*Ri);
	//printf(" \n");*/

	return 0;

}

int process_round_16_encryption(uint32_t* L15, uint32_t* R15, SUB_KEY K16){ //PK CEST UNE SIMPLE INVERSION? 

	uint32_t L16 = *R15;
	if(inner_function(K16, R15)) 
			return des_errno=ERR_BIT, 1;

	*R15= (*L15)^(*R15); // R(16)=F(K16, R15) XOR L(15)
	*L15=L16; // L(16)=R(15)

	/*printf_uint32_t_binary(*L15);
	printf(" ");
	printf_uint32_t_binary(*R15);
	printf(" :15 \n");*/

	
	return 0;
}

int process_round_1_decryption(uint32_t* L0, uint32_t* R0, SUB_KEY K16){

	// L15 = R16 XOR F(K16, L16)
	// R15 = L16
	uint32_t R1=*R0;
	uint32_t L1=*L0;
	*R0=L1; // R1=L0
	//*L15=R16; //L1=R0
	if(inner_function(K16, L0)) 
			return des_errno=ERR_BIT, 1;
	*L0=(*L0)^(R1);

	/*printf_uint32_t_binary(*L0);
	printf(" ");
	printf_uint32_t_binary(*R0);
	printf(" :1 \n");*/

	return 0;
}

int process_round_2_15_decryption(uint32_t* Li, uint32_t* Ri, SUB_KEY Ki){


	uint32_t RiMin1 = *Li;
	if(inner_function(Ki, Li)) //modification de Li
			return des_errno=ERR_BIT, 1;
	*Li = (*Ri)^(*Li); //L(i+1)=R(i) XOR f(Li, Ki)
	*Ri = RiMin1; //R(i+1)=L(i)

	/*printf_uint32_t_binary(*Li);
	printf(" ");
	printf_uint32_t_binary(*Ri);*/
	

	return 0;
}

int encryption_des(uint64_t* data, uint64_t* key_64){
	/*printf("AVANT ENCRYPTION : \n");
	printf_uint64_t_hexa(*data);
	printf("\n");*/
	// permutation initiale
	if(process_permutation(data, IP)) 
		return des_errno=ERR_BIT, 1;
	/*printf("APRES PERMUTATION : \n");
	printf_uint64_t_hexa(*data);
	printf("\n");*/

	// division en L0 et R0
	uint32_t Li, Ri;
	if(build_L0_R0(*data, &Li, &Ri)) 
		return des_errno=ERR_BIT, 1;

	// création des 16 sous clés
	KEY key;
	if(key_schedule (key_64, &key))
		return des_errno=ERR_BIT, 1;

	// Schema de Feistel
	int i=0;
	for(i=0;i<15;i++){
		if(process_round_1_15_encryption(&Li, &Ri, key.sub_key[i])) 
			return des_errno=ERR_BIT, 1;
		//printf(" :%d\n", i);
	}
	if(process_round_16_encryption(&Li, &Ri, key.sub_key[15])) 
		return des_errno=ERR_BIT, 1;

	// permutation initiale inverse
	if(rebuild_R16_L16(data, Li, Ri))
		return des_errno=ERR_BIT, 1;
	/*printf("AVANT PERMUTATION : \n");
	printf_uint64_t_hexa(*data);
	printf("\n");*/
	if(process_permutation(data, IP_inv)) 
		return des_errno=ERR_BIT, 1;
	/*printf("APRES ENCRYPTION : \n");
	printf_uint64_t_hexa(*data);*/

	return 0;
}

int decryption_des(uint64_t* data, uint64_t* key_64){
	/*printf("AVANT DECRYPTION : \n");
	printf_uint64_t_hexa(*data);
	printf("\n");*/

	// permutation initiale 
	if(process_permutation(data, IP)) 
		return des_errno=ERR_BIT, 1;
	/*printf("APRES PERMUTATION : \n");
	printf_uint64_t_hexa(*data);
	printf("\n");*/

	// division en L0 et R0
	uint32_t Li, Ri;
	if(build_L0_R0(*data, &Li, &Ri)) 
		return des_errno=ERR_BIT, 1;

	// création des 16 sous clés
	KEY key;
	if(key_schedule (key_64, &key))
		return des_errno=ERR_BIT, 1;

	// Schema de Feistel
	/*if(process_round_0_decryption(&Li, &Ri, key.sub_key[15])) 
		return des_errno=ERR_BIT, 1;*/
	if(process_round_1_decryption(&Li, &Ri, key.sub_key[15])) 
		return des_errno=ERR_BIT, 1;
	int i; int j=2;
	for(i=14;i>=0;i--){
		if(process_round_2_15_decryption(&Li, &Ri, key.sub_key[i])) 
			return des_errno=ERR_BIT, 1;
		//printf(" :%d-->%d\n", j, i);
		j++;
	}
	
	// permutation initiale 
	if(rebuild_R16_L16(data, Li, Ri))
		return des_errno=ERR_BIT, 1;
	if(process_permutation(data, IP_inv)) 
		return des_errno=ERR_BIT, 1;

	//printf_uint64_t_hexa(*data);
	/*printf("APRES DECRYPTION : \n");
	printf_uint64_t_hexa(*data);*/

	return 0;
}

uint32_t get_R15(uint64_t cipher){
	// permutation initiale 
	if(process_permutation(&cipher, IP)) 
		return des_errno=ERR_BIT, 1;

	// division en L0 et R0
	uint32_t L16, R16;
	if(build_L0_R0(cipher, &L16, &R16)) 
		return des_errno=ERR_BIT, 1;
	//inversion des L16 et R16 donc R16<->L16

	return R16;
}

uint32_t get_R16(uint64_t cipher){
	// permutation initiale 
	if(process_permutation(&cipher, IP)) 
		return des_errno=ERR_BIT, 1;

	// division en L0 et R0
	uint32_t L16, R16;
	if(build_L0_R0(cipher, &L16, &R16)) 
		return des_errno=ERR_BIT, 1;
	//inversion des L16 et R16 donc R16<->L16
	return L16;
}
