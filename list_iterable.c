#include "list_iterable.h"

#include "func_iter.h"

#include <stdlib.h>

/* Create IntNode from given value */
static IntNode* create_intnode(int val)
{
    IntNode* node = malloc(sizeof(*node));
    if (node == NULL) {
        /* Exiting here for demonstration purposes, don't exit in real code */
        fputs("Memory allocation failure", stderr);
        exit(EXIT_FAILURE);
    }
    node->val  = val;
    node->next = NULL;
    return node;
}

IntList prepend_intnode(IntList list, int val)
{
    IntNode* node = create_intnode(val);
    node->next    = list;
    return node;
}

IntList free_intlist(IntList list)
{
    IntList tmp = Nil;
    while (list != Nil) {
        tmp  = list;
        list = list->next;
        free(tmp);
    }
    return Nil;
}

static Maybe(Int) intlistnxt(IntList* x, IntListItCtx* ctx)
{
    (void)x;
    IntNode* node = ctx->curr;
    if (node == Nil) {
        return Nothing(Int);
    }
    ctx->curr = node->next;
    return Just(node->val, Int);
}

impl_iterator(IntList, Int, IntListItCtx, intlistnxt, prep_intlist_itr)
