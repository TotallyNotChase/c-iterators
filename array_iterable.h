#ifndef IT_ARR_ITRBLE_H
#define IT_ARR_ITRBLE_H

#include "iterator.h"

typedef struct
{
    size_t i;
    size_t size;
} ArrItCtx;

Iterator(Int) prep_intarr_itr(int** x);

#endif /* !IT_ARR_ITRBLE_H */
