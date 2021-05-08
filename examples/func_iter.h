#ifndef FUNC_ITER_H
#define FUNC_ITER_H

#include "iterator.h"
#include "maybe.h"

#include <stdint.h>

typedef char* string;

// clang-format off
DefineMaybe(int, Int)
DefineMaybe(string, Str)
DefineMaybe(uint32_t, U32)

DefineIteratorOf(Int);
DefineIteratorOf(Str);
DefineIteratorOf(U32);
// clang-format on

#endif /* !FUNC_ITER_H */
