#ifndef IT_LIST_ITRBLE_H
#define IT_LIST_ITRBLE_H

#include "func_iter.h"

#include <stdlib.h>

#define Nil NULL

typedef struct int_node
{
    int val;
    struct int_node* next;
} IntNode, *IntList;

typedef IntNode const* ConstIntList;

#define ListIter(ElmntTypename) ElmntTypename##ListIter

#define DefineListIterOf(T, Typename)                                                                                  \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        T curr;                                                                                                        \
    } ListIter(Typename)

#define list_into_iter(head, ElmntTypename)                                                                            \
    (ListIter(ElmntTypename)) { .curr = head }

DefineListIterOf(IntNode const*, Int);

/* Create and prepend an IntNode to given IntList and return the new list */
IntList prepend_intnode(int val, IntList list);
/* Print the given int list */
void print_intlist(ConstIntList head);
/* Free the given IntList */
IntList free_intlist(IntList head);

Iterable(Int) prep_intlist_itr(ListIter(Int) * x);

#endif /* !IT_LIST_ITRBLE_H */
