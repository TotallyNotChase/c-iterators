cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(iterators LANGUAGES C)

##################################################
# Configure some universal settings

# Set C language standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Don't skimp on warnings
add_compile_options(-Wall -Wextra -Wpedantic)
if(WIN32)
    # But ignore the stupid "Use *_s function instead of deprecated function bla bla" warning
    add_compile_options(-Wno-deprecated-declarations)
endif()
# Also add link time optimization (if possible) in release mode
if(CMAKE_BUILD_TYPE MATCHES Release)
  include(CheckIPOSupported)
  check_ipo_supported(RESULT supported OUTPUT error)
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

##################################################
# Configure target for building the executable

# Add the main test executable
add_executable(iterators "func_iter.h" "main.c" "typeclass.h" "maybe.h" "iterator.h" "array_iterable.c" "array_iterable.h")
