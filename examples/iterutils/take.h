#include "../func_iter.h"

/*
Utilities to define an IterTake type for a specific element and its corresponding functions and iterator impl.

An IterTake struct is a struct that keeps track of how many elements have been extracted out of an iterator
as well as a limit to stop at - the iterator impl for this struct will keep consuming from an iterator until
it hits that limit.

This allows a general function to be implemented that takes in an iterable and a number representing how many elements
to "take" out of the iterable and returns an iterable that consumes from the given iterable, stopping at the limit.

This is identical to the `take` function iterator typeclasses usually have.
*/

#define CONCAT_(A, B) A ## B
#define CONCAT(A, B) CONCAT_(A, B)

#define IterTake(ElmntTypename) IterTake##ElmntTypename

#define DefineIterTake(ElmntTypename)                                                                                  \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        size_t i;                                                                                                      \
        size_t const limit;                                                                                            \
        Iterable(ElmntTypename) const src;                                                                             \
    } IterTake(ElmntTypename)

// clang-format off
#define define_itertake_func(ElmntTypename, name)                                                                      \
    static Maybe(ElmntTypename) CONCAT(IterTake(ElmntTypename), _nxt)(IterTake(ElmntTypename) * self)                  \
    {                                                                                                                  \
        if (self->i < self->limit) {                                                                                   \
            Iterable(ElmntTypename) srcit = self->src;                                                                 \
            Maybe(ElmntTypename) x        = srcit.tc.next(srcit.self);                                                 \
            if (is_nothing(x)) {                                                                                       \
                return Nothing(ElmntTypename);                                                                         \
            }                                                                                                          \
            self->i++;                                                                                                 \
            return Just(from_just_(x), ElmntTypename);                                                                 \
        }                                                                                                              \
        return Nothing(ElmntTypename);                                                                                 \
    }                                                                                                                  \
    static inline impl_iterator(IterTake(ElmntTypename)*, ElmntTypename, CONCAT(IterTake(ElmntTypename), _nxt),        \
                                CONCAT(CONCAT(prep_, IterTake(ElmntTypename)), _itr))                                  \
    Iterable(ElmntTypename) name(Iterable(ElmntTypename) src, size_t n)                                                \
    {                                                                                                                  \
        IterTake(ElmntTypename) _takestruct = {.limit = n, .src = src};                                                \
        IterTake(ElmntTypename)* takestruct = malloc(sizeof(*takestruct));                                             \
        memcpy(takestruct, &_takestruct, sizeof(*takestruct));                                                         \
        return CONCAT(CONCAT(prep_, IterTake(ElmntTypename)), _itr)(takestruct);                                       \
    }
