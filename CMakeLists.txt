﻿cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(generic_iterators LANGUAGES C)

##################################################
# Configure some universal settings

# Set C language standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
# Disable non standard extensions
set(CMAKE_C_EXTENSIONS OFF)

# Don't skimp on warnings
if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()
if(WIN32)
  # But ignore the Microsoft "Use *_s functions" warning
  if (MSVC)
    add_compile_options(/wd4996)
  else()
    add_compile_options(-Wno-deprecated-declarations)
  endif()
endif()

# Add link time optimization (if possible) in release mode
if(CMAKE_BUILD_TYPE MATCHES Release)
  include(CheckIPOSupported)
  check_ipo_supported(RESULT supported OUTPUT output)
  if(supported)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
  else()
    message(WARNING "IPO is not supported: ${output}")
  endif()
endif()

# Set library name
set(LIBNAME iterators)

# Add the interface library target
add_library(${LIBNAME} INTERFACE)

# Add the headers to the library interface
target_include_directories(${LIBNAME} INTERFACE .)

add_subdirectory(examples)
