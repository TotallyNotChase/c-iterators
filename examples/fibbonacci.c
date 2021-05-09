#include "examples.h"
#include "fibonacci_iterable.h"
#include "iterutils/iterable_utils.h"

#include <inttypes.h>
#include <stdlib.h>

void test_fibonacci(void)
{
    Fibonacci fib      = {.curr = 0, .next = 1};
    Iterable(uint32_t) it   = prep_fib_itr(&fib); /* Infinite fibonacci sequence iterable */
    Iterable(uint32_t) it10 = take_u32it(it, 10); /* Iterable of the first 10 items in the sequence */
    /* Print the first 10 items */
    foreach (uint32_t, res, it10) {
        printf("%" PRIu32 " ", from_just_(res));
    }
    /* Free the self argument of the iterable returned by `take_u32it` */
    free(it10.self);
    puts("");
}
