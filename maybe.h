/**
 * @file
 * Utilities to define and use a Maybe type.
 *
 * @note The Maybe struct members **must not** be accessed manually unless you know
 * exactly what you're doing. Use `from_just` *after* `is_just`/`is_nothing` instead.
 */

#ifndef IT_MAYBE_H
#define IT_MAYBE_H

#include <stdio.h>
#include <stdlib.h>

/**
 * @struct MaybeTag 
 * @brief The enum used for tagging the `Maybe` type.
 */
typedef enum
{
    MaybeTag_Nothing = 0, /**< `Nothing` tag - indicates absence of a value. */
    MaybeTag_Just /**< `Just` tag - indicates presence of a value. */
} MaybeTag;

/**
 * @def Maybe(Typename)
 * @brief Convenience macro to get the name of the Maybe defined with a certain type name.
 * 
 * # Example
 * 
 * @code
 * DefineMaybe(Int)
 * Maybe(Int) const x = {0}; // Uses the maybe type defined in the previous line
 * @endcode
 * 
 * @param ElmntTypename The "canonical" type name of a type, must be the same type name passed to #DefineMaybe(T, Typename).
 */
#define Maybe(Typename) Maybe##Typename

/**
 * @def DefineMaybe(T, Typename)
 * @brief Define a Maybe<T> type.
 * 
 * # Example
 * 
 * @code
 * DefineMaybe(Int) // Defines a Maybe(Int) type as well as its corresponding functions
 * @endcode
 * 
 * @param T The concrete (semantic) type for the `Just` branch of this `Maybe`.
 * @param Typename The "canonical" type name of the type this iterator will yield, this is purely subjective and
 * upto the user to decide, however the type names for each type **must** be consistent.
 * 
 * @note This should not be delimited by a semicolon.
 */
#define DefineMaybe(T, Typename)                                                                                       \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        MaybeTag tag;                                                                                                  \
        /* Don't access this member manually */                                                                        \
        T val;                                                                                                         \
    } Maybe(Typename);                                                                                                 \
    static inline T Typename##_from_maybe(Maybe(Typename) maybex, T defaultval)                                        \
    {                                                                                                                  \
        return is_just(maybex) ? maybex.val : defaultval;                                                              \
    }                                                                                                                  \
    static inline T Typename##_from_just(Maybe(Typename) maybex)                                                       \
    {                                                                                                                  \
        if (is_just(maybex)) {                                                                                         \
            return maybex.val;                                                                                         \
        } else {                                                                                                       \
            fputs("Attempted to extract Just value from Nothing", stderr);                                             \
            abort();                                                                                                   \
        }                                                                                                              \
    }

/**
 * @def Just(v, Typename)
 * @brief Wrap a `Just` value into a `Maybe(Typename)`.
 * 
 * # Example
 * 
 * @code
 * DefineMaybe(Int)
 * Maybe(Int) const x = Just(42, Int); // Initializes a Maybe(Int) with the value 42
 * @endcode
 * 
 * @param v The concrete value to wrap in `Just` (must be of the correct type).
 * @param Typename The "canonical" type name of a type used to define the Maybe type.
 * 
 * @note The value is simply assigned to the Maybe<T> struct. No implicit copying is done.
 */
#define Just(v, Typename) ((Maybe(Typename)){.tag = MaybeTag_Just, .val = (v)})

/**
 * @def Nothing(Typename)
 * @brief Wrap a `Nothing` value into a `Maybe(Typename)`.
 * 
 * # Example
 * 
 * @code
 * DefineMaybe(Int)
 * Maybe(Int) const x = Nothing(Int); // Initializes a Maybe(Int) with no value
 * @endcode
 * 
 * @param Typename The "canonical" type name of a type used to define the Maybe type
 */
#define Nothing(Typename) ((Maybe(Typename)){0})

/**
 * @def is_nothing(x)
 * @brief Check if the given Maybe type is tagged with `Nothing`.
 * 
 * @param x The Maybe<T> struct to check against.
 */
#define is_nothing(x) ((x).tag == MaybeTag_Nothing)
/**
 * @def is_just(x)
 * @brief Check if the given Maybe type is tagged with `Just`.
 * 
 * @param x The Maybe<T> struct to check against.
 */
#define is_just(x) ((x).tag == MaybeTag_Just)

/**
 * @def from_just(x, Typename)
 * @brief Extract the `Just` value from given `Maybe<T>`.
 * 
 * @param x The `Maybe` type to extract the value from.
 * @param Typename The "canonical" type name of a type used to define the Maybe type.
 * 
 * @return `Just` value of type corresponding to the given Maybe(Typename) if it's not `Nothing`.
 * @note Aborts the program if given Maybe<T> struct was tagged with `Nothing`.
 */
#define from_just(x, Typename) Typename##_from_just(x)

#endif /* !IT_MAYBE_H */
