/**
 * \file attack.c
 * \brief Représente les fonctions concernant l'attaque DFA sur DES. 
 * \author Clément CAUMES
 * */

#include <stdio.h>
#include <stdlib.h>

#include "../inc/key_schedule.h"
#include "../inc/inner_function.h"
#include "../inc/manip_bits.h"
#include "../inc/feistel.h"
#include "../inc/attack.h"
#include "../inc/constants.h"
#include "../inc/errors.h"

/**
 * \fn DATA initialize_data()
 * \brief Fonction d'initialisation des données pour réaliser l'attaque.  
 * \return renvoie la structure DATA initialisée avec les données obtenues. 
 */
DATA initialize_data(){
	DATA d;
	uint64_t chiffre_juste = 0x670994D1365D5EAD; //mon chiffre juste
	d.chiffre_juste.output = chiffre_juste; //mon chiffre juste
	d.chiffre_juste.R15 = get_R15(chiffre_juste);
	d.chiffre_juste.R16 = get_R16(chiffre_juste);
	d.message_clair=0xFBA2DC5EEAA7FEC2;

	uint64_t chiffre_faux[32] = {
		0x650C94D5365C5EB9, 0x671B94D5365C5EAD, 0x67099695365D5EAD, 0x66599097365C5EAD,
		0x665994D5345D5EAD, 0x664990D1265F5EAD, 0x660990D1365D5CAD, 0x664990D0364D5EAF,
		0x6F4990D076595EAD, 0x670194D0364D5EAD, 0x67099CD036595EAD, 0x270984D8765D5EAD,
		0x270994D03E195EAD, 0x670984D136555EAC, 0x270994D1365D56AC, 0x270984D1325D5EE4,
		0x470994D1361D4EEC, 0x672994D1325D4EED, 0x6709B4D1325D4FAD, 0x6709D5F1325D4FED,
		0x730994D1165D4EED, 0x7309D4D1367D5EAD, 0x630995D1365D7EAD, 0x6309D5C1365D5A8D,
		0xE30995C1365D1EAD, 0x678994C1365D1AAD, 0x670914C1375D5EAD, 0x670D9451365D1ABD,
	    0x670D94D1B75D5AB9, 0x670D94D136DD5EA9, 0x670C94D1365DDEB9, 0x671D9491365C5E3D,
	};

	int i;
	for(i=0;i<32;i++){
		d.chiffre_faux[i].output = chiffre_faux[i];
		d.chiffre_faux[i].R15 = get_R15(chiffre_faux[i]);
		d.chiffre_faux[i].R16 = get_R16(chiffre_faux[i]);
	}
	return d;
}

/**
 * \fn uint32_t get_R15(uint64_t cipher)
 * \brief Fonction qui calcule la valeur de R15 pour un chiffré donné.
 * \param cipher chiffré dont on veut calculer R15.   
 * \return renvoie R15 de cipher. 
 */
uint32_t get_R15(uint64_t cipher){ 
	// permutation initiale 
	if(process_permutation(&cipher, IP)) 
		return des_errno=ERR_ATTACK, 1;

	// division en L0 et R0
	uint32_t L16, R16;
	build_L0_R0(cipher, &L16, &R16);

	//inversion des L16 et R16 donc R16<->L16
	return R16;
}

/**
 * \fn uint32_t get_R16(uint64_t cipher)
 * \brief Fonction qui calcule la valeur de R16 pour un chiffré donné.
 * \param cipher chiffré dont on veut calculer R16.   
 * \return renvoie R16 de cipher. 
 */
uint32_t get_R16(uint64_t cipher){ 
	// permutation initiale 
	if(process_permutation(&cipher, IP)) 
		return des_errno=ERR_ATTACK, 1;

	// division en L0 et R0
	uint32_t L16, R16;
	build_L0_R0(cipher, &L16, &R16);

	//inversion des L16 et R16 donc R16<->L16
	return L16;
}

/**
 * \fn uint8_t find_intersection(int selection_key[64])
 * \brief Fonction qui renvoie l'intersection des clés candidates 
 * lors de l'attaque sur les S Box.
 * \param selection_key tableau représentant toutes les clés candidates.   
 * \return renvoie l'intersection des clés candidates. 
 */
uint8_t find_intersection(int selection_key[64]){
	uint8_t rang_max=0;
	uint8_t max=selection_key[0];
	int i;
	for(i=0;i<64;i++){
		if (max<selection_key[i]){
			max=selection_key[i];
			rang_max=i;
		}
	}
	return rang_max;
}

/**
 * \fn int attack_sbox(DATA* data, uint8_t* uint48_t_part, int num_sbox)
 * \brief Fonction qui fait l'attaque exhaustive pour trouver la portion de sous clé
 * associé à la sbox.
 * \param *data données qui contiennent les informations nécessaires à l'attaque.
 * \param *uint48_t_part représente la portion de sous clé associé à la sbox et qui sera 
 * donc initialisée.  
 * \param num_sbox représente le numéro de la sbox demandé. 
 * \return renvoie 0 en cas de réussite et 1 en cas d'échec. 
 */
int attack_sbox(DATA* data, uint8_t* uint48_t_part, int num_sbox){
	// Renvoie une erreur si le numéro de la sbox est incorrect. 
	if ((num_sbox<=0)||(num_sbox>8)) return 1;

	// Initialiation de toutes les variables
	int w, s, q, v, u, l, h; 
	int candidate_key[32][65];
	int* selection_key=malloc(64*sizeof(int));
	for(s=0;s<32;s++){
		for(w=0;w<64;w++){
			candidate_key[s][w]=0;
			selection_key[w]=0;
		}
		candidate_key[s][64]=1;
	}
	
	int compte=0; int temp=0; int to_do=1; int i=0; int compteur=0;
	uint32_t R15_chiffre_faux, perm;
	uint8_t val_sbox1, input1, input2, input3, input4, input5, input6, input7, input8;
	uint8_t k16_val_sbox1[4], k16_val_sbox2[4], val_sbox2[4], bit;
	uint8_t k16_1, k16_2, k16_3, k16_4, k16_5, k16_6, k16_7, k16_8, expand_R15_faux_xor_k16;
	uint8_t bits6_expand_R15=0; uint8_t bits6_expand_R15_faux; uint8_t boite=0x00; 
	uint64_t expand_R15, expand_R15_faux;
	
	// Calcul de E(R15) 
	expand(&expand_R15, data->chiffre_juste.R15); 

	// Calcul de E(R15)x->y avec [x,y] l'ensemble des bits correspondant à la boite s cherchée. 
	l=6;
	for(u=(48-(num_sbox-1)*6);u>=(43-(num_sbox-1)*6);u--){ 
		bit=get_bit_uint64_t(expand_R15, u);
		set_bit_uint8_t(&bits6_expand_R15, bit, l);
		l--;
	} 

	// Pour chaque chiffré faux. 
	for(i=0;i<32;i++){

		to_do=1;
		// Calcul de E(R15*) avec R15* le R15 du chiffré faux. 
		expand(&expand_R15_faux, data->chiffre_faux[i].R15);

		// Calcul de E(R15*)x->y avec [x,y] l'ensemble des bits correspondant à la boite s cherchée. 
		l=6;
		for(u=(48-(num_sbox-1)*6);u>=(43-(num_sbox-1)*6);u--){ 
			bit=get_bit_uint64_t(expand_R15_faux, u);
			set_bit_uint8_t(&bits6_expand_R15_faux, bit, l);
			l--;
		} 

		// Calcul de P-1(R16 XOR R16*).
		permutation_inv_inner_function(&perm, ((data->chiffre_faux[i].R16) ^ (data->chiffre_juste.R16)));

		// Calcul de P-1(R16 XOR R16*)x->y avec [x,y] l'ensemble des bits correspondant à la boite s cherchée. 
		boite=0x00; l=4;
		for(u=(32-(num_sbox-1)*4);u>=(29-(num_sbox-1)*4);u--){
			bit=get_bit_uint32_t(perm, u);
			set_bit_uint8_t(&boite, bit, l);
			l--;
		}

		/*
		 * Si P-1(R16 XOR R16*)x->y=0 alors ce chiffré n'apporte pas d'informations sur la 
		 * portion de sous clé associée à la boite S recherchée. 
		 */
		if (boite==0) { 
			to_do=0;
			candidate_key[i][64]=0;
		}

		/*
		 * Si E(R15)x->y=E(R15*)x->y alors ce chiffré n'apporte pas non plus d'informations sur la 
		 * portion de sous clé associée à la boite S recherchée. 
		 */
		if (bits6_expand_R15_faux==bits6_expand_R15){ 
			to_do=0;
			candidate_key[i][64]=0;
		}

		// Si ce chiffré approte des informations sur la portion de sous clé. 
		if (to_do==1){
			// On teste pour Sz(E(R15) XOR K16)x->y possible (avec Sz la boite S numéro z). 
			for(val_sbox1=0; val_sbox1<=15 ; val_sbox1++){
				/* 
				 * On fait la procédure inverse de la boite S (4 inputs donne le même output)
				 * On en déduit donc 4 possibilités pour E(R15) XOR K16.
				 */
				if (num_sbox==1) get_input_sbox(val_sbox1, S1, &input1, &input2, &input3, &input4);
				else if (num_sbox==2) get_input_sbox(val_sbox1, S2, &input1, &input2, &input3, &input4);
				else if (num_sbox==3) get_input_sbox(val_sbox1, S3, &input1, &input2, &input3, &input4);
				else if (num_sbox==4) get_input_sbox(val_sbox1, S4, &input1, &input2, &input3, &input4);
				else if (num_sbox==5) get_input_sbox(val_sbox1, S5, &input1, &input2, &input3, &input4);
				else if (num_sbox==6) get_input_sbox(val_sbox1, S6, &input1, &input2, &input3, &input4);
				else if (num_sbox==7) get_input_sbox(val_sbox1, S7, &input1, &input2, &input3, &input4);
				else get_input_sbox(val_sbox1, S8, &input1, &input2, &input3, &input4);

				// On en déduit donc 4 portions de clés possibles par Sz(E(R15) XOR K16).
				k16_val_sbox1[0]=input1^bits6_expand_R15;
				k16_val_sbox1[1]=input2^bits6_expand_R15;
				k16_val_sbox1[2]=input3^bits6_expand_R15;
				k16_val_sbox1[3]=input4^bits6_expand_R15;

				// Pour chaque portions de clés possibles. 
				for(v=0;v<4;v++){
					// On calcule (E(R15*) XOR K16) avec K16 la clé possible en question. 
					expand_R15_faux_xor_k16=bits6_expand_R15_faux^k16_val_sbox1[v];

					// Et on en déduit Sz(E(R15*) XOR K16).
					if (num_sbox==1) val_sbox2[v]=process_S_box_particular(expand_R15_faux_xor_k16, S1);
					else if (num_sbox==2) val_sbox2[v]=process_S_box_particular(expand_R15_faux_xor_k16, S2);
					else if (num_sbox==3) val_sbox2[v]=process_S_box_particular(expand_R15_faux_xor_k16, S3);
					else if (num_sbox==4) val_sbox2[v]=process_S_box_particular(expand_R15_faux_xor_k16, S4);
					else if (num_sbox==5) val_sbox2[v]=process_S_box_particular(expand_R15_faux_xor_k16, S5);
					else if (num_sbox==6) val_sbox2[v]=process_S_box_particular(expand_R15_faux_xor_k16, S6);
					else if (num_sbox==7) val_sbox2[v]=process_S_box_particular(expand_R15_faux_xor_k16, S7);
					else val_sbox2[v]=process_S_box_particular(expand_R15_faux_xor_k16, S8);

					// Si P-1(R16 XOR R16*)x->y = Sz(E(R15) XOR K16) Sz(E(R15*) XOR K16) avec K16 la clé possible.
					if (boite==(val_sbox2[v]^val_sbox1)){
						// K16 devient une clé CANDIDATE
						candidate_key[i][k16_val_sbox1[v]]++;
						selection_key[k16_val_sbox1[v]]++;
					}
				}
			}
		}
	}

	// Affichage des clés candidates. 
	printf("Recherche SBOX : ");
	for(s=0;s<32;s++){
		h=0;
		if (candidate_key[s][64]==1){
			for(w=0;w<64;w++){
				if (candidate_key[s][w]!=0) {
					printf_uint8_t_hexa(w);
					printf(" ");
					h++;
				}
			}
			printf("--> %d portions de sous clés possibles\n", h);
		}
		
	}

	// Recherche de l'intersection entre tous les chiffrés qui ont un impact sur la portion de sous clé. 
	*uint48_t_part=find_intersection(selection_key);
	printf("Portion de clé trouvée : %x\n\n", *uint48_t_part);

	free(selection_key);
	return 0;
}

/**
 * \fn int find_K16(DATA* data)
 * \brief Fonction qui trouve K16 à partir des données contenues dans *data.
 * \param *data données qui contiennent les informations nécessaires à l'attaque.
 * \return renvoie 0 en cas de réussite et 1 en cas d'échec. 
 */
int find_K16(DATA* data){

	printf("\nRECHERCHE DE K16 :\n");

	int i; uint8_t part_k16; uint64_t k16_temp;
	data->k16.bytes=0x00;
	// On cherche les 8 portions de sous clé de K16 (1 par Sbox).
	for(i=1; i<=8;i++){
		part_k16=0x00; k16_temp=0x00;
		if (attack_sbox(data, &part_k16, i)) 
			return des_errno=ERR_ATTACK, 1;
		k16_temp=part_k16;
		k16_temp<<=((8-i)*6);
		data->k16.bytes|=k16_temp;
	}

	printf("\nK16 trouvée : %LX\n", data->k16.bytes);
	return 0;
}

/**
 * \fn int build_C16_D16(uint48_t k16, uint32_t* C16, uint32_t* D16)
 * \brief Fonction qui déduit C16 et D16 à partir de k16.
 * \param k16 sous clé trouvée précedemment.
 * \param *C16 partie de l'algorithme de key schedule à initialiser.
 * \param *D16 partie de l'algorithme de key schedule à initialiser.
 * \return renvoie 0 en cas de réussite et 1 en cas d'échec. 
 */
int build_C16_D16(uint48_t k16, uint32_t* C16, uint32_t* D16){
	*C16=0; *D16=0;
	int i; uint8_t rang; uint8_t bit;
	for(i=0;i<48;i++){
		rang=PC2[i];
		bit=get_bit_uint64_t(k16.bytes, 48-i);
		if (rang<=28){ //C16
			if (set_bit_uint32_t(C16, bit, 29-rang))
				return 1;
		}
		else { //D16
			if (set_bit_uint32_t(D16, bit, (29-(rang-28))))
				return 1;
		}
	}
	return 0;
}

/**
 * \fn uint64_t build_K56(uint32_t C0, uint32_t D0)
 * \brief Fonction qui construit K (sur 56 bits) à partir de C0 et D0.
 * \param C0 partie de l'algorithme de key schedule avec C0=C16.
 * \param D0 partie de l'algorithme de key schedule avec D0=D16.
 * \return renvoie K sur 56 bits (avec 8 bits non encore trouvés). 
 */
uint64_t build_K56(uint32_t C0, uint32_t D0){
	uint64_t K56=0x00;
	K56=C0;
	K56<<=28;
	K56|=D0;
	return K56;
}

/**
 * \fn int build_K(uint64_t* K, uint32_t C16, uint32_t D16)
 * \brief Fonction qui construit K (sur 64 bits) à partir de C16 et D16.
 * \param *K clé à trouver sur 64 bits. 
 * \param C16 partie de l'algorithme de key schedule.
 * \param D16 partie de l'algorithme de key schedule.
 * \return renvoie K sur 64 bits (avec 16 bits non encore trouvés dont 8 bits de parité). 
 */
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

/**
 * \fn int set_parity_bits(uint64_t* K)
 * \brief Fonction qui modifie la clé pour mettre correctement les bits de parité.
 * \param *K clé à trouver sur 64 bits. 
 * \return renvoie 0 en cas de réussite et 1 en cas d'échec. 
 */
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

/**
 * \fn int find_K(DATA* data)
 * \brief Fonction qui trouve K sur 64 bits.
 * \param *data données qui contiennent les informations nécessaires à l'attaque.
 * \return renvoie 0 en cas de réussite et 1 en cas d'échec. 
 */
int find_K(DATA* data){

	// Deduit C16 et D16 à partir de K16
	uint32_t C16, D16;
	if (build_C16_D16(data->k16, &C16, &D16))
		return 1;

	// Construit K (sur 64 bits) avec 8 bits à trouver et 8 bits de parité non encore déduits. 
	if (build_K(&(data->key), C16, D16))
		return 1;

	// Faire une recherche exhaustive afin de trouver les 8 bits manquants 
	int j;
	uint8_t i, bit1, bit2, bit3, bit4, bit5, bit6, bit7, bit8;
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

		// Si le chiffrement du message clair avec la clé donne le chiffré correct alors la clé sur 64 bits a été trouvée. 
		if ((data->message_clair)==data->chiffre_juste.output){
			printf("K trouvée : ");
			printf_uint64_t_hexa(data->key);
			
			return 0;
		}
	}
	return 1;
}
