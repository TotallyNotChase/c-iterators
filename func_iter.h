#ifndef FUNC_ITER_H
#define FUNC_ITER_H

#include "iterator.h"
#include "maybe.h"
#include "typeclass.h"

typedef char* string;

DefineMaybe(int, Int)
DefineMaybe(string, Str)

DeclareIteratorOf(Int);
DeclareIteratorOf(Str);

#endif /* !FUNC_ITER_H */
