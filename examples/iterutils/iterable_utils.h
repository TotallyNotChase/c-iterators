#ifndef IT_ITRBLE_UTILS_H
#define IT_ITRBLE_UTILS_H

#include "../func_iter.h"
#include "take.h"

#define foreach(T, x, it) for (Maybe(T) x = (it).tc.next((it).self); !is_nothing(x); x = (it).tc.next((it).self))

DefineIterTake(U32);

/* Generic function to sum values from any iterable yielding int */
int sum_intit(Iterable(Int) it);

/* Generic function to print values from any iterable yielding string */
void print_strit(Iterable(Str) it);

/* Make an iterable of the first n elements of given iterable */ 
Iterable(U32) take_u32it(Iterable(U32) it, size_t n);

#endif /* !IT_ITRBLE_UTILS_H */
