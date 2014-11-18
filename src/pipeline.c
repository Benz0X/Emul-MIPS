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
#include "emul.h"


int exceptionHandler(exception number) {
    //Permet de parser les erreurs et exceptions
    switch(number) {

//Exceptions relatives au pipeline
    case OK:
        break;

    case EmptyPipe:     //Lorsque le pipe est tout juste initialisé et encore vide au début du programme
        //WARNING_MSG("Empty pipe");
        break;


    case InvalidInstruction:        //Lorsque l'instruction fetch n'existe pas
        if (verbose==1||verbose>2) {
            WARNING_MSG("Invalid instruction at adress %X",reg_mips[PC]);
        }
        break;

    case InvalidExecution:          //Lorsque l'execution n'est pas possible
        if (verbose>=5) {
            WARNING_MSG("Invalid execution in pipe : Pipe may be empty");
        }
        break;

    case flush:         //A la suite d'un jump, renvoie le flag pour flush en fin de cycle du pipeline (et non après la detection)
        return flush;
        break;


//Exceptions relatives au programme
    case memFail:       //Lorsqu'une instruction effectue une écriture mémoire illégale
        if (verbose==1||verbose>2) {
            WARNING_MSG("Invalid memory write");
        }
        break;

    case IntegerOverflow:
        if (verbose==1||verbose>2) {
            WARNING_MSG("IntegerOverflow : %8.8X",vpipeline[EX].ins.value);
        }
        break;

    case BreakPoint:
        if (verbose==1||verbose>2) {
            WARNING_MSG("Break : %8.8X",vpipeline[EX].ins.value);
        }
        return BreakPoint;
        break;

    case SysCall:
        if (verbose==1||verbose>2) {
            INFO_MSG("Syscall : %d",reg_mips[2]);
        }
        switch (reg_mips[2]) { //v0
        case 1:
            printf("%d\n",reg_mips[4]); //a0
            break;
        case 4:
            ;
            int i=0,c;
            do {
                memRead(reg_mips[4]+i,0,&c);
                printf("%c",(char)c);
                i+=1;
            }
            while(((char)c)!='\0');
            printf("\n");
            break;
        case 5:
            if(verbose>0)printf("Entrez un décimal ou héxadécimal:\n");
            char n;
            while((scanf("%X%c",&reg_mips[2],&n)!=2 || n!='\n') && clean_stdin()) {
                WARNING_MSG("Ceci n'est pas un entier valide");
            };
            break;
        case 8:
            ;
            int j=0;
            char *input=calloc(INPUT_SIZE,sizeof(char));
            //fgets(input,reg_mips[5],stdin);
            if(verbose>0)printf("Entrez une chaine de caractère :\n");
            scanf("%s",input);
            do
            {
                memWrite(reg_mips[4]+j,0,input[j]);
                j++;
            } while((input[j-1])!='\0' && j < reg_mips[5]);
            memWrite(reg_mips[4]+j-1,0,'\0'); //Fin forcée de la chaine de caractère
            break;
        case 10:
            INFO_MSG("Exit called by program");
            return quit;
            break;
        default:
            if (verbose==1||verbose>2) {
                WARNING_MSG("Unknown SysCall, %d",reg_mips[2]);
            }
            break;
        }
        break;



    default :
        WARNING_MSG("Unknown error - Number %d", number);
        break;
    }
    return OK;
}


int fetch(instruction* pinsIF) {
    if(getInstr(reg_mips[PC],pinsIF)==1) {  //Si l'instruction est valide on la stocke dans IF
        reg_mips[PC]+=4;                    //On incrémente le PC
        return OK;
    } else {
        return InvalidInstruction;          //Sinon exception
    }
}


int decode(instruction insID, int* res) {
    if(insID.value==-1) return EmptyPipe; //On ne decode pas au demarrage

    int dico_entry=0;

    while((insID.value & dico_data[dico_entry].mask)!= dico_data[dico_entry].instr) {       //Tant que l'instruction n'est pas detectée on incrémente dico_entry
        dico_entry++;
    }
    if (dico_entry>=nbinstr) return InvalidInstruction; //Si elle ne correspond pas -> exception
    else *res=dico_entry;                               //Sinon on fixe le dico_entry dans ID
    return OK;
}

int execute(instruction insEX, pipestep EX, int dico_entry, int* tmp) {
    if (dico_entry==-1) return InvalidExecution;        //N'execute rien quand le pipeline est vide
    return dico_data[dico_entry].exec(insEX,EX,tmp);    //Sinon on execute la fonction pointée par le dictionnaire
}










int pipeline(uint32_t end, state running, int affichage) {
//Initialisation des variables internes
    int flag[5];
    int stall=0;

    if(verbose>3) {
        INFO_MSG("New clock cycle");  //Affiche l'état courant du pipeline
        printf("\tPipeline current state :\n ID %8.8X\t EX %8.8X\t MEM %8.8X\t WB %8.8X\n\n",vpipeline[ID].ins.value,vpipeline[EX].ins.value,vpipeline[MEM].ins.value,vpipeline[WB].ins.value);
    }
//Clock
    int tick= clock();


//Test memoire chargee
    if(memory==NULL) {
        WARNING_MSG("No memory loaded");
        return -1;
    }

//Test RegStall
    if(overlap(listWritedReg(vpipeline[MEM].ins,vpipeline[MEM].dico_entry),listReadedReg(vpipeline[EX].ins,vpipeline[EX].dico_entry))==1) {
        //Si les registres écrits par MEM et lus par EX coincident
        if(verbose>4) {
            INFO_MSG("Stall");
        }
        stall=1;    //Alors un stall est necéssaire
    }


    /*
    Execution principale
    */
//Write Back
    flag[WB] = exceptionHandler(execute(vpipeline[WB].ins,vpipeline[WB].step,vpipeline[WB].dico_entry,&(vpipeline[WB].tmp)));
//Memory
    exceptionHandler(execute(vpipeline[MEM].ins,vpipeline[MEM].step,vpipeline[MEM].dico_entry,&(vpipeline[MEM].tmp)));
//Execute
    flag[EX] = exceptionHandler(execute(vpipeline[EX].ins,vpipeline[EX].step,vpipeline[EX].dico_entry,&(vpipeline[EX].tmp)));
//Decode
    //Resolution des adresses registre ?
    int dico_entry=-1;
    exceptionHandler(decode(vpipeline[ID].ins,&dico_entry));
//Fetch
    exceptionHandler(fetch(&(vpipeline[IF].ins)));



//Gestion du stall (partie 1)
    if (stall==1) {
        reg_mips[PC]-=4;    //If stall PC doesn't increase
    }

//Temporisation
    if(clocktime!=0) {      //Si la clock est fixée
        tick=clock()-tick;
        if(verbose>0) {
            printf("Time spent: %g us\t", (double)tick/CLOCKS_PER_SEC*1000);
            printf("Waiting %g us\n", (double)(clocktime-tick/CLOCKS_PER_SEC*1000));
        }
        if((double)tick/CLOCKS_PER_SEC*1000<clocktime) DELAY((clocktime-tick/CLOCKS_PER_SEC*1000)*1); //On DELAY
    }

//Affichage
    if(verbose>3) {
        //disasm(reg_mips[PC]-4,1);
        printf("PC: %X->%X\t Fetched: %8.8X\n",reg_mips[PC]-4, reg_mips[PC], vpipeline[IF].ins.value);
        printf("Decoding: %8.8X  Dico: %d-> %s\n", vpipeline[ID].ins.value, dico_entry, dico_data[dico_entry].name);
        printf("Executing: %s\n",dico_data[vpipeline[EX].dico_entry].name);
        printf("MEM writing: %s\n", dico_data[vpipeline[MEM].dico_entry].name);
        printf("REG writing: %s\n", dico_data[vpipeline[WB].dico_entry].name);
        printf("\n");
    }

//Flush
    if(flag[EX]==flush) {
        if(verbose>4) {
            INFO_MSG("Flush Instruction Fetch");
        }
        addNOP(&vpipeline[IF]);         //On remplace l'instruction fetched par NOP
    }
//Avancement du pipeline
    pipecpy(&vpipeline[WB],vpipeline[MEM]);//WB becomes MEM
    addNOP(&vpipeline[MEM]);                    //If stall not needed, will be overwrited
    if(stall==0) {
        pipecpy(&vpipeline[MEM],vpipeline[EX]); //MEM becomes EX
        pipecpy(&vpipeline[EX],vpipeline[ID]);  //EX becomes ID

        vpipeline[EX].tmp=0;                    //Init EX stage
        vpipeline[EX].dico_entry=dico_entry;

        pipecpy(&vpipeline[ID],vpipeline[IF]);  //ID becomes IF
    }

//Step
    if (running==step_return && reg_mips[PC]==return_addr) {    //Lors d'un step, on attend d'arriver sur return_addr pour break
        return 0;
    }
    if (running==stepinto) {        //stepinto break
        running=stop;
    }
    if (running==step) {            //step passe le pipeline en mode step_return si EX est JAL ou JALR -> appel de procedure
        if (strcmp(dico_data[vpipeline[EX].dico_entry].name,"JAL")==0 || strcmp(dico_data[vpipeline[EX].dico_entry].name,"JALR")==0) {
            running=step_return; //will wait for return adress
            return_addr=reg_mips[PC];
            if (verbose==1||verbose>2) {
                printf("RETURN ADRESS SET TO %d\n",return_addr);
            }
        }
        else {
            running=stop;       //Sinon il agit comme stepinto
        }
    }

//Gestion fin de programme
    if(reg_mips[PC]>=end+16) {
        vpipeline[ID].ins.value=-1; //L'execution continue jusqu'a end+16, mais on fixe l'ID à -1 pour ne pas fetch illégalement -> EmptyPipe en sortie
    }

//Test de sortie
    if(flag[WB]==quit) {
        reg_mips[PC]=end+16;
        return 0;
    }
    if(reg_mips[PC]<textStart||reg_mips[PC]>end+16) {//Should not happen if program is correct
        WARNING_MSG("PC out of .text, halt");
        return -1;
    }
    if (reg_mips[PC]==end+16) {//end of file
        INFO_MSG("END OF PROGRAM, NEXT STEP WILL START IT AGAIN");
        return 0;
    }
    else if(present(reg_mips[PC],breaklist)!=NULL || flag[WB]==BreakPoint || running==stop) {//BreakPoint
        printf("Break\n");
        return 0;
    }
    else {//Execution
        return pipeline(end,running,affichage);
    }
}
