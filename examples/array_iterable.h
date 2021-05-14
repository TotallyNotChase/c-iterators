#ifndef IT_ARR_ITRBLE_H
#define IT_ARR_ITRBLE_H

#include "func_iter.h"

#include <stdlib.h>

#define ArrIter(ElmntType) ElmntType##ArrIter

#define DefineArrIterOf(T)                                                                                             \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        size_t i;                                                                                                      \
        size_t const size;                                                                                             \
        T const* const arr;                                                                                            \
    } ArrIter(T)

/* Macro to consistently name the arriter -> iterable functions based on element type */
#define prep_arriter_of(T) prep_##T##arr_itr

/*
Take in a source array, its size and its element type, build an `ArrIter` from it, and call the wrapper function to
turn it into an `Iterable`
*/
#define arr_into_iter(srcarr, sz, T) prep_arriter_of(T)(&(ArrIter(T)){.i = 0, .size = sz, .arr = srcarr})

/* Define `ArrIter` struct for int arrays */
DefineArrIterOf(int);
/* Define `ArrIter` struct for char* arrays */
DefineArrIterOf(string);

/* Convert a pointer to an `ArrIter(int)` to an `Iterable(int)` */
Iterable(int) prep_arriter_of(int)(ArrIter(int) * x);
/* Convert a pointer to an `ArrIter(string)` to an `Iterable(string)` */
Iterable(string) prep_arriter_of(string)(ArrIter(string) * x);

#endif /* !IT_ARR_ITRBLE_H */
