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
