
#  Class 07

##  Aside: CRTP (Curiously Recurring Template Pattern)

This section will not be tested in the class, but it is very interesting to talk about since this technique is used sometimes for optimizing performance. As we have mentioned in the previous class, templates can be used to replace static polymorphisim.  A common way to do this is through CRTP. CRTP is an idiom where a class derives from a class template instantiation using the derived class itself as a template parameter. This maye seem confusing and circular in dependancy (hence the name) but because how templates need very little information about their parameters, their is actually no dependency from the base class to the derived class that is being used as its argument. 



https://github.com/CIS-1901-F24/Notes/blob/ea3ac76409ab3461948881888bb12cea3cbf8baf/07/CRTP.cpp#L7-L36

CRTP allows you define the dog class to inherit from Animal which is templated on dog. When the animal class needs to use the "virtual" function sound, instead of dispatching a virtual call as you would had you been using an abstract class (look at refernce virtual implementation), you can call that function through the Derived template argument and avoid making a virtual function call, which has an overhead.


##  Value Template Arguments

  

In addition to type arguments, templates can also have value arguments.

Using these instead of regular function arguments is sometimes *necessary*, for example for arrays, where the size of the array must be a compile-time constant:

  

https://github.com/CIS-1901-F24/Notes/blob/ea3ac76409ab3461948881888bb12cea3cbf8baf/07/array.cpp#L1-L23

  

This example also has the benefit of allocating an array on the stack instead of the heap, avoiding having to manually manage the array allocated using `new []` if we made the size of the array a regular parameter.

  

Other times, value template arguments are useful for performance, since, as usual, the template will act as a code-generation mechanism.

  

https://github.com/CIS-1901-F24/Notes/blob/ea3ac76409ab3461948881888bb12cea3cbf8baf/07/pow.cpp#L1-L20

  

In this example, we avoid having to pass in `2` as a normal function argument at runtime, instead generating custom code for the case when `base` is `2`.

Doing this may also aid the compiler in doing optimizations, since it sees that the code is just multiplying by `2` inside a loop, and can be replaced with a single bit shift.

  

##  Variadic Templates
In some cases, the fucntions that you will attempt to write will not have a fixed number of arguments or argument types. An example of this is the `printf` function in `C`. You might have noticed that you can pass it virtually any number of types. While this is implemented in `C` using macros, C++ has a more convineint way of doing that using templates. C++ introduces variadic templates which make use of elipsis "`...`" to define any number of template arguments. To implement a variadic function, you will usually have a base case and a recursive implementation. For example, we can define our print function using variadic tempaltes.

https://github.com/CIS-1901-F24/Notes/blob/ea3ac76409ab3461948881888bb12cea3cbf8baf/07/print.cpp#L1-L32

Notice that inside the variadic function, we also use the ellipsis to unpack the parameters. We can also use variadic templates to define classes that take an arbitrary number of template parameters. An example of this is `std::tuple`. `std::tuple` can be defined with any number of types like this `std::tuple<int, std::string, bool> t1; std::tuple<std::string, std::string> t2;` and so on. We will not go into details on how these are implemented since the implementation can be complex.

##  `constexpr`

  

While we talked about compile-time constants above, how exactly do we characterize these constants?

`const` may seem tempting, but it is not sufficient.

`const` is basically a *promise* that you won't change this variable in the future.

For example, we could obtain an `int` from the user, and then copy that into a `const int`:

```c++

int i;

cin >> i;

const  int c {i};

```

`c` is *not* a compile-time constant here, though it is `const`.

  

`constexpr`, standing for *constant expression*, is a stronger notion of immutability than `const`.

A variable that is declared `constexpr` means that it will be known as compile time, and is suitable for use as a template argument:

  

https://github.com/CIS-1901-F24/Notes/blob/ea3ac76409ab3461948881888bb12cea3cbf8baf/07/constexpr1.cpp#L1-L26

  

`constexpr` variables are also `const` automatically, and are useful for declaring constants, similar to how `#define` is used in C.

While this is useful, the real power of `constexpr` is the ability to define `constexpr`  *functions*, which will force computation to happen at compile time.

  

A function can be declared as `constexpr`, which signals to the compiler that this function should be run at compile-time.

There are some restrictions on the function for it to be `constexpr`, such as that it can only use local variables (no globals!) and cannot have other side effects.

These restrictions are similar to pure functions in functional programming and are detailed [here](https://en.cppreference.com/w/cpp/language/constexpr).

  

https://github.com/CIS-1901-F24/Notes/blob/ea3ac76409ab3461948881888bb12cea3cbf8baf/07/constexpr2.cpp#L1-L12

  

A `constexpr` function can also be run on non-`constexpr` arguments, in which case it will run normally (at runtime).

When a `constexpr` function is run on `constexpr` arguments, then the output is `constexpr` as well, and this is all computed at compile-time.

This example runs the `constexpr` function `factorial` on a `constexpr`, `12`, and computes the factorial of `12` at compile-time.

To verify this, you can run `g++ -s -S constexpr2.cpp` to see the assembly output of the compiler in `constexpr2.s`.

`-s` removes debugging information, and `-S` generates assembly instead of an executable.

You can see that the answer is precomputed and stored in the assembly, and not computed on demand by the program.



##  Template Metaprogramming

  

*Metaprogramming* is where programs can themselves deal with programs as data.

Templates are already an instance of metaprogramming---they generate code for you.

  

The term *template metaprogramming* in C++ typically refers to more complex usage of templates: using templates to do compile-time compilation, similar to `constexpr` functions.

This style is actually Turing-complete[^1], and was not the original intent of the C++ designers.

Basically, it is possible to use class templates as a functional programming language, where template specializations are the base cases, and values are stored as `static` class variables:

  

https://github.com/CIS-1901-F24/Notes/blob/ea3ac76409ab3461948881888bb12cea3cbf8baf/07/fib.cpp#L1-L24

  

In this example, we define a class template for calculating the `N`th Fibonnaci number.

It is essential that we store the value of interest in a `static` variable, rather than in a member function.

If we instead defined a `int value()` member function, then `fib<3>::value()` would expand to `fib<2>::value() + fib<1>::value()`, which would result in these functions being called at runtime, defeating the purpose of template metaprogramming.

By using a `static` variable, `fib<3>::value` expands out to `fib<2>::value + fib<1>::value`, which is `1 + 1`, and the compiler simplifies that to the answer, `2`.

  

Note that for template specializations, the general case must come first, with specializations following it.

  

As a second example, we will consider the [Collatz conjecture](https://en.wikipedia.org/wiki/Collatz_conjecture), which has to do with sequences of positive integers.

When the number `n` is even, the next number in the sequence is `n / 2`.

When the number `n` is odd, then the next number is `3n + 1`.

Empirically, using all numbers anyone has tried to start from, the sequence goes to `1` eventually.

But there is no mathematical proof that the sequence will always reach `1` from any starting number.

  

This example will focus on counting how many steps it takes to reach 1, starting from an arbitrary starting number:

  

https://github.com/CIS-1901-F24/Notes/blob/ea3ac76409ab3461948881888bb12cea3cbf8baf/07/collatz.cpp#L1-L54

  

This program uses a helper class template, `CollatzHelper`, to use a second template parameter to keep track of whether the first parameter is even or odd.

It is capable of doing quite a lot of work at compile-time, though this program may not compile on your machine, due to the recursion being too deep.

Using GCC, the error message mentions the `ftemplate-depth` flag to change the depth of template instantiations, and increasing it to `1000` successfully computes the number of steps for all of the examples in `main`.

  

Compared to `constexpr` functions, template metaprogramming is far more cumbersome, though occasionally it can be more expressive.

For example, templates can deal with any type, whereas `constexpr` can only deal with simple types.

`constexpr` is also a newer feature than templates, so you may see more instances of template metaprogramming in older code.

  

[^1]: Veldhuizen, T. L. (2003). C++ templates are turing complete.