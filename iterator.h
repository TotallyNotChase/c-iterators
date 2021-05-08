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
 * @brief Convenience macro to get the type of the Iterator (typeclass) with given element type name.
 *
 * # Example
 *
 * @code
 * DefineIteratorOf(Int);
 * Iterator(Int) i; // Declares a variable of type Iterator(Int) (a typeclass)
 * @endcode
 *
 * @param ElmntTypename The "canonical" type name of a type, must be the same type name passed to
 * #DefineIteratorOf(ElmntTypename).
 */
#define Iterator(ElmntTypename) ElmntTypename##Iterator

/**
 * @def Iterable(ElmntTypename)
 * @brief Convenience macro to get the type of the Iterable (typeclass instance) with given element type name.
 *
 * # Example
 *
 * @code
 * DefineIteratorOf(Int);
 * Iterator(Int) i; // Declares a variable of type Iterable(Int) (the typeclass instance)
 * @endcode
 *
 * @param ElmntTypename The "canonical" type name of a type, must be the same type name passed to
 * #DefineIteratorOf(ElmntTypename).
 */
#define Iterable(ElmntTypename) ElmntTypename##Iterable

/**
 * @def DefineIteratorOf(ElmntTypename)
 * @brief Define an Iterator typeclass and its Iterable instance for given element type.
 *
 * # Example
 *
 * @code
 * DefineIteratorOf(Int); // Defines an Iterator(Int) typeclass as well as its instance
 * @endcode
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
 * # Example
 *
 * @code
 * // Example of implementing an infinite iterator representing the fibonacci sequence
 * 
 * #include <stdint.h>
 * 
 * typedef struct fibonacci
 * {
 *     uint32_t curr;
 *     uint32_t next;
 * } Fibonacci;
 * 
 * DefineMaybe(uint32_t, U32)
 * DefineIteratorOf(U32);
 * 
 * static Maybe(U32) fibnxt(Fibonacci* self)
 * {
 *     uint32_t new_nxt = self->curr + self->next;
 *     self->curr       = self->next;
 *     self->next       = new_nxt;
 *     return Just(new_nxt, U32);
 * }
 * 
 * // Define a function named `prep_fib_itr`, which takes in a `Fibonacci*` and returns an `Iterable(Int)`
 * // The returned iterable is an infinite fibonacci sequence
 * impl_iterator(Fibonacci*, U32, fibnxt, prep_fib_itr)
 * @endcode
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
