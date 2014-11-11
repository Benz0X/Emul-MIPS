#ifndef _liste_h
#define _liste_h

typedef int element;
struct cellule
{
    element val;
    struct cellule *suiv;
};
typedef struct cellule* list;


list create(void);
int empty(list L);
list present(element e, list L);
list push(element e, list L);
list pop(list L);
list insert(element e, list L);
int overlap(list L1, list L2);
list del(element e, list L);
void printList(list L);

#endif