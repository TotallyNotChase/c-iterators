#include "fibonacci_iterable.h"

#include "func_iter.h"

#include <stdlib.h>

/* `next` implementation for the `Fibonacci` struct */
static Maybe(uint32_t) fibnxt(Fibonacci* self)
{
    uint32_t new_nxt = self->curr + self->next;
    self->curr       = self->next;
    self->next       = new_nxt;
    return Just(new_nxt, uint32_t);
}

// clang-format off
/* Implement `Iterator` for `Fibonacci*` */
impl_iterator(Fibonacci*, uint32_t, prep_fib_itr, fibnxt)
