#include <stdio.h>
#include <stdlib.h>

#include "../inc/key_schedule.h"
#include "../inc/inner_function.h"
#include "../inc/manip_bits.h"
#include "../inc/feistel.h"
#include "../inc/attack.h"

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
	}
	return d;
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

int brute_force_attack(DATA* data){
	uint32_t C16, D16;
	if (build_C16_D16(data->k16, &C16, &D16))
		return 1;
	/*printf("C16= ");
	printf_uint32_t_binary(C16);
	printf("\nD16= ");
	printf_uint32_t_binary(D16);
	printf("\n");*/

	if (build_K(&(data->key), C16, D16))
		return 1;

	/*printf("\n:-------B-----XXB--XX---B-------B-------B-------B--X--X-B-X-X---B");
	printf("\n>");
	printf_uint64_t_binary((data->key));
	printf("!");
	//printf_uint64_t_binary(0x133457799BBCDFF1); //13 34 57 79 9B BC DF F1
	printf_uint64_t_binary(0x123556789ABDDEF0);*/

	uint8_t i;
	int j;
	uint8_t bit1, bit2, bit3, bit4, bit5, bit6, bit7, bit8;
	uint64_t message_clair=data->message_clair;
	for (i=0;i<0xFF; i++){
		/*uint64_t m=0x0123456789ABCDEF;
		uint64_t k=0x133457799BBCDFF1;
		encryption_des(&m, &k);*/
		

		// recherche exhaustive sur les 8 bits non connus
		bit1=get_bit_uint8_t(i,8);
		if (set_bit_uint64_t(&(data->key), bit1, 7)) //le premier bit a modifier est a la position 7
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
		/*printf(">KEY:");
		printf_uint64_t_hexa((data->key));
		//printf("\n");
		if ((data->key)==0x123556789ABDDEF0) printf(" TROUVE ! ");
		printf("\n CLAIR: ");*/
		//printf_uint64_t_hexa(data->message_clair);
		encryption_des(&(data->message_clair), (data->key));
		
		/*printf("\n CHIFFRE: ");
		printf_uint64_t_hexa(data->message_clair);
		printf("\n");*/
		if ((data->message_clair)==data->chiffre_juste.output){
			//printf("OK");
			return 0;
		}

		
		//remettre message clair comme avant

	}
	//printf("NOT OK");
	return 1;
}
