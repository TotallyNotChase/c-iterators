#ifndef IT_ARR_ITRBLE_H
#define IT_ARR_ITRBLE_H

#include "func_iter.h"
#include "iterator.h"

#include <stdlib.h>

typedef struct
{
    size_t i;
    size_t size;
} ArrItCtx;

Iterator(Int) prep_intarr_itr(int** x, ArrItCtx* init_ctx);

#endif /* !IT_ARR_ITRBLE_H */
