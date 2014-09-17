#include <stdio.h>
#include <stdlib.h>
#include "user_int.h"

int main(int argc, char *argv[])
{
FILE *script_file = NULL;
if (argc > 1)
	{
	script_file = fopen (argv[1], "r");
	if (script_file != NULL)
		{
		//exécution du script while command != debug
		printf("exécution du script\n");
		//while (command != debug || strtok != EOF)
		return 0;
		}
    	else
		{
		//renvoi d'erreur
		printf("erreur à l'ouverture du script\n");
		exit(1);
		}
	}
//exécution normale
printf("en attente de commande utilisateur\n");
//while (command != quit || command != resume)
return 0;
}
