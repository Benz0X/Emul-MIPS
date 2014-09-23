#ifndef _fonctions_h
#define _fonctions_h
#include "user_int.h"
#include <stdio.h>
#include <stdlib.h>

#define INPUT_SIZE 1024


int getFromScript(FILE *fileptr,char input[]);//recupere ligne par ligne, renvoi 1 si le programme doit se quitter
int getFromUser(char input[]);// a coder, renvoi 1 si le programme doit se quitter
command getCommand(char word[]);

int what_type(char * word);

int isHexa(char * word);
int isDecimal(char * word);
int isOctal(char * word);

#endif
