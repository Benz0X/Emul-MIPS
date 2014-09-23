#include "user_int.h"
#include "fonctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "notify.h"

/* syntaxe strtok

   const char str[80] = "This is - www.tutorialspoint.com - website";
   const char s[2] = "-";
   char *token;
   
   // get the first token 
   token = strtok(str, s);
   
   // walk through other tokens 
   while( token != NULL ) 
   {
      printf( " %s\n", token );
    
      token = strtok(NULL, s);
   }
*/

command decrypt(char input [])
{
	int n=1; 
    char* word;
    word = strtok(input, " \n");
    command current_cmd=getCommand(word);

    switch (current_cmd){

case LOAD:
	printf("Chargement d'un script\n");
	if(nextword(&word,input,&n)==0){
		WARNING_MSG("Too few arguments. Syntax is 'load <filename> [<adress>]");
	}else{
		printf("Fichier '%s' à charger ",word);
		if(nextword(&word,input,&n)==0){
			printf(" sans specification d'adresse\n");		
		}else{
			if(isHexa(word)==0){
				WARNING_MSG("Adress must be hexadecimal");
			}else{
				printf("à l'adresse '%s'\n",word);
			}
		}
	}

	 

    break;


case EXIT:
printf("%d \n",current_cmd);
    break;
case DISP:
printf("%d \n",current_cmd);
    break;
case DISASM:
printf("%d \n",current_cmd);
    break;
case SET:
printf("%d \n",current_cmd);
    break;
case ASSERT:
printf("%d \n",current_cmd);
    break;
case DEBUG:
printf("%d \n",current_cmd);
    break;
case RESUME:
printf("%d \n",current_cmd);
    break;
case RUN:
printf("%d \n",current_cmd);
    break;
case STEP:
printf("%d \n",current_cmd);
    break;
case BREAK:
printf("%d \n",current_cmd);
    break;
case UNKNOWN:
printf("%d \n",current_cmd);
    break;

default:

    break;
    }
    return 0;
}
