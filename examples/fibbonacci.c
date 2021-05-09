#include "examples.h"
#include "fibonacci_iterable.h"
#include "iterutils/iterable_utils.h"

#include <inttypes.h>
#include <stdlib.h>

void test_fibonacci(void)
{
    Fibonacci fib      = {.curr = 0, .next = 1};
    Iterable(U32) it   = prep_fib_itr(&fib); /* Infinite fibonacci sequence iterable */
    Iterable(U32) it10 = take_u32it(it, 10); /* Iterable of the first 10 items in the sequence */
    /* Print the first 10 items */
    foreach (U32, res, it10) {
        printf("%" PRIu32 " ", from_just_(res));
    }
    /* Free the self argument of the iterable returned by `take_u32it` */
    free(it10.self);
    puts("");
}
