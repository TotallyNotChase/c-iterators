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

/*
The `T` passed to `ListIter`, `DefineListIterOf` and so on need to be alphanumeric.
So here's an alphanumeric alias to `IntNode const*`.
*/
typedef IntNode const* ConstIntList;

#define ListIter(T) T##ListIter

#define DefineListIterOf(T)                                                                                            \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        T curr;                                                                                                        \
    } ListIter(T)

/* Macro to consistently name the arriter -> iterable functions based on element type */
#define prep_listiter_of(T) prep_##T##_itr

/*
Take in a source list and its element type, build an `ListIter` from it, and call the wrapper function to
turn it into an `Iterable`
*/
#define list_into_iter(head, T) prep_listiter_of(T)(&(ListIter(T)){.curr = head})

/* Define `ListIter` struct for an int list */
DefineListIterOf(ConstIntList);

/* Create and prepend an IntNode to given IntList and return the new list */
IntList prepend_intnode(int val, IntList list);
/* Print the given int list */
void print_intlist(ConstIntList head);
/* Free the given IntList */
IntList free_intlist(IntList head);

/* Convert a pointer to an `ListIter(ConstIntList)` to an `Iterable(int)` */
Iterable(int) prep_listiter_of(ConstIntList)(ListIter(ConstIntList) * x);

#endif /* !IT_LIST_ITRBLE_H */
