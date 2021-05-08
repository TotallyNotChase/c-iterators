#include "examples.h"
#include "fibonacci_iterable.h"

#include <inttypes.h>
#include <stdlib.h>

void test_fibonacci(void)
{
    Fibonacci fib    = {.curr = 0, .next = 1};
    Iterable(U32) it = prep_fib_itr(&fib);
    /* Print the first 10 items */
    for (size_t i = 0; i < 10; i++) {
        printf("%" PRIu32 " ", from_just_(it.tc.next(it.self)));
    }
    puts("");
}
