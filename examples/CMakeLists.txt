##################################################
# Configure target for building the executable

# Add the main executable
add_executable(iterators_example "array_iterable.h" "list_iterable.h" "func_iter.h" "iterable_utils.h" "array_iterable.c" "list_iterable.c" "iterable_utils.c" "main.c")

# Link the iterators interface lib
target_link_libraries(iterators_example ${LIBNAME})
