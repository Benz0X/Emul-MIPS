#include "user_int.h"
#include "fonctions.h"
#include "common/notify.h"
#include "emul.h"
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>
#define PROMPT_STRING "EmulMips : > "


int getFromScript(FILE *fileptr,char * input)
{
    if( fgets( input, INPUT_SIZE, fileptr ) == NULL)
    {
        printf("end of file\n");
        return 0;
    }

    return 1;
}
int getFromUser(char * input)
{
    //fgets(input,INPUT_SIZE,stdin);
    char* temp;
     temp = readline( PROMPT_STRING );
     add_history( temp );
     strcpy(input, temp);
     //printf("l'entrée est %s\n",input);
    return 1;
}

command getCommand(char word[]){
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

return UNKNOWN;
}


int what_type(char * word){
	enum { INIT , DECIMAL_ZERO , HEXA , DECIMAL , OCTAL } ;
	int i=0;
	int c ;
	int S=INIT ;
	while (word[i] != '\0' && word[i] != '\n') {
			c=word[i];
			i++;
			switch (S) {
		case INIT:
			if (isdigit(c)){
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
	return S;
}


int isHexa(char * word){
	if (what_type(word)==2){return 1;}
	return 0;
}


int isDecimal(char * word){
	if (what_type(word)==3){return 1;}
	return 0;
}

int isOctal(char * word){
	if (what_type(word)==4){return 1;}
	return 0;
}

int nextword(char** token, char* input, int* n){
	if(*n==0){
		*token=strtok(input, " \n");
		*n=*n+1;	
	}else{
		*token=strtok(NULL, " \n");
		*n=*n+1;	
	}
	//printf("prochain mot : %s\n",*token);
	if(*token==NULL){return 0;}else{return 1;}
}

void string_standardise( char* in, char* out ) {
    unsigned int i=0, j;
    //removes spaces
    while(in[i]==' ' && i<strlen(in)){i++;}

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

        /* remove command */
        else if (in[i]=='#') {
            out[j++]='\0';
        }
        /* translate tabs into white spaces*/
        else if (isblank((int) in[i])) out[j++]=' ';
        else out[j++]=in[i];
    }
    out[j++]='\0';
}

int readReg(char * reg_name, int32_t* value){
	int i=isReg(reg_name);
	if(i==-1){								//test -1 et 0 non modifiable
		WARNING_MSG("%s isn't a valid register",reg_name);
		return -1;
	}else{
		*value=reg_mips[i];
		return 0;
	}
}

int writeReg(char * reg_name, int32_t value){
	int i=isReg(reg_name);
	if(i<1){								//test -1 et 0 non modifiable
		WARNING_MSG("%s isn't a valid register",reg_name);
		return -1;
	}else{
		reg_mips[i]=value;
		return i;
	}
}



int isReg(char* reg_name){
	char * string;

	int index=-1;
	index = strtol(reg_name,&string,10);

	if (isdigit(reg_name[0]) && index<32 && index > -1){
		return index;
	}else if (!isdigit(reg_name[0])){
			if (!strcmp(string,"zero")){index=0;}
			else if (!strcmp(string,"at")){index=1;}
			else if (!strcmp(string,"v0")){index=2;}
			else if (!strcmp(string,"v1")){index=3;}
			else if (!strcmp(string,"a0")){index=4;}
			else if (!strcmp(string,"a1")){index=5;}
			else if (!strcmp(string,"a2")){index=6;}
			else if (!strcmp(string,"a3")){index=7;}
			else if (!strcmp(string,"t0")){index=8;}
			else if (!strcmp(string,"t1")){index=9;}
			else if (!strcmp(string,"t2")){index=10;}
			else if (!strcmp(string,"t3")){index=11;}
			else if (!strcmp(string,"t4")){index=12;}
			else if (!strcmp(string,"t5")){index=13;}
			else if (!strcmp(string,"t6")){index=14;}
			else if (!strcmp(string,"t7")){index=15;}
			else if (!strcmp(string,"s0")){index=16;}
			else if (!strcmp(string,"s1")){index=17;}
			else if (!strcmp(string,"s2")){index=18;}
			else if (!strcmp(string,"s3")){index=19;}
			else if (!strcmp(string,"s4")){index=20;}
			else if (!strcmp(string,"s5")){index=21;}
			else if (!strcmp(string,"s6")){index=22;}
			else if (!strcmp(string,"s7")){index=23;}
			else if (!strcmp(string,"t8")){index=24;}
			else if (!strcmp(string,"t9")){index=25;}
			else if (!strcmp(string,"k0")){index=26;}
			else if (!strcmp(string,"k1")){index=27;}
			else if (!strcmp(string,"gp")){index=28;}
			else if (!strcmp(string,"sp")){index=29;}
			else if (!strcmp(string,"fp")){index=30;}
			else if (!strcmp(string,"ra")){index=31;}
			else index=-1;
	}
	return index;
};