#ifndef _fonctions_h
#define _fonctions_h

#include <stdio.h>
#include <stdlib.h>
int getFromScript(FILE *fileptr,char input[]);//recupere ligne par ligne, renvoi 1 si le programme doit se quitter
int getFromUser(char input[]);// a coder, renvoi 1 si le programme doit se quitter
int decrypt(char input []);//doit décrypter mot par mot, faudrait une fonction get-word qui ne renvoi rien si c'est des comment





#endif
