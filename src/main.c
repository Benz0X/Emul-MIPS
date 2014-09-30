#include <stdio.h>
#include <stdlib.h>
#include "fonctions.h"
#include "notify.h"
#include "emul.h"

int reg_mips[32];

int main(int argc, char *argv[])
{
    

//test stuff
    int i;

    for (i=0; i < 32; ++i)
    {
        reg_mips[i]=-i;
    }
            char  string[200];
            int regwrite;
            int index;
    while(1){
        scanf("%s",string);
        readReg(string, &i);
        printf("input : %s reg value :%d \n Write ?\n", string,i);
    
        scanf("%s %d",string, &regwrite);
        
        index=writeReg(string,regwrite);
        printf("input: %s reg value: %d\n  index : %d Read ?\n",string,reg_mips[index],index);
    }











    FILE *script_file = NULL;
    int scriptmode=0;
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


    while(1)
    {
        int test=isReg("t1");
        printf("Test is registre : %d\n", test);
        char input[1024];
        char normalized_input[INPUT_SIZE];
        input[0]='\0';
        normalized_input[0]='\0';
        do{
            if (scriptmode==1)
            {
                scriptmode=getFromScript(script_file,input);
            }
            else
            {
                printf("execution en mode interactif, entrez une commande \n");
                getFromUser(input);
            }
            string_standardise(input,normalized_input);
        } while (normalized_input[0]=='\n' || normalized_input[0]=='\0');
        
        //printf(" normalized :%s\n",normalized_input);
        printf(" input :%s\n",input);
        decrypt(normalized_input);//variable globale ?

    }
    return 0;
}


