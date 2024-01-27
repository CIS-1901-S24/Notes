# Class 3

## Classes

The `struct`s that you are familiar with from C are classes in C++.
Whereas C they were special "`struct` types" and had to be used with the `struct keyword` (e.g. `struct point p`), in C++ they are first class types, and do not require a special keyword (e.g. `point p`).

https://github.com/CIS-1901-F24/Notes/blob/c6d2bac48e123afba17050fec0d19c704f508574/03/point1.cpp#L1-L22

As an aside, in this code we use the `cmath` header for `sqrt`.
This is just the C `math.h` header, but converted to C++ conventions, with improvements like namespaces and more precise types.
All C headers are provided in in this way, by prepending "c" and removing the file extension.

Creating an object in C++ is similar to initializing a built-in type.
You would only use `new` if you wanted to dynamically allocate an object and obtain a pointer to it, just like with other types.

Within member functions of the class, we have access to the `this` keyword, which is a pointer to the object that the member function was invoked on.
Note that this is a pointer rather than a reference, so using it requires us to do `this->x`, not `this.x`.

### Access Modifiers

C++ has the familiar access modifiers of private (only accessible within the class), protected (only accessible within the class and descendant classes), and public (accessible to all).
Rather than writing the access specifiers for each member of the class like in Java, you can set the accessibility of all *following* members using things like `public:`.
With a `class`, members are private by default and with a `struct`, members are public by default.
This is the *only* difference between `class` and `struct`.

https://github.com/CIS-1901-F24/Notes/blob/c6d2bac48e123afba17050fec0d19c704f508574/03/point2.cpp#L1-L27

### Constructors

Like many other languages, C++ has constructors to initialize objects.
A *default constructor* is a constructor with no arguments.
This is the constructor used (by default) when creating an object without any arguments (e.g. `point p`), and is required for some applications like arrays.
In the code above, C++ generates an implicit default constructor for us since we didn't define one, and it is called when we create the object.
Now we can define our own constructor, which also removes the auto-generated default constructor:

https://github.com/CIS-1901-F24/Notes/blob/c6d2bac48e123afba17050fec0d19c704f508574/03/point3.cpp#L1-L25

The part after the constructor's parameter list is known as the *initializer list*, and is used to initialize member variables.
Note that initializing a member variable in the initializer list is different from assigning to it in the body of the constructor.
Assigning to it in the constructor first initializes it to whatever initial value it gets, then the body of the constructor is run.
This means that if a member object of a class isn't initialized in the initializer list of the constructor, its default constructor is first called.
This is an issue if, for example, we have a class which contains a member variable of type `point`.
`point` has no default constructor, only a constructor taking two arguments, so it must be initialized in the initializer list: `... : p {0, 0}`.
Similarly, since references must be initialized, they too must also be initialized in the initializer list, not set in the body of the constructor.

Going back to the auto-generated constructors, we can explicitly ask for this using `point() = default;`, which gives us the implicitly generated default constructor that was removed when we defined our own constructor.
Note that this terminology can be confusing.
Default constructor in C++ means a constructor with no arguments, but `default` here means the "default-generated" constructor.
We can also explicitly remove this auto-generated constructor, using `point() = delete;`.
If we do this and remove our other constructor as well, then this would make it so that we cannot create `point` objects, since there are no constructors.

Default values for member variables can also be specified where they are declared, which is used to initialize the variable if it was not already initialized in an initializer list.

https://github.com/CIS-1901-F24/Notes/blob/c6d2bac48e123afba17050fec0d19c704f508574/03/point4.cpp#L1-L24

### `const`

`const` variables of built-in types like `int` cannot be modified using `=`, but objects can be modified using member functions.
Disallowing all member functions to be called on `const` objects would be too restrictive, however, so C++ allows us to mark which operations are safe to be called on `const` objects.
This is done by adding `const` to the end of the member function declaration, and the compiler checks that the definition of the function does not mutate the object.
These `const` member functions are the only ones that can be called on `const` objects.

https://github.com/CIS-1901-F24/Notes/blob/c6d2bac48e123afba17050fec0d19c704f508574/03/point5.cpp#L1-L25


### Operator Overloading

Calling a member function can be thought of as calling a free function (a regular non-member function) with an extra first argument.
i.e. `a.foo(b)` can be thought of as `foo(a, b)`, where the first argument is a reference to the class.
Following this intuition, overloading an operator for your class can be a member function, as long as the first argument to the operator is an object of your class.

For example, overloading addition for two `point`s can be either a member function inside the `point` class or a free function.
But since the operator has to access private members of your class, it must be defined as a member function here:

https://github.com/CIS-1901-F24/Notes/blob/c6d2bac48e123afba17050fec0d19c704f508574/03/point6.cpp#L1-L30


## C++ (and C) Compilation Model

While it's possible to write all your code in one file, it is typically not desirable.
For large projects, changing any code would require the entire file to be recompiled, which can take a long time.
Breaking up work and using others' code in the form of libraries would also be very difficult.

The compilation process looks something like this:

1. The preprocessor runs on each source file, expanding `#include`s and `#define`s, among other tasks.
2. The compiler compiles the output of the preprocessor (called a *translation unit*) into `object files`.
3. The linker links multiple object files together, creating the executable.

Headers are necessary so that compilation succeeds even when the code in broken up into multiple files.
Headers typically include declarations that specify the types of functions defined in other translation units.
In the linking phase, the linker finally goes into the different translation units to find the definitions, "linking" everything together into one executable.

Here is the split-up version of the `point` code above, which you can compile by just listing both `.cpp` files together in the usual compilation command:

https://github.com/CIS-1901-F24/Notes/blob/c6d2bac48e123afba17050fec0d19c704f508574/03/separate/point.hpp#L1-L24
https://github.com/CIS-1901-F24/Notes/blob/c6d2bac48e123afba17050fec0d19c704f508574/03/separate/point.cpp#L1-L37
https://github.com/CIS-1901-F24/Notes/blob/c6d2bac48e123afba17050fec0d19c704f508574/03/separate/main.cpp#L1-L14

Notice that we need to prepend `point::` to the function name when defining them in the `.cpp` file.
This is like a namespace, where we are specifying that we mean the function found in the `point` class.

Now we go over some of the reasoning behind these choices.

### One Definition Rule

Just as in C, multiple *declarations* of functions are allowed as long as the declarations are the same.
Multiple definitions are not allowed.
There is an exception to this for `inline` functions, which signals to the compiler that this function could be inlined (that the body of the function could just be substituted in, rather than be compiled into an actual function call).
You are also allowed to put member function definitions in the header file, which will work because C++ will automatically treat that as an `inline` function.
This, however, is not typically good practice and you should avoid it.

With classes, things get a little more complex.
Multiple declarations of classes (e.g. `class point;`, without the body of the class) are still allowed, but there can be only definition per *translation unit*.
However there can be multiple definitions in different translation units, as long as they are the same.
This is because the definition of the class is required for the compiler to determine its size (by looking at its members), which is needed to generate code for things like function arguments.

More details about the one definition rule can be found [here](https://en.wikipedia.org/wiki/One_Definition_Rule).

### Include Guards

Include guards (the preprocessor directives around the entire header file) prevent headers from being included more than once in the *same translation unit*.
As mentioned above, this will violate the one definition rule if the header contains a class definition.

Remember that translation units are compiled separately into object files and only linked at the end, so include guards cannot help with enforcing the one definition rule for the entire program.
Multiple translation units can all include the same header even with include guards, and if that header contains a function *definition*, for example, this would result in a *linker* error since the one definition rule was violated.
You can try this by adding a function definition to the header and compiling it.
Both translation units include the header, and you will get a linker error.
Note that linker errors look different from compiler errors, and will typically result from an executable like `/usr/bin/ld`, the linker.

## Inheritance

To define a class `derived` inheriting from the class `base`, declare it as `class derived : public base ...`.
To call the base class's constructors, you can just call them normally.
To call the base class's member functions, you need to specify the class name (e.g. `base::f()`).
This is the alternative to using something like Java's `super()`.

The entire example is presented here:

https://github.com/CIS-1901-F24/Notes/blob/c6d2bac48e123afba17050fec0d19c704f508574/03/inheritance.cpp#L1-L58

We now go over the different portions one by one.

### `virtual`

In the base class, member functions that you want to behave polymorphically must be marked as `virtual`, before its return type.
In a derived class, `virtual` is not necessary in the declaration since the `virtual` marker in the base class remains.
Without `virtual`, if you have a `base` reference and call the non-`virtual` function, even if the reference is to a `derived` object, it will call the `base` version, rather than the `derived` version.
This allows us to customize the exact behavior of our objects, whether we want *dynamic dispatch* or only *static dispatch*.
The function to call is determined at compile-time for non-`virtual` functions, and determined at runtime for `virtual` functions.

This option exists because there is overhead to using `virtual` functions, which are the default in many languages.
Objects must hold what's called a "virtual table" or *vtable*, which has pointers to its member functions.
During runtime, the program must perform a lookup through the vtable to find what function should *really* be called.
Near the beginning of object-oriented programming, many programmers were skeptical about using dynamic dispatch in this way, since they viewed the overhead as completely impractical.

When overriding member functions in derived classes, you can add `override` after the parameter list.
This is not necessary, but it is useful for documentation and can catch bugs (e.g. if you try to override a function but misspelled the function name, the compiler can tell you that you are not actually overriding anything).

You can also add `final` in the class declaration (e.g. `class derived final : public base`) or member function declaration (e.g. `void foo() final {...}`) to disallow inheritance and overriding, respectively.

### Abstract Classes

In C++, an *abstract class* is one with a pure virtual function, which is a `virtual` function declared as `... f(...) = 0;`.
A pure virtual function does not need to have a definition, but it can, if you want to provide some common functionality.
You cannot instantiate objects of abstract classes, since there is conceptually some operation (the pure virtual function) that is not yet implemented.

If a derived class overrides the pure virtual function, then it is not pure virtual anymore in that derived class.
A class without any pure virtual functions is *concrete*, and objects of that class can be instantiated.
Abstract classes allow us to define an *interface* with operations that are forced to be implemented by derived classes if they want to use the class.


### I/O classes

## Standard Streams

Standard streams are an abstraction that generalizes communication with its execution environment.
There are three standard (std) streams, stdin, stdout, and stderr.
By default, these are directed to the terminal and result in input from the keyboard and output to the terminal, but these can be redirected to other programs or files.

stdin represents input, and comes from the keyboard by default, which is read when you press enter.
We can also redirect input to come from a file, for example using `./program < in.txt` which uses the contents of `in.txt` for stdin, instead of the keyboard.

stdout represents output, and goes to the terminal by default.
stderr represents error messages, and also goes to the terminal by default.
While by default these two sources would be interspersed in the terminal, we can also redirect them.
For example, we could redirect strerr to store errors to a log file.
To redirect stdout you would do `./program > out.txt`, and to redirect stderr, you would use `./program 2> err.txt`.

See the [wikipedia article](https://en.wikipedia.org/wiki/Standard_streams) for more detailed information.

## Input/Output in C++ via Standard Streams

C++ also provides objects called streams which are used for input/output.
The first ones we will talk about are streams which interact with the standard streams.

### Output

We have already seen the C++ stream that prints to stdout: `std::cout`.
`std::cout` is an *object* of type `std::ostream`.
For the rest of the file we will drop the `std` namespace for convenience.
A second stream object that C++ provides in `iostream` is `cerr`, which is also an `ostream` that prints to stderr.

The primary operation that `ostream`s provide is `<<`, the operator which prints something to a stream.
As mentioned in last week's notes, this works via operator overloading, where the usual bit shift operation `<<` is made to have different functionality when used on `ostream`s.
We also saw that we can chain multiple things to be printed.
This works because of two things: the fact that `<<` is left associative and that it returns a reference.

Left associativity means that `a << b << c` is interpreted as `(a << b) << c`, and not `a << (b << c)`.
If it were interpreted in the second way, then `cout << 2 << 1` would be `cout << (2 << 1)` and print 4, rather than the 21 that we expect.

`<<` should be thought of as a function that is just called in a different way.
Its first argument is an `ostream &`, and it has overloads for many different types as its second argument, like `int` and `string`.
The return value is an `ostream &`, returning the same reference that was passed in.
Unlike our example from class 2, this is safe and commonly done, since we are not creating a new thing and returning a reference to it.
Rather, we are just giving back the same reference that was passed in, which should still be active, since it is "owned" by something outside the function call.
This enables us to chain the `<<` applications, since `cout << 2 << 1` first prints the 2, and returns `cout` again, effectively becoming `cout << 1`, and the `1` can then be printed.

In addition to just printing, you can also format how certain things are printed, using *I/O manipulators*, which change the internal settings of streams.
Documentation for the various manipulators can be found [here](https://en.cppreference.com/w/cpp/io/manip).
For example, `cout << scientific << 1.1` will set `cout` to print `double`s and `float`s in scientific notation.

### Buffering

While not an inherent feature of stdout and stderr, many languages *buffer* stdout and do not buffer stderr.
This means that when you print to stdout it is not written immediately to the screen, but stored in a buffer until there is a good amount of text to write (typically a line of text).
Conversely, when printing to stderr, it is written immediately no matter how much text there is.
Buffered output is better for performance, however if your program crashes then it might lose some output that is in the buffer, but not printed yet.
For this reason, strerr is not buffered so no error messages are lost in this way.
An example of this can be seen by the below program:

https://github.com/CIS-1901-F24/Notes/blob/c6d2bac48e123afba17050fec0d19c704f508574/03/buffered.cpp#L1-L21

This example uses `flush()`, a *member function* (what we call methods in C++) on `ostream`s to manually flush the buffer, forcing the print to stdout to occur.
It also uses `endl`, which represents printing a newline to the stream and then flushing its buffer.

The unbuffered nature of `cerr` is not always desirable, since it is less performant.
For this reason, C++ provides another `ostream` that prints to `stderr` but *is* buffered, called `clog`.
As its name suggests, it is meant for printing large amounts of text to `stderr`, like logging output.

Because of the difference in buffering behavior between the different stream objects, the terminal output may be slightly different from what you expect when using a combination of `cout`, `cerr`, and `clog`.

### Input

Input streams are objects of the type `istream`.
Unlike in C, where using functions like `gets` and `scanf` to read strings can be unsafe due to buffer overflows, `istream`s are safe, handling buffers and other details for you.

`cin` is the `istream` provided that reads from stdin.
The main operation that `istream`s provide is `>>`, or "get from", which reads data from stdin into a variable that you provide, like `cin >> i`.
Just like with `<<`, these operations can be chained together.
You can remember the `<<` and `>>` operations as pointing in the direction of the data.
For `ostream`s, the data goes into the stream to be printed, and for `istream`s, the data goes from the stream into the variable.

`>>` reads (skipping any leading whitespace) until it encounters something unexpected, like whitespace or a newline.
This could also be something like a letter when you are reading an `int`.
For example, if I am reading an `int` using `cin >> i` and I type in `1a`, `1` will be read into `i` and `cin` will stop there.
Later on you can read the `a` separately by reading a `char` or a `string` (more on `string`s after the example).

https://github.com/CIS-1901-F24/Notes/blob/c6d2bac48e123afba17050fec0d19c704f508574/03/cin.cpp#L1-L13

A useful type that we use here is `string`, a much safer alternative to using C strings (`char` arrays).
`string`s automatically resize and can be easily appended using `+`, as with most other languages.
This is crucial, since otherwise we would have to worry about buffer overflows if we were reading input into C strings.

To avoid `>>`'s behavior of skipping whitespace, there is also the `getline` function, which takes an `istream` and the string variable to read into.
This function reads until it encounters a newline, and also returns a reference to the first argument, just like with `>>`.

Streams can also be implicitly converted to `bool`, so we can do things like `if (cin)`.
This is mostly useful for `istream`s, which will convert to `false` if they are in a failure state.
The most common way this can happen if they have reached the end of input by encountering the end of file character (the actual end of file if reading from a file, or ctrl-D if you are entering input on the command line).
Alternatively, you could also call the `fail()` member function to check if the implicit conversion returns `false` (e.g. `if (!cin.fail())` is equivalent to `if (cin)`).
Using these features together, we can write programs that read all of the input:

https://github.com/CIS-1901-F24/Notes/blob/c6d2bac48e123afba17050fec0d19c704f508574/03/readall.cpp#L1-L12

## File Streams

File streams allow us to read from and write to a file.
These declarations are found in the `<fstream>` header.

For input, we have the `ifstream` type which inherits from `istream`.
For output there is the `ofstream` type which inherits from `ostream`.
When creating an object of these classes, it can be initialized with a file name (e.g. `ifstream ifs {"in.txt"};`), or the file to open can be specified later using the member function `open()` (e.g. `ifstream ifs; ... ifs.open("in.txt");`).
Both of these examples call a constructor of `ifstream`, the first of which takes one argument and second of which takes no arguments.
When the file stream object goes out of scope, it cleans up the file handle, so there is usually no need to call the `close` member function, though one is available.

Apart from these ways of associating the stream with a file, these streams are subclasses of `istream` and `ostream`, so they can be used in the same way as the streams we have already seen:

https://github.com/CIS-1901-F24/Notes/blob/c6d2bac48e123afba17050fec0d19c704f508574/03/fstream.cpp#L1-L25

Since these streams need to open files, there are also new ways the streams can fail, as shown in the example.

Finally, there is also an `fstream` type which can do both input and output, which inherits from `iostream`.
`iostream` is a class which inherits from both `istream` and `ostream`, allowing one to both read from and write to it.

## String Streams

String streams allow us to read from and write to a `string`.
This can be useful if you need to read/write a string from/to some other source, like over the network, from a GUI, or from using `getline`.
These declarations are found in the `<sstream>` header.


## I/O for User-Defined Classes

We've seen that streams can perform I/O with a variety of types, printing terms of types like `int`s and `string`s and reading in text and converting it to types like `double`.
These operations depend on the type we are working with, and we can define them for classes we write ourselves as well.
This is the idiomatic way of printing objects in C++, as opposed to solutions like `toString` in Java and other languages.

## Streams Polymorphism

To summarize, the stream types we have described form the inheritance diagram (with slightly different names which are used in the implementation of these classes) shown at [this link](https://en.cppreference.com/w/cpp/io). This means you can write functions that take any type of input or output streams that will work with standard, file or string streams.


### Friend functions

For stream operators like `>>` and `<<`, the first argument is not a `point`, it is a stream.
So to define it as a member function, it would have to be a member function of the `istream` or `ostream` class, which we do not have access to.
Furthermore, neither this nor defining it as a free function would give us access to the private members of `point`, which would be needed when reading or writing a `point`.

The solution to this is the `friend` keyword, which marks a free function as a "friend" of your class, giving it the same access as a member function.

https://github.com/CIS-1901-F24/Notes/blob/c6d2bac48e123afba17050fec0d19c704f508574/03/point7.cpp#L1-L58