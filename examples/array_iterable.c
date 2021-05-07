#include "array_iterable.h"

#include "func_iter.h"

#include <stdlib.h>

static Maybe(Int) intarrnxt(ArrIter(Int) * self)
{
    int const* const arr = self->arr;
    return self->i < self->size ? Just(arr[self->i++], Int) : Nothing(Int);
}

static Maybe(Str) strarrnxt(ArrIter(Str) * self)
{
    string const* const arr = self->arr;
    return self->i < self->size ? Just(arr[self->i++], Str) : Nothing(Str);
}

// clang-format off
impl_iterator(ArrIter(Int)*, Int, intarrnxt, prep_intarr_itr)

impl_iterator(ArrIter(Str)*, Str, strarrnxt, prep_strarr_itr)
