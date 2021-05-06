#include "array_iterable.h"

#include "func_iter.h"

#include <stdlib.h>

static Maybe(Int) intarrnxt(ArrIter(Int)* arriter)
{
    int const* const arr = arriter->arr;
    return arriter->i < arriter->size ? Just(arr[arriter->i++], Int) : Nothing(Int);
}

static Maybe(Str) strarrnxt(ArrIter(Str)* arriter)
{
    string const* const arr = arriter->arr;
    return arriter->i < arriter->size ? Just(arr[arriter->i++], Str) : Nothing(Str);
}

impl_iterator(ArrIter(Int)*, Int, intarrnxt, prep_intarr_itr)

impl_iterator(ArrIter(Str)*, Str, strarrnxt, prep_strarr_itr)

