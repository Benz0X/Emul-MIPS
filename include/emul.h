#ifndef _emul_h
#define _emul_h





// Fonction permettant de verifier si une chaine de caracteres
// est bien dans la liste des symboles du fichier ELF
// parametres :
// 		name : le nom de la chaine recherchée
//  symtab : la table des symboles
//
// retourne 1 si present, 0 sinon
int is_in_symbols(char* name, stab symtab);

// fonction permettant d'extraire une section du fichier ELF et de la chargée dans le segment du même nom
// parametres :
//   fp         : le pointeur du fichier ELF
//   memory     : la structure de mémoire virtuelle
//   scn        : le nom de la section à charger
//   permission : l'entier représentant les droits de lecture/ecriture/execution
//   add_start  : l'addresse virtuelle à laquelle la section doit être chargée
//
// retourne 0 en cas de succes, une valeur non nulle sinon
unsigned int get_nsegments(stab symtab,char* section_names[],int nb_sections);

// fonction affichant les octets d'un segment sur la sortie standard
// parametres :
//   seg        : le segment de la mémoire virtuelle à afficher

void print_segment_raw_content(segment* seg);


int loadELF (char* name,int mode,uint32_t addr);					//charge un ELF en mémoire
int dispmemPlage(uint32_t start_adrr,uint32_t size);				//affiche une plage mémoire
//Ces fonctions retournent -1 en cas d'accès hors zone mémoire :
int memRead(uint32_t start_addr,int type, int* value);				//lit en mémoire à start_addr et stocke dans value
int memWrite(uint32_t start_addr,int type, int value);				//écrit value en mémoire à start_addr

#endif
