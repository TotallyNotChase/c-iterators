# The Typeclass Pattern
This document aims to describe the typeclass (or trait, or interface) based polymorphism pattern used in this demonstration.

*Note: An extended and updated version of this document can be found at [typeclass-interface-pattern](https://github.com/TotallyNotChase/typeclass-interface-pattern).*

# Goals
Alongside describing the core parts of this pattern, this document will also describe how to **combine multiple typeclass constraints into one constraint**. As in, how you can have a type that is required to implement multiple typeclasses.

# Brief
Function pointer based polymorphism isn't new to C by any means. The major difference in the typeclass pattern, and the typical vtable based approach is simply that typeclasses are based around actions, rather than objects. This is very similar to an interface in OOP terms.

# Core parts
There are 3 core parts to this pattern. These parts will be demonstrated by implementing the [`Show`](https://hackage.haskell.org/package/base-4.15.0.0/docs/Text-Show.html#t:Show) typeclass.

## The `typeclass` struct definition
This is the struct containing the function pointers related to the typeclass. For `Show`, we'll just be using the `show` function here, it takes in a value of the type for which `Show` is implemented (i.e `self`) and returns a printable string.
```c
typedef struct
{
    char* (*const show)(void* self);
} Show;
```
This can be simplified using the [`typeclass`](https://totallynotchase.github.io/c-iterators/typeclass_8h.html#ab36f9f0d3603452a867a683078618034) macro provided in [typeclass.h](./typeclass.h).
```c
typedef typeclass(char* (*const show)(void* self)) Show;
```

A simple struct containing the virtual function(s). When the wrapper function is first called (to convert a certain type to its typeclass instance), a typeclass struct of `static` storage duration is created with the function pointers for that specific type (a vtable of sorts). The pointer to this struct is then used in all typeclass instances. More on this will be discussed in the `impl_` macro part.

## The `typeclass_instance` struct definition
This is the concrete instance to be used as a type constraint. It should contain a pointer to the typeclass, and the `self` member containing the value to pass to the functions in the typeclass struct.
```c
typedef struct
{
    void* self;
    Show const* tc;
} Showable;
```
This can also be simplified using the [`typeclass_instance`](https://totallynotchase.github.io/c-iterators/typeclass_8h.html#af018200b2431a3ab6c296cc8940ecbe3) macro provided in [typeclass.h](./typeclass.h).
```c
typedef typeclass_instance(Show) Showable;
```

## The `impl_` macro used to implement the typeclass
This macro is the real heavy lifter when it comes to type safety.

It takes in some information about the type you're implementing a typeclass for, and the exact function implementations that will be used for that type, and defines a function which does the following-
* Takes in an argument of the type the implementation is for.
* Type checks the function implementations given.
  
  This is done by storing the given function implementations into function pointers of an exact and expected type.
* Initializes the typeclass struct to store these function pointers, with static storage duration.
* Creates and returns the typeclass instance, which stores a pointer to the aforementioned typeclass struct, and the function argument into the `self` member.

Following these rules, this is what `impl_show` would look like-
```c
#define impl_show(T, Name, show_f)                                                                                     \
    Showable Name(T x)                                                                                                 \
    {                                                                                                                  \
        char* (*const show_)(T e) = (show_f);                                                                          \
        (void)show_;                                                                                                   \
        static Show const tc = {.show = (char* (*const)(void*))(show_f) };                                             \
        return (Showable){.tc = &tc, .self = x};                                                                       \
    }
```
It takes the `show` implementation as its third argument. In the function definition, it stores that impl in a variable of type `char* (*const show_)(T e)`, which is the exact type it should be - `T` is the specific type the implementation is for. It **must be a pointer type**. Since it's stored into `void* self`.

The `(void)show_;` line is to suppress the unused variable warning emitted by compilers, since `show_` isn't actually used. It's only there for typechecking purposes. These 2 typechecking lines will be completely eliminated by any decent compiler.

Then it simply defines a static typeclass and stores the function pointer inside. Then it creates and returns the `Showable` struct, containing the `x` argument, and a pointer to the typeclass struct.

# Usage
Once the typeclass and typeclass instance structs have been defined, all the user has to do is call the `impl_` macro with their own type and the function implementation(s) required for the typeclass. The declaration of the function defined by said macro can then be included in a header. After that, that function can be used to turn the concrete type into its typeclass instance.

Here's an example of implementing the previously defined `Show` typeclass for an enum-
```c
typedef enum
{
    holy,
    hand,
    grenade
} Antioch;

static inline char* strdup_(char const* x)
{
    char* s = malloc((strlen(x) + 1) * sizeof(*s));
    strcpy(s, x);
    return s;
}

/* The `show` function implementation for `Antioch*` */
static char* antioch_show(Antioch* x)
{
    /*
    Note: The `show` function of a `Showable` is expected to return a malloc'ed value
    The users of a generic `Showable` are expected to `free` the returned pointer from the function `show`.
    */
    switch (*x)
    {
        case holy:
            return strdup_("holy");
        case hand:
            return strdup_("hand");
        case grenade:
            return strdup_("grenade");
        default:
            return strdup_("breakfast cereal");
    }
}

/*
Implement the `Show` typeclass for the type `Antioch*`

This will define a function to convert a value of type `Antioch*` into a `Showable`, the function will be named `prep_antioch_show`

The `show` implementation used will be the `antioch_show` function
*/
impl_show(Antioch*, prep_antioch_show, antioch_show)
```
The `impl_show` macro here, simply translates to-
```c
Showable prep_antioch_show(Antioch* x)
{
    char* (*const show_)(Antioch* e) = (show_f);
    (void)show_;
    static Show const tc = {.show = (char* (*const)(void*)(show_f) };
    return (Showable){.tc = &tc, .self = x};
}
```

Now, you can convert an `Antioch` into a `Showable` like so-
```c
Antioch ant = holy;
Showable antsh = prep_antioch_show(&ant);
```
And this `Showable` will automatically dispatch to the `antioch_show` function whenever someone calls the `show` function inside it.

Now you can make polymorphic functions that works on `Showable`s. Here's one of them-
```c
void print(Showable showable)
{
    char* s = showable.tc->show(showable.self);
    puts(s);
    free(s);
}
```
You can now easily print an `Antioch` with these abstractions-
```c
Antioch ant = holy;
print(prep_antioch_show(&ant));
```
Where this really shines though, is when you have multiple types that implement `Show` - all of them can be used with `print`. Or any other function that works on a generic `Showable`!

# Combining multiple typeclasses
One of the core design goals of a typeclass is to be modular. A `Show` typeclass should only have actions directly related to "showing", a `Num` typeclass should only have actions directly related to numerical operations. Unlike objects, that may contain many different methods of arbitrary relevance to each other.

This means that, more often than not, you'll want a type that can do multiple different classes of actions. A type that implements multiple typeclasses.

You can model that pretty easily with this pattern-
```c
/* Type constraint that requires both `Show` and `Enum` to be implemented */
typedef struct
{
    void* self;
    Show const* showtc;
    Enum const* enumtc;
} ShowableEnumerable;

#define impl_show_enum(T, Name, showimpl, enumimpl)                                                                    \
    ShowableEnumerable Name(T x)                                                                                       \
    {                                                                                                                  \
        Showable showable = showimpl(x);                                                                               \
        Enumerable enumerable = enumimpl(x);                                                                           \
        return (ShowableEnumerable){.showtc = showable.tc, .enumtc = enumerable.tc, .self = x};                        \
    }
```

Where [`Enum`](https://hackage.haskell.org/package/base-4.15.0.0/docs/Prelude.html#t:Enum) is also a typeclass defined like-
```c
typedef typeclass(
    size_t (*const from_enum)(void* self);
    void* (*const to_enum)(size_t x)
) Enum;

typedef typeclass_instance(Enum) Enumerable;

#define impl_enum(T, Name, from_enum_f, to_enum_f)                                                                     \
    Enumerable Name(T x)                                                                                               \
    {                                                                                                                  \
        size_t (*const from_enum_)(T e) = (from_enum_f);                                                               \
        T (*const to_enum_)(size_t x)   = (to_enum_f);                                                                 \
        (void)from_enum_;                                                                                              \
        (void)to_enum_;                                                                                                \
        static Enum const tc = {                                                                                       \
            .from_enum = (size_t (*const)(void*))(from_enum_f), .to_enum = (void* (*const)(size_t x))(to_enum_f)       \
        };                                                                                                             \
        return (Enumerable){.tc = &tc, .self = x};                                                                     \
    }
```

Essentially, you can have a struct that stores each of the typeclass pointers you want to combine, and the `self` member. The `impl` macro would also be very simple. It should simply define a function that puts the given value into `ShowableEnumerable`, into `self`, as well as use the impl functions to get the typeclass instances of that type.

With this, if you implemented `Show` for `Antioch*` and defined the function as `prep_antioch_show`, and also implemented `Enum` with the function name `prep_antioch_enum`, you could call `impl_show_enum` using-
```c
impl_enum(Antioch*, prep_antioch_show_enum, prep_antioch_show, prep_antioch_enum)
```
The defined function would have the signature-
```c
ShowableEnumerable prep_antioch_show_enum(Antioch* x);
```

That's it!

You can now have functions that require their argument to implement multiple typeclasses-
```c
void foo(ShowableEnumerable se)
{
    /* Use the enumerable abilities */
    size_t x = se.enumtc->from_enum(se.self);
    /* Use the showable abilities */
    char* s = se.showtc->show(se.self);
}
```

# Putting it all together into a fully generic `Iterator`
Following the typeclass pattern - you can make an `Iterator` where each element is a typeclass instance. The typeclass instance can either just be for one typeclass (like `Show`), or it can be for multiple typeclasses (like `ShowableEnumerable`).

Here's a full demonstration all in one snippet, turning an array of `Antioch`s into an `Iterable` of `Showable`s-
```c
DefineMaybe(Showable)

DefineIteratorOf(Showable);

static void print(Showable showable)
{
    char* s = showable.tc->show(showable.self);
    puts(s);
    free(s);
}

static void printit(Iterable(Showable) it)
{
    foreach (Showable, x, it) {
        print(x);
    }
}

typedef struct
{
    size_t i;
    size_t const size;
    Antioch* const arr;
} AntiochArrIter;

static Maybe(Showable) antiochshow_arr_nxt(AntiochArrIter* self)
{
    if (self->i >= self->size) {
        return Nothing(Showable);
    }
    /*
    Note: This is re-using the address of the element in the array
    This means that the return value goes out of scope once the src array
    goes out of scope (or is freed).
    */
    return Just(prep_antioch_show(self->arr + self->i++), Showable);
}

static impl_iterator(AntiochArrIter*, Showable, prep_antshowarr_itr, antiochshow_arr_nxt)

int main(void)
{
    Antioch antarr[] = {grenade, holy, hand};
    Iterable(Showable) antshowit =
        prep_antshowarr_itr(&(AntiochArrIter){.i = 0, .size = sizeof(antarr) / sizeof(*antarr), .arr = antarr});
    printit(antshowit);
    return 0;
}
```
Note: `Show` and `Antioch` here have their origins in [Core Parts](#core-parts). You'll remember, the `Show` impl function for `Antioch` has the signature- `Showable prep_antioch_show(Antioch* x);`. The `foreach` macro is defined in [iterutils](./examples/iterutils/iterable_utils.h). It's using the `CONCAT` macro, which is defined in [func_iter.h](./examples/func_iter.h)
