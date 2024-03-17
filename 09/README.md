#  Class 10

##  Higher-Order Functions


Higher-order functions, as you've seen in CIS 1200, are really useful.

To pass around functions, we can use function pointers, but these are not ideal.

Combining function pointers with template arguments can be difficult, and function pointers only work with regular functions, defined at compile-time.

This means you cannot dynamically build a function at runtime and obtain a function pointer to it.

Function objects solve both of these problems in C++.

  
##  Function Objects


A function object, also called a *functor* in C++, is an object that can be called as a function.

This is done by overloading `operator()` in the class.

The parameter list and return type of `operator()` can be fully customized, and serves as the parameter list and return type of the functor as a whole.

Since a function object is still a normal object, you could give the class some member variables in order to change its behavior:


https://github.com/CIS-1901-S24/Notes/blob/1d1e140815a465d31911bc0b3726670a8ef8fc92/09/functor1.cpp#L1-L25

  

The class of a function object can also be made into a class template, to accomodate different type arguments.

Then, a function template that expects a function argument can use a template argument to specify the type of the function, as shown in this example:
  

https://github.com/CIS-1901-S24/Notes/blob/1d1e140815a465d31911bc0b3726670a8ef8fc92/09/functor2.cpp#L1-L40
  

This code uses the `predicate<T>` concept, which constrains the type to be something that can be called with a `T` and returns something that can be converted to a `bool`.

This allows for both functors and function pointers to be passed in as arguments.
  

Other useful concepts involving "callable" types like functors and function pointers are `invocable` and `regular_invocable`, which specify argument types.

The difference between the two is purely semantic (i.e. their definitions are equal, but they are supposed to have different meanings when used).

`regular_invocable` requires that the function is equality-preserving (i.e. equal inputs always give equal outputs), which rules out functions like random number generators.

These concepts do not specify the return type, which can instead be specified using the `invoke_result` and `convertible_to` concepts together.
  

##  Lambdas


While function objects are a good solution for representing functions, it is very inconvenient to write a new class for each function.

One useful feature in C++ is that classes can be nested within other classes and functions, reducing the amount of classes polluting the overall context.

An even more useful feature for shortening your function object code is the *lambda*.
  

A lambda expression is basically shorthand for declaring a function object (not the class of that function object!):


https://github.com/CIS-1901-S24/Notes/blob/1d1e140815a465d31911bc0b3726670a8ef8fc92/09/lambda.cpp#L1-L19
  

The syntax is fairly straightforward, allowing you to write the parameter list and body of the function as usual.

The `[]` part is the capture list, discussed below.
  

The type of a lambda expression is *unspecified*, meaning you cannot write down the "real" type of the lambda, forcing you to use `auto`.

If you do need to write down the type of the lambda (e.g. if you want to use a lambda as the comparison type for `map`), then you can use `decltype` to get the type of the lambda.

For example, if you have an `auto lam = [](...){...};`, then you can make a copy using `decltype(lam) copy {lam};`.

`decltype` stands for "declared type", and gives the type of an expression.

  

The `[]` is the *capture list* of the lambda expression.

The lambda "captures" variables listed inside the brackets, allowing the body of the lambda to use them.

`[var]` captures `var` by value, copying the value of `var` when the lambda is created.

On the other hand, `[&var]` captures `var` by reference, taking a reference to the original variable, allowing for the lambda to see and make changes to the variable.

Multiple variables can be captured by separating them by commands in the capture list.

Finally, `[=]` and `[&]` capture *everything* by value and reference respectively.

  

https://github.com/CIS-1901-S24/Notes/blob/1d1e140815a465d31911bc0b3726670a8ef8fc92/09/capture.cpp#L1-L24

  

Lambda expressions can also be *generic*, by using `auto` for its arguments.

This is basically shorthand for declaring a template class for a function object:

  

https://github.com/CIS-1901-S24/Notes/blob/1d1e140815a465d31911bc0b3726670a8ef8fc92/09/generic_lambda.cpp#L1-L14

  

To write the return type of a lambda, you can use the syntax `-> T` after the parameter list to specify that the lambda has return type `T`.

Typically this can be inferred and it can be omitted, but sometimes it must be manually specified.

When dealing with generic lambdas, `decltype` is often useful here, since you do not know the types in advance.

  

##  Functional Programming in C++

  
C++ is a multi-paradigm language.

We've seen object-oriented programming, generic programming, and procedural programming so far, as well as some rudimentary functional programming features.

More sophisticated functional programming is also possible.

  

`bind` in `<functional>` partially applies a function, a useful operation for functional programming:
  
https://github.com/CIS-1901-S24/Notes/blob/1d1e140815a465d31911bc0b3726670a8ef8fc92/09/bind.cpp#L21-L31

To specify that an argument should be left unbound, use `placeholders::_1`, `placeholders::_2`, etc . (in order).

In this example, we use `bind` to set the  last two arguments of `f` to `40` and `50` respectively. We leave the first three arguments unbound with placeholders (in a different order).

We can also bind to lambdas, member functions and data members and nest bindings as you can see through the rest of the examples.

https://github.com/CIS-1901-S24/Notes/blob/1d1e140815a465d31911bc0b3726670a8ef8fc92/09/bind.cpp#L33-L44

###  Function Types

  

So far we have used templates to let us take arguments of "function type", which could be the type of a function object or a function pointer type.

In the case when the functor is a lambda, the type is some *unspecified* unique type generated by the compiler, so you cannot write its type without `auto`.

An alternative is `std::function<T>`, which is a class that can hold any callable function of a type `T`.

This is useful for writing recursive lambdas, since `auto` needs to know the type of what it's capturing, but that type depends on itself:

  
https://github.com/CIS-1901-S24/Notes/blob/1d1e140815a465d31911bc0b3726670a8ef8fc92/09/recursion.cpp#L1-L17

  
In this example, we need to capture by reference since capturing by value gets an uninitialized `function`.

Note that you can't return this lambda since it depends on a reference to a local variable (itself).

There are also other methods to create recursive lambdas, but this is the most straightforward.


`function` is also useful for storing a heterogeneous collction of functions with the same function signature, but not necessarily the same type:

  
https://github.com/CIS-1901-S24/Notes/blob/1d1e140815a465d31911bc0b3726670a8ef8fc92/09/functions.cpp#L1-L29

  

This cannot be done using templates, just like how templates cannot store a collection of objects of different types in the same container, and something like object-oriented programming must be used instead.

Just like that use case, you should prefer templates to using `function` if possible, since `function` has some runtime overhead, due to indirection from having to follow a pointer to the actual function, as well as possibly having to copy the function object.


**We will talk more about how we can use these functional programming techniques after we introduce the standard template library since it where many applications can be found.**