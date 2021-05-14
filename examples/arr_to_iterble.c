#include "array_iterable.h"
#include "examples.h"
#include "iterutils/iterable_utils.h"

void test_array(void)
{
    /* For int array */
    int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    /* Turn the array into an Iterable */
    Iterable(int) arrit = arr_into_iter(arr, sizeof(arr) / sizeof(*arr), int);

    int const sumarr = sum_intit(arrit);
    printf("Sum of array values: %d\n", sumarr);

    /* For string array */
    string strarr[] = {"fear", "surprise", "ruthless-efficiency"};
    /* Turn the array into an Iterable */
    Iterable(string) strarrit = arr_into_iter(strarr, sizeof(strarr) / sizeof(*strarr), string);

    print_strit(strarrit);
}
