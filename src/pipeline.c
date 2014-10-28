#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>			//Gestion du temps
#include "define.h"
#include "liste.h"			//Gestion des breakpoints
#include "disasm.h"
#include "execute.h"
#include "fonctions.h"
#include "pipeline.h"
#include "common/notify.h"

void exceptionHandler(exception number){
	//Permet de parser les erreurs et exceptions
	switch(number) {
		case OK:
		break;

		case EmptyPipe:
			WARNING_MSG("Initialisation du pipeline");
		break;


		case InvalidInstruction:
			WARNING_MSG("Invalid instruction at adress %X",reg_mips[PC]);
		break;


		default :
			WARNING_MSG("Unknown error - Number %d", number);
		break;
	}
}


int fetch(instruction* pinsIF) {	//Manque appartenance au .text    -> Probleme de pipe en fin de programme
    if(getInstr(reg_mips[PC],pinsIF)==1) {
        reg_mips[PC]+=4;
        return OK;
    }else{
    	return InvalidInstruction;
    }
}


int decode(instruction insID, int* res) {
	if(insID.value==-1) return EmptyPipe; //On ne decode pas au demarrage

	int dico_entry=0;

    while((insID.value & dico_data[dico_entry].mask)!= dico_data[dico_entry].instr) {
        dico_entry++;
    }
    if (dico_entry>=nbinstr) return InvalidInstruction;
    else *res=dico_entry; return OK;
}


int pipeline(instruction insID, instruction insEX, instruction insMEM, instruction insWB, uint32_t end, state running, int affichage) {
	if(affichage==1)WARNING_MSG("Nouvelle iteration");
//Clock
	int tick= clock();


//Test memoire chargee
    if(memory==NULL) {
        WARNING_MSG("No memory loaded");
        return -1;
    }

//Fetch
    instruction insIF; //Creation de la nouvelle instruction
    exceptionHandler(fetch(&insIF));
//Decode
    //Resolution des adresses registre ?
    int dico_entry=-1;
 	exceptionHandler(decode(insID,&dico_entry));
//Affichage
    if(affichage==1){
    	//disasm(reg_mips[PC]-4,1);
    	printf("\nPC: %8.8X->%8.8X\t Fetched: %8.8X\n",reg_mips[PC]-4, reg_mips[PC], insIF.value);
    	printf("Decoding: %8.8X  Dico: %d-> %s\n", insID.value, dico_entry, dico_data[dico_entry].name);
    }

//Execute
    //int tmp;
    //exceptionHandler(execute(insEX,EX,&tmp));

//Memory
    //exceptionHandler(execute(insMEM,MEM,&tmp));

//Write Back
    //exceptionHandler(execute(insWB,WB,&tmp));


//Temporisation
    if(clocktime!=0){
	    tick=clock()-tick;
	    printf("Temps mis: %fms\t", (double)tick/CLOCKS_PER_SEC*1000);
	  	printf("Attente de %ldms\n", clocktime-tick/CLOCKS_PER_SEC*1000);
	    if((double)tick/CLOCKS_PER_SEC*1000<clocktime) DELAY((clocktime-tick/CLOCKS_PER_SEC*1000)*1000); //*1000 pour le passage en us->ms
	}

//Test de sortie
    if(reg_mips[PC]>=end || present(reg_mips[PC],breaklist)!=NULL) {
        printf("\nFin du run\n");
        return 0;
    } else {
        return pipeline(insIF, insID, insEX, insMEM, end, running, affichage);
    }
}