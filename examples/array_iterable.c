#include "array_iterable.h"

#include "func_iter.h"

#include <stdlib.h>

/* `next` function impl for int arrays */
static Maybe(int) intarrnxt(ArrIter(int) * self)
{
    int const* const arr = self->arr;
    return self->i < self->size ? Just(arr[self->i++], int) : Nothing(int);
}

/* `next` function impl for char* arrays */
static Maybe(string) strarrnxt(ArrIter(string) * self)
{
    string const* const arr = self->arr;
    return self->i < self->size ? Just(arr[self->i++], string) : Nothing(string);
}

// clang-format off
/* Implement `Iterator` for ArrIter(int)*, which in turn is for int arrays */
impl_iterator(ArrIter(int)*, int, prep_arriter_of(int), intarrnxt)
/* Implement `Iterator` for ArrIter(string)*, which in turn is for char* arrays */
impl_iterator(ArrIter(string)*, string, prep_arriter_of(string), strarrnxt)
