#include "list_iterable.h"

#include "func_iter.h"

#include <stdlib.h>

/* Create IntNode from given value */
static IntNode* create_intnode(int val)
{
    IntNode* node = malloc(sizeof(*node));
    node->val     = val;
    node->next    = NULL;
    return node;
}

IntList prepend_intnode(int val, IntList list)
{
    IntNode* node = create_intnode(val);
    node->next    = list;
    return node;
}

IntList free_intlist(IntNode* head)
{
    IntList tmp = Nil;
    while (head != Nil) {
        tmp  = head;
        head = head->next;
        free(tmp);
    }
    return Nil;
}

static Maybe(Int) intlistnxt(ListIter(Int) * self)
{
    IntNode const* node = self->curr;
    if (node == Nil) {
        return Nothing(Int);
    }
    self->curr = node->next;
    return Just(node->val, Int);
}

impl_iterator(ListIter(Int) *, Int, intlistnxt, prep_intlist_itr)
