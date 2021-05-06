#ifndef FUNC_ITER_H
#define FUNC_ITER_H

#include "iterator.h"
#include "maybe.h"
#include "typeclass.h"

typedef char* string;

// clang-format off
DefineMaybe(int, Int)
DefineMaybe(string, Str)

DefineIteratorOf(Int);
DefineIteratorOf(Str);
// clang-format on

#endif /* !FUNC_ITER_H */
