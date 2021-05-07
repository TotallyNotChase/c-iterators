#include "array_iterable.h"
#include "examples.h"
#include "iterable_utils.h"

void test_array(void)
{
    /* For int array */
    int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    /* Turn the array into an ArrIter */
    ArrIter(Int) arriter = arr_into_iter(arr, sizeof(arr) / sizeof(*arr), Int);
    /* Use the ArrIter's Iterator impl */
    Iterable(Int) arrit = prep_intarr_itr(&arriter);
    int const sumarr    = sum_intit(arrit);
    printf("Sum of array values: %d\n", sumarr);

    /* For string array */
    string strarr[] = {"fear", "surprise", "ruthless-efficiency"};
    /* Turn the array into an ArrIter */
    ArrIter(Str) strarriter = arr_into_iter(strarr, sizeof(strarr) / sizeof(*strarr), Str);
    /* Use the ArrIter's Iterator impl */
    Iterable(Str) strarrit = prep_strarr_itr(&strarriter);
    print_strit(strarrit);
}
