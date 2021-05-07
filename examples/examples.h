#ifndef IT_EXAMPLES_H
#define IT_EXAMPLES_H

#include "func_iter.h"
#include "list_iterable.h"

/* Use array's iterator instance */
void test_array(void);
/* Use list's iterator instance */
void test_list(void);
/* Turn an array into an iterator and use it to build a list */
void test_list_from_arr(void);

/* Generic function to create a reversed IntList from any iterable yielding int */
IntList revlist_from_intit(Iterable(Int) it);

#endif /* !IT_EXAMPLES_H */