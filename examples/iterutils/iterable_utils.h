#ifndef IT_ITRBLE_UTILS_H
#define IT_ITRBLE_UTILS_H

#include "../func_iter.h"
#include "take.h"

#define foreach(T, x, it) for (Maybe(T) x = (it).tc.next((it).self); !is_nothing(x); x = (it).tc.next((it).self))

DefineIterTake(uint32_t);

/* Generic function to sum values from any iterable yielding int */
int sum_intit(Iterable(int) it);

/* Generic function to print values from any iterable yielding string */
void print_strit(Iterable(string) it);

/* Make an iterable of the first n elements of given iterable */
Iterable(uint32_t) take_u32it(Iterable(uint32_t) it, size_t n);

#endif /* !IT_ITRBLE_UTILS_H */
