#  Class 8

  

For this week we will cover a few small topics, since the project are due next week.

##  Forwarding and Universal References

Recall that `c++` offers two different types of references, lvalue references `T&` and rvalue references `T&&`. In some cases, you might want to have a function that catches both lvalues and rvalues references. To do that in `c++`, you wild need to declare your function to be a template function and use the `&&`. When there is type deduction involved, C++ will treat the `&&` syntax as something call universal references (more recently known as forwarding references).

https://github.com/CIS-1901-F24/Notes/blob/67f67a719744a4f8e24bf0c626aca27767c4948d/08/forwarding.cpp#L4-L8

In general, this is not very helpful, since if you wanted to have a function that will handle both rvalues and lvalues, you could have used a const lvalue reference, which can also bind to rvalues. This will however be useful if you already have seperate overloads for an lvalue and rvalue reference of some other functions that you want to conditionally call within your current template function, i.e forward the arguments that your function takes regardless of weather they are lvalue of rvalue. C++ uses the `std::forward<T>` function to allow to pass the arguments conserving their type, reference and constness information to other functions.

https://github.com/CIS-1901-F24/Notes/blob/67f67a719744a4f8e24bf0c626aca27767c4948d/08/forwarding.cpp#L10-L24

This is generally known as perfect forwarding and is used in places like `std::make_unique`, `std::make_shared`, `std::make_pair` and `emplace` functions that we will cover in our 	`STL` class.



### Side Note:  Type Deduction

C++ uses type deduction for multiple cases. The keyword `auto`, templates, `lambdas` and so many different things uses type deduction. Type deduction may not only deduces but also alo information about references, pointers, constness, etc. Especially with the addition of features like universal references, rules for type deduction become very complex and certainly out of the scope of the class, but I will leave a link to this [talk](https://www.youtube.com/watch?v=wQxj20X-tIU&t=1591s&ab_channel=CppCon) that does a great job explaining it.



##  `enum class`es

  

In C, you may have used [`enum`](https://en.cppreference.com/w/c/language/enum)s to represent named constants.

While these work, they effectively just define names for `int` (or another underlying type) constants, and these names can be used in place of `int`s and vice versa.

This means that if you have a `color`  `enum`, you could do things like `RED - BLUE` or pass numbers to functions that expect a `color`, both of which might be undesirable.

  

An alternative that C++ provides is an [`enum class`](https://en.cppreference.com/w/cpp/language/enum), which are similar but are more strongly typed.

Values of `enum class`es do not have implicit conversions to `int` or vice versa, though you can expicitly convert between the two.

In addition to this, the constants that are part of the enumeration are placed in the scope of the enumeration, so they must be accessed using `::`:

  

https://github.com/CIS-1901-F24/Notes/blob/67f67a719744a4f8e24bf0c626aca27767c4948d/08/enum.cpp#L1-L14

  

There are some other features of `enum class`es, like using an underlying type aside from `int` and defining more operations on values, but we won't go further into those features.

In general, you should prefer `enum class` to `enum` in C++, since they are more restrictive and are thus less likely to cause bugs from unexpected behavior.

  

##  Union types

  

There are some common patterns in C that are dangerous or suboptimal.

Here, we'll present some safer solutions in C++ for these use cases.

  

###  [`optional`](https://en.cppreference.com/w/cpp/utility/optional)

  

A common pattern in C to signify that an operation has failed to produce a value is to return a pointer, where null signifies failure and a non-null pointer to a value signifies success.

This works, and we could make it safer by using a `unique_ptr`, but it still has drawbacks since it uses dynamic memory.

In general, you should avoid heap allocations if possible, since they require manual memory management (solved using the smart pointer) and have some overhead in the form of pointer dereferencing.

Every time you want to use an object through a pointer, you must dereference that pointer, which is an extra instruction to run in the program.

  

C++ provides an alternative: the `optional<T>` type (in the `<optional>` header).

The type `T` in the angle brackets signifies what type it should contain.

Values of this type holds either a value of type `T` or an "empty" value, which we write as either `{}` or `nullopt`.

  

https://github.com/CIS-1901-F24/Notes/blob/67f67a719744a4f8e24bf0c626aca27767c4948d/08/optional.cpp#L1-L30

  

To check whether the `optional` has a value, we can either use an implicit convertion to `bool` or use the member function `has_value()`.

Once we know that the `optional` is not `nullopt`, we can access the value using `*` and `->`, just like a pointer.

Alternatively, we can use `value()` to do a checked access, which throws an exception if the `optional` is actually `nullopt`.

Conversely, `*` and `-> access the value directly without any checking or the possibility of throwing an exception, which is slightly faster.

Finally, we can use `value_or(...)`, which takes a value to return if the `optional` is a `nullopt`.

  

Importantly, this type does not involve any pointers internally, and holds the value of type `T` directly (on the stack).

This means that we cannot use `optional` to implement something like a linked list, since the compiler would not be able to calculate how big an object should be.

If a class `foo` contains a member of type `optional<foo>`, then the size of a `foo` object would not be calculable at compile time, so this would result in a compiler error.

  

###  [`variant`](https://en.cppreference.com/w/cpp/utility/variant)

  

A more general case than just a value or nothing is when a value can be one of several types.

To handle this, C has `union`s, a type that can hold one of several types.

In C this is typically used to access the same memory as the different types of a union, like a shortcut to having to cast manually.

These `union`s are *untagged*, meaning the `union` doesn't keep track of which type is being stored inside it, which you must do yourself using a separate variable.

  

C++ provides a type called `variant<T1, T2, ...>`, which holds a value of one of the types `T1, T2, ...`.

This is a *tagged* union, meaning that the `variant` keeps track of which type it currently holds.

  

https://github.com/CIS-1901-F24/Notes/blob/67f67a719744a4f8e24bf0c626aca27767c4948d/08/variant.cpp#L1-L25

  

To check if a `variant`  `v` contains a value of type `T`, you can use `holds_alternative<T>(v)`.

And to access the value, use `get<T>(v)`, which will throw if `v` does not hold a `T`.

There is no fast, unchecked version of this operation, which is a little surprising to me.

[This discussion](https://stackoverflow.com/questions/48817080/why-stdget-for-variant-throws-on-failure-instead-of-being-undefined-behaviour) speculates on why.

  

Like `optional`, `variant`s holds its internal value on the stack, avoiding any pointers internally.

So like `optional`, a class cannot hold recursive instances of itself using a `variant`.

  

###  [`any`](https://en.cppreference.com/w/cpp/utility/any)

  

Finally, one last thing that is fairly common in C is `void *`, a pointer to anything.

These are useful for `malloc`, wihch returns a `void *`, and for representing unknown function arguments.

There are alternatives to both in C++, `new` in place of `malloc` and various forms of function types, which we'll see later.

However, if you still want a type that can represent any other type, you can use `any`.

An `any` can be initialized with anything, and you can use `any_cast<T>(a)` to convert the `any`  `a` into the type `T`.

If that `a` does not hold a `T`, this throws an exception.

Alternatively, you can try to `any_cast` a pointer to an `any`, and that will return `nullptr` on failure and a `T *` on success.

  

https://github.com/CIS-1901-F24/Notes/blob/67f67a719744a4f8e24bf0c626aca27767c4948d/08/any.cpp#L1-L30

  

Using `any` should be a last resort, since it is so unstructured.

[This discussion](https://stackoverflow.com/questions/52715219/when-should-i-use-stdany) contains some examples, though I think `variant` would be a better option for many of those cases.


##  Casts

  

In general, casts do not do any sort of actual conversion.

They only operate on references and pointers, giving the programmer a new "view" of some object using a new reference/pointer.

  

###  `dynamic_cast`

  

```c++

shape * s {...};

if  (square * sq {dynamic_cast<square>(s)})

... // the shape pointer s actually did point to a square, not some other shape

```

  

`dynamic_cast` is used for casting within a class hierarchy.

The cast is checked at runtime (*dynamically*), returning `nullptr` (when casting a pointer) or throwing an exception (when casting a reference) if it fails.

It is most commonly used for downcasting: casting a base class pointer/reference to a derived class.

In general, this is a *code smell* that tells you there may be something wrong with the design of your classes.

Typically your code should not require knowledge of the exact subclass a base class pointer/reference points to, and requiring it makes the code more difficult to extend with more subclasses.

  

###  `static_cast`

  

[`static_cast.cpp`](static_cast.cpp)

  

`static_cast` performs "safe" casts, such as those between numeric types, and explicit upcasts in a class hierarchy.

The compiler checks that the types are convertible to each other at compile-time (*statically*).

These casts can often be performed implicitly, but `static_cast` is useful when you need to make the type you are casting to explicit.

`static_cast` can also be used to perform downcasts, but these are not checked like `dynamic_cast` is.

Use this if you need the performance, and are *sure* that it will succeed (e.g. if you have already checked earlier using a `dynamic_cast`)

  

###  `const_cast`

  

[`const_cast.cpp`](const_cast.cpp)

  

`const_cast` removes `const` from a reference or pointer.

Note that you can always add `const` without a cast.

However, if the original object is `const`, modifying it is undefined behavior.

  

###  `reinterpret_cast`

  

[`reinterpret_cast.cpp`](reinterpret_cast.cpp)

  

`reinterpret_cast` changes a pointer or reference type without any checking. It interprets the argument just as a pointer/reference to a sequence of bytes and just gives it the new type.

`reinterpret_cast` cannot remove `const`, you need to use `const_cast` for that.

No actual conversion occurs. `reinterpret_cast` just tells the compiler it can view this block of memory as the new type.

  

###  C-style casts

  

You should never use C-style casts (`(int)v`) in C++!

They behave in often unexpected ways.

It performs (approximately) `const_cast`, `static_cast`, `static_cast` then `const_cast`, `reinterpret_cast`, and `reinterpret_cast` then `const_cast` until one of them succeeds.