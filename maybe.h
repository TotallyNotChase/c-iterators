#ifndef IT_MAYBE_H
#define IT_MAYBE_H

typedef enum
{
    MaybeTag_Nothing = 0,
    MaybeTag_Some
} MaybeTag;

#define DefineMaybe(T, Typename)                                                                                       \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        MaybeTag tag;                                                                                                  \
        T val;                                                                                                         \
    } Maybe##Typename

#define Maybe(Typename) Maybe##Typename

#define Some(V, Typename)                                                                                              \
    (Maybe(Typename)) { .tag = MaybeTag_Some, .val = (V) }

#define Nothing(Typename)                                                                                              \
    (Maybe(Typename)) { 0 }

#define is_nothing(x) (x).tag == MaybeTag_Nothing

#endif /* !IT_MAYBE_H */
