#ifndef IT_MAP_H
#define IT_MAP_H

#include "iterable_utils.h"

#define IterMap(ElmntType, FnRetType) IterMap##ElmntType##FnRetType

#define DefineIterMap(ElmntType, FnRetType)                                                                            \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        FnRetType (*const mapfn)(ElmntType x);                                                                         \
        Iterable(ElmntType) const src;                                                                                 \
    } IterMap(ElmntType, FnRetType)

/* Name of the function that wraps an IterMap(ElmntType, FnRetType) for given ElmntType and FnRetType into an iterable
 */
#define prep_itermap_of(ElmntType, FnRetType) CONCAT(CONCAT(prep_, IterMap(ElmntType, FnRetType)), _itr)

/* Map the function `fn` of type `FnRetType (*)(ElmntType)` over `it` to make a new iterable */
#define map_over(it, fn, ElmntType, FnRetType)                                                                         \
    prep_itermap_of(ElmntType, FnRetType)(&(IterMap(ElmntType, FnRetType)){.mapfn = fn, .src = it})

/*
Define the iterator implementation function for an IterMap struct
Also define a function with the given `Name` - which takes in an iterable and a function to map over said iterable,
wraps said iterable and function in an `IterMap` struct and wraps that around its `Iterable` impl
*/
#define define_itermap_func(ElmntType, FnRetType)                                                                      \
    static Maybe(FnRetType) CONCAT(IterMap(ElmntType, FnRetType), _nxt)(IterMap(ElmntType, FnRetType) * self)          \
    {                                                                                                                  \
        Iterable(ElmntType) const srcit = self->src;                                                                   \
        Maybe(ElmntType) res            = srcit.tc.next(srcit.self);                                                   \
        if (is_nothing(res)) {                                                                                         \
            return Nothing(FnRetType);                                                                                 \
        }                                                                                                              \
        return Just(self->mapfn(from_just_(res)), FnRetType);                                                          \
    }                                                                                                                  \
    impl_iterator(IterMap(ElmntType, FnRetType)*, FnRetType, CONCAT(IterMap(ElmntType, FnRetType), _nxt),              \
                  prep_itermap_of(ElmntType, FnRetType))

#endif /* !IT_MAP_H */
