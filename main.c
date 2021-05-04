#include "array_iterable.h"
#include "func_iter.h"
#include "list_iterable.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Generic function to sum values from any iterator yielding int */
static int sum_intit(Iterator(Int) it)
{
    int sum = 0;
    while (1) {
        Maybe(Int) res = it.next(it.self, it.ctx);
        if (is_nothing(res)) {
            return sum;
        }
        sum += from_just(res, Int);
    }
}

/* Generic function to create IntList from any iterator yielding int */
static IntList list_from_intit(Iterator(Int) it)
{
    IntList list = Nil;
    while (1) {
        Maybe(Int) res = it.next(it.self, it.ctx);
        if (is_nothing(res)) {
            return list;
        }
        int val = from_just(res, Int);
        list    = prepend_intnode(list, val);
    }
}

static int sum_intarr(int** arr, size_t sz)
{
    ArrItCtx ctx        = {.size = sz};
    Iterator(Int) intit = prep_intarr_itr(arr, &ctx);
    return sum_intit(intit);
}

static int sum_intlist(IntList* list)
{
    IntListItCtx ctx    = {.curr = *list};
    Iterator(Int) intit = prep_intlist_itr(list, &ctx);
    return sum_intit(intit);
}

int main(void)
{
    /* Use array's iterator instance */
    int arr[]        = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int* parr        = arr;
    int const sumarr = sum_intarr(&parr, sizeof(arr) / sizeof(*arr));
    printf("Sum of array values: %d\n", sumarr);

    /* Use list's iterator instance */
    IntList list      = prepend_intnode(prepend_intnode(prepend_intnode(prepend_intnode(Nil, 5), 6), 1), 9);
    int const sumlist = sum_intlist(&list);
    printf("Sum of list values: %d\n", sumlist);
    list = free_intlist(list);

    /* Use an iterator to build a list */
    /* Prepare an iterator from an array */
    int arrex[]         = {42, 3, 17, 25};
    int* parrex         = arrex;
    ArrItCtx arrctx     = {.size = sizeof(arrex) / sizeof(*arrex)};
    Iterator(Int) intit = prep_intarr_itr(&parrex, &arrctx);
    /* Use the iterator to build the list */
    IntList list_from_it      = list_from_intit(intit);
    int const sumlist_from_it = sum_intlist(&list_from_it);
    printf("Sum of list from iterator values: %d\n", sumlist_from_it);
    list_from_it = free_intlist(list_from_it);

    return 0;
}
