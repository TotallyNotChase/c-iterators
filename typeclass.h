#ifndef IT_TYPECLASS_H
#define IT_TYPECLASS_H

/*
Defines a typeclass with the given members/functions and the self argument

@param membs - A semicolon separated list of typeclass members.
These are usually functions that take a `void* self` (or more arguments)
*/
#define typeclass(membs)                                                                                               \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        membs;                                                                                                         \
        void* self;                                                                                                    \
    }

#endif /* !IT_TYPECLASS_H */
