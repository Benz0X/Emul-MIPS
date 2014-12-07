#include "define.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fonctions.h"
#include "liste.h"
#include "common/notify.h"
#include "emul.h"
#include "user_int.h"
#include "elf/syms.h"
#include <SDL/SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_draw.h>
#include <SDL_phelma.h>

#include "textedition.h"


#include "affichage.h"







//Declaration des variables globales
int scriptmode;                     //0 for interactive mode, 1 for script mode
int clocktime=0;                    //Mips clock : 0 for max speed
int nbinstr=0;                      //Nombre d'instruction du dictionnaire
list breaklist;                     //Initialisation de la liste de points d'arret

uint32_t textStart=DEFAULT_S_ADDR;  //Début du segment .text
uint32_t textEnd=0;                //Limites des segments rx
uint32_t libcTextStart=0;
uint32_t libcTextEnd=0;

uint32_t return_addr;               //Utilisée pour break au bon moment de step
short verbose=0;                    //Module l'affichage
uint nbcycle=0;

pipeblock vpipeline[5];             //Structure de pipeline : 5 block -> IF, ID, EX, MEM, WB




int mainold(int argc, char *argv[])
{

    readDico(DICONAME);


    FILE *script_file = NULL;
    scriptmode=0;                       //Mode interactif par defaut
    if ( argc > 2 ) {
        ERROR_MSG("Too much argument");
        exit( -1 );
    }
    if (argc > 1)
    {
        script_file = fopen (argv[1], "r");
        if (script_file != NULL)
        {
            scriptmode=1;               //Si on a un argument, et que l'ouverture du script est possible, on passe en mode script
        }
        else
        {
            //renvoi d'erreur
            printf("erreur a l'ouverture du script\n");
            exit(1);
        }
    }


    while(1)                            //Boucle infinie de l'interpreteur
    {
        char input[INPUT_SIZE];         //Input buffer
        int res=-1;                     //Resultat d'execution
        char normalized_input[INPUT_SIZE];
        input[0]='\0';
        normalized_input[0]='\0';
        do {
            if (scriptmode==1)
            {
                if (!(script_file==NULL)) {
                    getFromScript(script_file,input);   //En mode script, on lit le fichier : cf fonctions.c
                }
                else {
                    WARNING_MSG("No open script, use ./EXENAME SCRIPTNAME to run with a script");
                    scriptmode=0;

                }
            }
            else
            {
                getFromUser(input);                 //Read stdin
            }
            string_standardise(input,normalized_input);     //On normalise l'entree - echappement, commentaires, etc
            string_standardise(normalized_input,input);     //Deux fois pour enlever les lignes avec uniquement des ' ' a cause des '\t' qui deviennent ' '.
            //TODO : il est probablement possible de faire plus malin
        } while (input[0]=='\0'); //Jusqu'à une fin de chaine.

        //printf("'%s'\n",normalized_input );
        res=decrypt(input);                                 //On execute la commande : cf user_int.c




        switch(res) {
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
            DEBUG_MSG("END OF FILE");
            fclose( script_file );
            exit(0);
        }

    }
    return 0;
}



int main(int argc, char *argv[])
{
    readDico(DICONAME);


    FILE *script_file = NULL;
    scriptmode=0;                       //Mode interactif par defaut
    if ( argc > 2 ) {
        ERROR_MSG("Too much argument");
        exit( -1 );
    }
    if (argc > 1)
    {
        script_file = fopen (argv[1], "r");
        if (script_file != NULL)
        {
            scriptmode=1;               //Si on a un argument, et que l'ouverture du script est possible, on passe en mode script
        }
        else
        {
            //renvoi d'erreur
            printf("erreur a l'ouverture du script\n");
            exit(1);
        }
    }


    while(1)                            //Boucle infinie de l'interpreteur
    {
        char input[INPUT_SIZE];         //Input buffer
        int res=-1;                     //Resultat d'execution
        char normalized_input[INPUT_SIZE];
        input[0]='\0';
        normalized_input[0]='\0';
        do {
            if (scriptmode==1)
            {
                if (!(script_file==NULL)) {
                    getFromScript(script_file,input);   //En mode script, on lit le fichier : cf fonctions.c
                }
                else {
                    WARNING_MSG("No open script, use ./EXENAME SCRIPTNAME to run with a script");
                    scriptmode=0;

                }
            }
            else
            {
                getFromUser(input);                 //Read stdin
            }
            string_standardise(input,normalized_input);     //On normalise l'entree - echappement, commentaires, etc
            string_standardise(normalized_input,input);     //Deux fois pour enlever les lignes avec uniquement des ' ' a cause des '\t' qui deviennent ' '.
            //TODO : il est probablement possible de faire plus malin
        } while (input[0]=='\0'); //Jusqu'à une fin de chaine.

        //printf("'%s'\n",normalized_input );
        res=decrypt(input);                                 //On execute la commande : cf user_int.c




        switch(res) {
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
            DEBUG_MSG("END OF FILE");
            fclose( script_file );
            exit(0);
        }

    }
    return 0;
}
