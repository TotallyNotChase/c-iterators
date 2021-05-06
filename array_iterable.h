#ifndef IT_ARR_ITRBLE_H
#define IT_ARR_ITRBLE_H

#include "func_iter.h"

#include <stdlib.h>

#define ArrIter(ElmntTypename) ElmntTypename##ArrIter

#define DefineArrIterOf(T, Typename) typedef struct { size_t i; size_t const size; T const* const arr; } ArrIter(Typename)

#define arr_into_iter(srcarr, sz, ElmntTypename)  (ArrIter(ElmntTypename)){ .i = 0, .size = sz, .arr = srcarr }

DefineArrIterOf(int, Int);
DefineArrIterOf(string, Str);

Iterable(Int) prep_intarr_itr(ArrIter(Int)* x);
Iterable(Str) prep_strarr_itr(ArrIter(Str)* x);

#endif /* !IT_ARR_ITRBLE_H */
