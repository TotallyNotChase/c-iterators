#ifndef IT_TYPECLASS_H
#define IT_TYPECLASS_H

#define typeclass(membs)                                                                                               \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        membs;                                                                                                         \
        void* self;                                                                                                    \
    }

#endif /* !IT_TYPECLASS_H */
