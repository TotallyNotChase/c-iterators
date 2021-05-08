#include "fibonacci_iterable.h"

#include "func_iter.h"

#include <stdlib.h>

static Maybe(U32) fibnxt(Fibonacci* self)
{
    uint32_t new_nxt = self->curr + self->next;
    self->curr       = self->next;
    self->next       = new_nxt;
    return Just(new_nxt, U32);
}

// clang-format off
impl_iterator(Fibonacci*, U32, fibnxt, prep_fib_itr)
