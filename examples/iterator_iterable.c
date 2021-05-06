#include "iterator_iterable.h"

#include "func_iter.h"

#include <stdlib.h>

#define VALSTR_SIZE 100 /* An assumption for the size to allocate valstr to (see inttostr) */

/* Pop a value from an iterable of int elements, convert it into a string and return it */
static Maybe(Str) inttostr(Iterable(Int) * self)
{
    Maybe(Int) res = self->tc.next(self->self);
    if (is_nothing(res)) {
        return Nothing(Str);
    }
    int val = from_just(res, Int);
    /*
    This is just for demonstration purposes, never assume the size of the buffer
    where the result of sprintf will be stored - calculate it first by doing a
    dry snprintf run
    */
    char* const valstr = malloc(VALSTR_SIZE * sizeof(*valstr));
    snprintf(valstr, VALSTR_SIZE, "%d", val);

    /* Return the string representation */
    return Just(valstr, Str);
}

/*
Implementing the iterator typeclass for an iterable instance
This is essentially overriding the `next` typeclass function of the given
iterable - which in turn, achieves a functionality very similar to mapping
*/
impl_iterator(Iterable(Int) *, Str, inttostr, map_inttostr_itr)
