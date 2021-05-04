#include "array_iterable.h"
#include "func_iter.h"

#include <stdio.h>
#include <stdlib.h>

/* Generic function for any iterator of element type int */
static void print_intit(Iterator(Int) it)
{
    while (1) {
        Maybe(Int) res = it.next(it.self, it.ctx);
        if (is_nothing(res)) {
            break;
        }
        printf("%d ", from_just(res, Int));
    }
    puts("\n");
}

static void print_intarr(int** arr, size_t sz)
{
    ArrItCtx ctx      = {.size = sz};
    IntIterator intit = prep_intarr_itr(arr, &ctx);
    print_intit(intit);
}

int main(void)
{
    /* Use array's iterator instance */
    int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int* parr = arr;
    print_intarr(&parr, sizeof(arr) / sizeof(*arr));

    return 0;
}
