/**
 * @file
 * Utilities to define a typeclass and its instance.
 */

#ifndef IT_TYPECLASS_H
#define IT_TYPECLASS_H

/**
 * @def typeclass(funcs)
 * @brief Define a typeclass with the given functions.
 *
 * # Example
 *
 * @code
 * typedef typeclass(char* (*show)(void* self)) Show;  // Defines a typeclass and names it `Show`
 * @endcode
 *
 * @param funcs A semicolon separated list of typeclass functions.
 *
 * @note The functions usually take the `self` from the typeclass instance (and possibly more arguments).
 */
#define typeclass(funcs)                                                                                               \
    struct                                                                                                             \
    {                                                                                                                  \
        funcs;                                                                                                         \
    }

/**
 * @def typeclass_instance(Typeclass)
 * @brief Define a typeclass instance for the given typeclass.
 *
 * This just contains a `void* self` member and the typeclass itself.
 * # Example
 *
 * @code
 * typedef typeclass(char* (*show)(void* self)) Show;
 * typdef typeclass_instance(Show) Showable; // Defines the typeclass instance for `Show` typeclass
 * @endcode
 *
 * @param Typeclass The semantic type (C type) of the typeclass defined with #typeclass(funcs).
 */
#define typeclass_instance(Typeclass)                                                                                  \
    struct                                                                                                             \
    {                                                                                                                  \
        void* self;                                                                                                    \
        Typeclass tc;                                                                                                  \
    }

#endif /* !IT_TYPECLASS_H */
