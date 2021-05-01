#ifndef IT_TYPECLASS_H
#define IT_TYPECLASS_H

#define typeclass(funcs, Name)                                                                                         \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        funcs;                                                                                                         \
    } Name##Instance;                                                                                                  \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        Name##Instance inst;                                                                                           \
        void* self;                                                                                                    \
    } Name

#endif /* !IT_TYPECLASS_H */
