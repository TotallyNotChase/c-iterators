#ifndef IT_ITERATOR_H
#define IT_ITERATOR_H

#include "maybe.h"
#include "typeclass.h"

/*
Convenience macro to get the name of the Iterator (typeclass) with given element type name

@param ElmntTypename The "canonical" type name of a type, must be the same type name passed to DefineIteratorOf
*/
#define Iterator(ElmntTypename) ElmntTypename##Iterator

/*
Convenience macro to get the name of the Iterable (typeclass instance) with given element type name

@param ElmntTypename The "canonical" type name of a type, must be the same type name passed to DefineIteratorOf
*/
#define Iterable(ElmntTypename) ElmntTypename##Iterable

/*
Convenience macro to define an Iterator typeclass and its Iterable instance for given element type

@param ElmntTypename The "canonical" type name of the type this iterable will yield, this is purely subjective and
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

@param IterType The semantic type this impl is for, must be a pointer type
@param ElmntTypename The "canonical" type name of the element the iterable will yield
@param next_f Pointer to the function that serves as the `next` implementation for IterType. This function must have
the signature of `Maybe(ElmntTypename) (*)(IterType self)` - i.e, should take IterType (the self value of the instance)
and return a value of the corresponding element type wrapped in a `Maybe` - `Nothing` value indicates end of iteration
@param Name Name of the wrapper function being defined
*/
#define impl_iterator(IterType, ElmntTypename, next_f, Name)                                                           \
    Iterable(ElmntTypename) Name(IterType x)                                                                           \
    {                                                                                                                  \
        Maybe(ElmntTypename) (*const next_)(IterType self) = (next_f);                                                 \
        (void)next_;                                                                                                   \
        return (Iterable(ElmntTypename)){.tc = {.next = (Maybe(ElmntTypename)(*const)(void*))next_f}, .self = x};      \
    }

#endif /* !IT_ITERATOR_H */
