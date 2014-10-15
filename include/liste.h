#ifndef _liste_h
#define _liste_h

typedef int element;
struct cellule
{
    element val;
    struct cellule *suiv;
};
typedef struct cellule* liste;


liste creer_liste(void);
int est_vide(liste L);
liste rech(element e, liste L);
liste ajout_tete(element e, liste L);
void visualiser(liste L);

#endif