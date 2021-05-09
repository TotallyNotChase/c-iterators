#ifndef IT_TAKE_H
#define IT_TAKE_H

#include "../func_iter.h"

/*
Utilities to define an IterTake type for a specific element type and its corresponding functions and iterator impl.

An IterTake struct is a struct that keeps track of how many elements have been extracted out of an iterator
as well as a limit to stop at - the iterator impl for this struct will keep consuming from an iterator until
it hits that limit. (or if the source iterable has been exhausted)

This allows a general function to be implemented that takes in an iterable and a number representing how many elements
to "take" out of the iterable and returns an iterable that consumes from the given iterable, stopping at the limit.

This is identical to the `take` function iterator typeclasses usually have.
*/

#define CONCAT_(A, B) A##B
#define CONCAT(A, B)  CONCAT_(A, B)

#define IterTake(ElmntType) IterTake##ElmntType

#define DefineIterTake(ElmntType)                                                                                      \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        size_t i;                                                                                                      \
        size_t const limit;                                                                                            \
        Iterable(ElmntType) const src;                                                                                 \
    } IterTake(ElmntType)

// clang-format off
/*
Define the iterator implementation function for an IterTake struct
Also define a function with the given `name` - which takes in an iterable and the number of elements to
"take" from that iterable (`n`) - wraps said iterable in an `IterTake` struct and wraps that around its
`Iterable` impl

The `self` argument of the returned `Iterable` must be freed after use
*/
#define define_itertake_func(ElmntType, name)                                                                      \
    static Maybe(ElmntType) CONCAT(IterTake(ElmntType), _nxt)(IterTake(ElmntType) * self)                  \
    {                                                                                                                  \
        if (self->i < self->limit) {                                                                                   \
            Iterable(ElmntType) srcit = self->src;                                                                 \
            Maybe(ElmntType) x        = srcit.tc.next(srcit.self);                                                 \
            if (is_nothing(x)) {                                                                                       \
                return Nothing(ElmntType);                                                                         \
            }                                                                                                          \
            self->i++;                                                                                                 \
            return Just(from_just_(x), ElmntType);                                                                 \
        }                                                                                                              \
        return Nothing(ElmntType);                                                                                 \
    }                                                                                                                  \
    static inline impl_iterator(IterTake(ElmntType)*, ElmntType, CONCAT(IterTake(ElmntType), _nxt),        \
                                CONCAT(CONCAT(prep_, IterTake(ElmntType)), _itr))                                  \
    Iterable(ElmntType) name(Iterable(ElmntType) src, size_t n)                                                \
    {                                                                                                                  \
        IterTake(ElmntType) _takestruct = {.limit = n, .src = src};                                                \
        IterTake(ElmntType)* takestruct = malloc(sizeof(*takestruct));                                             \
        memcpy(takestruct, &_takestruct, sizeof(*takestruct));                                                         \
        return CONCAT(CONCAT(prep_, IterTake(ElmntType)), _itr)(takestruct);                                       \
    }

#endif /* !IT_TAKE_H */
