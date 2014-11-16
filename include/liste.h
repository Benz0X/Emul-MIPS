#ifndef _liste_h
#define _liste_h

typedef int element;
struct cellule
{
    element val;
    struct cellule *suiv;
};
typedef struct cellule* list;


list create(void);					//Crée une liste d'éléments
int empty(list L);					//Test si la liste est vide
list present(element e, list L);	//Test si l'élément est présent dans la liste, et renvoie la liste débutant par cet élément (le premier)
list push(element e, list L);		//Rajoute e en tête
list pop(list L);					//Supprimme la tête
list insert(element e, list L);		//Insère e à sa place dans une liste triée 
int overlap(list L1, list L2);		//Test si au moins un élément est commun dans les deux listes
list del(element e, list L);		//Supprime toutes les occurences d'un élément dans la liste
void printList(list L);				//Affiche la liste

#endif