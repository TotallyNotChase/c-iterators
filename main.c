#include "array_iterable.h"
#include "func_iter.h"
#include "list_iterable.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define Cons prepend_intnode

/* Generic function to sum values from any iterable yielding int */
static int sum_intit(Iterable(Int) it)
{
    int sum = 0;
    while (1) {
        Maybe(Int) res = it.tc.next(it.self);
        if (is_nothing(res)) {
            return sum;
        }
        sum += from_just(res, Int);
    }
}

/* Generic function to print values from any iterable yielding string */
static void print_strit(Iterable(Str) it)
{
    while (1) {
        Maybe(Str) res = it.tc.next(it.self);
        if (is_nothing(res)) {
            puts("");
            return;
        }
        printf("%s ", from_just(res, Str));
    }
}

/* Generic function to create IntList from any iterable yielding int */
static IntList list_from_intit(Iterable(Int) it)
{
    IntList list = Nil;
    while (1) {
        Maybe(Int) res = it.tc.next(it.self);
        if (is_nothing(res)) {
            return list;
        }
        int val = from_just(res, Int);
        list    = Cons(val, list);
    }
}

/* TODO: Try passing incorrect types to the abstractions */
int main(void)
{
    /* Use array's iterator instance */
    /* For int array */
    int arr[]            = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    ArrIter(Int) arriter = arr_into_iter(arr, sizeof(arr) / sizeof(*arr), Int);
    Iterable(Int) arrit  = prep_intarr_itr(&arriter);
    int const sumarr     = sum_intit(arrit);
    printf("Sum of array values: %d\n", sumarr);
    /* For string array */
    string strarr[]         = {"fear", "surprise", "ruthless-efficiency"};
    ArrIter(Str) strarriter = arr_into_iter(strarr, sizeof(strarr) / sizeof(*strarr), Str);
    Iterable(Str) strarrit  = prep_strarr_itr(&strarriter);
    print_strit(strarrit);

    /* Use list's iterator instance */
    IntList list           = Cons(9, Cons(1, Cons(6, Cons(5, Nil))));
    ListIter(Int) listiter = list_into_iter(list, Int);
    Iterable(Int) listit   = prep_intlist_itr(&listiter);
    int const sumlist      = sum_intit(listit);
    printf("Sum of list values: %d\n", sumlist);
    list = free_intlist(list);

    /* Use an iterator to build a list */
    /* Prepare an iterator from an array */
    int arrex[]            = {42, 3, 17, 25};
    ArrIter(Int) arrexiter = arr_into_iter(arrex, sizeof(arrex) / sizeof(*arrex), Int);
    Iterable(Int) arrexit  = prep_intarr_itr(&arrexiter);
    /* Use the iterator to build the list */
    IntList listex           = list_from_intit(arrexit);
    ListIter(Int) listexiter = list_into_iter(listex, Int);
    Iterable(Int) listexit   = prep_intlist_itr(&listexiter);
    int const sumlistex      = sum_intit(listexit);
    printf("Sum of list from iterator values: %d\n", sumlistex);
    listex = free_intlist(listex);

    return 0;
}
