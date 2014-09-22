#ifndef _fonctions_h
#define _fonctions_h
#include "user_int.h"
#include <stdio.h>
#include <stdlib.h>
int getFromScript(FILE *fileptr,char input[]);//recupere ligne par ligne, renvoi 1 si le programme doit se quitter
int getFromUser(char input[]);// a coder, renvoi 1 si le programme doit se quitter
command getCommand(char word[]);





#endif
