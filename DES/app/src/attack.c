#include <stdio.h>
#include <stdlib.h>

#include "../inc/key_schedule.h"
#include "../inc/inner_function.h"
#include "../inc/manip_bits.h"
#include "../inc/feistel.h"
#include "../inc/attack.h"
#include "../inc/constants.h"
#include "../inc/errors.h"

DATA initialize_data(){
	DATA d;
	uint64_t chiffre_juste = 0x670994D1365D5EAD; //mon chiffre juste
	d.chiffre_juste.output = chiffre_juste; //mon chiffre juste
	d.chiffre_juste.R15 = get_R15(chiffre_juste);
	d.chiffre_juste.R16 = get_R16(chiffre_juste);


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

	int i;
	for(i=0;i<32;i++){
		d.chiffre_faux[i].output = chiffre_faux[i];
		d.chiffre_faux[i].R15 = get_R15(chiffre_faux[i]);
		d.chiffre_faux[i].R16 = get_R16(chiffre_faux[i]);
	}
	return d;
}


uint32_t get_R15(uint64_t cipher){ //OOKKKKKKK
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

uint32_t get_R16(uint64_t cipher){ //OKKKKKK
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

void calcul_xor(DATA* data){
	int w, q, v;
	int candidate_key[64];
	for(w=0;w<64;w++){
		candidate_key[w]=0;
	}

	int compte=0;
	int temp=0;
	
	int i=0; uint32_t R15_chiffre_faux;
	uint8_t val_sbox1/*, val_sbox2*/;
	uint8_t input1, input2, input3, input4; 
	uint8_t input5, input6, input7, input8;
	uint8_t k16_val_sbox1[4];
	uint8_t k16_val_sbox2[4];
	uint8_t k16_1, k16_2, k16_3, k16_4;
	uint8_t k16_5, k16_6, k16_7, k16_8;
	uint8_t expand_R15_faux_xor_k16;
	uint8_t val_sbox2[4];
	uint32_t perm;
	int compteur=0;
	uint64_t expand_R15, expand_R15_faux;
	uint8_t bits6_expand_R15=0; uint8_t bits6_expand_R15_faux;


	expand(&expand_R15, data->chiffre_juste.R15); //expand_R15=E(R15)

	int u; uint8_t bit; uint8_t boite=0x00; int l;
	l=6;
	for(u=48;u>=43;u--){ //pour otenir les bits 1 a 6 (boiteS1)
		bit=get_bit_uint64_t(expand_R15, u);
		set_bit_uint8_t(&bits6_expand_R15, bit, l);
		l--;
	} //bits6_expand_R15=E(R15)1->6

	printf("R15=");
	printf_uint32_t_binary(data->chiffre_juste.R15); //32 bits ok
	printf("\nE(R15)=");
	printf_uint64_t_binary(expand_R15); //48 bits
	printf("E(R15)1->6=");
	printf_uint8_t_binary(bits6_expand_R15); //E(R15)1->6
	printf("\n\n");
	


	
	//for(i=0;i<32;i++){
		expand(&expand_R15_faux, data->chiffre_faux[i].R15); //expand_R15=E(R15*)

		l=6;
		for(u=48;u>=43;u--){ //pour otenir les bits 1 a 6 (boiteS1)
			bit=get_bit_uint64_t(expand_R15_faux, u);
			set_bit_uint8_t(&bits6_expand_R15_faux, bit, l);
			l--;
		} //bits6_expand_R15_faux=E(R15*)1->6

		printf("chiffre faux n°%d : ", i+1);
		printf_uint64_t_hexa(data->chiffre_faux[i].output);
		printf(" ; R15*=");
		printf_uint32_t_hexa(data->chiffre_faux[i].R15);
		printf("\nE(R15*)=");
		printf_uint64_t_binary(expand_R15_faux); //48 bits
		printf("E(R15*)1->6=");
		printf_uint8_t_binary(bits6_expand_R15_faux); //E(R15)1->6
		printf("\n");
		printf(" \nP-1(R16 XOR R16*)=");
		permutation_inv_inner_function(&perm, ((data->chiffre_faux[i].R16) ^ (data->chiffre_juste.R16)));
		printf_uint32_t_binary(perm);
		printf("\n");
		printf("P-1(R16^R16*)_S1_(1->4)= ");

		boite=0x00; l=4;
		for(u=32;u>=29;u--){
			bit=get_bit_uint32_t(perm, u);
			set_bit_uint8_t(&boite, bit, l);
			l--;
		}
		printf_uint8_t_binary(boite); //contient P −1 (R 16 ⊕ R 16 ∗)
		printf("\n");
		if (boite==0) {
			printf("P −1 (R 16 ⊕ R 16 ∗) nul");
		}

		// test
		/*bits6_expand_R15^=0x00;
		bits6_expand_R15_faux^0x00;
		printf("E(R15 )1->6 XOR K16 1->6=");
		printf_uint8_t_binary(bits6_expand_R15);
		printf("\n");
		printf("E(R15*)1->6 XOR K16 1->6=");
		printf_uint8_t_binary(bits6_expand_R15_faux);
		printf("\n");

		bits6_expand_R15=process_S_box_particular(bits6_expand_R15, S1);
		bits6_expand_R15_faux=process_S_box_particular(bits6_expand_R15_faux, S1);
		printf("S1(E(R15 )1->6 XOR K16 1->6)=");
		printf_uint8_t_binary(bits6_expand_R15);
		printf("\n");
		printf("S1(E(R15*)1->6 XOR K16 1->6)=");
		printf_uint8_t_binary(bits6_expand_R15_faux);
		printf("\n");
		printf_uint8_t_binary(bits6_expand_R15_faux^bits6_expand_R15);
		printf("\n");*/

		for(val_sbox1=0; val_sbox1<=15 ; val_sbox1++){
			//met les differentes possibilites pour E(R15)⊕K16
			get_input_sbox(val_sbox1, S1, &input1, &input2, &input3, &input4);
			printf("Si S1((E(R15) XOR K16))=");
			printf_uint8_t_binary(val_sbox1);
			printf("\n");
			printf("K16(1->6): ");
			// contient les clés possibles pour S1((E(R15) XOR K16))
			k16_val_sbox1[0]=input1^bits6_expand_R15;
			k16_val_sbox1[1]=input2^bits6_expand_R15;
			k16_val_sbox1[2]=input3^bits6_expand_R15;
			k16_val_sbox1[3]=input4^bits6_expand_R15;
			printf_uint8_t_binary(k16_val_sbox1[0]);
			printf(" OU ");
			printf_uint8_t_binary(k16_val_sbox1[1]);
			printf(" OU ");
			printf_uint8_t_binary(k16_val_sbox1[2]);
			printf(" OU ");
			printf_uint8_t_binary(k16_val_sbox1[3]);


			// ma technique
			for(v=0;v<4;v++){
				expand_R15_faux_xor_k16=bits6_expand_R15_faux^k16_val_sbox1[v];
				val_sbox2[v]=process_S_box_particular(expand_R15_faux_xor_k16, S1);
				printf("\nS1((E(R15*) XOR K16))=");
				printf_uint8_t_binary(val_sbox2[v]);
				
				if (boite==(val_sbox2[v]^val_sbox1)) printf("!");
			}
			printf("\n");
			

			/*
			//technique maxime
			val_sbox2 = boite ^ val_sbox1; //val_sbox2=S1((E(R15*) XOR K16))
			printf("\nS1((E(R15*) XOR K16))=");
			printf_uint8_t_binary(val_sbox2);

			get_input_sbox(val_sbox2, S1, &input5, &input6, &input7, &input8);
			printf("\n");
			printf("K16(1->6): ");
			// contient les clés possibles pour S1((E(R15*) XOR K16))
			k16_val_sbox2[0]=input5^bits6_expand_R15_faux;
			k16_val_sbox2[1]=input6^bits6_expand_R15_faux;
			k16_val_sbox2[2]=input7^bits6_expand_R15_faux;
			k16_val_sbox2[3]=input8^bits6_expand_R15_faux;
			printf_uint8_t_binary(k16_val_sbox2[0]);
			printf(" OU ");
			printf_uint8_t_binary(k16_val_sbox2[1]);
			printf(" OU ");
			printf_uint8_t_binary(k16_val_sbox2[2]);
			printf(" OU ");
			printf_uint8_t_binary(k16_val_sbox2[3]);*/

			

			/*for (w=0;w<4;w++){
				for(q=0;q<4;q++){
					if (k16_val_sbox1[w]==k16_val_sbox2[q]){
						printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Clé candidate : ");
						printf_uint8_t_binary(k16_val_sbox1[w]);
						candidate_key[k16_val_sbox1[w]]++;

					}
				}
			}
			temp=0;*/
			printf("\n\n");
		}
		printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n\n");

	//}

	for(w=0;w<64;w++){
		if (candidate_key[w]!=0) {
			printf("K16 : %d a %d itérations\n", w, candidate_key[w]);
		}
	}
}

void calcul_boite_s1 (DATA data){
	int i=0; uint32_t R15_chiffre_faux;
	uint32_t perm;
	int compteur=0;
	for(i=0;i<32;i++){
		printf_uint64_t_hexa(data.chiffre_faux[i].output);
		printf(": 0x");
		printf_uint32_t_hexa(data.chiffre_faux[i].R15);
		printf(" ");
		
		permutation_inv_inner_function(&perm, data.chiffre_faux[i].R16 ^ data.chiffre_juste.R16);
		printf_uint32_t_binary(perm);
		printf("\n");
		printf("P-1(R16^R16*)_S1= ");
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
			printf("P −1 (R 16 ⊕ R 16 ∗) nul");
		}
		uint64_t expand_R15, expand_R15_faux;
		expand(&expand_R15, data.chiffre_juste.R15);
		expand(&expand_R15_faux, data.chiffre_faux[i].R15);
		if (expand_R15==expand_R15_faux){
			printf("S 1 (E(R 15 ) ⊕ K 16 ) b 1 →b 4 ⊕ S 1 (E(R 15 ∗) ⊕ K 16 ) b 1 →b 4 nul");
		}

		uint8_t partie_K16; uint64_t K16=0x00;
		for(partie_K16=0; partie_K16<64;partie_K16++){
			K16=0x00;
			K16|=partie_K16;
			K16<<=42;
			SUB_KEY k1, k2;
			k1.bytes=expand_R15 ^ K16; //S1(E(R15) ⊕ K16) 
		
			k2.bytes=expand_R15_faux ^ K16; //S1(E(R15*) ⊕ K16) 
			uint32_t result_Sbox_juste, result_Sbox_faux;
			process_S_box(&result_Sbox_juste, k1);
			process_S_box(&result_Sbox_faux, k2);

			/*printf("S1(E(R15 ) XOR K16) :");
			printf_uint32_t_binary(result_Sbox_juste);
			printf("\n");
			printf("S1(E(R15*) XOR K16) :");
			printf_uint32_t_binary(result_Sbox_faux);
			printf("\n");*/

			result_Sbox_juste^=result_Sbox_faux; //S1(E(R15) ⊕ K16) ⊕ S1(E(R15*) ⊕ K16) 
			/*printf("------------------- :");
			printf_uint32_t_binary(result_Sbox_juste);
			printf("\n\n");*/

			
			// si result_sbox_juste : equation a ignorer

			uint8_t boite_s_xor=0x00;
			l=4;
			for(u=32;u>=29;u--){
				bit=get_bit_uint32_t(result_Sbox_juste, u);
				set_bit_uint8_t(&boite_s_xor, bit, l);
				l--;
			}
			if (boite_s_xor!=0) compteur++;
			/*printf_uint8_t_binary(boite_s_xor); //contient P −1 (R 16 ⊕ R 16 ∗)
			printf("\n");*/

			if (boite==boite_s_xor){
				if (partie_K16==0) printf("COUUUUUUUUUUUUUUCOUUUUUUUUUUUU");
				//compteur++;
				/*printf_uint64_t_hexa(K16);
				printf("\n");*/
				printf("OKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK, %d\n", partie_K16);
			}
		}
	}
	printf("\n\n>>>>>>>>%d\n", compteur);
}






int build_C16_D16(SUB_KEY k16, uint32_t* C16, uint32_t* D16){
	*C16=0;
	*D16=0;
	int i; uint8_t rang; uint8_t bit;
	for(i=0;i<48;i++){
		rang=PC2[i];
		bit=get_bit_uint64_t(k16.bytes, 48-i);
		if (rang<=28){ //Ci
			if (set_bit_uint32_t(C16, bit, 29-rang))
				return 1;
		}
		else { //Di

			if (set_bit_uint32_t(D16, bit, (29-(rang-28))))
				return 1;
		}
	}
	return 0;
}

// construit K56 a partir de C0=C16 et D0=D16
uint64_t build_K56(uint32_t C0, uint32_t D0){
	uint64_t K56=0x00;
	K56=C0;
	K56<<=28;
	K56|=D0;
	//printf_uint64_t_binary(K56);
	return K56;
}

int build_K(uint64_t* K, uint32_t C16, uint32_t D16){
	int i;
	uint8_t rang; uint8_t bit;

	uint64_t k56 = build_K56(C16, D16);
	*K=0x00;

	for(i=0;i<56;i++){
		rang=PC1[i];
		bit=get_bit_uint64_t_most(k56, i+1+8);
		if (set_bit_uint64_t(K, bit, 65-rang))
			return 1;
	}
	return 0;
}

int set_parity_bits(uint64_t* K){


	uint8_t bit1, bit2, bit3, bit4, bit5, bit6, bit7, bit8;
	int i; int j=1; int compteur;
	for(i=0;i<8;i++){
		compteur=0;
		bit1 = get_bit_uint64_t_most(*K, j);
		if (bit1==1) compteur++;
		bit2 = get_bit_uint64_t_most(*K, j+1);
		if (bit2==1) compteur++;
		bit3 = get_bit_uint64_t_most(*K, j+2);
		if (bit3==1) compteur++;
		bit4 = get_bit_uint64_t_most(*K, j+3);
		if (bit4==1) compteur++;
		bit5 = get_bit_uint64_t_most(*K, j+4);
		if (bit5==1) compteur++;
		bit6 = get_bit_uint64_t_most(*K, j+5);
		if (bit6==1) compteur++;
		bit7 = get_bit_uint64_t_most(*K, j+6);
		if (bit7==1) compteur++;

		if ((compteur%2)==0) bit8=1;
		else bit8=0;

		if (set_bit_uint64_t(K, bit8, ((8*(8-i))-7)))
			return 1;
		j+=8;
	}
	return 0;
}




int find_K(DATA* data){
	uint32_t C16, D16;
	if (build_C16_D16(data->k16, &C16, &D16))
		return 1;

	if (build_K(&(data->key), C16, D16))
		return 1;

	uint8_t i;
	int j;
	uint8_t bit1, bit2, bit3, bit4, bit5, bit6, bit7, bit8;
	uint64_t message_clair=data->message_clair;
	for (i=0;i<0xFF; i++){
	
		// recherche exhaustive sur les 8 bits non connus
		bit1=get_bit_uint8_t(i,8);
		if (set_bit_uint64_t(&(data->key), bit1, 7))
			return 1;
		bit2=get_bit_uint8_t(i,7);
		if (set_bit_uint64_t(&(data->key), bit2, 14))
			return 1;
		bit3=get_bit_uint8_t(i,6);
		if (set_bit_uint64_t(&(data->key), bit3, 46))
			return 1;
		bit4=get_bit_uint8_t(i,5);
		if (set_bit_uint64_t(&(data->key), bit4, 5))
			return 1;
		bit5=get_bit_uint8_t(i,4);
		if (set_bit_uint64_t(&(data->key), bit5, 50))
			return 1;
		bit6=get_bit_uint8_t(i,3);
		if (set_bit_uint64_t(&(data->key), bit6, 11))
			return 1;
		bit7=get_bit_uint8_t(i,2);
		if (set_bit_uint64_t(&(data->key), bit7, 51))
			return 1;
		bit8=get_bit_uint8_t(i,1);
		if (set_bit_uint64_t(&(data->key), bit8, 45))
			return 1;

		//ajout des bits de parité
		if (set_parity_bits(&(data->key)))
			return 1;

		data->message_clair=message_clair;
		encryption_des(&(data->message_clair), (data->key));

		if ((data->message_clair)==data->chiffre_juste.output) return 0;
	}
	return 1;
}
