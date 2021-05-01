#include "func_iter.h"

#include <stdio.h>

typedef struct
{
    size_t i;
    size_t size;
} ArrItCtx;

static Maybe(Int) intarrnxt(int** arr, ArrItCtx* ctx)
{
    return ctx->i < ctx->size ? (Maybe(Int)){.tag = Some, .val = (*arr)[ctx->i++]} : (Maybe(Int)){0};
}

instance_iterator(int*, Int, ArrItCtx, intarrnxt, prep_intarr_itr)

int main(void)
{
    int arr[]         = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int* parr         = arr;
    IntIterator intit = prep_intarr_itr(&parr);
    ArrItCtx ctx      = {.size = sizeof(arr) / sizeof(*arr)};
    while (1) {
        Maybe(Int) res = intit.inst.next(intit.self, &ctx);
        if (res.tag == None) {
            break;
        }
        printf("%d\n", res.val);
    }
    return 0;
}
