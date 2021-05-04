#ifndef IT_LIST_ITRBLE_H
#define IT_LIST_ITRBLE_H

#include "func_iter.h"

#include <stdlib.h>

#define Nil NULL

typedef struct int_node
{
    int val;
    struct int_node* next;
} IntNode;

typedef IntNode* IntList;

typedef struct
{
    IntNode* curr;
} IntListItCtx;

/* Create and prepend an IntNode to given IntList and return the new list */
IntList prepend_intnode(IntList list, int val);
/* Free the given IntList */
IntList free_intlist(IntList list);

Iterator(Int) prep_intlist_itr(IntList* x, IntListItCtx* init_ctx);

#endif /* !IT_LIST_ITRBLE_H */
