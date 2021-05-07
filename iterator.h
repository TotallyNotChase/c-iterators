/**
 * @file
 * Utilities to define and implement an Iterable.
 */

#ifndef IT_ITERATOR_H
#define IT_ITERATOR_H

#include "maybe.h"
#include "typeclass.h"

/**
 * @def Iterator(ElmntTypename)
 * @brief Convenience macro to get the name of the Iterator (typeclass) with given element type name.
 *
 * @param ElmntTypename The "canonical" type name of a type, must be the same type name passed to
 * #DefineIteratorOf(ElmntTypename).
 */
#define Iterator(ElmntTypename) ElmntTypename##Iterator

/**
 * @def Iterable(ElmntTypename)
 * @brief Convenience macro to get the name of the Iterable (typeclass instance) with given element type name.
 *
 * @param ElmntTypename The "canonical" type name of a type, must be the same type name passed to
 * #DefineIteratorOf(ElmntTypename).
 */
#define Iterable(ElmntTypename) ElmntTypename##Iterable

/**
 * @def DefineIteratorOf(ElmntTypename)
 * @brief Define an Iterator typeclass and its Iterable instance for given element type.
 *
 * @param ElmntTypename The "canonical" type name of the type this iterable will yield. This is purely subjective and
 * upto the user to decide, however the type names for each type **should** be consistent.
 *
 * @note A #Maybe(Typename) for the given `ElmntTypename` **must** exist.
 */
#define DefineIteratorOf(ElmntTypename)                                                                                \
    typedef typeclass(Maybe(ElmntTypename) (*const next)(void* self)) Iterator(ElmntTypename);                         \
    typedef typeclass_instance(Iterator(ElmntTypename)) Iterable(ElmntTypename)

/**
 * @def impl_iterator(IterType, ElmntTypename, next_f, Name)
 * @brief Define a function to turn given `IterType` into an #Iterable(ElmntTypename).
 *
 * Implement the Iterator typeclass for a type. Essentially defining a wrapper function that returns the Iterable.
 *
 * The defined function takes in a value of `IterType` and wraps it in an `Iterable` - which can be passed around to
 * generic functions working on an iterable.
 *
 * The term "generic" is used here in the context of the **input**.
 * As in, the function taking a generic iterable, does not care about what type is backing up the iterable; but,
 * does care about what element type the iterator yields.
 *
 * @param IterType The semantic type (C type) this impl is for, must be a pointer type.
 * @param ElmntTypename The "canonical" type name of the element the iterable will yield.
 * @param next_f Function pointer that serves as the `next` implementation for `IterType`. This function must have
 * the signature of `Maybe(ElmntTypename) (*)(IterType self)` - i.e, should take IterType and return a value of the
 * corresponding element type wrapped in a `Maybe` - `Nothing` value indicates end of iteration.
 * @param Name Name to define the function as.
 *
 * @note A #Maybe(Typename) for the given `ElmntTypename` **must** exist.
 */
#define impl_iterator(IterType, ElmntTypename, next_f, Name)                                                           \
    Iterable(ElmntTypename) Name(IterType x)                                                                           \
    {                                                                                                                  \
        Maybe(ElmntTypename) (*const next_)(IterType self) = (next_f);                                                 \
        (void)next_;                                                                                                   \
        return (Iterable(ElmntTypename)){.tc = {.next = (Maybe(ElmntTypename)(*const)(void*))next_f}, .self = x};      \
    }

#endif /* !IT_ITERATOR_H */
