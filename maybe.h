#ifndef IT_MAYBE_H
#define IT_MAYBE_H

#include <stdio.h>
#include <stdlib.h>

/* The enum used for tagging the `Maybe` type */
typedef enum
{
    MaybeTag_Nothing = 0,
    MaybeTag_Just
} MaybeTag;

/*
Define a Maybe<T> type

@param T - The concrete (semantic) type for the `Just` branch of this `Maybe`
@param Typename - The "canonical" type name of the type this iterator will yield, this is purely subjective and
upto the user to decide, however the type names for each type **must** be consistent
*/
#define DefineMaybe(T, Typename)                                                                                       \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        MaybeTag tag;                                                                                                  \
        /* Don't access this member manually */                                                                        \
        T val;                                                                                                         \
    } Maybe##Typename;                                                                                                 \
    static inline T Typename##_from_maybe(Maybe##Typename maybex, T defaultval)                                        \
    {                                                                                                                  \
        return is_just(maybex) ? maybex.val : defaultval;                                                              \
    }                                                                                                                  \
    static inline T Typename##_from_just(Maybe##Typename maybex)                                                       \
    {                                                                                                                  \
        if (is_just(maybex)) {                                                                                         \
            return maybex.val;                                                                                         \
        } else {                                                                                                       \
            fputs("Attempted to extract Just value from Nothing", stderr);                                             \
            abort();                                                                                                   \
        }                                                                                                              \
    }

/*
Convenience macro to get the name of the Maybe with given `Just` value type name

@param ElmntTypename - The "canonical" type name of a type, must be the same type name used while defining the Maybe
type
*/
#define Maybe(Typename) Maybe##Typename

/*
Wrap a `Just` value into a `Maybe(Typename)`

@param v - The concrete value to wrap in `Just` (must be of the correct type)
@param Typename - The "canonical" type name of a type used to define the Maybe type
*/
#define Just(v, Typename) ((Maybe(Typename)){.tag = MaybeTag_Just, .val = (v)})

/*
Wrap a `Nothing` value into a `Maybe(Typename)`

@param Typename - The "canonical" type name of a type used to define the Maybe type
*/
#define Nothing(Typename) ((Maybe(Typename)){0})

/* Check if the given Maybe type is `Nothing` */
#define is_nothing(x) ((x).tag == MaybeTag_Nothing)
/* Check if the given Maybe type has a `Just` value */
#define is_just(x) ((x).tag == MaybeTag_Just)

/*
Extract the `Just` value from given `Maybe(Typename)`. Aborts the program if
 `Maybe(Typename)` consisted of nothing

@param x - `Maybe` type to extract the value from
@param Typename - The "canonical" type name of a type used to define the Maybe type

@returns - `Just` value of type corresponding to the given Maybe(Typename) if it's not `Nothing`
*/
#define from_just(x, Typename) Typename##_from_just(x)

#endif /* !IT_MAYBE_H */
