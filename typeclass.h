#ifndef IT_TYPECLASS_H
#define IT_TYPECLASS_H

/*
Defines a typeclass with the given functions

@param funcs - A semicolon separated list of typeclass functions
These are usually functions that take the `self` from the typeclass instance (or more arguments)
*/
#define typeclass(funcs, Name)                                                                                         \
    struct Name                                                                                                        \
    {                                                                                                                  \
        funcs;                                                                                                         \
    }

/*
Defines a typeclass instance for the given typeclass

This just contains a `void* self` member and the typeclass itself
*/
#define typeclass_instance(TypeclassName, Name)                                                                        \
    struct Name                                                                                                        \
    {                                                                                                                  \
        void* self;                                                                                                    \
        TypeclassName tc;                                                                                              \
    }

#endif /* !IT_TYPECLASS_H */
