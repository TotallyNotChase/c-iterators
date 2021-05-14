#ifndef IT_FIB_H
#define IT_FIB_H

#include "func_iter.h"

#include <stdint.h>

typedef struct fibonacci
{
    uint32_t curr;
    uint32_t next;
} Fibonacci;

/* Create an infinite `Iterable` representing the fibonacci sequence */
#define get_fibitr() prep_fib_itr(&(Fibonacci){.curr = 0, .next = 1})

/* Turn a pointer to a `Fibonacci` struct to an iterable */
Iterable(uint32_t) prep_fib_itr(Fibonacci* self);

#endif /* !IT_FIB_H */
