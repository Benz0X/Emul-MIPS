#include "user_int.h"
#include "fonctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common/notify.h"
#include "mem.h"
#include "emul.h"


int scriptmode;

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
	if(!nextword(&word,input,&n)){
		WARNING_MSG("Too few arguments. Syntax is 'load <filename> [<adress>]'");
	}else{
		char filename[INPUT_SIZE];
		strcpy(filename,word);
		if(!nextword(&word,input,&n)){
			INFO_MSG("Chargement du fichier '%s'",filename);
				loadELF(filename);	
		}else{
			if(isHexa(word)==0){
				WARNING_MSG("Adress must be hexadecimal");
			}else{
				uint32_t adress = strtol(word,NULL,16);
				INFO_MSG("Chargement du fichier '%s' à l'adresse '0x%8.8X'",filename,adress);
				loadELF(filename,adress);
			}
		}
	}
    break;


case EXIT:
	INFO_MSG("Sortie du programme");
	exit(0);
    break;


case DISP:
	INFO_MSG("Display stuff");

	if(!nextword(&word,input,&n)){								
		WARNING_MSG("Too few arguments. Syntax is :\n\t'disp mem <plage>+' or\n\t'disp mem map'  or\n\t'disp reg <register>+'");
	}else{
		if(strcmp(word,"mem")==0){								//Disp mem
			printf("Affichage memoire ");
			if(nextword(&word,input,&n)){							//ICI le test n'est pas necessaire, car il y a le cas puit
				if(strcmp(word,"map")==0){
					printf("de la map\n");
					print_mem(memory);
				}else if(1){ 			 //Il faut creer isplage
					printf("de la plage\n");				
				}else{
					WARNING_MSG("Second argument of 'disp mem' must be : \t<plage>+ (uint:uint)   or map");
				}
			}else{
				WARNING_MSG("Second argument of 'disp mem' must be : \t<plage>+ (uint:uint)   or map");
			}

		}else if(strcmp(word,"reg")==0){							//Disp reg
			while(nextword(&word,input,&n)){
				printf("registre \n");
			}
			
		}else{				
			WARNING_MSG("First argument of 'disp' must be : \tmem    or reg");
		}
	}
	
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
	INFO_MSG("Mode interactif debug");
	scriptmode=0;
    break;
case RESUME:
	INFO_MSG("Resume script");
	scriptmode=1;
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
	WARNING_MSG("Unknown command : %s",word);
    break;

default:

    break;
    }
    return 0;
}
