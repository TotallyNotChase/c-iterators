#include "array_iterable.h"

#include "func_iter.h"

#include <stdlib.h>

static Maybe(Int) intarrnxt(int** arr, ArrItCtx* ctx)
{
    return ctx->i < ctx->size ? (Maybe(Int)){.tag = Some, .val = (*arr)[ctx->i++]} : (Maybe(Int)){0};
}

instance_iterator(int*, Int, ArrItCtx, intarrnxt, prep_intarr_itr)
