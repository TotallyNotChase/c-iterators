#ifndef IT_ITERATOR_H
#define IT_ITERATOR_H

#include "maybe.h"
#include "typeclass.h"

/*
Convenience macro to get the name of the Iterator with given element type name

@param ElmntTypename - The "canonical" type name of a type, must be the same type name used while declaring the iterator
typeclass
*/
#define Iterator(ElmntTypename) ElmntTypename##Iterator

/*
Convenience macro to get the name of the Iterator with given element type name

@param ElmntTypename - The "canonical" type name of a type, must be the same type name used while declaring the iterator
typeclass
*/
#define Iterable(ElmntTypename) ElmntTypename##Iterable

/*
Convenience macro to define an Iterator typeclass of a specific element type

@param ElmntTypename - The "canonical" type name of the type this iterator will yield, this is purely subjective and
upto the user to decide, however the type names for each type **must** be consistent. A Maybe(ElmntTypename) for the
given ElmntTypename **must** also exist
*/
#define DefineIteratorOf(ElmntTypename)                                                                                \
    typedef typeclass(Maybe(ElmntTypename) (*const next)(void* self), Iterator(ElmntTypename##_))                      \
        Iterator(ElmntTypename);                                                                                       \
    typedef typeclass_instance(Iterator(ElmntTypename), Iterable(ElmntTypename##_)) Iterable(ElmntTypename)

/*
Implement a wrapper function for a specific type, which takes in a value of that type and wraps it in an `Iterator`
instance - essentially making the type a generic "Iterable" - which can be passed around to functions that take in a
generic `Iterable`.

Note that the term "generic" is used here in the context of the **input**.
As in, the function taking a generic iterable, does not care about what type the iterable was generated from; but, does
care about what element type the iterator yields

@param ItrbleType - The concrete (semantic) type this impl is for - e.g `int*` if the impl is for an array of
ints
@param ElmntTypename - The "canonical" type name of the element of the type this impl is for - e.g type name of `int` if
the impl is for an array of ints
*/
#define impl_iterator(ItrbleType, ElmntTypename, next_f, Name)                                                         \
    Iterable(ElmntTypename) Name(ItrbleType x)                                                                         \
    {                                                                                                                  \
        Maybe(ElmntTypename) (*const next_)(ItrbleType self) = (next_f);                                               \
        (void)next_;                                                                                                   \
        return (Iterable(ElmntTypename)){.tc = {.next = (Maybe(ElmntTypename)(*const)(void*))next_f}, .self = x};      \
    }

#endif /* !IT_ITERATOR_H */
