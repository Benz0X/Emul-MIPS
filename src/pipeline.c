#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "define.h"
#include "liste.h"
#include "disasm.h"
#include "execute.h"
#include "fonctions.h"
#include "pipeline.h"
#include "common/notify.h"


int fetch(instruction insIF){
	if(getInstr(reg_mips[PC],&insIF)==1) {
		reg_mips[PC]+=4;
		return 0;
	}
}

int pipeline(instruction insID, instruction insEX, instruction insMEM, instruction insWB, uint32_t end, int running, int affichage){
//Clock tick



//Test memoire chargee
	if(memory==NULL) {
        WARNING_MSG("No memory loaded");
        return -1;
    }

//Fetch
    instruction insIF; //Creation de la nouvelle instruction
    fetch(insIF);

//Decode 
    //Resolution des adresses registre

//Affichage
   if(affichage==1)disasm(reg_mips[PC]-4,1);

//Execute
	 

//Memory


//Write Back


//Test de sortie
	if(reg_mips[PC]>=end || present(reg_mips[PC],breaklist)!=NULL){
		printf("Fin du run\n");
		return 0;
	}else{
		return pipeline(insIF, insID, insEX, insMEM, end, running, affichage);
	}
}