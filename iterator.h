/**
 * @file
 * Utilities to define and implement an Iterable.
 */

#ifndef IT_ITERATOR_H
#define IT_ITERATOR_H

#include "maybe.h"
#include "typeclass.h"

/**
 * @def Iterator(T)
 * @brief Convenience macro to get the type of the Iterator (typeclass) with given element type.
 *
 * # Example
 *
 * @code
 * DefineIteratorOf(int);
 * Iterator(int) i; // Declares a variable of type Iterator(int) (a typeclass)
 * @endcode
 *
 * @param T The type of value the `Iterator` instance will yield. Must be the same type name passed to
 * #DefineIteratorOf(T).
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 */
#define Iterator(T) T##Iterator

/**
 * @def Iterable(T)
 * @brief Convenience macro to get the type of the Iterable (typeclass instance) with given element type.
 *
 * # Example
 *
 * @code
 * DefineIteratorOf(int);
 * Iterable(int) i; // Declares a variable of type Iterable(Int) (the typeclass instance)
 * @endcode
 *
 * @param T The type of value the `Iterable` will yield. Must be the same type name passed to #DefineIteratorOf(T).
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 */
#define Iterable(T) T##Iterable

/**
 * @def DefineIteratorOf(T)
 * @brief Define an Iterator typeclass and its Iterable instance for given element type.
 *
 * # Example
 *
 * @code
 * DefineIteratorOf(int); // Defines an Iterator(int) typeclass as well as its instance
 * @endcode
 *
 * @param T The type of value the `Iterator` instance will yield. Must be alphanumeric.
 *
 * @note If `T` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only alphanumerics.
 * @note A #Maybe(T) for the given `T` **must** also exist.
 */
#define DefineIteratorOf(T)                                                                                            \
    typedef typeclass(Maybe(T) (*const next)(void* self)) Iterator(T);                                                 \
    typedef typeclass_instance(Iterator(T)) Iterable(T)

/**
 * @def impl_iterator(IterType, ElmntType, next_f, Name)
 * @brief Define a function to turn given `IterType` into an #Iterable(ElmntType).
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
 * DefineMaybe(uint32_t)
 * DefineIteratorOf(uint32_t);
 *
 * static Maybe(uint32_t) fibnxt(Fibonacci* self)
 * {
 *     uint32_t new_nxt = self->curr + self->next;
 *     self->curr       = self->next;
 *     self->next       = new_nxt;
 *     return Just(new_nxt, uint32_t);
 * }
 *
 * // Define a function named `prep_fib_itr`, which takes in a `Fibonacci*` and returns an `Iterable(int)`
 * // The returned iterable is an infinite fibonacci sequence
 * impl_iterator(Fibonacci*, uint32_t, fibnxt, prep_fib_itr)
 * @endcode
 *
 * @param IterType The semantic type (C type) this impl is for, must be a pointer type.
 * @param ElmntType The type of value the `Iterator` instance will yield.
 * @param next_f Function pointer that serves as the `next` implementation for `IterType`. This function must have
 * the signature of `Maybe(ElmntType) (*)(IterType self)` - i.e, should take IterType and return a value of the
 * corresponding element type wrapped in a `Maybe` - `Nothing` value indicates end of iteration.
 * @param Name Name to define the function as.
 *
 * @note If `ElmntType` is a pointer, it needs to be typedef-ed into a type that does not contain the `*`. Only
 * alphanumerics.
 * @note A #Maybe(T) for the given `ElmntType` **must** exist.
 * @note This should not be delimited by a semicolon.
 */
#define impl_iterator(IterType, ElmntType, Name, next_f)                                                               \
    Iterable(ElmntType) Name(IterType x)                                                                               \
    {                                                                                                                  \
        Maybe(ElmntType) (*const next_)(IterType self) = (next_f);                                                     \
        (void)next_;                                                                                                   \
        static Iterator(ElmntType) const tc = {.next = (Maybe(ElmntType)(*const)(void*))(next_f)};                     \
        return (Iterable(ElmntType)){.tc = &tc, .self = x};                                                            \
    }

#endif /* !IT_ITERATOR_H */
