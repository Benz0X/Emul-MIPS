#include "define.h"
#include "user_int.h"
#include "fonctions.h"
#include "liste.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common/notify.h"
#include "mem.h"
#include "emul.h"
#include "disasm.h"
#include "common/bits.h"
#include "pipeline.h"
#include <ctype.h>

//Declaration des variables globales
int scriptmode;
list breaklist;                     //Initialisation de la liste de points d'arret
int clocktime=0;
instruction insIF, insID, insEX, insMEM, insWB;
int EXtmp,MEMtmp,WBtmp;
int EXdic=-1,MEMdic=-1,WBdic=-1;

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
    int i;                      //compteur
    int n=1;                    //indice du mot
    char* word;                 //buffer du mot
    word = strtok(input, " \n");

    command current_cmd=getCommand(word);   // On recupere la prochaine commande

    switch (current_cmd) {      //Return -1 en cas d'erreur, 0 sinon

    case LOAD:
        if(!nextword(&word,input,&n)) {
            WARNING_MSG("Too few arguments. Syntax is 'load <filename> [<adress>]'");
            return -1;
        } else {
            char filename[INPUT_SIZE]; //On retient le nom du fichier
            strcpy(filename,word);
            if(!nextword(&word,input,&n)) { //Si il n'y a pas d'autre argument, on loadElf
                //INFO_MSG("Chargement du fichier '%s'",filename); Déplacé dans loadELF
                return loadELF(filename,1);
            } else {
                if(isHexa(word)==0) {
                    WARNING_MSG("Adress must be hexadecimal");
                    return -1;
                } else {        //Sinon si l'arguement suivant est une adresse hexa, on charge à cette adresse.
                    uint32_t adress = strtol(word,NULL,16);
                    INFO_MSG("Chargement du fichier '%s' à l'adresse '0x%8.8X'(arrondi au ko superieur)",filename,adress);
                    return loadELF(filename,adress,2);
                }
            }
        }
        break;


    case EXIT:
        INFO_MSG("Sortie du programme");
        return 2;
        break;


    case DISP:

        if(!nextword(&word,input,&n)) {
            WARNING_MSG("Too few arguments. Syntax is :\n\t'disp mem <plage>+' or\n\t'disp mem map'  or\n\t'disp reg <register>+'");
            return -1;
        } else {
            if(strcmp(word,"mem")==0) {                             //Disp mem
                if(nextword(&word,input,&n)) {
                    if(strcmp(word,"map")==0) {
                        if(memory==NULL) {
                            WARNING_MSG("No memory loaded");
                            return -1;
                        }
                        INFO_MSG("Affichage de la map mémoire");
                        print_mem(memory);
                        return 0;
                    } else if(what_type(word)>1) {  //il faudrait vérifier qu'il est <0 et prendre en compte le décimal pour
                        //coller au cahier des charges mais faudrait utiliser un uint64
                        uint32_t adress1=strtol(word,NULL,0);
                        if(nextword(&word,input,&n)) {
                            if (!strcmp(word,":")) {
                                if(nextword(&word,input,&n)) {
                                    if(what_type(word)>1) {
                                        uint32_t adress2=strtol(word,NULL,0);
                                        int size=adress2-adress1;
                                        if((int)size<0) {
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
                                    if(isDecimal(word)&& isdigit(word[0])) {
                                        uint32_t size=strtol(word,NULL,10);
                                        INFO_MSG("Affichage de la mémoire de 0x%8.8X à 0x%8.8X",adress1,adress1+size);
                                        dispmemPlage(adress1,size);
                                        return 0;
                                    }
                                    WARNING_MSG("Range must be >0 decimal");
                                    return -1;
                                }
                            }
                            WARNING_MSG("Syntax Error, syntax is :\n\t'disp mem <plage>+' or\n\t'disp mem map'  or\n\t'disp reg <register>+'");
                            return -1;

                        } else {
                            WARNING_MSG("Second argument of 'disp mem' must be : \t<plage>+ (uint:uint)   or map");
                            return -1;
                        }
                    } else {
                        WARNING_MSG("Second argument of 'disp mem' must be : \t<plage>+ (uint:uint)   or map");
                        return -1;
                    }
                }
            }

            else if(strcmp(word,"reg")==0) {               //Disp reg
                int index;
                char name[INPUT_SIZE];
                while(nextword(&word,input,&n)) {
                    if(strcmp(word,"all")==0) {
                        for(i=0; i<NBREG+3; i++) {                  //Si all, on boucle
                            parseReg(i,name);               //Recuperation du nom complet
                            if(i%4==0) {
                                printf("\n");   //Affichage 4 par ligne
                            }
                            printf("%5s: %-10d ",name,reg_mips[i]);   //Affichage du registre
                        }

                    } else {
                        index=isReg(word);

                        if (index!=-1) {
                            if(i%4==0) {
                                printf("\n");
                            }
                            parseReg(index,name);
                            printf("%5s: %-10d ",name,reg_mips[index]);   //Affichage du registre
                            i++;
                        }
                        else {
                            WARNING_MSG("Le registre '%s' n'existe pas",word); //On ne renvoie pas -1, on continue à lire les prochains arguments.
                        }
                    }
                }
                printf("\n");
                return 0; //Affichage registre terminé

            } else {
                WARNING_MSG("First argument of 'disp' must be : \tmem    or reg");
                return -1;
            }

        }
        break;


    case DISASM:
        if(!nextword(&word,input,&n)) {
            WARNING_MSG("Too few arguments. Syntax is :\n\t'disp mem <plage>+' or\n\t'disp mem map'  or\n\t'disp reg <register>+'");
            return -1;
        } else {
            if(what_type(word)>1) {  //il faudrait vérifier qu'il est <0 et prendre en compte le décimal pour
                //coller au cahier des charges mais faudrait utiliser un uint64
                uint32_t adress1=strtol(word,NULL,0);
                if(nextword(&word,input,&n)) {
                    if (!strcmp(word,":")) {
                        if(nextword(&word,input,&n)) {
                            if(what_type(word)>1) {
                                uint32_t adress2=strtol(word,NULL,0);
                                int size=adress2-adress1;
                                if((int)size<0) {
                                    WARNING_MSG("Adress 2 must be superior to adress 1");
                                    return -1;
                                }
                                INFO_MSG("Désassemblage de la mémoire de 0x%8.8X à 0x%8.8X",adress1,adress2);

                                return disasm(adress1,size);;
                            }
                            WARNING_MSG("Adresses must be hexadecimal");//en fait non faut que ce soit un uint c'est tout->a changer
                            return -1;
                        }
                    }
                    else if (!strcmp(word,"+")) {
                        if(nextword(&word,input,&n)) {
                            if(isDecimal(word)&& isdigit(word[0])) {
                                uint32_t size=strtol(word,NULL,10);
                                INFO_MSG("Désassemblage de la mémoire de 0x%8.8X à 0x%8.8X",adress1,adress1+size);

                                return disasm(adress1,size);;
                            }
                            WARNING_MSG("Range must be decimal");
                            return -1;
                        }
                    }
                    WARNING_MSG("Syntax Error, syntax is :\n\t'Disasm <plage>+ (uint:uint) or (uint+uint)");
                    return -1;

                } else {
                    WARNING_MSG("Second argument of 'disasm' must be : \t<plage>+ (uint:uint) or (uint+uint)");
                    return -1;
                }
            } else {
                WARNING_MSG("Second argument of 'disasm' must be : \t<plage>+ (uint:uint) or (uint+uint)");
                return -1;
            }



        }
        break;

    case SET:
        INFO_MSG("Modification memoire");

        if(!nextword(&word,input,&n)) {
            WARNING_MSG("Too few arguments. Syntax is :\n\t'set mem <type> <adress> <value>'  or\n\t'set reg <register> <value>'");
        } else {
            if(strcmp(word,"mem")==0) {                             //set mem
                if(nextword(&word,input,&n)) {
                    if(strcmp(word,"byte")==0) {
                        if(nextword(&word,input,&n)&& isHexa(word)) { //soucis?
                            uint32_t adress=strtol(word,NULL,0);
                            if(nextword(&word,input,&n)) {
                                if(what_type(word)>1) {
                                    int32_t value=strtol(word,NULL,0);

                                    if(nextword(&word,input,&n)) {
                                        WARNING_MSG("Too much arguments");
                                        return -1;
                                    }
                                    else {
                                        if(memWrite(adress,0,value)==0) {
                                            return 0;
                                        }
                                        else {
                                            WARNING_MSG("Out of memory map");
                                            return -1;
                                        }
                                    }


                                    //SET MEM BYTE ADRESS VALUE
                                } else {
                                    WARNING_MSG("Value must be an int");
                                    return -1;
                                }
                            } else {
                                WARNING_MSG("Last argument of 'set mem' must be : \t<value>");
                            }
                        } else {
                            WARNING_MSG("Second argument of 'set mem' must be : \t<adress>");
                        }
                    } else if(strcmp(word,"word")==0) {
                        if(nextword(&word,input,&n)&& isHexa(word)) { //soucis?
                            uint32_t adress=strtol(word,NULL,0);
                            if(nextword(&word,input,&n)) {
                                if(what_type(word)>1) {
                                    int32_t value=strtol(word,NULL,0);

                                    if(nextword(&word,input,&n)) {
                                        WARNING_MSG("Too much arguments");
                                        return -1;
                                    }
                                    else {
                                        if(memWrite(adress,1,value)==0) {
                                            return 0;
                                        }
                                        else {
                                            WARNING_MSG("Out of memory map");
                                            return -1;
                                        }
                                    }


                                    //SET  WORD ADRESS VALUE
                                } else {
                                    WARNING_MSG("Value must be an int");
                                    return -1;
                                }
                            } else {
                                WARNING_MSG("Last argument of 'set mem' must be : \t<value>");
                            }
                        } else {
                            WARNING_MSG("Second argument of 'set mem' must be : \t<adress>");
                        }
                    } else {
                        WARNING_MSG("First argument of 'set mem' must be : \t<type> (byte or word)");
                    }
                } else {
                    WARNING_MSG("First argument of 'set mem' must be : \t<type> (byte or word)");
                }


            } else if(strcmp(word,"reg")==0) {                   //set reg
                if(!nextword(&word,input,&n)) {
                    WARNING_MSG("Too few arguments. Syntax is : 'set reg <register> <value>");
                    return -1;
                } else {
                    char reg_name[INPUT_SIZE];

                    strcpy(reg_name,word);

                    if(isReg(reg_name)<1) {
                        WARNING_MSG("%s isn't a valid register",reg_name);
                        return -1;
                    }
                    else if(nextword(&word,input,&n)) {
                        int32_t value;
                        if(what_type(word)>1) {
                            value=strtol(word,NULL,0);
                        }
                        else {
                            WARNING_MSG("Value must be an int");
                            return -1;
                        }


                        if(nextword(&word,input,&n)) {
                            WARNING_MSG("Too much arguments");
                            return -1;
                        }
                        writeReg(reg_name,value);
                        //printf("Registre : %s\t Value : %d\n",reg_name,value);
                        //printf("%d\n", reg_mips[isReg(reg_name)]);
                        return 0;

                    } else {
                        WARNING_MSG("Set reg missing value");
                        return -1;
                    }

                }



            } else {
                WARNING_MSG("Syntax error : arguments of set are 'mem' or 'reg'");
                return -1;
            }
        }
        break;



    case ASSERT:
        INFO_MSG("Test de valeur");

        if(!nextword(&word,input,&n)) {
            WARNING_MSG("Too few arguments. Syntax is :\n\t'assert reg <register> <value>'  or\n\t'assert <type> <adress> <value>'");
        } else {
            if(strcmp(word,"reg")==0) {                     //assert reg
                if(!nextword(&word,input,&n)) {
                    WARNING_MSG("Too few arguments. Syntax is : 'assert reg <register> <value>'");
                    return -1;
                } else {
                    char reg_name[INPUT_SIZE];
                    strcpy(reg_name,word);
                    if(isReg(reg_name)<0) {
                        WARNING_MSG("%s isn't a valid register",reg_name);
                        return -1;
                    }
                    else if(nextword(&word,input,&n)) {
                        if (what_type(word)>1)
                        {
                            int32_t value=strtol(word,NULL,0);
                            if(nextword(&word,input,&n)) {
                                WARNING_MSG("Too much arguments");
                                return -1;
                            }
                            if(reg_mips[isReg(reg_name)]==value) {
                                printf("Le test est correct\n");
                                return 0;
                            } else {
                                printf("Le test est incorrect\n");
                                return 1;
                            }
                        } else {
                            WARNING_MSG("Value must be an int");
                            return -1;
                        }
                    } else {
                        WARNING_MSG("Assert reg missing value");
                        return -1;
                    }
                }
            } else if(strcmp(word,"word")==0) {          //assert word
                if(!nextword(&word,input,&n)) {
                    WARNING_MSG("Too few arguments. Syntax is : 'assert word <adress> <value>'");
                    return -1;
                } else {
                    if (isHexa(word))
                    {
                        uint32_t adress=strtol(word,NULL,0);
                        //Il faut ici tester les depassements de memoire
                        if(nextword(&word,input,&n)) {
                            if(what_type(word)>1) {
                                int32_t value1=strtol(word,NULL,0);
                                int32_t value2;

                                if(nextword(&word,input,&n)) {
                                    WARNING_MSG("Too much arguments");
                                    return -1;
                                }
                                if(memRead(adress,1,&value2)==0) {
                                    if(value1==value2) {
                                        printf("Le test est correct\n");
                                        return 0;
                                    } else {
                                        printf("Le test est incorrect\n");
                                        return -1;
                                    }
                                } else {
                                    WARNING_MSG("Out of memory map");
                                    return -1;
                                }
                            }
                            else {
                                WARNING_MSG("Assert byte missing value");
                                return -1;
                            }
                        } else {
                            WARNING_MSG("Assert word missing value");
                            return -1;
                        }
                    } else {
                        WARNING_MSG("Adress must be hexadecimal");
                        return -1;
                    }
                }

            } else if(strcmp(word,"byte")==0) {          //assert byte
                if(!nextword(&word,input,&n)) {
                    WARNING_MSG("Too few arguments. Syntax is : 'assert byte <adress> <value>'");
                    return -1;
                } else {
                    if (isHexa(word))
                    {
                        uint32_t adress=strtol(word,NULL,0);
                        //Il faut ici tester les depassements de memoire
                        if(nextword(&word,input,&n)) {
                            if(what_type(word)>1) {
                                int32_t value1=strtol(word,NULL,0);
                                int32_t value2;
                                if(nextword(&word,input,&n)) {
                                    WARNING_MSG("Too much arguments");
                                    return -1;
                                }
                                if(memRead(adress,0,&value2)==0) {
                                    if(value1==value2) {
                                        printf("Le test est correct\n");
                                        return 0;
                                    } else {
                                        printf("Le test est incorrect\n");
                                        return -1;
                                    }
                                } else {
                                    WARNING_MSG("Out of memory map");
                                    return -1;
                                }
                            }
                            else {
                                WARNING_MSG("Value must be an int");
                                return -1;
                            }
                        } else {
                            WARNING_MSG("Assert byte missing value");
                            return -1;
                        }
                    } else {
                        WARNING_MSG("Adress must be hexadecimal");
                        return -1;
                    }
                }
            } else {
                WARNING_MSG("Second argument must be 'reg' or 'word' or 'byte' ");
                return -1;
            }
        }
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

        if(memory==NULL) {
            WARNING_MSG("No memory loaded");
            return -1;
        }
        //Recuperation de l'adress facultative
        if(nextword(&word,input,&n)) {
            if(isHexa(word)) reg_mips[PC]=strtol(word,NULL,0);
            else {
                WARNING_MSG("Syntax error, syntax is run [adress], with adress hexadecimal");
                return -1;
            }
        }
        if(nextword(&word,input,&n)) {
            WARNING_MSG("Too much argument, syntax is 'step' or 'step into'");
            return -1;
        }
        //Recuperation de la plage .text
        int k;
        int start,end;

        for (k = 0; k < memory->nseg; k++) {
            if(strcmp(memory->seg[k].name,".text")==0) {
                start=memory->seg[k].start._32;
                end=memory->seg[k].start._32+memory->seg[k].size._32;
            }
        }

        //Verification des depassements .text
        if(reg_mips[PC]>=end+16 || reg_mips[PC]<start) {    //+16 pour finir le pipe
            reg_mips[PC]=start;
            initprog();
            WARNING_MSG("Out of memory map, start adress set to default");
        }

        //Initialisation des differentes instructions a traiter
        if(reg_mips[PC]==start) {
            initprog();
        }
        return pipeline(end,running,1);
        break;





    case STEP:
        if(memory==NULL) {
            WARNING_MSG("No memory loaded");
            return -1;
        }

        int textstart,textend,l,rem=0;
        for (l = 0; l < memory->nseg; l++) {
            if(strcmp(memory->seg[l].name,".text")==0) {
                textstart=memory->seg[l].start._32;
                textend=memory->seg[l].start._32+memory->seg[l].size._32;
            }
        }

        //Verification des depassements .text
        if(reg_mips[PC]>=textend+16 || reg_mips[PC]<textstart) {       //+16 pour finir le pipe
            reg_mips[PC]=textstart;
            initprog();
        }

        //cas "into"
        if(nextword(&word,input,&n)) {
            if(strcmp(word,"into")==0) {
                if(nextword(&word,input,&n)) {
                    WARNING_MSG("Too much argument, syntax is 'step' or 'step into'");
                    return -1;
                }
                        INFO_MSG("step into");
                        return 0;

            }
            else {
                WARNING_MSG("Syntax error, syntax is 'step' or 'step into'");
                return -1;
            }
        }
        if(nextword(&word,input,&n)) {
            WARNING_MSG("Too much argument, syntax is 'step' or 'step into'");
            return -1;
        }
        int adress;
        if(reg_mips[PC]<textstart+16){adress=reg_mips[PC]+4;}
            else{adress=reg_mips[PC]-12;}
        
        if(empty(present(adress,breaklist))) {
            breaklist=insert(adress,breaklist);
            rem=1;
        };
        pipeline(textend,running,1);
        if (rem==1) {
            breaklist=del(adress,breaklist);
        }
        rem=0;
        return 0;




        break;




    case BREAK:
        if(!nextword(&word,input,&n)) {
            WARNING_MSG("Too few arguments. Syntax is :\n\t'break add <adress>+'  or\n\t'break del <adress>+|all'   or\n\t'break list'");
            return -1;
        } else {
            if (memory==NULL) {
                WARNING_MSG("No program loaded");
                return -1;
            }
            //Recuperation de la plage .text
            int k;
            int start,end;

            for (k = 0; k < memory->nseg; k++) {
                if(strcmp(memory->seg[k].name,".text")==0) {
                    start=memory->seg[k].start._32;
                    end=memory->seg[k].start._32+memory->seg[k].size._32;
                }
            }


            if(strcmp(word,"list")==0) {
                printf("Liste des points d'arrêt :\n");
                printList(breaklist);
                return 0;
            } else if(strcmp(word,"del")==0) {
                while(nextword(&word,input,&n)) {
                    if(strcmp(word,"all")==0) {
                        breaklist=NULL;
                        return 0;
                    } else {
                        if (isHexa(word)) {

                            uint32_t adress=strtol(word,NULL,0);


                            if(adress>=start && adress<end) { //                             Test de seg
                                breaklist=del(adress,breaklist);    //suppression du breakpoint
                            } else {
                                WARNING_MSG("Adress %8.8X can't be breakpoint, segment not allowed",adress);
                                return -1;
                            }

                        } else {
                            WARNING_MSG("Adress %s not valid.",word);
                            return -1;
                        }
                    }
                }
                return 0;
            } else if(strcmp(word,"add")==0) {
                while(nextword(&word,input,&n)) {

                    if (isHexa(word)) {

                        uint32_t adress=strtol(word,NULL,0);
                        adress-=adress % 4;
                        if(adress>=start && adress<end) { //                             Test de seg .text
                            if(empty(present(adress,breaklist))) breaklist=insert(adress,breaklist); //Si le point n'existe pas, on le rajoute
                        } else {
                            WARNING_MSG("Adress 0x%8.8X can't be breakpoint, segment not allowed",adress);
                            //return -1;
                        }

                    } else {
                        WARNING_MSG("Adress %s not valid",word);
                        return -1;
                    }
                }
                return 0;
            } else {
                WARNING_MSG("Syntax is :\n\t'break add <adress>+'  or\n\t'break del <adress>+|all'   or\n\t'break list'");
                return -1;
            }
        }
        break;




    case CLOCK:
        ;
        int tmp;
        if(nextword(&word,input,&n) && isDecimal(word)) {
            tmp=strtol(word,NULL,0);
            if(tmp>=0 && !nextword(&word,input,&n)) {
                clocktime=tmp;
                return 0;
            }
        }

        WARNING_MSG("Syntax is :\n\t'clock <time in ms, 0 for max speed>'");
        return -1;
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
