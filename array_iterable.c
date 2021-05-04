#include "array_iterable.h"

#include "func_iter.h"

#include <stdlib.h>

static Maybe(Int) intarrnxt(int** arr, ArrItCtx* ctx)
{
    return ctx->i < ctx->size ? Just((*arr)[ctx->i++], Int) : Nothing(Int);
}

static Maybe(Str) strarrnxt(string** arr, ArrItCtx* ctx)
{
    return ctx->i < ctx->size ? Just((*arr)[ctx->i++], Str) : Nothing(Str);
}

impl_iterator(int*, Int, ArrItCtx, intarrnxt, prep_intarr_itr)

impl_iterator(string*, Str, ArrItCtx, strarrnxt, prep_strarr_itr)
