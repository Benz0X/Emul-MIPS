#ifndef _fonctions_h
#define _fonctions_h



void getFromScript(FILE *fileptr,char* input);				//recupere ligne par ligne
void getFromUser(char* input);								//recupère depuis stdin
command getCommand(char word[]);							//renvois la commande utilisateur

int what_type(char * word);									//Renvois 2,3,4 selon si le nombre est décimal, héxa, octal
int isHexa(char * word);
int isDecimal(char * word);
int isOctal(char * word);									//Utilisent what_type

int nextword(char** token, char* input, int* n);			//Récupère le prochain token d'une chaine
void string_standardise( char* in, char* out );				//normalise une chaine pour qu'elle puisse être comprise par le programme
int readReg(char * reg_name, int32_t* value); 				//lit la valeur d'un registre et la stocke dans value, renvoi -1 si erreur, sinon le num du registre
int writeReg(char * reg_name,int32_t value); 				//écrit sur un registre, renvoi -1 si erreur, sinon le num du registre
int writeRegindex(int index, int32_t value); 		        //Ecrit value dans un registre

int isReg(char* reg_name);									//test si le nom ou numero de registre est correct et renvoie l'entier associe.
int parseReg(int index, char* regname);						//Prends un numéro de registre et associe regname au nom de registre adequat. renvoie -1 si index est innaproprié
int readDico(char* dico_name); 								//Stocke les info du dico en mémoire dans la variable globale Dico.
int getInstr(uint32_t adress, instruction* instr_ptr);		//Recupere une instruction en memoire


void initprog();											//Initialise les registres et le pipeline en début de programme
int pipecpy(pipeblock* A, pipeblock B);
int addNOP(pipeblock * block);
list listWritedReg(instruction ins, int dico_entry);
list listReadedReg(instruction ins, int dico_entry);

#endif
