#ifndef IT_TYPECLASS_H
#define IT_TYPECLASS_H

#define typeclass(membs, Name)                                                                                         \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        membs;                                                                                                         \
    } Name##Instance;                                                                                                  \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        Name##Instance inst;                                                                                           \
        void* self;                                                                                                    \
    } Name

#endif /* !IT_TYPECLASS_H */
