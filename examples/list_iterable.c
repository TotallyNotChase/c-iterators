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

void print_intlist(IntNode const* head)
{
    while (head != Nil) {
        printf("%d ", head->val);
        head = head->next;
    }
    puts("");
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

/* `next` implementation for `ListIter(ConstIntList)` */
static Maybe(int) intlistnxt(ListIter(ConstIntList) * self)
{
    IntNode const* node = self->curr;
    if (node == Nil) {
        return Nothing(int);
    }
    /* Progress the stored list pointer */
    self->curr = node->next;
    return Just(node->val, int);
}

/* Implement `Iterator` for `ListIter(ConstIntList) *`, which in turn is for a singular linked list of ints */
impl_iterator(ListIter(ConstIntList) *, int, prep_listiter_of(ConstIntList), intlistnxt)
