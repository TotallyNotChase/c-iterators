#ifndef FUNC_ITER_H
#define FUNC_ITER_H

#include "iterator.h"
#include "maybe.h"

#include <stdint.h>

typedef char* string;

// clang-format off
DefineMaybe(int)
DefineMaybe(string)
DefineMaybe(uint32_t)

DefineIteratorOf(int);
DefineIteratorOf(string);
DefineIteratorOf(uint32_t);
// clang-format on

#endif /* !FUNC_ITER_H */
