#include "array_iterable.h"
#include "examples.h"
#include "func_iter.h"

void test_list_from_arr(void)
{
    /* Prepare an iterator from an array */
    int arr[]            = {42, 3, 17, 25};
    ArrIter(Int) arriter = arr_into_iter(arr, sizeof(arr) / sizeof(*arr), Int);
    Iterable(Int) arrit  = prep_intarr_itr(&arriter);

    /* Use the iterator to build the list of reversed iterator */
    IntList list = revlist_from_intit(arrit);
    /* Print the list to verify values */
    print_intlist(list);

    /* Free the list */
    list = free_intlist(list);
}
