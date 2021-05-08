##################################################
# Configure target for building the executable

# Add the main executable
add_executable(iterators_example
  "fibonacci_iterable.h"
  "array_iterable.h"
  "list_iterable.h"
  "examples.h"
  "func_iter.h"
  "iterable_utils.h"
  "fibonacci_iterable.c"
  "array_iterable.c"
  "list_iterable.c"
  "iterable_utils.c"
  "fibbonacci.c"
  "arr_to_iterble.c"
  "list_to_iterble.c"
  "list_from_arr.c"
  "main.c"
)

# Link the iterators interface lib
target_link_libraries(iterators_example ${LIBNAME})
