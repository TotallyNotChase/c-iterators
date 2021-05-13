#ifndef IT_TAKE_H
#define IT_TAKE_H

#include "../func_iter.h"

/*
Utilities to define an IterTake type for a specific element type and its corresponding iterator impl.

An IterTake struct is a struct that keeps track of how many elements have been extracted out of an iterator
as well as a limit to stop at - the iterator impl for this struct will keep consuming from an iterator until
it hits that limit. (or if the source iterable has been exhausted)

This allows to implement the `take_from` macro - which takes in the source iterable, and the number of elements
to extract from it, and returns an iterable with an iterable of those elements.

This is identical to the `take` function iterator typeclasses usually have.
*/

#define IterTake(ElmntType) IterTake##ElmntType

#define DefineIterTake(ElmntType)                                                                                      \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        size_t i;                                                                                                      \
        size_t const limit;                                                                                            \
        Iterable(ElmntType) const src;                                                                                 \
    } IterTake(ElmntType)

/* Name of the function that wraps an IterTake(ElmntType) for given ElmntType into an iterable  */
#define prep_itertake_of(ElmntType) CONCAT(CONCAT(prep_, IterTake(ElmntType)), _itr)

/* Build an iterable that consists of at most `n` elements from given `it` iterable */
#define take_from(it, n, T) prep_itertake_of(T)(&(IterTake(T)){.i = 0, .limit = n, .src = it})

/*
Define the iterator implementation function for an IterTake struct

The function is named `prep_itertake_of(ElmntType)`
*/
#define define_itertake_func(ElmntType)                                                                                \
    static Maybe(ElmntType) CONCAT(IterTake(ElmntType), _nxt)(IterTake(ElmntType) * self)                              \
    {                                                                                                                  \
        if (self->i < self->limit) {                                                                                   \
            ++(self->i);                                                                                               \
            Iterable(ElmntType) srcit = self->src;                                                                     \
            return srcit.tc->next(srcit.self);                                                                         \
        }                                                                                                              \
        return Nothing(ElmntType);                                                                                     \
    }                                                                                                                  \
    impl_iterator(IterTake(ElmntType)*, ElmntType, prep_itertake_of(ElmntType), CONCAT(IterTake(ElmntType), _nxt))

#endif /* !IT_TAKE_H */
