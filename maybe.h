#ifndef IT_MAYBE_H
#define IT_MAYBE_H

#define DefineMaybe(T, Typename)                                                                                       \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        enum                                                                                                           \
        {                                                                                                              \
            None = 0,                                                                                                  \
            Some                                                                                                       \
        } tag;                                                                                                         \
        union                                                                                                          \
        {                                                                                                              \
            void* none;                                                                                                \
            T val;                                                                                                     \
        };                                                                                                             \
    } Maybe##Typename

#define Maybe(Typename) Maybe##Typename

#endif /* !IT_MAYBE_H */
