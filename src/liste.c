#include "liste.h"
#include <stdio.h>
#include <stdlib.h>


liste creer_liste(void) {
    return NULL;
}

int est_vide(liste L) {
    return !L;
}

liste ajout_tete(element e, liste L) {
    liste p=(liste) calloc(1,sizeof(*p));
    if (p==NULL) return NULL;
    p->val=e;
    p->suiv=L; 
    return p;
}

liste ajout_tri(element e, liste L) {
    liste res=L; // pointeur de debut de chaine
    liste k=L; //parcours de L
    liste p=(liste) calloc(1,sizeof(*p)); //nouveau maillon
    if (p==NULL) return NULL;
    while(!est_vide(k)&&(k->val<e)) k=k->suiv;
    p->val=e;
    p->suiv=k->suiv;
    k->suiv=p;
    
    return res;
}

liste rech(element e, liste L) {
    liste p=L;
    while(!est_vide(p)&&(p->val!=e)) p=p->suiv;
    return p;
}

void visualiser(liste L) {
    liste p=L;
    for(p=L; !est_vide(p); p=p->suiv) {
        printf("%d \t",p->val);
    }
    puts("");
}