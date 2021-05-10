##################################################
# Configure target for building the executable

# Add the main executable
add_executable(iterators_example
  "iterutils/take.h"
  "iterutils/map.h"
  "iterutils/iterable_utils.h"
  "iterutils/iterable_utils.c"
  "fibonacci_iterable.h"
  "array_iterable.h"
  "list_iterable.h"
  "examples.h"
  "func_iter.h"
  "fibonacci_iterable.c"
  "array_iterable.c"
  "fibbonacci.c"
  "list_iterable.c"
  "arr_to_iterble.c"
  "list_to_iterble.c"
  "list_from_arr.c"
  "main.c"
  "map_over.c"
)

# Link the iterators interface lib
target_link_libraries(iterators_example ${LIBNAME})
