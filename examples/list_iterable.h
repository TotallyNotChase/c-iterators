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

#define ListIter(T) T##ListIter

#define list_into_iter(head, T) prep_##T##_itr(&(ListIter(T)){.curr = head})

#define DefineListIterOf(T)                                                                                            \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        T curr;                                                                                                        \
    } ListIter(T)

DefineListIterOf(ConstIntList);

/* Create and prepend an IntNode to given IntList and return the new list */
IntList prepend_intnode(int val, IntList list);
/* Print the given int list */
void print_intlist(ConstIntList head);
/* Free the given IntList */
IntList free_intlist(IntList head);

Iterable(int) prep_ConstIntList_itr(ListIter(ConstIntList) * x);

#endif /* !IT_LIST_ITRBLE_H */
