# Architecture
This document aims to describe the file structure and contents of this project.

# Root
The root directory contains the primary header files to define and implement iterators and iterator instances (i.e iterables)
<table>
<tr>
  <th>File</th>
  <th>Description</th>
</tr>
<tr>
  <td>

  `typeclass.h`
 
  </td>
  <td>

  Utility macros to define a typeclass and its instance.
  
  </td>
</tr>
<tr>
  <td>

  `maybe.h`
 
  </td>
  <td>

  Utility macros to define and use a `Maybe` type.
  
  </td>
</tr>
<tr>
  <td>

  `iterator.h`
 
  </td>
  <td>

  Primary file containing macros to define the `Iterator` typeclass, `Iterable` typeclass instance and a utility macro, `impl_iterable`, that defines a function to wrap a pointer type into an `Iterable` - essentially implementing the `Iterator` typeclass for that type.
  
  </td>
</tr>
</table>

# Examples
The `examples/` directory contains usage examples of the Iterator typeclass pattern.

This also contains a subdirectory - `iterutils`, which contains utilities to work with generic `Iterable`s.

## `iterutils`
<table>
<tr>
  <th>File</th>
  <th>Description</th>
</tr>
<tr>
  <td>

  `take.h`
 
  </td>
  <td>

  Macros to define an `IterTake` struct of a certain type.

  This struct stores a source iterable, and keeps track of how many elements to extract from it. This is used to implement a `take`-like function.

  Also defines a macro to implement `Iterator` for an `IterTake` struct, as well as a function that takes in an `Iterable` and the number of elements to extract from that iterable (`n`), and returns the corresponding `IterTake` struct wrapped in its `Iterable` impl. **This `Iterable`'s `self` member must be freed after usage**.
  
  </td>
</tr>
<tr>
  <td>

  `iterable_utils.h`
 
  </td>
  <td>

  Declarations of all the utility functions to be exposed to the examples.
  
  </td>
</tr>
<tr>
  <td>

  `iterable_utils.c`
 
  </td>
  <td>

  Definitions of the utility functions.

  </td>
</tr>
</table>

The usage of these iterutils and the primary header files are demonstrated by the files in `examples/`-
<table>
<tr>
  <th>File</th>
  <th>Description</th>
</tr>
<tr>
  <td>

  `func_iter.h`
 
  </td>
  <td>

  Includes all the primary headers from the root directory. Also defines `Maybe` and `Iterator` for the following types-
  * `int`
  * `char*` (Typedef-ed to `string`)
  * `uint32_t`
  
  </td>
</tr>
<tr>
  <td>

  `examples.h`
 
  </td>
  <td>

  Declarations for all the functions demonstrating iterable usage.
  
  </td>
</tr>
<tr>
  <td>

  `main.c`
 
  </td>
  <td>

  The main function definition, executes the functions demonstrating iterable usage. (Declared in `examples.h`)
  
  </td>
</tr>
<tr>
  <td>

  `array_iterable.h`
 
  </td>
  <td>

  Declarations for functions and structs to be used to use an array as an `Iterable`.

  This defines the `ArrIter` struct - this struct wraps an array and has an `Iterator` implementation.
  
  </td>
</tr>
<tr>
  <td>

  `array_iterable.c`
 
  </td>
  <td>

  Definitons for functions be used to use an array as an `Iterable`.

  This implements the `Iterator` typeclass for the `ArrIter` struct.
  
  </td>
</tr>
<tr>
  <td>

  `list_iterable.h`
 
  </td>
  <td>

  Declarations for functions and structs to be used to use a singly linked list as an `Iterable`.

  This defines the `ListIter` struct - this struct wraps a singly linked list and has an `Iterator` implementation.
  
  </td>
</tr>
<tr>
  <td>

  `list_iterable.c`
 
  </td>
  <td>

  Definitons for functions to be used to use a singly linked list as an `Iterable`.

  This implements the `Iterator` typeclass for the `ListIter` struct.
  
  </td>
</tr>
<tr>
  <td>

  `fibonacci_iterable.h`
 
  </td>
  <td>

  Declarations for functions and structs to be used to use an `Iterable` representing the infinite fibonacci sequence.
  
  </td>
</tr>
<tr>
  <td>

  `fibonacci_iterable.c`
 
  </td>
  <td>

  Definitions for functions to be used to use an `Iterable` representing the infinite fibonacci sequence.
  
  </td>
</tr>
<tr>
  <td>

  `arr_to_iterble.c`
 
  </td>
  <td>

  Example function that wraps an array into an `Iterable` to be used by a generic function operating on iterables.
  
  </td>
</tr>
<tr>
  <td>

  `list_to_iterble.c`
 
  </td>
  <td>

  Example function that wraps a singly linked list into an `Iterable` to be used by a generic function operating on iterables.
  
  </td>
</tr>
<tr>
  <td>

  `list_from_arr.c`
 
  </td>
  <td>

  Example function that wraps an array into an `Iterable` that can be used to build a singly linked list from a generic function operating on iterables.
  
  </td>
</tr>
<tr>
  <td>

  `fibonacci.c`
 
  </td>
  <td>

  Example function that prints the first 10 elements from an `Iterable` representing the infinite fibonacci sequence.
  
  </td>
</tr>
</table>
