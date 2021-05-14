# c-iterators
A demonstration of implementing, and using, a "type safe", extensible, and lazy iterator interface in pure C99. The iterable is generic on the input side, but not output side - functions taking an `Iterable` don't need to know the concrete data structure backing up the `Iterable`, but the type of value the `Iterator` yields must be concrete and exact, no `void*`. (Well, you can still make it `void*` if you want - but I wouldn't suggest it.)

This doesn't mean you *can't* have an iterable of generic elements though. More on that can be found in [Iterable of Generic Elements](#iterable-of-generic-elements) and [The Typeclass Pattern](./Typeclass%20Pattern.md) document.

The only files you need to implement the `Iterator` typeclass, for your own types, are- `typeclass.h`, `maybe.h`, and `iterator.h`. The usages of these files, as well as extra utilities operating on iterables are shown in `examples/`. `examples/iterutils` also demonstrates the implementation of `take` and `map` utilites.

More info about the file structure can be found in the [Architecture document](./ARCHITECTURE.md).

You can find the generated docs [here](https://TotallyNotChase.github.io/c-iterators).

# A small taste
Here's a snippet where the `Iterator` typeclass has been implemented for a "fibonacci struct". Giving you an iterable representing the infinite Fibonacci sequence.
```c
Iterable(uint32_t) it   = get_fibitr();                /* Create an infinite fibonacci sequence iterable */
Iterable(uint32_t) it10 = take_from(it, 10, uint32_t); /* Iterable of the first 10 items in the sequence */
/* Print the first 10 items */
foreach (uint32_t, n, it10) {
    printf("%" PRIu32 " ", n);
}
puts("");
```

The fibonacci struct for which `Iterator` has been implemented looks like-
```c
typedef struct fibonacci
{
    uint32_t curr;
    uint32_t next;
} Fibonacci;
```

The `get_fibitr` macro does nothing but just initialize that struct with `curr = 0`, and `next = 1`, turn it into an `Iterable` and return it.

The `take_from` macro is explained in [Lazy Abstractions](#the-take-utility). You may be familiar with [`take`](https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:take) already though.

This entire construct is lazy. No extra iteration is performed. The only iteration that happens here is in the explicit `foreach` loop. Neither `get_fibitr` nor `take_from` does eager generation.

# Highlights
* Pure C99 support, no non standard extensions used
* Type safety (through the usage of abstracted macros that monomorphize based on the type given)

  Though you may have to actually turn on the warnings, preferably **all warnings**.
* Lazy-ness (the iterables are all lazily consumed, iterator utilites can also be chained lazily to evaluate all at once on demand)
* Extensible (`Iterable` uses dynamic dispatch, allowing library functions to use it in a public API)
* Functions working on `Iterables` can return `Iterables` - allowing them to be chained together lazily. This means you can have a `map` that returns an `Iterable`, pass it through a `filter` that also returns an `Iterable` - and both the map and filter will be evaluated in one singular iteration, on demand. Or you could `take` from an `Iterable` and then `map` on it, there won't be 2 iterations to do this, just one. (see [iterutils](./examples/iterutils) and [Advanced Usage](#lazy-abstractions))

# Building
Although you don't really need to build anything per se, since the primary files (`maybe.h`, `typeclass.h`, and `iterator.h`) are just headers that you can include in your project, you may still use the provided `CmakeLists.txt` (CMake 3.15 or higher) to build an executable running all the examples. The built executable should be present in the `examples/` directory inside the build directory.

## UNIX
```sh
mkdir build
cd build
cmake -G "Unix Makefiles" ..
```
This will create the required `make` config inside `build/`. Now you can run `make` to build the executable.

## Windows
### Visual Studio (2017 or higher)
You must have CMake integration for Visual Studio installed.

Open this project in Visual Studio (2017 or higher) and hit `Build -> Build All` from the toolbar. This will build the executable and put it into a directory named `out/` (by default). You can also directly run the built executable by choosing the "Startup Item" near the debug configuration.

### MinGW/Cygwin
Same as the unix way.

# A word on terminology
I like to use the word `Iterator` to refer to the typeclass, or the interface, or trait - whatever you wanna call it. `Iterable`, on the other hand, I use to refer to the `Iterator` instance, the concrete type that functions can take in, the type that holds the `self` member. This terminology is just for clarity - I don't really mind if you use the 2 words interchangably.

# Usage
In general, there are some contraints to implementing `Iterator` for a type-
* The type must be a pointer
* The type must be able to hold iteration information about itself - specifically, how much of itself has been iterated already and what's the next element.

  This is because the `next` function of the typeclass needs to just take in this type and figure out what to return.
* The element that will be yielded from the iterator instance of this type, must have an alphanumeric type name. If the element is a pointer, `typedef` it into some alphanumeric type name.
* A `Maybe(T)` for the corresponding `T` (type that the `Iterator` will yield) must exist.

Of course, you'll also need to have the `Iterator(T)` and `Maybe(T)` for a certain `T` (element the `Iterable` will yield) defined before you can implement `Iterator(T)` for anything. Remember to define those using [`DefineIteratorOf`](https://TotallyNotChase.github.io/c-iterators/iterator_8h.html#a938f3e7187cb386e2bf4049753e2ff84) and [`DefineMaybe`](https://TotallyNotChase.github.io/c-iterators/maybe_8h.html#a0daf26d181160b7fb96d640ed20bb466) respectively.

The examples define the common `Maybe` and `Iterator` types in [func_iter.h](./examples/func_iter.h), this file is then included by most other files.

## Iterating through an `Iterable`
To iterate through an `Iterable`, you call the `Iterable`'s `next` function and pass it the `self` member of the iterator.

This function's return value is of type [`Maybe`](https://wiki.haskell.org/Maybe). In case you're unfamiliar- a `Maybe` type represents the *presence* **or** *absence* of a value. As a C programmer, think of how a pointer can either point to a valid object, or be `NULL` - indicating the absence of any valid object to point to.

When the iteration ends, a `Nothing` value is returned. Otherwise, a `Just` value is returned. Use the `is_nothing` or `is_just` macros to check whether or not a certain `Maybe` struct contains a value. If `is_nothing` returns false, or `is_just` returns true - you can use `from_just` or `from_just_` to extract the actual value from the `Maybe` struct.

### Quick note: Difference between `from_just` and `from_just_`
`from_just_` is simply the "unsafe" version of `from_just`. `from_just_` assumes the passed maybe actually has a `Just` value and simply returns it, if the passed maybe did not have a value (i.e was `Nothing`), the behavior is undefined (the value *can be* indeterminate, but if the `Maybe` struct was built using the provided `Nothing` macro, the value will actually be zero initialized). `from_just` on the other hand, will abort the program with an error if the passed maybe did not have a value (i.e was `Nothing`). It is **totally safe** to use `from_just_` after you've checked `is_nothing` or `is_just` accordingly, though.

When you call `next` on an `Iterable`, you **consume** the iterator. This is a mutating process - the next time you call `next`, it'll return the next element - until it is fully consumed. Once that happens, calling `next` on it will keep returning `Nothing` values.

**Note**: This behavior is actually dependent on the `next` function implementation that the user provides when making their own data stucture an `Iterable`. An erroneous `next` function will not have defined behavior.

So, you can iterate through the entire iterable (therefore consuming it) by doing-
```c
Iterable(int) it = ...; /* Acquire the iterable */
for (Maybe(int) res = it.tc->next(it.self); is_just(res); res = it.tc->next(it.self)) {
    int x = from_just_(res);
    /* do stuff with x */
}
/* Iterable returned `Nothing`, iteration finished - iterable has been fully consumed */
```
Seems like a repeating pattern for any given type (here the type is `int`), right? You can implement a macro instead-
```c
#define UNIQVAR(x) CONCAT(CONCAT(x, _4x2_), __LINE__) /* "Unique" variable name */

/* Iterate through given `it` iterable that contains elements of type `T` - store each element in `x` */
#define foreach(T, x, it)                                                                                              \
    Maybe(T) UNIQVAR(res) = (it).tc->next((it).self);                                                                  \
    for (T x          = from_just_(UNIQVAR(res)); is_just(UNIQVAR(res));                                               \
         UNIQVAR(res) = (it).tc->next((it).self), x = from_just_(UNIQVAR(res)))
```
(You can find this macro in [iterable_utils.h](./examples/iterutils/iterable_utils.h))

Using this macro instead of the manual loop, the above snippet could look like-
```c
Iterable(int) it = ...; /* Acquire the iterable */
foreach (int, x, it) {
    /* do stuff with x */
}
```
Much cleaner!

## Some basic functions that work on an `Iterable`
You can find these in [iterable_utils.c](./examples/iterutils/iterable_utils.c)

### `sum_intit` - Sum all values in an `Iterable(int)`
```c
int sum_intit(Iterable(int) it)
{
    int sum = 0;
    foreach (int, x, it) {
        sum += x;
    }
    return sum;
}
```

### `print_strit` - Print all values in an `Iterable(string)`
```c
/* Note: `string` is just `typedef`-ed `char*` */
void print_strit(Iterable(string) it)
{
    foreach (string, s, it) {
        printf("%s ", s);
    }
    puts("");
}
```

### `revlist_from_intit` - Build a singly linked list from an iterable (list is built in reverse order)
This one is much more useful and practical, but slightly less basic.

```c
IntList revlist_from_intit(Iterable(int) it)
{
    IntList list = Nil;
    foreach (int, val, it) {
        list = Cons(val, list);
    }
    return list;
}
```
This allows you to build a linked list from any `Iterable`, and since `Iterable` is lazy - and can be implemented for any type, you not only have a generic function to build a list but you also essentially skip an extra iteration.

Note: `Cons` is just an alias to `prepend_intnode`, which is a function that prepends values to a singly linked list of ints. `Nil` is an alias to `NULL`.

## Expected behavior of `next`
When you're implementing `Iterator` for your desired type, the next function implementation you provide must follow some rules (outside of the context of the type system). These are as following-
* The function must return `Nothing` at the end of iteration, all returns before this must be `Just`.
* Once `Nothing` has been returned (i.e iteration has ended) - any extra calls to the `next` function must keep returning `Nothing`.

## Implementing `Iterator`
### For Arrays
We'll start with implementing `Iterator` for good ol' arrays. You can find the code for the implementation part in [array_iterable.h](./examples/array_iterable.h) and [array_iterable.c](./examples/array_iterable.c).

We need to keep track of the index and the size of the array to implement `Iterator` for it. So we need a `struct` like-
```c
struct arriter
{
    size_t i;
    size_t const size;
    T const* const arr;
}
```
A struct that keeps track of the `i` (the current index), the `size` (the length of the array), and the array itself. It forbids modifications to `size` and `arr`.

Notice the `T`, this isn't an actual type. The snippet just demonstrates that each `arriter` will need to hold its own type of array. Of course, you could cheat and use `void*` - but let's keep it type safe here.

We'll call this struct concept `ArrIter`. This concept is similar to rust's `IntoIter`.

An `ArrIter` where `T = int` (i.e array of ints) would look like-
```c
struct intarriter
{
    size_t i;
    size_t const size;
    int const* const arr;
}
```

You can generalize this into a macro-
```c
#define ArrIter(ElmntType) ElmntType##ArrIter

#define DefineArrIterOf(T)                                                                                             \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        size_t i;                                                                                                      \
        size_t const size;                                                                                             \
        T const* const arr;                                                                                            \
    } ArrIter(T)
```
The macro will define the `ArrIter` struct based on the given array element type. It'll also use that type name to name the struct itself. You can later refer to this struct using `ArrIter(T)`. Let's define one for `int`-
```c
DefineArrIterOf(int);
```

Now, we need to implement `Iterator` for `ArrIter(T)`. To do that, we use the [`impl_iterator`](https://TotallyNotChase.github.io/c-iterators/iterator_8h.html#a01e620430099d2eb6192db193e88cff0) macro provided by `iterator.h`. But before that, we need the `next` function that can work on `ArrIter`-

```c
static Maybe(int) intarrnxt(ArrIter(int) * self)
{
    int const* const arr = self->arr;
    return self->i < self->size ? Just(arr[self->i++], int) : Nothing(int);
}
```

To implement `Iterator` for a type `ItrbleType`, that yields elements of type `T` - you need a `next` function of the signature- `Maybe(T) (*)(ItrbleType self)`. Here, `ItrbleType` is `ArrIter(int)*` and `T` is `int`.

Now that we have the `next` implementation for this struct, we can implement the typeclass.

```c
impl_iterator(ArrIter(int)*, int, prep_intarr_itr, intarrnxt)
```

This defines a function named `prep_intarr_itr` (feel free to name it whatever you want), that takes in a `ArrIter(int)*` and returns an `Iterable`. Any function can now take in this `Iterable` and iterate through it using the same interface without caring about the internals.

You now need to include the declaration of `prep_intarr_itr` in a corresponding header file, so it's exposed to the outside world. You can also make this handy macro-

```c
#define arr_into_iter(srcarr, sz, T)                                                                                   \
    prep_##T ##arr_itr(&(ArrIter(T)) { .i = 0, .size = sz, .arr = srcarr })
```

This will take in an array (`src`), its size (`sz`), and its element type name (`T`) - and call the prep function, automatically creating and passing in the `ArrIter` struct. Note that this **heavily** relies on consistent naming of the function previously defined by `impl_iterator`. This assumes that such functions will be named `prep_:T:arr_iter`, so for `T = int`, the function should be named `prep_intarr_itr`. You can maintain this convention strictly by having another macro to name the function during definition-

```c
#define prep_arriter_of(T) prep_##T ##arr_itr
```

And use this name during implementing-
```c
impl_iterator(ArrIter(int)*, int, prep_arriter_of(int), intarrnxt)
```

And also when referring to it-
```c
#define arr_into_iter(srcarr, sz, T)                                                                                   \
    prep_arriter_of(T)(&(ArrIter(T)) { .i = 0, .size = sz, .arr = srcarr })
```

Consistency is key to safety!
### For Linked Lists
Now, we'll implement `Iterator` for a singly linked list. You can find the code for the implementation part in [list_iterable.h](./examples/list_iterable.h) and [list_iterable.c](./examples/list_iterable.c).

Just so we're on the same page, the linked list impl looks like-
```c
typedef struct int_node
{
    int val;
    struct int_node* next;
} IntNode, *IntList;

typedef IntNode const* ConstIntList;
```
It has the following functions to work with it-
```c
/* Create and prepend an IntNode to given IntList and return the new list */
IntList prepend_intnode(int val, IntList list);
/* Print the given int list */
void print_intlist(ConstIntList head);
/* Free the given IntList */
IntList free_intlist(IntList head);
```

Just like we did for array, we will need a struct to keep track of the state of iteration for the list. We'll call this `ListIter`-
```c
#define ListIter(T) T##ListIter

#define DefineListIterOf(T)                                                                                            \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        T curr;                                                                                                        \
    } ListIter(T)
```
This is much the same construct as an `ArrIter`. We have a `ListIter` for a particular type of list. The `T` here refers to the type of the list struct itself. Let's define a `ListIter` for our linked list built using `IntNode`s. We'll use a `IntNode const*` - to make sure the iterable has no way to modify the actual contents of the node. Since `T` has to be alphanumeric - we typedef-ed it into `ConstIntList`-
```c
DefineListIterOf(ConstIntList);
```
Now, we will implement `Iterator` for `ListIter(T)`. Just like last time, we use [`impl_iterator`](https://TotallyNotChase.github.io/c-iterators/iterator_8h.html#a01e620430099d2eb6192db193e88cff0). Here's the `next` function we'll use for our `ListIter(ConstIntList)` impl-
```c
static Maybe(int) intlistnxt(ListIter(ConstIntList) * self)
{
    IntNode const* node = self->curr;
    if (node == Nil) {
        return Nothing(int);
    }
    self->curr = node->next;
    return Just(node->val, int);
}
```
Note: `Nil` is an alias to `NULL`.

Now that we have the `next` implementation for this struct, we can implement the typeclass-
```c
impl_iterator(ListIter(ConstIntList) *, int, prep_listiter_of(ConstIntList), intlistnxt)
```
Where `prep_listiter_of` is a similar construct to the previously encountered `prep_arriter_of`, defined as a macro-
```c
#define prep_listiter_of(T) prep_##T ##_itr
```

Finally, we can have a nice helper macro to convert a list to an iterable-
```c
#define list_into_iter(head, T) prep_listiter_of(T)(&(ListIter(T)){.curr = head})
```

## Examples
* [Using an array's iterator instance](./examples/arr_to_iterble.c)
* [Using a list's iterator instance](./examples/list_to_iterble.c)
* [Using an iterable to build a list](./examples/list_from_arr.c)
* [Using an iterator to represent the infinite fibonacci sequence](./examples/fibbonacci.c)
* [Mapping over an iterable](./examples/map_over.c)

# Things to keep in mind
* Mutation is inherent to iterators. During every iteration, the state of the structure backing up the iterable is altered. Once an iterator has been fully consumed, it can no longer be iterated over - it'll just keep returning `Nothing`. You may already be used to this behavior if you're using a non-pure language with built in iterators though.
* If you're making a custom iterable utility that is backed up by another iterable (see [`map`](./examples/iterutils/map.h), [`take`](./examples/iterutils/take.h)) - the source iterable is **also consumed** when you iterate over the wrapper. This is demonstrated, and taken advantage of, in the [fibonacci example](./examples/fibbonacci.c).
* Be very careful about lifetimes when you're using the very convenient macros showcased in the examples! All the macros that create and return an `Iterable` **take the address** of a [*compound literal*](https://en.cppreference.com/w/c/language/compound_literal). Compound literals are local to the scope and hence the address is valid for the [lifetime of that scope](https://stackoverflow.com/questions/34880638/compound-literal-lifetime-and-if-blocks). Don't use the `Iterable` outside that scope. As a rule of thumb, **always** declare and initialize the `Iterable`s in the same line (using the macros).
* If you **need to return** an `Iterable` from a function - you should make sure its `self` member's lifetime doesn't end upon returning. Since `self` is a pointer, the data it is pointing to may have any storage duration. If you're responsible for filling this `self` member - make sure you pay attention to its lifetime.
  
  As mentioned previously, the utility macros, used in the examples to build `Iterable`s, use compound literals - whose lifetimes end once the enclosing scope ends. `Iterable`s built in this way are **not suitable** to be returned (or used) outside of their enclosing scope.
* The `tc` member of the typeclass contains a pointer to a struct with `static` storage duration - so this pointer is totally reusable in any scope.

# Semantics
## `maybe.h`
The `Maybe` struct is quite simple. This is what it *generally* looks like (without a concrete type)-
```c
typedef enum
{
    MaybeTag_Nothing = 0, /**< `Nothing` tag - indicates absence of a value. */
    MaybeTag_Just         /**< `Just` tag - indicates presence of a value. */
} MaybeTag;

struct maybe_t
{
    MaybeTag tag;
    /* Don't access this member manually */
    T val;
};  
```
It's a tagged "union", nothing special. The `Just` tag is used to indicate the presence of a value in `val`, and `Nothing` is used to indicate the absence of a value in `val`.

The `DefineMaybe` macro is used to define an actual concrete `Maybe` struct for the given type, the type is also used to name the `Maybe` struct.
```c
#define Maybe(T) Maybe##T
```
The name of the `Maybe` struct containing a value of type `T` is just `Maybe` and the type name concatenated together. *This is why* `T` **must be alphanumeric**.

There's one more thing `DefineMaybe` does, it defines a `static inline` function-
```c
static inline T T_from_just(Maybe(T) maybex)
{
    if (is_just(maybex)) {
        return maybex.val;
    } else {
        fputs("Attempted to extract Just value from Nothing", stderr);
        abort();
    }
}
```
This is the function the `from_just` macro calls. The function ensures type safety - but most importantly, it ensures that referring to `maybex` doesn't execute it multiple times. In case the user passes an expression with side effects (that returns a `Maybe(T)`) to `from_just`.

So, a call like `DefineMaybe(int)` translates to-
```c
typedef struct
{
    MaybeTag tag;
    /* Don't access this member manually */
    int val;
} Maybeint;

static inline int int_from_just(Maybeint maybex)
{
    if (is_just(maybex)) {
        return maybex.val;
    } else {
        fputs("Attempted to extract Just value from Nothing", stderr);
        abort();
    }
}
```

Alongside the utilities to define and refer to a `Maybe` of a certain type - there's also type constructors and value extractors, as well as macros to check if the `Maybe` is a `Just` or `Nothing`.

* `Just` takes in a value, and the type of said value (alphanumeric, same one used during `Maybe` definition) and constructs a `Maybe(T)`. This is "type safe", `v` must actually be of type `T`, otherwise there will be, at worst a warning, and at best an explicit error.

  `Just(1, int)` translates to `((Maybe(int)){.tag = MaybeTag_Just, .val = (1)})`. Just the compound literal to build the struct. Type safety comes from the fact that the `val` member for `Maybe(int)` is of type `int`.
* `Nothing` takes in a type (alphanumeric, same one used during `Maybe` definition) and constructs a `Maybe(T)` tagged with `Nothing`.

  `Nothing(int)` translates to `((Maybe(int)){0})`. I decided to zero initialize the struct since I've set the `Nothing` tag to `0` explicitly. However, it'd be totally valid to only set the tag to `Nothing` and leave the `val` member indeterminate. Since you shouldn't access `val` if tag is `Nothing` anyway.
* `from_just` and `from_just_` have previously been mentioned briefly.
  
  `from_just` takes in a `Maybe` struct, and the `T` (type the `Maybe` contains) and calls the `T##_from_just` function above. The function checks if the `Maybe` is indeed `Just`, and returns the value. If it is `Nothing`, however, the program aborts.

  `from_just_` directly accesses and returns the `val` member of the given `Maybe` struct, it does not take in the `T` parameter, since it doesn't need to. Only use this after you've made sure the `Maybe` struct is a `Just`. Otherwise the behavior is undefined. Though in practical terms, if the `Maybe` struct was built using the `Nothing` macro, `val` would just be zero initialized. This should not be relied on however.
* `is_just` and `is_nothing` are self explanatory, they compare the `tag` to `MaybeTag_Just` and `MaybeTag_Nothing` respectively.

## `typeclass.h`
This file provides utility macros to define a typeclass and its instance.

A typeclass is just a struct containing a bunch of function pointers.

A typeclass instance contains a pointer to the typeclass struct in its `tc` member. It also contains the `self` member of type `void*`. This is the concrete type that will be used by functions.

* The `typeclass` macro takes in a semicolon separated list of function pointer members and puts them into a struct. It doesn't name the struct, that's upto the caller.

  ```c
  typedef typeclass(
      size_t (*const from_enum)(void* self);
      void* (*const to_enum)(size_t x)
  ) Enum;
  ```
  translates to
  ```c
  typedef struct
  {
      size_t (*const from_enum)(void* self);
      void* (*const to_enum)(size_t x);
  } Enum;
  ```
* The `typeclass_instance` macro takes in the type name of the previously defined typeclass and defines the typeclass instance struct. It also doesn't name the struct.
  ```c
  typedef typeclass_instance(Enum) Enumerable;
  ```
  translates to
  ```c
  typedef struct
  {
      void* self;
      Enum const* tc;
  } Enumerable;
  ```

## `iterator.h`
This uses the typeclass utilities mentioned previously to define the `Iterator` typeclass and its instance, `Iterable`. The naming of each of these defined structs is dependent on the type of the element the iterator yields.
```c
#define Iterator(T) T##Iterator
#define Iterable(T) T##Iterable
```

The `DefineIteratorOf` macro takes in the typename of the element the `Iterator` will yield, and uses that to define the iterator and iterable with appropriate names. This is why the type names must be alphanumeric.
```c
DefineIteratorOf(int);
```
translates to
```c
typedef typeclass(Maybe(int) (*const next)(void* self)) intIterator;
typedef typeclass_instance(Iterator(int)) intIterable;
```
Two structs, of names `Iterator(int)` (i.e `intIterator`) and `Iterable(int)` (i.e `intIterable`), respectively.

Now, we need a function to implement `Iterator` for our own type. That's where the `impl_iterator` macro comes in. This is its signature-
```c
#define impl_iterator(IterType, ElmntType, Name, next_f) ...
```
It defines a function, which turns the custom type (for which the impl is for) into an `Iterable`.

It takes in-
* `IterType`, the custom type `Iterator` is being implemented for
* `ElmntType`, the type this `Iterator` will yield.
  
  An `Iterator(ElmntType)` (and `Iterable(ElmntType)`) should already exist, obviously.

  Must be alphanumeric just like everywhere else.

  A `Maybe(ElmntType)` must also exist.
* `Name` to give to the function being defined.
* `next_f`, the next function implementation for this `IterType`.

  Must be of type `Maybe(ElmntType) (*)(IterType)`. It should take in a value of `IterType`, and return a `Maybe(ElmntType)`.

Generally, you need to include the declaration of this function in a header file yourself. However, you can also mark this function as `static` if you so desire; all you have to do, is prepend `static` to the `impl_iterator` call- `static impl_iterator(...)`.

In the [array_iterable.c](./examples/array_iterable.c) example. The `impl_iterator(ArrIter(int)*, int, prep_arriter_of(int), intarrnxt)` translates to-
```c
Iterable(int) prep_intarr_itr(ArrIter(int)* x)
{
    Maybe(int) (*const next_)(ArrIter(int)* self) = (intarrnxt);
    (void)next_;
    static Iterator(int) const tc = {.next = (Maybe(int) (*const)(void*))intarrnxt};
    return (Iterable(int)){.tc = &tc, .self = x};
}
```
Note the first 2 lines - those make sure the passed `next` function impl has the exact correct type (no `void*` shenanigans). The 2 lines will not generate any extra code when compiled with a decent compiler - they are just there to present warnings/errors when the type of the passed function is implicitly wrong.

# Advanced Usage
## Lazy Abstractions
This lazy iterator interface demonstrated, lets you achieve strong abstractions that are also lazy. 2 such abstractions, `take` and `map` are demonstrated in [fibonacci.c](./examples/fibbonacci.c) and [map_over.c](./examples/map_over.c) respectively.

Many of these abstractions follow the same basic pattern. Have a custom struct that wraps a given iterable - have some extra context in that struct to operate on the elements of said iterable, and a `next` function implementation that simply iterates through the src iterable and does some necessary action based on the stored context. No extra iteration is done, the extra functionality is simply applied on top of the source iterable's `next` function on demand.

This allows `take` to simply wrap the given iterable inside a struct with some context and turn that struct into its iterable implementation. No iteration needs to happen in this process, it's a completely lazy process.

### The `take` utility
Let's look at how the `take`-like utility is implemented. You can find the implementation in [iterutils](./examples/iterutils/take.h).

The struct we'll use to implement this utility looks like-
```c
struct
{ 
    size_t i;
    size_t const limit;
    Iterable(T) const src;
}
```
It stores the source iterable (`src`), the iteration step (`i`), and the limit to stop the iteration at (`limit`). We'll call this struct `IterTake`, just like `Iterator` and `Iterable`, the exact name will depend on the element type.
```c
#define IterTake(ElmntType) IterTake##ElmntType

#define DefineIterTake(ElmntType)                                                                                      \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        size_t i;                                                                                                      \
        size_t const limit;                                                                                            \
        Iterable(ElmntType) const src;                                                                                 \
    } IterTake(ElmntType)
```
Now we need the `next` function impl to implement `Iterator` for this `IterTake` struct. That function should look like-
```c
static Maybe(T) IterTake(T)_nxt(IterTake(T) * self)
{
    if (self->i < self->limit) {
        ++(self->i);
        Iterable(T) srcit = self->src;                                                                     
        return srcit.tc->next(srcit.self);
    }
    return Nothing(T);
}
```
It simply iterates through the source iterable but stops if it reaches the limit specified in the `IterTake` struct. Now, `impl_iterator` can be used to implement `Iterator` for the `IterTake(T)`-
```c
impl_iterator(IterTake(T)*, T, prep_IterTake(T)_itr, IterTake(T)_nxt)
```

And that's it! Now an `IterTake` can be converted into an `Iterable`. But how about we also make an abstraction to turn a given `Iterable` into another `Iterable` with the `IterTake` applied directly?
```c
#define take_from(it, n, T) prep_IterTake(T)_itr(&(IterTake(T)){.i = 0, .limit = n, .src = it})
```
`take_from` can be used to take `n` elements of type `T` from an iterable, `it`.

You'll notice that in the previous snippets, the pre processor token concatenation for the function names isn't going to quite work - that was just for simplification, in reality `IterTake(T)_nxt` is written as `CONCAT(IterTake(T), _nxt)` where `CONCAT` is-
```c
#define CONCAT_(A, B) A##B
#define CONCAT(A, B)  CONCAT_(A, B)
```

While we're at it, let's also make a macro to name the `prep_` function consistently, just like we did for arrays and lists-
```c
/* Name of the function that wraps an IterTake(ElmntType) for given ElmntType into an iterable  */
#define prep_itertake_of(ElmntType) CONCAT(CONCAT(prep_, IterTake(ElmntType)), _itr)
```

We can use that in the `take_from` macro instead, as well as in the function name in the `impl_iterator` macro.

Combining all of that, gives you-
```c
/*
Define the iterator implementation function for an IterTake struct

The function is named `prep_itertake_of(ElmntType)`
*/
#define define_itertake_func(ElmntType)                                                                                \
    static Maybe(ElmntType) CONCAT(IterTake(ElmntType), _nxt)(IterTake(ElmntType) * self)                              \
    {                                                                                                                  \
        if (self->i < self->limit) {                                                                                   \
            ++(self->i);                                                                                               \
            Iterable(ElmntType) srcit = self->src;                                                                     \
            return srcit.tc->next(srcit.self);                                                                         \
        }                                                                                                              \
        return Nothing(ElmntType);                                                                                     \
    }                                                                                                                  \
    impl_iterator(IterTake(ElmntType)*, ElmntType, prep_itertake_of(ElmntType), CONCAT(IterTake(ElmntType), _nxt))
```

You need to call this macro with a concrete type (a type for which an `Iterator`, and `Iterable`, already exist) inside a C source file. In the examples, this macro is called inside [iterable_utils.c](./examples/iterutils/iterable_utils.c) - for defining `IterTake` for a couple of types. The declrations of the `prep_` function is then included in the [iterable_utils.h](./examples/iterutils/iterable_utils.h) header file.

That's the implementation, let's use it now!
```c
Iterable(uint32_t) it   = get_fibitr();                /* Create an infinite fibonacci sequence iterable */
Iterable(uint32_t) it10 = take_from(it, 10, uint32_t); /* Iterable of the first 10 items in the sequence */
/* Print the first 10 items */
foreach (uint32_t, n, it10) {
    printf("%" PRIu32 " ", n);
}
puts("");
```
You can find this code in [fibonacci.c](./examples/fibbonacci.c). `get_fibitr` simply returns an infinite `Iterable(uint32_t)` representing the fibonacci sequence. `take` shines in its ability to operate on infinite iterators. In the above snippet, we take the first 10 elements of `it` (the fibonacci sequence) and store the built iterable in `it10`.

Since this is a lazy process - `it10` is actually just backed by `it`. When you consume from `it10`, `it` is also being consumed from.

This means that you can call `take_from` on `it` again and try to get 10 more items, and this time you'll get *the next 10 items*. Cool!

If the `Iterable` is implemented correctly (i.e the `next` function behaves as expected), Using `take_from` on an already consumed `Iterable` does not have any traps - it simply gives you back an already consumed `Iterable`.

*Even* if you use `take_from` with a `n` value **greater** than *the length* of the source `Iterable`, the returned `Iterable` will simply be fully consumed before reaching `n` - no surprises.

### The `map` utility
The `map` implementation will follow a very similar pattern. We need a struct to hold the mapping function, a `next` function impl to use that mapping function while iterating over the source iterable, and the `Iterator` implementation for this struct.

However, we now need to think about the mapping function's type. It's argument should be the same type as that of the source iterable's elements - but what about its return type? The return type can be anything. So each one of these special map structs, need to be parametrize on not only the element type, but also the return type of the mapping function.
```c
#define IterMap(ElmntType, FnRetType) IterMap##ElmntType##FnRetType

#define DefineIterMap(ElmntType, FnRetType)                                                                            \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        FnRetType (*const mapfn)(ElmntType x);                                                                         \
        Iterable(ElmntType) const src;                                                                                 \
    } IterMap(ElmntType, FnRetType)
```
As usual, type safety is a priority.

The `next` function impl should look like-
```c
static Maybe(FnRetType) CONCAT(IterMap(ElmntType, FnRetType), _nxt)(IterMap(ElmntType, FnRetType) * self)
{
    Iterable(ElmntType) const srcit = self->src;
    Maybe(ElmntType) res            = srcit.tc->next(srcit.self);
    if (is_nothing(res)) {
        return Nothing(FnRetType);
    }
    return Just(self->mapfn(from_just_(res)), FnRetType);
}
```
It simply consumes the source iterable, applies the mapping function on each element and returns the result.

Now, we can use `impl_iterator`-
```c
/* Name of the function that wraps an IterMap(ElmntType, FnRetType) for given ElmntType and FnRetType into an iterable */
#define prep_itermap_of(ElmntType, FnRetType) CONCAT(CONCAT(prep_, IterMap(ElmntType, FnRetType)), _itr)

impl_iterator(IterMap(ElmntType, FnRetType)*, FnRetType, prep_itermap_of(ElmntType, FnRetType), CONCAT(IterMap(ElmntType, FnRetType), _nxt))
```

Combine all of that together, and you get a similar macro as last time-
```c
/*
Define the iterator implementation function for an IterMap struct
Also define a function with the given `Name` - which takes in an iterable and a function to map over said iterable,
wraps said iterable and function in an `IterMap` struct and wraps that around its `Iterable` impl
*/
#define define_itermap_func(ElmntType, FnRetType)                                                                      \
    static Maybe(FnRetType) CONCAT(IterMap(ElmntType, FnRetType), _nxt)(IterMap(ElmntType, FnRetType) * self)          \
    {                                                                                                                  \
        Iterable(ElmntType) const srcit = self->src;                                                                   \
        Maybe(ElmntType) res            = srcit.tc->next(srcit.self);                                                  \
        if (is_nothing(res)) {                                                                                         \
            return Nothing(FnRetType);                                                                                 \
        }                                                                                                              \
        return Just(self->mapfn(from_just_(res)), FnRetType);                                                          \
    }                                                                                                                  \
    impl_iterator(IterMap(ElmntType, FnRetType)*, FnRetType, prep_itermap_of(ElmntType, FnRetType),                    \
                  CONCAT(IterMap(ElmntType, FnRetType), _nxt))
```

The key difference is that this works on `ElmntType` and `FnRetType`, as opposed to just `ElmntType`. You can call this macro to define the functions in a source file, and include the declarations in a header file. In the examples, this is done in [iterable_utils.c](./examples/iterutils/iterable_utils.c) and [iterable_utils.h](./examples/iterutils/iterable_utils.h) respectively.

We should also have a convenient macro in the same style as `take_from`-
```c
/* Map the function `fn` of type `FnRetType (*)(ElmntType)` over `it` to make a new iterable */
#define map_over(it, fn, ElmntType, FnRetType)                                                                         \
    prep_itermap_of(ElmntType, FnRetType)(&(IterMap(ElmntType, FnRetType)){.mapfn = fn, .src = it})
```

And that's all there is to implementing, let's use it!
```c
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
```
where `incr` is-
```c
static int incr(int x) { return x + 1; }
```

You can find this code in [map_over.c](./examples/map_over.c). The above snippet maps the `incr` function over the `Iterable(int)`. Once again, this is a lazy process - no iteration is done by `map_over`. The iteration, as well as the mapping function application, is only done in the `foreach`.

### A quick glance at implementing `filter`
Implementing `filter` would also be just as simple as the previous examples. Though no concrete implementation is included in this repo, the pattern is really the exact same. Here's what the `next` function impl would be-
```c
static Maybe(ElmntType) CONCAT(IterFilter(ElmntType), _nxt)(IterFilter(ElmntType) * self)
{
    Iterable(ElmntType) const srcit = self->src;
    while (1) {
        Maybe(ElmntType) res = srcit.tc->next(srcit.self);
        if (is_nothing(res) || self->filterfn(from_just_(res))) {
            return res;
        }
    }
}
```
where `IterFilter(T)` would be-
```c
struct
{
    bool (*const filterfn)(T x);
    Iterable(T) const src;
}
```

## Iterable of Generic Elements
In the beginning of this README, while introducing this `Iterator` interface, I talked about how an `Iterator` is only generic on the *input* side, not on the *output* side. The element the `Iterator` yields must be a concrete type - which separates `Iterator(int)` and `Iterator(string)`, and forbids you from using them interchangably.

But what if you wanted generic elements without giving up type safety? Well, you'd need some sort of constraint - I **should** be able to sum both an `Iterable(int)` and an `Iterable(float)`. Ignoring the bounds issues, it *is* agreeable that there are multiple types that *can* perform a **general action**.

Emphasis on "general action". Typeclasses (or interfaces) allow you to have a generic constraint based around the ability to do some action(s). What if, we had an `Iterator` that yielded typeclass instances?

```c
typedef typeclass(char* (*const show)(void* self)) Show;
typedef typeclass_instance(Show) Showable;
```
This is the [`Show`](https://hackage.haskell.org/package/base-4.15.0.0/docs/Text-Show.html#t:Show) typeclass. It represents the ability of a type to be converted into a string (that may then be printed).

If we had an `Iterator(Showable)` - we could turn each element into the strings representing them, doesn't really matter what type the actual data is, as long as it implements `Show`. In the same way, you could have a [`Num`](https://hackage.haskell.org/package/base-4.15.0.0/docs/Prelude.html#t:Num) typeclass for arithmetic operations.

The pattern for defining and implementing such typeclasses in a type safe way, is the same as the pattern used to define and implement `Iterator`. The typeclass and typeclass_instance struct, and an `impl_iterator` macro that takes in some necessary info about the type for which the typeclass is being implemented, as well as the function implementations, type checks the function impls as a no-op, and returns the typeclass instance.

You can find examples for this usage, as well as more info in the [Typeclass Pattern](./Typeclass%20Pattern.md) document.

# Motivation
I needed some way to implement generic interfaces for a C library I'm working on. The library functions just needed a type that could *do* a certain thing, without caring about which exact type it could be backed by. It'd be the library function's responsibility to make the type do the action *if* **and** *when* needed.

Of course, I could segment a good bunch of these functions so one part of them do everything up until the point where the generic action needs to be performed, return control to the user, and let them continue to the next part of the library function by performing the action on their concrete type themselves and passing the result to the next part of the function. But this felt rather unintuitive, especially when the functions were already pretty small and performed extra computation which would be stored in the local scope.

I instead made the functions so that they take in a struct containing a `void*` for the generic type and a function pointer for the required action, of the correct type. The library function wouldn't use this `void*` directly (since that'd be unsafe) - but it could just pass it to the function pointer and call it.

After some experimenting with that approach to make it more extensible and general, with a heavy dosage of inspiration from haskell (and also from rust) - this is where it ended up. An `Iterator` isn't the only typeclass I needed to implement, but it had the most potential for a demonstration - so here it is.
