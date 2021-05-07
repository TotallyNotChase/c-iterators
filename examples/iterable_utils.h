#ifndef IT_ITRBLE_UTILS_H
#define IT_ITRBLE_UTILS_H

#include "func_iter.h"

/* Generic function to sum values from any iterable yielding int */
int sum_intit(Iterable(Int) it);

/* Generic function to print values from any iterable yielding string */
void print_strit(Iterable(Str) it);

#endif /* !IT_ITRBLE_UTILS_H */