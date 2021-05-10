#include "array_iterable.h"

#include "func_iter.h"

#include <stdlib.h>

static Maybe(int) intarrnxt(ArrIter(int) * self)
{
    int const* const arr = self->arr;
    return self->i < self->size ? Just(arr[self->i++], int) : Nothing(int);
}

static Maybe(string) strarrnxt(ArrIter(string) * self)
{
    string const* const arr = self->arr;
    return self->i < self->size ? Just(arr[self->i++], string) : Nothing(string);
}

// clang-format off
impl_iterator(ArrIter(int)*, int, intarrnxt, prep_intarr_itr)

impl_iterator(ArrIter(string)*, string, strarrnxt, prep_stringarr_itr)
