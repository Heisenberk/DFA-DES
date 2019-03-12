#include <stdio.h>
#include <stdlib.h>

#include "../inc/key_schedule.h"
#include "../inc/errors.h"
#include "../inc/inner_function.h"
#include "../inc/manip_bits.h"
#include "../inc/attack.h"
#include "../inc/constants.h"

int E[] = { 32, 1, 2, 3, 4, 5,
            4, 5, 6, 7, 8, 9,
            8, 9, 10, 11, 12, 13,
            12, 13, 14, 15, 16, 17,
            16, 17, 18, 19, 20, 21,
            20, 21, 22, 23, 24, 25,
            24, 25, 26, 27, 28, 29,
            28, 29, 30, 31, 32, 1};

int S1[4][16] = { 14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
			 	   0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
			       4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
                  15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13};

int S2[4][16] = { 15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
			       3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
			       0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
			      13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9};

int S3[4][16] = { 10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
			      13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
			      13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
			       1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12};

int S4[4][16] = { 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
			     13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
			     10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
			      3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14};

int S5[4][16] = { 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
			     14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
			      4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
			     11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3};

int S6[4][16] = { 12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
			      10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
			       9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
			       4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13};

int S7[4][16] = { 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
			     13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
			      1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
			      6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12};

int S8[4][16] = { 13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
			       1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
			       7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
                   2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11};

int P[32] = { 16, 7, 20, 21,
              29, 12, 28, 17,
               1, 15, 23, 26,
               5, 18, 31, 10,
               2, 8, 24, 14,
              32, 27, 3, 9,
              19, 13, 30, 6,
              22, 11, 4, 25 };

int expand(uint64_t* expand, uint32_t R){
	*expand=0;
	int i; uint8_t bit;
	for(i=0;i<48;i++){
		bit = get_bit_uint32_t_most(R, E[i]);
		if(set_bit_uint64_t (expand, bit, 48-i))
			return des_errno=ERR_BIT, 1;
	}
	return 0;
}

int process_S_box(uint32_t* result, SUB_KEY elem){
	*result=0;
	int i, j, k;
	uint8_t bit1, bit2;
	uint8_t input_sbox;
	uint8_t x, y; 
	uint32_t output_shift;
	int shift;
	*result=0;
	for(i=0;i<8;i++){
		x=y=0;
		input_sbox = get_6bits_uint64_t_most (elem, i+1);
		bit1 = get_bit_uint8_t(input_sbox, 6);
		bit2 = get_bit_uint8_t(input_sbox, 1);
		bit1<<=1;
		y|=bit1;
		y|=bit2;

		k=3;
		for(j=5; j>1;j--){
			bit1 = get_bit_uint8_t(input_sbox, j);
			bit1<<=k;
			x|=bit1;
			k--;
		}

		uint8_t val_sbox;
		if(i==0) val_sbox=S1[y][x];
		else if(i==1) val_sbox=S2[y][x];
		else if(i==2) val_sbox=S3[y][x];
		else if(i==3) val_sbox=S4[y][x];
		else if(i==4) val_sbox=S5[y][x];
		else if(i==5) val_sbox=S6[y][x];
		else if(i==6) val_sbox=S7[y][x];
		else if(i==7) val_sbox=S8[y][x];

		shift=(8-i-1)*4;
		output_shift=val_sbox;
		output_shift<<=shift;
		*result|=output_shift;
	}
	return 0;
}

int permutation_inner_function(uint32_t* output, uint32_t input){
	int i;
	*output=0x00;
	for(i=0;i<32;i++){
		uint8_t bit=get_bit_uint32_t_most(input,P[i]);
		if(set_bit_uint32_t(output, bit, 32-i)) 
			return des_errno=ERR_BIT, 1;
	}
	return 0;
}

int permutation_inv_inner_function(uint32_t* output, uint32_t input){
	*output=0;
	int i;
	*output=0x00;
	for(i=0;i<32;i++){
		uint8_t bit=get_bit_uint32_t_most(input, i+1);
		if (set_bit_uint32_t(output, bit, 33-P[i]))
			return des_errno=ERR_BIT, 1;
	}
	return 0;
}

int inner_function(SUB_KEY sub_key, uint32_t* R){

	// calcul de E(Ri)
	uint64_t expande;
	if(expand(&expande, *R))
		return des_errno=ERR_BIT, 1;

	// E(Ri) ^ Ki+1
	sub_key.bytes ^= expande;

	// calcul des S-Box
	uint32_t output_sbox;
	if(process_S_box(&output_sbox, sub_key))
		return des_errno=ERR_BIT, 1;

	// Permutation
	uint32_t final;
	if(permutation_inner_function(&final, output_sbox)) 
			return des_errno=ERR_BIT, 1;
	*R = final;
	return 0;
}

int get_input_sbox(uint8_t output, int S[4][16], uint8_t* input1, uint8_t* input2, uint8_t* input3, uint8_t* input4){
	int tour=0;
	uint8_t input; uint8_t i,j; uint8_t bit;
	input=0;
	for(i=0;i<4;i++){
		for(j=0;j<16;j++){
			input=0;
			if (S[i][j]==output) {
				bit=get_bit_uint8_t(i,2);
				if (set_bit_uint8_t(&input, bit, 6)) 
					return 1;
				bit=get_bit_uint8_t(i,1);
				if (set_bit_uint8_t(&input, bit, 1))
					return 1;

				bit=get_bit_uint8_t(j,4);
				if (set_bit_uint8_t(&input, bit, 5))
					return 1;
				bit=get_bit_uint8_t(j,3);
				if (set_bit_uint8_t(&input, bit, 4))
					return 1;
				bit=get_bit_uint8_t(j,2);
				if (set_bit_uint8_t(&input, bit, 3))
					return 1;
				bit=get_bit_uint8_t(j,1);
				if (set_bit_uint8_t(&input, bit, 2))
					return 1;
				if (tour==0) *input1=input;
				else if (tour==1) *input2=input;
				else if (tour==2) *input3=input;
				else if (tour==3) *input4=input;
				tour++;
			}	
		}
	}
	return 0;
}

int compte_possibilites_S1(uint8_t sortie){
	int i,j; 
	int compteur=0;
	for(i=0;i<4;i++){
		for(j=0;j<16;j++){
			if (S1[i][j]==sortie) compteur++;
		}
	}
	return compteur;
}

uint8_t process_S_box_particular(uint8_t input, int S[4][16]){
	uint8_t bit1, bit2;
	bit1=get_bit_uint8_t(input, 6);
	bit2=get_bit_uint8_t(input, 1);
	int x, y;
	x=bit1; x<<=1; x|=bit2;
	y=(0x1e)&input;
	y>>=1;
	return (uint8_t)S[x][y];

}

void calcul_boite_s1_inv (DATA *data){
	int j;
	uint8_t input1, input2, input3, input4; 
	uint8_t input5, input6, input7, input8;
	uint8_t k16_1, k16_2, k16_3, k16_4;
	SUB_KEY k;
	uint64_t input_chiffre_faux;
	int i=0; uint32_t R15_chiffre_faux;
	uint32_t perm;
	int compteur=32;
	uint32_t result_output_sbox_faux;

	uint64_t expand_R15, expand_R15_faux;
	uint8_t bits6_expand_R15=0; uint8_t bit6_expand_R15_faux;
	expand(&expand_R15, data->chiffre_juste.R15); //expand_R15=E(R15)

	printf("E(R15)=");
	printf_uint64_t_hexa(expand_R15);
	printf("\n");

	int l=6; int u; uint8_t bit;
	for(u=48;u>=43;u--){ //pour otenir les bits 1 a 6 (boiteS1)
		bit=get_bit_uint64_t(expand_R15, u);
		set_bit_uint8_t(&bits6_expand_R15, bit, l);
		l--;
	} //bits6_expand_R15=E(R15)1->6
	
	printf("E(R15)1->6=");
	printf_uint8_t_binary(bits6_expand_R15); //E(R15)1->6
		
	// pour chaque chiffré faux
	for(i=0;i<32;i++){
		printf("\n");
		printf_uint64_t_hexa(data->chiffre_faux[i].output);
		printf(": 0x");
		printf_uint32_t_hexa(data->chiffre_faux[i].R15);
		printf(" ");
		
		// calcul de P-1(R16^R16*)
		permutation_inv_inner_function(&perm, data->chiffre_faux[i].R16 ^ data->chiffre_juste.R16);

		printf_uint32_t_binary(perm);
		printf("<<<<<<<<<<\n");
		printf("P-1(R16^R16*)_S1= "); //PROBLEME AU NIVEAU DE LA PERMUTATION
		int u; uint8_t bit; uint8_t boite=0x00;
		int l=4;
		for(u=32;u>=29;u--){
			bit=get_bit_uint32_t(perm, u);
			set_bit_uint8_t(&boite, bit, l);
			l--;
		}
		printf_uint8_t_binary(boite); //contient P −1 (R 16 ⊕ R 16 ∗)
		printf("\n");
		if (boite==0) {
			printf("P −1 (R 16 ⊕ R 16 ∗)_S1 nul");
			compteur--;
		}
		uint32_t test;
		permutation_inner_function(&test, perm);
		if (test==(data->chiffre_faux[i].R16 ^ data->chiffre_juste.R16)){
			printf("OK");
		}
		else printf("FAUX");
		uint8_t val_sbox1, val_sbox2;
		for(val_sbox1=0; val_sbox1<15; val_sbox1++){
			for(val_sbox2=0; val_sbox2<15; val_sbox2++){
				if ((val_sbox1^val_sbox2)==boite){
					printf_uint8_t_binary(val_sbox1); //contient S1(E(R15)⊕K16)
					printf(" XOR ");
					printf_uint8_t_binary(val_sbox2); //contient S1(E(R15*)⊕K16)
					printf(":\n");

					//met les differentes possibilites pour E(R15)⊕K16
					get_input_sbox(val_sbox1, S1, &input1, &input2, &input3, &input4);
					


					printf("K16(1->6): ");
					k16_1=input1^bits6_expand_R15;
					k16_2=input2^bits6_expand_R15;
					k16_3=input3^bits6_expand_R15;
					k16_4=input4^bits6_expand_R15;
					printf_uint8_t_binary(k16_1);
					if (k16_1==0) printf("---------");
					printf(" OU ");
					printf_uint8_t_binary(k16_2);
					if (k16_2==0) printf("---------");
					printf(" OU ");
					printf_uint8_t_binary(k16_3);
					if (k16_3==0) printf("---------");
					printf(" OU ");
					printf_uint8_t_binary(k16_4);
					if (k16_4==0) printf("---------");
					printf("\n");

					/*expand(&expand_R15_faux, data->chiffre_faux[i].R15); //expand_R15_faux=E(R15*)
					l=6;
					for(u=48;u>=43;u--){ //pour otenir les bits 1 a 6 (boiteS1)
						bit=get_bit_uint64_t(expand_R15_faux, u);
						set_bit_uint8_t(&bit6_expand_R15_faux, bit, l);
						l--;
					} //bits6_expand_R15_faux=E(R15*)1->6

					result_output_sbox_faux=bit6_expand_R15_faux^k16_1; //E(R15*)1->6 XOR K16(1->6)
					if (val_sbox2==process_S_box_particular(result_output_sbox_faux, S1)){
						printf("OOOOOOOOOOOOOOOO:%d\n\n\n\n",k16_1);

					}

					result_output_sbox_faux=bit6_expand_R15_faux^k16_2; //E(R15*)1->6 XOR K16(1->6)
					if (val_sbox2==process_S_box_particular(result_output_sbox_faux, S1)){
						printf("OOOOOOOOOOOOOOOO:%d\n\n\n\n", k16_2);
					}

					result_output_sbox_faux=bit6_expand_R15_faux^k16_3; //E(R15*)1->6 XOR K16(1->6)
					if (val_sbox2==process_S_box_particular(result_output_sbox_faux, S1)){
						printf("OOOOOOOOOOOOOOOO:%d\n\n\n\n", k16_3);
					}

					result_output_sbox_faux=bit6_expand_R15_faux^k16_4; //E(R15*)1->6 XOR K16(1->6)
					if (val_sbox2==process_S_box_particular(result_output_sbox_faux, S1)){
						printf("OOOOOOOOOOOOOOOO:%d\n\n\n\n", k16_4);
					}*/
				}
			}
		}

	}
	printf(">>>>>>>>>%d\n", compteur);
}



