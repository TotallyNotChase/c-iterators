#ifndef IT_ARR_ITRBLE_H
#define IT_ARR_ITRBLE_H

#include "func_iter.h"

#include <stdlib.h>

#define ArrIter(ElmntType) ElmntType##ArrIter

#define DefineArrIterOf(T)                                                                                             \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        size_t i;                                                                                                      \
        size_t const size;                                                                                             \
        T const* const arr;                                                                                            \
    } ArrIter(T)

#define arr_into_iter(srcarr, sz, T)                                                                                   \
    prep_##T ##arr_itr(&(ArrIter(T)) { .i = 0, .size = sz, .arr = srcarr })

DefineArrIterOf(int);
DefineArrIterOf(string);

Iterable(int) prep_intarr_itr(ArrIter(int) * x);
Iterable(string) prep_stringarr_itr(ArrIter(string) * x);

#endif /* !IT_ARR_ITRBLE_H */
