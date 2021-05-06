#include "func_iter.h"

#include <stdio.h>
#include <stdlib.h>

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
