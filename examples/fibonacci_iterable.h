#ifndef IT_FIB_H
#define IT_FIB_H

#include "func_iter.h"

#include <stdint.h>

typedef struct fibonacci
{
    uint32_t curr;
    uint32_t next;
} Fibonacci;

/* An iterable representing an infinite fibonacci sequence */
Iterable(U32) prep_fib_itr(Fibonacci* self);

#endif /* !IT_FIB_H */
