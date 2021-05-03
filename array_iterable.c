#include "array_iterable.h"

#include "func_iter.h"

#include <stdlib.h>

static Maybe(Int) intarrnxt(int** arr, ArrItCtx* ctx)
{
    return ctx->i < ctx->size ? Some((*arr)[ctx->i++], Int) : Nothing(Int);
}

instance_iterator(int*, Int, ArrItCtx, intarrnxt, prep_intarr_itr)
