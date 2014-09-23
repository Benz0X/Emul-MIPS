#include "user_int.h"
#include "fonctions.h"
#include <string.h>
#include "notify.h"



int getFromScript(FILE *fileptr,char input[])
{
    if( fgets( input, INPUT_SIZE, fileptr ) == NULL)
    {
        printf("end of file\n");
        return 0;
    }

    return 1;
}
int getFromUser(char input[])
{
    fgets(input,INPUT_SIZE,stdin);
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
			if(isdigit(c) || (c <= 'f' && c >= 'a') || (c <= 'F' ) && ( c >= 'A' )) S=HEXA;
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
