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

int decrypt(char input [])
{
    int n=1;
    char* word;
    word = strtok(input, " \n");
    command current_cmd=getCommand(word);

    switch (current_cmd) {

    case LOAD:
        if(!nextword(&word,input,&n)) {
            WARNING_MSG("Too few arguments. Syntax is 'load <filename> [<adress>]'");
        } else {
            char filename[INPUT_SIZE];
            strcpy(filename,word);
            if(!nextword(&word,input,&n)) {
                //INFO_MSG("Chargement du fichier '%s'",filename); Déplacé dans loadELF
                return loadELF(filename,1);
            } else {
                if(isHexa(word)==0) {
                    WARNING_MSG("Adress must be hexadecimal");
                } else {
                    uint32_t adress = strtol(word,NULL,16);
                    INFO_MSG("Chargement du fichier '%s' à l'adresse '0x%8.8X'(arrondi au ko superieur)",filename,adress);
                    return loadELF(filename,adress,2);
                }
            }
        }
        break;


    case EXIT:
        INFO_MSG("Sortie du programme");
        exit(0);
        break;


    case DISP:

        if(!nextword(&word,input,&n)) {
            WARNING_MSG("Too few arguments. Syntax is :\n\t'disp mem <plage>+' or\n\t'disp mem map'  or\n\t'disp reg <register>+'");
        } else {
            if(strcmp(word,"mem")==0) {								//Disp mem
                if(nextword(&word,input,&n)) {
                    if(strcmp(word,"map")==0) {
                        if(memory==NULL) {
                            WARNING_MSG("No memory loaded");
                            return -1;
                        }
                        INFO_MSG("Affichage de la map mémoire");
                        print_mem(memory);
                        return 0;
                    } else if(isHexa(word)) {
                        uint32_t adress1=strtol(word,NULL,16);
                        if(nextword(&word,input,&n)) {
                            if (!strcmp(word,":")) {
                                if(nextword(&word,input,&n)) {
                                    if(isHexa(word)) {
                                        uint32_t adress2=strtol(word,NULL,16);
                                        int size=adress2-adress1;
                                        if(size<0) {
                                            WARNING_MSG("Adress 2 must be superior to adress 1");
                                            return -1;
                                        }
                                        INFO_MSG("Affichage de la mémoire de 0x%8.8X à 0x%8.8X",adress1,adress2);
                                        dispmemPlage(adress1,size);
                                        return 0;
                                    }
                                    WARNING_MSG("Adresses must be hexadecimal");//en fait non faut que ce soit un uint c'est tout->a changer
                                    return -1;
                                }
                            }
                            else if (!strcmp(word,"+")) {
                                if(nextword(&word,input,&n)) {
                                    if(isDecimal(word)) {
                                        uint32_t size=strtol(word,NULL,10);
                                        INFO_MSG("Affichage de la mémoire de 0x%8.8X à 0x%8.8X",adress1,adress1+size);
                                        dispmemPlage(adress1,size);
                                        return 0;
                                    }
                                    WARNING_MSG("Range must be decimal");
                                    return -1;
                                }
                            }
                            WARNING_MSG("Syntax Error, syntax is :\n\t'disp mem <plage>+' or\n\t'disp mem map'  or\n\t'disp reg <register>+'");

                        } else {
                            WARNING_MSG("Second argument of 'disp mem' must be : \t<plage>+ (uint:uint)   or map");
                        }
                    } else {
                        WARNING_MSG("Second argument of 'disp mem' must be : \t<plage>+ (uint:uint)   or map");
                        return -1;
                    }
                }
            } else if(strcmp(word,"reg")==0) {							//Disp reg
                while(nextword(&word,input,&n)) {
                    printf("registre \n");
                }

            } else {
                WARNING_MSG("First argument of 'disp' must be : \tmem    or reg");
                return -1;
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
        return 0;
        break;
    case RESUME:
        INFO_MSG("Resume script");
        scriptmode=1;
        return 0;
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
        return -1;
        break;

    default:

        break;
    }
    printf("Il manque un retour d'erreur\n");
    return -1;
}
