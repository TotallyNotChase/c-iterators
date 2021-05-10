#include "examples.h"
#include "func_iter.h"
#include "iterutils/iterable_utils.h"
#include "list_iterable.h"

#define Cons prepend_intnode /* Convenience macro to prepend_intnode */

IntList revlist_from_intit(Iterable(int) it)
{
    IntList list = Nil;
    while (1) {
        Maybe(int) res = it.tc.next(it.self);
        if (is_nothing(res)) {
            return list;
        }
        int val = from_just_(res);
        list    = Cons(val, list);
    }
}

void test_list(void)
{
    IntList list = Cons(9, Cons(1, Cons(6, Cons(5, Nil))));

    /* Turn the list into an iterable */
    Iterable(int) listit = list_into_iter(list, ConstIntList);
    int const sumlist    = sum_intit(listit);
    printf("Sum of list values: %d\n", sumlist);

    /* Free the list */
    list = free_intlist(list);
}
