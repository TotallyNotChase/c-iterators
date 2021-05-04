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

static Iterator(Int) intit_from_arr(int** arr, size_t sz)
{
    ArrItCtx* ctx       = calloc(1, sizeof(*ctx));
    ctx->size           = sz;
    Iterator(Int) intit = prep_intarr_itr(arr, ctx);
    return intit;
}

static Iterator(Int) intit_from_list(IntList* list)
{
    IntListItCtx* ctx   = calloc(1, sizeof(*ctx));
    ctx->curr           = *list;
    Iterator(Int) intit = prep_intlist_itr(list, ctx);
    return intit;
}

int main(void)
{
    /* Use array's iterator instance */
    int arr[]           = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int* parr           = arr;
    Iterator(Int) arrit = intit_from_arr(&parr, sizeof(arr) / sizeof(*arr));
    int const sumarr    = sum_intit(arrit);
    printf("Sum of array values: %d\n", sumarr);
    free(arrit.ctx);

    /* Use list's iterator instance */
    IntList list         = prepend_intnode(prepend_intnode(prepend_intnode(prepend_intnode(Nil, 5), 6), 1), 9);
    Iterator(Int) listit = intit_from_list(&list);
    int const sumlist    = sum_intit(listit);
    printf("Sum of list values: %d\n", sumlist);
    list = free_intlist(list);
    free(listit.ctx);

    /* Use an iterator to build a list */
    /* Prepare an iterator from an array */
    int arrex[]           = {42, 3, 17, 25};
    int* parrex           = arrex;
    Iterator(Int) arrexit = intit_from_arr(&parrex, sizeof(arrex) / sizeof(*arrex));
    /* Use the iterator to build the list */
    IntList listex         = list_from_intit(arrexit);
    Iterator(Int) listexit = intit_from_list(&listex);
    int const sumlistex    = sum_intit(listexit);
    printf("Sum of list from iterator values: %d\n", sumlistex);
    listex = free_intlist(listex);
    free(arrexit.ctx);
    free(listexit.ctx);

    return 0;
}
