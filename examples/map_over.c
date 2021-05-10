#include "array_iterable.h"
#include "func_iter.h"
#include "iterutils/iterable_utils.h"

#define VALSTR_SIZE 100 /* An assumption for the size to allocate valstr to (see inttostr) */

static int incr(int x) { return x + 1; }

/* Convert an integer into a string, returned pointer must be freed */
static string inttostr(int x)
{
    /*
    This is just for demonstration purposes, never assume the size of the buffer
    where the result of sprintf will be stored - calculate it first by doing a
    dry snprintf run
    */
    char* const xstr = malloc(VALSTR_SIZE * sizeof(*xstr));
    snprintf(xstr, VALSTR_SIZE, "%d", x);

    /* Return the string representation */
    return xstr;
}

void test_mapping(void)
{
    int arr[] = {1, 2, 3};
    /* Turn the array into an Iterable */
    Iterable(int) arrit = arr_into_iter(arr, sizeof(arr) / sizeof(*arr), int);

    /* Map an increment function over the iterable */
    Iterable(int) mappedit = map_over(arrit, incr, int, int);
    /* Print the iterable */
    foreach (int, x, mappedit) {
        printf("%d ", x);
    }
    puts("");

    /* Make another iterable from the same array */
    Iterable(int) arrit1 = arr_into_iter(arr, sizeof(arr) / sizeof(*arr), int);

    /* Map the inttostr function over the iterable */
    Iterable(string) mappedit1 = map_over(arrit1, inttostr, int, string);
    /* Print the iterable */
    foreach (string, x, mappedit1) {
        printf("%s ", x);
    }
    puts("");
}