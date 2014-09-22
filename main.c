#include <stdio.h>
#include <stdlib.h>
#include "fonctions.h"


int main(int argc, char *argv[])
{
    FILE *script_file = NULL;
    int scriptmode=0;
    int quit=0;
    if (argc > 1)
    {
        script_file = fopen (argv[1], "r");
        if (script_file != NULL)
        {
            printf("execution en mode script, le script contient : \n");
            scriptmode=1;
        }
        else
        {
            //renvoi d'erreur
            printf("erreur a l'ouverture du script\n");
            exit(1);
        }
    }


    while(quit==0)
    {

        char input[100];
        if (scriptmode==1)
        {
            scriptmode=getFromScript(script_file,input);
        }
        else
        {
            printf("execution en mode intéractif, entrez une commande \n");
            getFromUser(input);
        }
         printf("%s \n",input);
        decrypt(input);//variable globale ?

    }
    return 0;
}


