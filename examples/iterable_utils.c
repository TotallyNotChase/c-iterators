#include "func_iter.h"

#include <stdio.h>

/*
* Definitions of generic functions that work on an Iterator instance, i.e Iterable
* 
* These functions don't need to care about the exact data structure backing the iterator
* 
* At the same time, the Iterator instance is lazy - so the data isn't pre-generated and put into
* a common struct, instead - it's extracted from the actual source backing the iterator on demand
*/

/* Generic function to sum values from any iterable yielding int */
int sum_intit(Iterable(Int) it)
{
    int sum = 0;
    while (1) {
        Maybe(Int) res = it.tc.next(it.self);
        if (is_nothing(res)) {
            return sum;
        }
        sum += from_just(res, Int);
    }
}

/* Generic function to print values from any iterable yielding string */
void print_strit(Iterable(Str) it)
{
    while (1) {
        Maybe(Str) res = it.tc.next(it.self);
        if (is_nothing(res)) {
            puts("");
            return;
        }
        printf("%s ", from_just(res, Str));
    }
}
