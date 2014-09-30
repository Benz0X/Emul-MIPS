#include <stdio.h>
#include <stdlib.h>
#include "user_int.h"

void getFromScript(FILE *fileptr,char input[]);
void getFromUser(char input[]);
void decrypt(char input [],int* quit,int* scriptmode);

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
		  scriptmode=1;
		  printf("execution en mode script");
		}
    	else
		{
		//renvoi d'erreur
		printf("erreur Ã  l'ouverture du script\n");
		exit(1);
		}
	}


 while(quit==0){
   char input[64];
   if (scriptmode==1)
     getFromScript(script_file,input);
   else
     getFromUser(input);
   decrypt(input,&quit,&scriptmode);//variable globale ?
 }
return 0;
}

 void getFromScript(FILE *fileptr,char input[]){
 }
 void getFromUser(char input[]){
}
 void decrypt(char input [],int* quit,int* scriptmode){
 }

