#ifndef _fonctions_h
#define _fonctions_h
#include "user_int.h"
#include <stdio.h>
#include <stdlib.h>
#define INPUT_SIZE 1024


int getFromScript(FILE *fileptr,char* input);//recupere ligne par ligne, renvoi 1 si le programme doit se quitter
int getFromUser(char* input);// a coder, renvoi 1 si le programme doit se quitter
command getCommand(char word[]);

int what_type(char * word);

int isHexa(char * word);
int isDecimal(char * word);
int isOctal(char * word);
int nextword(char** token, char* input, int* n);
void string_standardise( char* in, char* out );
int readReg(char * reg_name, int32_t* value); //lit la valeur d'un registre et la stocke dans value, renvoi -1 si erreur, sinon le num du registre
int writeReg(char * reg_name,int32_t value); //écrit sur un registre, renvoi -1 si erreur, sinon le num du registre

int isReg(char* reg_name); //test si le nom ou numero de registre est correct et renvoie l'entier associe.


#endif
