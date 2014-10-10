#include "define.h"
#include <stdio.h>
#include <stdlib.h>
#include "fonctions.h"
#include "common/notify.h"
#include "emul.h"
#include "user_int.h"


int main(int argc, char *argv[])
{

    int i;

    for (i=0; i < 32; ++i)
    {
        reg_mips[i]=-i;				//Initialisation des registres pour debug avant load
    }

    readDico("dico.dico");
    
    /*for (i = 0; i < 10; ++i)
    {
        printf("%s %X %X %d %d %s \n ",dico_data[i].name,dico_data[i].mask,dico_data[i].instr,dico_data[i].type,dico_data[i].nb_arg,dico_data[i].argname[0]);
    }
    */


    
    /*
    //test stuff
        int i;

        for (i=0; i < 32; ++i)
        {
            reg_mips[i]=-i;
        }
                char  string[200];
                int regwrite;
                int index; //test reg
        while(1){
            scanf("%s",string);
            readReg(string, &i);
            printf("input : %s reg value :%d \n Write ?\n", string,i);

            scanf("%s %d",string, &regwrite);

            index=writeReg(string,regwrite);
            printf("input: %s reg value: %d\n  index : %d Read ?\n",string,reg_mips[index],index);
        }
    */






    FILE *script_file = NULL;
    scriptmode=0;						//Mode interactif par defaut
    if ( argc > 2 ) {
        ERROR_MSG("Too much argument");
        exit( -1 );
    }
    if (argc > 1)
    {
        script_file = fopen (argv[1], "r");
        if (script_file != NULL)
        {
            scriptmode=1;				//Si on a un argument, et que l'ouverture du script est possible, on passe en mode script
        }
        else
        {
            //renvoi d'erreur
            printf("erreur a l'ouverture du script\n");
            exit(1);
        }
    }


    while(1)							//Boucle infinie de l'interpreteur
    {
        char input[INPUT_SIZE];				//Buffer
        int res=-1;						//Resultat d'execution
        char normalized_input[INPUT_SIZE];
        input[0]='\0';
        normalized_input[0]='\0';
        do {
            if (scriptmode==1)
            {
                if (!(script_file==NULL)) {
                    getFromScript(script_file,input);	//En mode script, on lit le fichier : cf fonctions.c
                }
                else {
                    WARNING_MSG("No open script, use ./EXENAME SCRIPTNAME to run with a script");
                    scriptmode=0;

                }
            }
            else
            {
                //printf("execution en mode interactif, entrez une commande \n");
                getFromUser(input);					//En mode interactif, on lit stdin
            }
            string_standardise(input,normalized_input);		//On normalise l'entree - echappement, commentaires, etc
        } while (normalized_input[0]=='\0'); //Jusqu'à une fin de chaine.


        res=decrypt(normalized_input);				 //On execute la commande : cf user_int.c




        switch(res) { 	//
        case 0:
            break;
        case 2:
            /* sortie propre du programme */
            if ( scriptmode==1 ) {
                fclose( script_file );
            }

            exit(0);
            break;
        default:
            /* erreur durant l'execution de la commande */
            /* En mode "fichier" toute erreur implique la fin du programme ! */
            if (scriptmode==1) {
                fclose( script_file );
                /*macro ERROR_MSG : message d'erreur puis fin de programme ! */
                ERROR_MSG("ERREUR DETECTEE. Aborts");
            }
            break;
        }

        if( scriptmode==1 && feof(script_file) ) {
            /* mode fichier, fin de fichier => sortie propre du programme */
            DEBUG_MSG("FIN DE FICHIER");
            fclose( script_file );
            exit(0);
        }

    }
    return 0;
}


