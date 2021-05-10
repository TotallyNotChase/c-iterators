#ifndef IT_ITRBLE_UTILS_H
#define IT_ITRBLE_UTILS_H

#include "../func_iter.h"
#include "map.h"
#include "take.h"

#define UNIQVAR(x) CONCAT(CONCAT(x, _4x2_), __LINE__) /* "Unique" variable name */

/* Iterate through given `it` iterable that contains elements of type `T` - store each element in `x` */
#define foreach(T, x, it)                                                                                              \
    Maybe(T) UNIQVAR(res) = (it).tc.next((it).self);                                                                   \
    for (T x          = from_just_(UNIQVAR(res)); !is_nothing(UNIQVAR(res));                                           \
         UNIQVAR(res) = (it).tc.next((it).self), x = from_just_(UNIQVAR(res)))

DefineIterTake(uint32_t);
DefineIterMap(int, int);
DefineIterMap(int, string);

/* Generic function to sum values from any iterable yielding int */
int sum_intit(Iterable(int) it);

/* Generic function to print values from any iterable yielding string */
void print_strit(Iterable(string) it);

/* Make an iterable of the first n elements of given iterable */
Iterable(uint32_t) prep_itertake_of(uint32_t)(IterTake(uint32_t) * x);
Iterable(int) prep_itermap_of(int, int)(IterMap(int, int) * x);
Iterable(string) prep_itermap_of(int, string)(IterMap(int, string) * x);

#endif /* !IT_ITRBLE_UTILS_H */
