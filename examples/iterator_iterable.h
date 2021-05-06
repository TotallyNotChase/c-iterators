#ifndef IT_ITERATOR_ITRBLE_H
#define IT_ITERATOR_ITRBLE_H

#include "func_iter.h"

/*
Maps a function over the given int iterable

The mapping function converts each int to its string representation

@param x - The iterable (of int elements) to map the function over

@returns An iterable of string elements, where each element is the result
of mapping the function over the corresponding int
*/
Iterable(Str) map_inttostr_itr(Iterable(Int) * x);

#endif /* !IT_ITERATOR_ITRBLE_H */
