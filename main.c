#include "array_iterable.h"
#include "func_iter.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* Generic function for any iterator of element type int */
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

static int sum_intarr(int** arr, size_t sz)
{
    ArrItCtx ctx      = {.size = sz};
    IntIterator intit = prep_intarr_itr(arr, &ctx);
    return sum_intit(intit);
}

int main(void)
{
    /* Use array's iterator instance */
    int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int* parr = arr;
    int sumarr = sum_intarr(&parr, sizeof(arr) / sizeof(*arr));
    printf("Sum of array values: %d\n", sumarr);

    return 0;
}
