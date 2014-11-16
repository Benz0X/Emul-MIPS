#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include "define.h"
#include "liste.h"
#include "user_int.h"
#include "fonctions.h"
#include "common/notify.h"
#include "emul.h"
#include "common/bits.h"
#include "execute.h"



void getFromScript(FILE *fileptr,char * input)
{
    if( fgets( input, INPUT_SIZE, fileptr ) == NULL)
    {
        scriptmode=0;
    }

}
void getFromUser(char * input)
{
    //fgets(input,INPUT_SIZE,stdin);
    char* temp;
    temp = readline( PROMPT_STRING );
    add_history( temp );
    strcpy(input, temp);
    //printf("l'entrée est %s\n",input);
}

command getCommand(char word[]) {
    if (!strcmp(word,"load"))
        return LOAD;
    if (!strcmp(word,"exit"))
        return EXIT;
    if (!strcmp(word,"disp"))
        return DISP;
    if (!strcmp(word,"disasm"))
        return DISASM;
    if (!strcmp(word,"set"))
        return SET;
    if (!strcmp(word,"assert"))
        return ASSERT;
    if (!strcmp(word,"debug"))
        return DEBUG;
    if (!strcmp(word,"resume"))
        return RESUME;
    if (!strcmp(word,"run"))
        return RUN;
    if (!strcmp(word,"step"))
        return STEP;
    if (!strcmp(word,"break"))
        return BREAK;
    if (!strcmp(word,"clock"))
        return CLOCK;
    if (!strcmp(word,"verbose"))
        return VERB;
    return UNKNOWN;
}


int what_type(char * word) {				// Test si une chaine de caractere est numerique et renvoie sa base.
    enum { INIT , DECIMAL_ZERO , HEXA , DECIMAL , OCTAL } ;
    int i=0;
    int c ;
    int S=INIT ;
    while (word[i] != '\0' && word[i] != '\n') {
        c=word[i];
        i++;
        switch (S) {
        case INIT:
            if(c=='-') {
                S=DECIMAL;
            }
            else if (isdigit(c)) {
                S = ( c=='0') ? DECIMAL_ZERO : DECIMAL ;
            }
            else return -1;
            break;
        case DECIMAL_ZERO:
            if ( c == 'x' || c == 'X' ) S=HEXA ;
            else if (isdigit(c) && c<'8') S=OCTAL;
            else return -1;
            break ;
        case HEXA:
            if(isdigit(c) || ((c <= 'f') && (c >= 'a')) || ((c <= 'F' ) && ( c >= 'A' ))) S=HEXA;
            else return -1;
            break ;
        case DECIMAL:
            if(isdigit(c)) S= DECIMAL;
            else return -1;
            break ;
        case OCTAL:
            if (isdigit(c) && c<'8') S=OCTAL;
            else return -1;

            break ;
        default:
            return -1;
        }
    }
    if (S==DECIMAL_ZERO)
    {
        S=DECIMAL;
    }
    return S;
}


int isHexa(char * word) {
    if (what_type(word)==2) {
        return 1;
    }
    return 0;
}


int isDecimal(char * word) {
    if (what_type(word)==3) {
        return 1;
    }
    return 0;
}

int isOctal(char * word) {
    if (what_type(word)==4) {
        return 1;
    }
    return 0;
}

int nextword(char** token, char* input, int* n) {				//Recupere dans un buffer le prochain mot d'un chaine de caractere.
    if(*n==0) {
        *token=strtok(input, " ");
        *n=*n+1;
    } else {
        *token=strtok(NULL, " ");
        *n=*n+1;
    }
    //printf("prochain mot : %s\n",*token);
    if(*token==NULL) {
        return 0;
    }
    else {
        return 1;
    }
}

void string_standardise( char* in, char* out ) {
    unsigned int i=0, j;
    //removes spaces
    while(in[i]==' ' && i<strlen(in)) {
        i++;
    }

    for ( j= 0; i< strlen(in); i++ ) {

        /* insert blanks around special characters*/
        if (in[i]==':' || in[i]=='+' || in[i]=='~') {
            out[j++]=' ';
            out[j++]=in[i];
            out[j++]=' ';

        }

        /* remove blanks after negation*/
        else if (in[i]=='-') {
            out[j++]=' ';
            out[j++]=in[i];
            while (isblank((int) in[i+1])) i++;
        }

        /* remove comment */
        else if (in[i]=='#') {
            out[j++]='\0';
        }


        /*remove nextline*/
        else if (in[i]=='\n') {
            out[j++]='\0';
        }

        /* translate tabs into white spaces*/
        else if (isblank((int) in[i])) out[j++]=' ';


        else out[j++]=in[i];
    }
    out[j++]='\0';
}

int readReg(char * reg_name, int32_t* value) {		//Lit dans un registre a travers value
    int i=isReg(reg_name);
    if(i==-1) {								//test -1 et 0 non modifiable
        WARNING_MSG("%s isn't a valid register",reg_name);
        return -1;
    } else {
        *value=reg_mips[i];
        return 0;
    }
}

int writeReg(char * reg_name, int32_t value) {		// Ecrit value dans un registre
    int i=isReg(reg_name);
    if(i<1) {								//test -1 et 0 non modifiable
        WARNING_MSG("%s isn't a valid register",reg_name);
        return -1;
    } else {
        reg_mips[i]=value;
        return i;
    }
}

int writeRegindex(int i, int32_t value) {      // Ecrit value dans un registre
    if(i<0 || i>NBREG+3) {                               //test -1 et 0 non modifiable
        WARNING_MSG("%d isn't a valid register",i);
        return -1;
    } else if(i>0) {
        reg_mips[i]=value;
    }
    return 0;
}



int isReg(char* reg_name) {					//Test si une chaine de caractere est un registre valide
    if(reg_name[0]=='$') {
        return isReg(reg_name+1); 					//Prise en charge du $ pour les reg
    }
    char * string;

    int index=-1;
    index = strtol(reg_name,&string,10);
    //printf("reg_name= %s string =%s, index=%d\n",reg_name,string,index );

    if (isdigit(reg_name[0]) && index<NBREG && index > -1) {
        return index;
    }
    else if (!isdigit(reg_name[0])) {
        if (!strcmp(string,"zero")) {
            index=0;
        }
        else if (!strcmp(string,"at")) {
            index=1;
        }
        else if (!strcmp(string,"v0")) {
            index=2;
        }
        else if (!strcmp(string,"v1")) {
            index=3;
        }
        else if (!strcmp(string,"a0")) {
            index=4;
        }
        else if (!strcmp(string,"a1")) {
            index=5;
        }
        else if (!strcmp(string,"a2")) {
            index=6;
        }
        else if (!strcmp(string,"a3")) {
            index=7;
        }
        else if (!strcmp(string,"t0")) {
            index=8;
        }
        else if (!strcmp(string,"t1")) {
            index=9;
        }
        else if (!strcmp(string,"t2")) {
            index=10;
        }
        else if (!strcmp(string,"t3")) {
            index=11;
        }
        else if (!strcmp(string,"t4")) {
            index=12;
        }
        else if (!strcmp(string,"t5")) {
            index=13;
        }
        else if (!strcmp(string,"t6")) {
            index=14;
        }
        else if (!strcmp(string,"t7")) {
            index=15;
        }
        else if (!strcmp(string,"s0")) {
            index=16;
        }
        else if (!strcmp(string,"s1")) {
            index=17;
        }
        else if (!strcmp(string,"s2")) {
            index=18;
        }
        else if (!strcmp(string,"s3")) {
            index=19;
        }
        else if (!strcmp(string,"s4")) {
            index=20;
        }
        else if (!strcmp(string,"s5")) {
            index=21;
        }
        else if (!strcmp(string,"s6")) {
            index=22;
        }
        else if (!strcmp(string,"s7")) {
            index=23;
        }
        else if (!strcmp(string,"t8")) {
            index=24;
        }
        else if (!strcmp(string,"t9")) {
            index=25;
        }
        else if (!strcmp(string,"k0")) {
            index=26;
        }
        else if (!strcmp(string,"k1")) {
            index=27;
        }
        else if (!strcmp(string,"gp")) {
            index=28;
        }
        else if (!strcmp(string,"sp")) {
            index=29;
        }
        else if (!strcmp(string,"fp")) {
            index=30;
        }
        else if (!strcmp(string,"ra")) {
            index=31;
        }
        else if (!strcmp(string,"HI")) {
            index=32;
        }
        else if (!strcmp(string,"LO")) {
            index=33;
        }
        else if (!strcmp(string,"PC")) {
            index=34;
        }

        else index=-1;
        return index;
    }
    index=-1;
    return index;
}

int parseReg(int index, char* reg_name) {			//Parse les registres a l'envers
    switch(index) {
    case 0 :
        strcpy(reg_name,"$zero");
        return 0;
    case 1 :
        strcpy(reg_name,"$at");
        return 0;
    case 2 :
        strcpy(reg_name,"$v0");
        return 0;
    case 3 :
        strcpy(reg_name,"$v1");
        return 0;
    case 4 :
        strcpy(reg_name,"$a0");
        return 0;
    case 5 :
        strcpy(reg_name,"$a1");
        return 0;
    case 6 :
        strcpy(reg_name,"$a2");
        return 0;
    case 7 :
        strcpy(reg_name,"$a3");
        return 0;
    case 8 :
        strcpy(reg_name,"$t0");
        return 0;
    case 9 :
        strcpy(reg_name,"$t1");
        return 0;
    case 10 :
        strcpy(reg_name,"$t2");
        return 0;
    case 11 :
        strcpy(reg_name,"$t3");
        return 0;
    case 12 :
        strcpy(reg_name,"$t4");
        return 0;
    case 13 :
        strcpy(reg_name,"$t5");
        return 0;
    case 14 :
        strcpy(reg_name,"$t6");
        return 0;
    case 15 :
        strcpy(reg_name,"$t7");
        return 0;
    case 16 :
        strcpy(reg_name,"$s0");
        return 0;
    case 17 :
        strcpy(reg_name,"$s1");
        return 0;
    case 18 :
        strcpy(reg_name,"$s2");
        return 0;
    case 19 :
        strcpy(reg_name,"$s3");
        return 0;
    case 20 :
        strcpy(reg_name,"$s4");
        return 0;
    case 21 :
        strcpy(reg_name,"$s5");
        return 0;
    case 22 :
        strcpy(reg_name,"$s6");
        return 0;
    case 23 :
        strcpy(reg_name,"$s7");
        return 0;
    case 24 :
        strcpy(reg_name,"$t8");
        return 0;
    case 25 :
        strcpy(reg_name,"$t9");
        return 0;
    case 26 :
        strcpy(reg_name,"$k0");
        return 0;
    case 27 :
        strcpy(reg_name,"$k1");
        return 0;
    case 28 :
        strcpy(reg_name,"$gp");
        return 0;
    case 29 :
        strcpy(reg_name,"$sp");
        return 0;
    case 30 :
        strcpy(reg_name,"$fp");
        return 0;
    case 31 :
        strcpy(reg_name,"$ra");
        return 0;
    case 32 :
        strcpy(reg_name,"$HI");
        return 0;
    case 33 :
        strcpy(reg_name,"$LO");
        return 0;
    case 34 :
        strcpy(reg_name,"$PC");
        return 0;

    default :
        return -1;
    }
}

int readDico(char* dico_name) {
    FILE* dico_file=NULL;
    dico_file = fopen (dico_name, "r");
    if (dico_file == NULL)
    {
        WARNING_MSG("Impossible d'ouvrir le dictionnaire '%s'",dico_name);
        return 1;
    }
    char line[INPUT_SIZE];
    char normalized_line[INPUT_SIZE];
    int i,j,k;
    do {
        getFromScript(dico_file,line);
        string_standardise(line, normalized_line);
    } while (normalized_line[0]=='\0');
//on obtient le nombre d'entrée du dico
    sscanf(line, "%d",&nbinstr); //retour d'erreur ?
    dico_data=calloc(nbinstr,sizeof(dico_info));
    for ( i = 0; i < nbinstr; ++i)
    {

        k=0;
        do {
            if(feof(dico_file)) {
                WARNING_MSG("End of dictionnary file reached, incorrect number of entry");
                return -1;
            }
            getFromScript(dico_file,line);
            string_standardise(line, normalized_line);
        } while (normalized_line[0]=='\0');
        char* word;
        //printf("%s\n",normalized_line );
        if(nextword(&word, normalized_line,&k)) {
            strcpy(dico_data[i].name,word);
            //	printf("%s\n",dico_data[i].name );
        }
        else {
            ERROR_MSG("Error reading name in dictionnary for entry %d",i);
        }
        if((nextword(&word, normalized_line,&k))&& (isHexa(word))) {
            dico_data[i].mask=strtol(word,NULL,16);
        }                                      //Recuperation du masque
        else {
            ERROR_MSG("Error reading mask in dictionnary for entry %d",i);
        }
        if((nextword(&word, normalized_line,&k))&& (isHexa(word))) {
            dico_data[i].instr=strtol(word,NULL,16);
        }                                 //Recuperation de la signature
        else {
            ERROR_MSG("Error reading instr in dictionnary for entry %d",i);
        }
        if((nextword(&word, normalized_line,&k))) {                                       //Recuperation du type
            if(!strcmp(word,"r")||!strcmp(word,"R")) {
                dico_data[i].type=0;
            }
            else if (!strcmp(word,"i")||!strcmp(word,"I"))
            {
                dico_data[i].type=1;
            }
            else if (!strcmp(word,"j")||!strcmp(word,"J"))
            {
                dico_data[i].type=2;
            }
            else {
                ERROR_MSG("Error reading type in dictionnary for entry %d",i);
            }
        }
        else {
            ERROR_MSG("Error reading type in dictionnary for entry %d",i);
        }
        if((nextword(&word, normalized_line,&k))&& (isDecimal(word))) {
            dico_data[i].nb_arg=strtol(word,NULL,10);
        }                                //Recuperation du nombre d'arguments
        else {
            ERROR_MSG("Error reading number of arg in dictionnary for entry %d",i);
        }
        for (j = 0; j < dico_data[i].nb_arg; ++j)                                         //Boucle
        {
            if((nextword(&word, normalized_line,&k))) {
                //printf("%s,%d,%d\n",word,i,j);

                strcpy(dico_data[i].argname[j],word);                                      //Recuperation de leurs noms
            }
            else {
                ERROR_MSG("Error reading argument dictionnary for entry %d, argument %d",i,j);
            }
        }
        if((nextword(&word, normalized_line,&k))) {
            WARNING_MSG("Too much argument in dictionnary data for instruction %d",i);
        }

//Ajout des pointeurs de fonctions
        if(!strcmp(dico_data[i].name,"ADD")) {
            dico_data[i].exec=ADD;
        }
        if(!strcmp(dico_data[i].name,"ADDI")) {
            dico_data[i].exec=ADDI;
        }
        if(!strcmp(dico_data[i].name,"ADDIU")) {
            dico_data[i].exec=ADDIU;
        }
        if(!strcmp(dico_data[i].name,"ADDU")) {
            dico_data[i].exec=ADDU;
        }
        if(!strcmp(dico_data[i].name,"AND")) {
            dico_data[i].exec=AND;
        }
        if(!strcmp(dico_data[i].name,"ANDI")) {
            dico_data[i].exec=ANDI;
        }
        if(!strcmp(dico_data[i].name,"BEQ")) {
            dico_data[i].exec=BEQ;
        }
        if(!strcmp(dico_data[i].name,"BGEZ")) {
            dico_data[i].exec=BGEZ;
        }
        if(!strcmp(dico_data[i].name,"BGTZ")) {
            dico_data[i].exec=BGTZ;
        }
        if(!strcmp(dico_data[i].name,"BLEZ")) {
            dico_data[i].exec=BLEZ;
        }
        if(!strcmp(dico_data[i].name,"BLTZ")) {
            dico_data[i].exec=BLTZ;
        }
        if(!strcmp(dico_data[i].name,"BNE")) {
            dico_data[i].exec=BNE;
        }
        if(!strcmp(dico_data[i].name,"BREAK")) {
            dico_data[i].exec=BREAKprog;
        }
        if(!strcmp(dico_data[i].name,"DIV")) {
            dico_data[i].exec=DIV;
        }
        if(!strcmp(dico_data[i].name,"J")) {
            dico_data[i].exec=J;
        }
        if(!strcmp(dico_data[i].name,"JAL")) {
            dico_data[i].exec=JAL;
        }
        if(!strcmp(dico_data[i].name,"JALR")) {
            dico_data[i].exec=JALR;
        }
        if(!strcmp(dico_data[i].name,"JR")) {
            dico_data[i].exec=JR;
        }
        if(!strcmp(dico_data[i].name,"LB")) {
            dico_data[i].exec=LB;
        }
        if(!strcmp(dico_data[i].name,"LBU")) {
            dico_data[i].exec=LBU;
        }
        if(!strcmp(dico_data[i].name,"LUI")) {
            dico_data[i].exec=LUI;
        }
        if(!strcmp(dico_data[i].name,"LW")) {
            dico_data[i].exec=LW;
        }
        if(!strcmp(dico_data[i].name,"MFHI")) {
            dico_data[i].exec=MFHI;
        }
        if(!strcmp(dico_data[i].name,"MFLO")) {
            dico_data[i].exec=MFLO;
        }
        if(!strcmp(dico_data[i].name,"MULT")) {
            dico_data[i].exec=MULT;
        }
        if(!strcmp(dico_data[i].name,"NOP")) {
            dico_data[i].exec=NOP;
        }
        if(!strcmp(dico_data[i].name,"OR")) {
            dico_data[i].exec=OR;
        }
        if(!strcmp(dico_data[i].name,"ORI")) {
            dico_data[i].exec=ORI;
        }
        if(!strcmp(dico_data[i].name,"XOR")) {
            dico_data[i].exec=XOR;
        }
        if(!strcmp(dico_data[i].name,"SB")) {
            dico_data[i].exec=SB;
        }
        if(!strcmp(dico_data[i].name,"SW")) {
            dico_data[i].exec=SW;
        }
        if(!strcmp(dico_data[i].name,"SEB")) {
            dico_data[i].exec=SEB;
        }
        if(!strcmp(dico_data[i].name,"SLL")) {
            dico_data[i].exec=SLL;
        }
        if(!strcmp(dico_data[i].name,"SRA")) {
            dico_data[i].exec=SRA;
        }
        if(!strcmp(dico_data[i].name,"SRL")) {
            dico_data[i].exec=SRL;
        }
        if(!strcmp(dico_data[i].name,"SLT")) {
            dico_data[i].exec=SLT;
        }
        if(!strcmp(dico_data[i].name,"SLTI")) {
            dico_data[i].exec=SLTI;
        }
        if(!strcmp(dico_data[i].name,"SLTIU")) {
            dico_data[i].exec=SLTIU;
        }
        if(!strcmp(dico_data[i].name,"SLTU")) {
            dico_data[i].exec=SLTU;
        }
        if(!strcmp(dico_data[i].name,"SUB")) {
            dico_data[i].exec=SUB;
        }
        if(!strcmp(dico_data[i].name,"SUBU")) {
            dico_data[i].exec=SUBU;
        }
        if(!strcmp(dico_data[i].name,"SYSCALL")) {
            dico_data[i].exec=SYSCALL;
        }





    }
    //tri dico pour éviter les mauvaises combi mask/instr,
    // la flemme d'implémenter un tri fusion
    i=0;
    dico_info temp;
    while(i<nbinstr) {
        if (dico_data[i].mask<dico_data[i+1].mask)
        {
            memcpy(&temp,&dico_data[i+1],sizeof(dico_info));
            memcpy(&dico_data[i+1],&dico_data[i],sizeof(dico_info));
            memcpy(&dico_data[i],&temp,sizeof(dico_info));
            i=0;
        }
        else i++;
    }




    return 0;
}

int getInstr(uint32_t adress, instruction* instr_ptr) {
    int32_t temp;
    if(memRead(adress,1,&temp)==-1) {
        temp=-1;   //En cas de depassement de la zone .text on ne recupere aucune instr
    }
    //FLIP_ENDIANNESS(temp);
    memcpy(instr_ptr,&temp,4);
    //printf("content : %8.8X\t",temp); printf("adress %8.8X\n", adress);
    return 1;
}







void initprog() {
    INFO_MSG("*\nProgram initialisation\n*");
//Initialisation des Pipeblocks
    vpipeline[IF].ins.value=-1;
    vpipeline[IF].dico_entry=-1;
    vpipeline[IF].step=IF;
    vpipeline[ID].ins.value=-1;
    vpipeline[ID].dico_entry=-1;
    vpipeline[ID].step=ID;
    vpipeline[EX].ins.value=-1;
    vpipeline[EX].dico_entry=-1;
    vpipeline[EX].step=EX;
    vpipeline[MEM].ins.value=-1;
    vpipeline[MEM].dico_entry=-1;
    vpipeline[MEM].step=MEM;
    vpipeline[WB].ins.value=-1;
    vpipeline[WB].dico_entry=-1;
    vpipeline[WB].step=WB;


    int j;
    for (j=0; j < NBREG+3; ++j)
    {
        reg_mips[j]=0;             //Initialisation des registres pour debug avant load
    }
    
    //Initialisation de stack
    reg_mips[29]=0xFFFFF000-4;
    //Initialisation de PC
    //Recuperation de la plage .text
    int k;

    for (k = 0; k < memory->nseg; k++) {
        if(strcmp(memory->seg[k].name,".text")==0) {
            reg_mips[PC]=memory->seg[k].start._32;
        }
    }
}

int pipecpy(pipeblock* A, pipeblock B) {
    A->ins.value=B.ins.value;
    A->dico_entry=B.dico_entry;
    A->tmp=B.tmp;
    A->tmp2=B.tmp2;

    return 0;
}


list listReadedReg(instruction ins, int dico_entry) {
    list L=NULL;
    if(ins.value==-1) {
        return L;   //Si l'instruction est invalide
    }
    switch(dico_data[dico_entry].type) {
    case 0: //R type
        if(ins.r.rs!=0) {
            L=insert(ins.r.rs,L);
        }
        if(ins.r.rt!=0) {
            L=insert(ins.r.rt,L);
        }
        break;

    case 1: //I type
        if(ins.r.rs!=0) {
            L=insert(ins.r.rs,L);
        }
        if(ins.r.rt!=0) {
            L=insert(ins.r.rt,L);
        }

        break;

    default: //J type no register
        break;
    }
    //printList(L);
    return L;
}

list listWritedReg(instruction ins, int dico_entry) {
    list L=NULL;
    if(ins.value==-1) {
        return L;   //Si l'instruction est invalide
    }
    switch(dico_data[dico_entry].type) {
    case 0: //R type
        if(ins.r.rd!=0) {
            L=insert(ins.r.rd,L);
        }
        break;

    case 1: //I type
        if(ins.r.rt!=0) {
            L=insert(ins.r.rt,L);
        }
        break;

    default: //J type no register
        break;
    }
    //printList(L);
    return L;
}

int addNOP(pipeblock * A) {
    A->ins.value=0;
    A->dico_entry=0;//toujours 0 car dico trié par masque
    A->tmp=0;
    return 0;
}
