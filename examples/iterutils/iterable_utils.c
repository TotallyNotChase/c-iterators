#include "iterable_utils.h"

#include "../func_iter.h"

#include <stdio.h>
#include <string.h>

/*
 * Definitions of generic functions that work on an Iterator instance, i.e Iterable
 *
 * These functions don't need to care about the exact data structure backing the iterator
 *
 * At the same time, the Iterator instance is lazy - so the data isn't pre-generated and put into
 * a common struct, instead - it's extracted from the actual source backing the iterator on demand
 */

/* Generic function to sum values from any iterable yielding int */
int sum_intit(Iterable(int) it)
{
    int sum = 0;
    foreach (int, x, it) {
        sum += x;
    }
    return sum;
}

/* Generic function to print values from any iterable yielding string */
void print_strit(Iterable(string) it)
{
    foreach (string, s, it) {
        printf("%s ", s);
    }
    puts("");
}

define_itertake_func(uint32_t)
