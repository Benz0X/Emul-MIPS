#include "liste.h"
#include <stdio.h>
#include <stdlib.h>


list create(void) {
    return NULL;
}

int empty(list L) {
    return !L;
}

list push(element e, list L) {
    list p=(list) calloc(1,sizeof(*p));
    if (p==NULL) return NULL;
    p->val=e;
    p->suiv=L; 
    return p;
}

list pop(list L){
  if (!empty(L)) {
    list p;
    p=L->suiv;
    free(L);
    return p;
  }
  else return NULL; 
}

list insert(element e, list L) {

    list k=L; //parcours de L
    list p=(list) calloc(1,sizeof(*p)); //nouveau maillon
    if (p==NULL) return NULL;

    if(empty(k)||e<k->val){
      return push(e,L);
    }
    for(k=L; !empty(k->suiv)&&(e>k->suiv->val); k=k->suiv);
    p->val=e;
    p->suiv=k->suiv;
    k->suiv=p;

    return L;
}

list del(element e, list L) {
    list p; //parcours de L

    if(!L)return L;

    if(L->val==e){
      p =L->suiv;
      free(L);
      p=del(e,p);
      return p;
    }else{
      L->suiv=del(e,L->suiv);
      return L;
    }
}


list present(element e, list L) {
    list p=L;
    while(!empty(p)&&(p->val!=e)) p=p->suiv;
    return p;
}

void printList(list L) {
    list p=L;
    for(p=L; !empty(p); p=p->suiv) {
        printf("0x%8.8X \t",p->val);
    }
    puts("");
}