#  Class 13


##  Performance
 Although C++ is usually associated with high-performance applications, the language itself is not inherently 'performant'. Writing your code in C++ does not automatically make it faster than other languages. What C++ offers is the low-level control that needs to be used efficiently by the programmer to write efficient code. Therefore, writing efficient C++ code is not only about the language, but is a mix of understanding lower-level concepts about computer architecture, operating systems, and how the internals of C++ work to produce good and performant code. (Since C++ doesn't have a VM or garbage collector, that is already some performance points, but it comes at the cost of manually managing memory which is very difficult and the primary source of most C++ bugs.)

This understanding is often helpful to develop intuition on what works best and what approaches might lead to more efficient code, but ultimately, theorizing about how your code will execute on the actual hardware is helpful, but it is almost impossible to be accurate because of the complexity of what compilers, operating systems, and processors do. Therefore, a key rule to writing efficient code is measuring. We will talk about different measuring tactics towards the end of the notes.

As a note, what I will provide here is an aggregate of concepts and tricks that I learned over a relatively short time developing in C++ and is not an objective guide on how to write efficient C++ code.

The first thing that we will talk about is what are the key factors that influence the performance of your code.

### The compiler

The C++ compiler is your number 1 friend in developing fast code. There are 2 mainstream compilers, GCC and Clang/LLVM (and a close third is MSVC for Windows), which are both huge projects that have been continuously improved. When you write C++ code, the first step is that it is translated from the source code into an intermediate representation (IR). The compiler will then usually run a series of optimizations on this IR and produce an optimized IR. Finally, the optimized IR is translated into machine code. These compiler optimizations are extremely powerful and being familiar with the main ones is useful for 1) not having to waste time doing what the compiler will already do for you and 2) choosing coding patterns that make it easier for compilers to make these optimizations leading into faster code.

Some of the key optimizations that compilers perform are Inlining, return value optimization, loop unrolling and flattening, dead code elimination, local value numbering, etc.

Compilers generally have different optimization levels that you can choose between. ranging from `O0` (no optimization) to `O3` (full optimization). In general, higher optimization levels will lead to  Compiler Explorer, which we will mention in a minute is a great place to see the effect of these optimizations. You can also compile with these flags. For example on gcc `g++ -std=c++20 -O3 ...`. On LLVM, since it is open source, you even have more power and you can test out individual optimizations (or write your own).

### Hardware and Cache

The code you develop will run on hardware. Hardware has limitations in terms of memory and speculative power. Understanding the memory model and cache of computers in general is very useful. In general, code that interacts with the cache is much faster than code that hits and needs to access the main memory. A crucial step to writing efficient C++ code is 'Cache-Friendly' design. In general the closer the data you use often and together, the better. Pointers generally are not cache-friendly. Therefore some data structures like unordered_maps or lists are inefficient. We will talk more about cache-friendly design in a second.

The cache is organized in what is called cache lines. These are the smallest divisions of cache and are always fetched together. They range in size depending on hardware between 16 to 256 bytes in most cases.

### Hot Path

The hotpath is a loose term for the part of your code that executes most frequently and is most critical to your program. This is generally some loop that processes some data and produces some output. You can benchmark and see where your code spends most of its time. In general, you should always look to optimize the hot path first and move all expensive operations outside of the hot path.


## Techniques and Tricks to Writing Efficient C++

Now with this background, we can talk a little bit about some of the important ways we can optimize performance.

### 1. In-depth understanding of C++ features

As we mentioned in the intro. C++ gives you the tools and lower-level control to write efficient code. You will need to use these tools effectively and here are some tips on how using C++ effectively can make you code faster.

#### 1.1 References & Const

In general use references and const as much as possible. You should pass values by const ref unless you have a reason not to do so. This also applies to the use of auto in range-based loops. Accidentally passing by value instead of reference can easily make your program much slower, so this is the first thing to look for.

https://github.com/CIS-1901-S24/Notes/blob/87cecbac5d9c30afd099d78ab3718bef32cae421/13/references1.cpp#L6-L26

In this contrived example, we have managed a 1000x speed up by simply passing values by const ref instead of by value
<pre>

-----------------------------------------------------
Benchmark           Time             CPU   Iterations
-----------------------------------------------------
BM_F1        51072074 ns     51072227 ns           13
BM_F2           44161 ns        44161 ns        15689
</pre>
<br>
Also, Define variables as const unless you have a reason not to. Surprisingly, this can make a lot of difference. In general, the more information the compiler has the more opportunities it has to make optimizations. By defining values as const, you give the compiler stronger guarantees that it can change your code without the risk of values being changed enabling better optimizations.

<br>

One interesting trick I saw a while back is that if you are repeatedly accessing a value in a data structure or class, it is best to grab a reference to it. For example, consider the following two functions.

https://github.com/CIS-1901-S24/Notes/blob/87cecbac5d9c30afd099d78ab3718bef32cae421/13/references2.cpp#L10-L32

These two examples achieve the same thing. Accessing values from a nested hash map requires first computing the hash value, finding the right bucket, following a linked list, and doing the same thing for the inner hash map. Since the value is being accessed multiple times, it is redundant to perform all of that work to access it multiple times. Instead, you can do that once, save a reference to the object, and use that reference every time you need to access the object without needing to repeat all the steps to access it.

The improvement I got was negligible, but if the hash function is more complex, or if the value is used more often, we can see better improvements.
<pre>

-----------------------------------------------------
Benchmark           Time             CPU   Iterations
-----------------------------------------------------
BM_Modify1        191 ns          191 ns      3523101
BM_Modify2        193 ns          193 ns      3633026
</pre>

#### 1.2 Copy & Move
The reason we spent a lot of time on special constructors in this class is that they are very crucial for efficient code writing. If you make an extra copy than you need, you will be wasting cycles and this can add up if you have a loop.

We have seen quite a few examples where you can end up making extra copies if your special constructors and not properly designed, so we won't repeat much here. In general, understanding the rules of 0,3,5 is important. If you are creating an object somewhere other than where you want to use it, use the move constructor to move it where you need it to be.

#### 1.3 Templating
Templates are resolved in compiler time so they have zero overhead. There is a reason the standard template library is full of templates because they are efficient. Always prefer using templates for common patterns and interfaces. Use functors and lambdas to allow users of interfaces to pass custom functions/predicates/comparators, etc. Since templates are resolved in compile time, all uses of the templates have all the information needed by the time they reach the optimizer, which means that the optimizer has more information about the code than other approaches like inheritance or branching, and that enables more optimization opportunities.

A lot of modern C++ projects will extensively use templates.

#### 1.4 Exceptions

Exceptions are a good way to handle errors in C++, but as the name suggests, they should be used for exceptions. I.e. rare events that don't occur as often. Exceptions generally incur no overhead if the safe path (no exception) is executed but can be incredibly costly if an exception does occur.  Exceptions should not be used for flow control. For example, if you call a function and expect that oftentimes the operation will not succeed, then you should use a boolean return value to signal failure instead of an exception.

#### 1.5 Memory

Memory allocation is generally very inefficient and should be avoided in the hot path at all costs. Some best practices to avoid are to reserve memory in advance and to use Arenas.

`reserve` is a member function of many sequence classes that will reserve sufficient memory to hold the necessary. In the next examples, we will look at two implementations of a function and notice that the second implementation will be significantly better since it doesn't need to do allocation in the hot path of the loop.

Arenas allows you to reserve a pool of memory at once and manually manage it. The bottleneck of memory allocation is that it has to go through the operating system every time, but you can avoid that by managing the memory at the user level, which is harder and more error-prone, but gains a performance boost.


#### 1.6 Types precision
C++ has many integer types that vary in size. You might be used to writing everything as an `int`. However, C++ `int` is a 32 (in most cases, but it is platform dependent) bits integer. If we have prior that our actual data won't need 32 bits, we can use a more precise type. For example, if we use `uint8_t` which is an 8-bit unsigned integer (0 - 255), we can achieve almost double the performance.

https://github.com/CIS-1901-S24/Notes/blob/87cecbac5d9c30afd099d78ab3718bef32cae421/13/types.cpp#L10-L30

Benchmark:
<pre>
-----------------------------------------------------
Benchmark           Time             CPU   Iterations
-----------------------------------------------------
BM_F2         3652630 ns      3652434 ns          192
BM_F1         5988678 ns      5986814 ns          115
</pre>

Hopefully from these small examples, you can see that efficiency is a product of understanding the tools you have and using them effectively. With benchmarking you can narrow down the bottlenecks of your program into individual functions and by small tweaks, you can improve the performance.

### 2.  Understanding of Data Structures

The hot path of most applications is a loop that processes data and produces output. This data will generally be organized in some data structure. Therefore, data structures are crucial for great performance. Choosing the right data structure and utilizing it correctly is very important. (If your project is big enough, it is generally a good idea to define a customized data structure for your use case, we will talk about this a bit later.)

#### 2.1 Emplace over Push

A very easy optimization is to use emplace over push whenever you want to add an object to a data structure

https://github.com/CIS-1901-S24/Notes/blob/87cecbac5d9c30afd099d78ab3718bef32cae421/13/emplace.cpp#L9-L26

Benchmark results:
<pre>

Benchmark           Time             CPU   Iterations
-----------------------------------------------------
BM_F2         7715268 ns      7715321 ns           67
BM_F1         8568722 ns      8568682 ns           74

</pre>


#### 2.2 Use Reserve

Whenever you have knowledge of how much capacity you will need, reserve that using `reserve` function to avoid repeated memory allocation which is slow.


#### 2.3 Know your data structure member functions and algorithms
if you want to find the kth smallest element in a data structure, you might sort it and index the kth element. If you have taken an algorithms class, you are probably familiar with the probabilistic quick select and deterministic "median of medians" algorithms for finding the kth smallest element, but these are tedious to implement. However, fortunately, STL algorithms provide an algorithm called `std::nth_element` that already does this for you! This will generally be better optimized than any algorithm that you will implement in a few minutes and will save you precious development time.

#### 2.4 Avoid non-contiguous data structures as much as possible.
Data structures like `std::list`, and `std::map` don't store all their elements in contiguous memory. This is extremely inefficient for cache, and should generally be avoided in any performance-critical application unless you absolutely need them. This effect can be significantly stronger than asymptotic performance. For example, although `list` and `vector` should both have constant time push back asymptotic complexity, in practice, vector is about 5-10 times faster and in some cases can even be about 40x faster.


https://github.com/CIS-1901-S24/Notes/blob/87cecbac5d9c30afd099d78ab3718bef32cae421/13/contigous.cpp#L10-L22

Benchmark:
<pre>

Benchmark           Time             CPU   Iterations
-----------------------------------------------------
BM_F2        16457338 ns     16455870 ns           51
BM_F1          429396 ns       429388 ns         1658
</pre>

Also, `std::unordered_*` is generally not very efficient. It is great as a general-purpose unordered container, but you should avoid using it in any performance-crucial application.

### 3. Cache Friendly Design

In C++, the memory layout of classes generally follows the order in which they are defined.  Therefore, you always want to follow a "cache-friendly" layout. Here are some ways to do that.

#### 3.1 Padding

Consider the following 2 almost identical class definitions.
https://github.com/CIS-1901-S24/Notes/blob/87cecbac5d9c30afd099d78ab3718bef32cae421/13/padding.cpp#L3-L21

Notice that the first class has size 24 bytes and the second has size 16 bytes. This is a result of padding. In C++, padding refers to the insertion of additional bytes between structure members or class data members by the compiler, with the goal of aligning the members in memory in a way such that memory access is aligned based on the architecture. Padding is primarily introduced for performance reasons, as aligned memory access can be more efficient on certain architectures. However, this can have drawbacks as it will leave a lot of memory unused making the effective capacity of your cache smaller. Yan avoid some of these issues by arranging your struct in order of larger to smaller data members.

#### 3.2 Hot Data vs Cold Data

Now, our next example is a little bit more different. Consider the following class. You want to have the first few bytes contain the most important data of your class.

A common pattern is to define a substruct for each class containing hot and cold data respectively like this.

https://github.com/CIS-1901-S24/Notes/blob/87cecbac5d9c30afd099d78ab3718bef32cae421/13/hodata.cpp#L4-L23

If you are doing extensive file processing, you might be attempting to access for example the path to the file and the id of the file very often. You might be interested in saving some other metadata, but this will not be referenced often or may not be used at all in the hot path. Therefore, you want to make sure when you access a file struct, you get the important first without needing to access a second cache line. Therefore, placing the important data first will ensure that. To make it easy to modify, using a struct to define hot and cold data is helpful.

#### 3.3 Struct of arrays vs array of structs
Another very important concept is struct of arrays vs array of structs. Imagine you want a program to represent a list of students. At the hotpath of your program, you want to process the grades of your students. If you are defining a student struct and then defining a vector of students and then looping over that vector to access grades. This is probably the most intuitive and readable approach, but it has some performance drawbacks. Since structs are contiguous in memory, each object will fill some part of the memory. If we assume the grade is the second byte, your access pattern will look like this.
<pre>
| 1 | 1 | 1 | 1 | 2 | 2 | 2 | 2 | 3 | 3 | 3 | 3 |
      ^               ^               ^
      |               |               |
</pre>

Now, if cache lines are 4 bytes for example, each byte you are interested in accessing will be in a different cache line. This is very inefficient.

https://github.com/CIS-1901-S24/Notes/blob/87cecbac5d9c30afd099d78ab3718bef32cae421/13/struct_of_arrays.cpp#L7-L22

If we know in advance that our access patterns will look like this and instead designed this application such that all students are one struct that consists of separate arrays for each property, looping over students to access their grades can easily be done by looping over the grades array and now everything is contiguous in memory. Another advantage of this design is that now we can use SIMD operations, which we will mention in a second to improve the performance.

https://github.com/CIS-1901-S24/Notes/blob/87cecbac5d9c30afd099d78ab3718bef32cae421/13/struct_of_arrays.cpp#L24-L39

<pre>

-----------------------------------------------------
Benchmark           Time             CPU   Iterations
-----------------------------------------------------
BM_F1            3384 ns         3384 ns       207000
BM_F2            2194 ns         2194 ns       319715
</pre>
This is the result of my benchmark of the two approaches.


#### `OutOfLine`

Recently, I ran into this blog from Headlands Technologies, a quantitative trading firm, that came up with an interesting tool called `OutOfLine`. Essentially, it solves the problems that the two ideas above try to solve more elegantly. I will just leave a link to it [here](https://blog.headlandstech.com/2018/08/15/outofline-a-memory-locality-pattern-for-high-performance-c/).

### 4. Compiler Hints
The compiler is your best ally for optimization. While 90% of the time the compiler will make better decisions on its own, if you have some priors you can give the compiler hints to optimize the code in specific ways you expect to be beneficial.

#### 4.1 Inlining
Inlining is one of the most powerful optimizations. From CIS 2400, You probably remember that a function call requires multiple instructions from moving stack framing, pushing arguments, etc, which are best avoided if we can. You can give the compiler a hint that you want to inline some specific functions by using the compiler hint `
__attribute__((always_inline))`. Note that there is another keyword called `inline` but counterintuitively, this does not actually inline and is used for other purposes (it still hints the compiler to inline, and the compiler will most likely ignore it).

https://github.com/CIS-1901-S24/Notes/blob/87cecbac5d9c30afd099d78ab3718bef32cae421/13/inline.cpp#L1-L20

Inlining is not always the best choice. For example, you might have code that isn't executed often and that is long and you don't want to inline so that it is not located near the rest of our core logic. (This is helpful because instructions, like data, are also cached and there is limited cache capacity. We want to reserve the more important instructions.)

#### 4.2 Branch Prediction
All modern processors have a pipelined design where multiple instructions are being executed simultaneously at different stages. When there is a branch instruction, the processor doesn't know the outcome until very late stages which means if the processor starts executing some path, but turns out the branch outcome is a different path, it needs to flush out the pipeline. This is extremely costly and modern processors have put a lot of effort into accurate branch prediction. Helping your processor predict branches correctly is a huge performance boost. If you have some prior on what branches/outcomes are more likely, you can pass that information into your compiler to organize the code in the best way that helps predict the correct branch. You can do this in GCC with `__builtin_expect`. In C++ 20, this has been generalized across platforms in `[[likely]]` and `[[unlikely]]`. For example:

https://github.com/CIS-1901-S24/Notes/blob/87cecbac5d9c30afd099d78ab3718bef32cae421/13/likely.cpp#L3-L12

#### 4.3 Builtin Prefecth

As we have seen, cache lines are generally very small and sometimes you are accessing contiguous data, so it helps to not only fetch the cache line you are interested in but also fetch nearby cache lines. In general, compilers will try to do that, but if you suspect that the compiler is missing some cases you can manually prefetch cache lines using `__builtin_prefetch`.

For example:

https://github.com/CIS-1901-S24/Notes/blob/87cecbac5d9c30afd099d78ab3718bef32cae421/13/prefetch.cpp#L7-L25

In This program, we are doing a binary search. Since the compiler generally has no way of knowing our access pattern, we can instruct it to prefetch the addresses we expect to access in the next iteration, which is the middle of the right and left halves.

These are my benchmark results
<pre>
---------------------------------------------------------
Benchmark               Time             CPU   Iterations
---------------------------------------------------------
BM_Prefetch    1.4224e+10 ns   1.4220e+10 ns            1
BM_No_Prefetch 2.0064e+10 ns   2.0050e+10 ns            1
</pre>


<br>

#### 4.4 SIMD Intrinsics (Single Instruction Multiple Data)
SIMD intrinsics allow you to process multiple data with a single instruction by utilizing hardware parallelism. In general, most modern processors have an architecture that permits the processing of vectors of data at once. Unfortunately, this is hardware and platform-dependent, so we won't cover it in much detail.

<br>
With all of these compiler hint techniques, always measure the performance gains or losses before using them. The compiler is smarter than you and if you overuse these techniques, you will interfere with the compiler and actually make performance worse if not the same. Unless you have knowledge the compiler doesn't have, it is best to leave it to do its work alone.

### 5. Good API Design

This overlaps with understanding C++ to a large extent, but sometimes, API Design can help improve performance. Consider the scenario where we want to have a function that takes in some data, processes it, and returns a string. You may define it like this.

https://github.com/CIS-1901-S24/Notes/blob/87cecbac5d9c30afd099d78ab3718bef32cae421/13/string.cpp#L5-L20

If we expect this code to be executed in a loop, a better API design is to let the user pass in a reference to a string and we fill it for them. The advantage of this is now the user can allocate the buffer string outside of their loop and pass in the same reference, while in contrast, the previous design will force us to allocate new memory for the string on each iteration.

benchmark results:

<pre>

Benchmark           Time             CPU   Iterations
-----------------------------------------------------
BM_F2      4470775460 ns   4470049525 ns            1
BM_F1      5421160209 ns   5419891024 ns            1
</pre>

### 6. Tools and Utilities

This is generally something you would do at a larger project or in a firm. Defining your own data structures or your Math utilities can help a lot. For example, we know that `std::unordered_*` is optimized for performance. In any large performance-critical application, it is always worth defining your own HashMap or using some other implementation. It is also useful to have some math utilities for quick computations if your application involves a lot of math applications. For large applications and firms, you will probably have specific use cases that could allow you to optimize these tools for your specific use case, which is why projects will even create their own version of good data structure to fit a specific use case.


## Measuring

As we eluded earlier, all of the above can give you good intuition on what would likely give more efficient code, but you will never be certain without actually measuring your code. Most efficient software has been achieved by rigorously measuring, benchmarking, and tweaking until the desired outcome is achieved. We will explore a few tools to measure your code.

Also, it is good to understand the difference between microbenchmarks and profiling. Microbenchmarks are benchmarks that test individual parts of the code such as functions, as opposed to profiling which will give you an overall picture of how your code performs.


### 1. Google Benchmark

[Google benchmark](https://github.com/google/benchmark) is a microbenhcmarking library that is developed by Google and performs microbenchmarks. The benchmarks for the examples above have been computed using Google Benchmark, I have not included the library in the repo, but you will have to install the repo and build it to be able to run the programs that use Google Benchmark. The installation guide is in the documentation, and once it is installed, you can compile the programs in the repo using something like:

`g++ string.cpp -std=c++11 -isystem benchmark/include   -Lbenchmark/build/src -lbenchmark -lpthread -O3 -o string`

### 2. Compiler Explorer
[Compiler Explorer](https://godbolt.org/) is a tool to see what machine code is generated for your code in different environments. While this isn't strictly measuring or benchmarking anything, it is a proxy to see what actual machine instructions are generated from your code. A rule of thumb is that fewer machine instructions = faster code, but this isn't always the case. This should be used with caution and in isolation. For example, comparing what instructions are generated from 2 statements in isolation from the rest. For larger chunks of code, this can be ineffective as longer code can still lead to better results. You can see by observing that running `-O3` vs `-O0` will sometimes generate more code.


### 3. Callgrind

[Callgrind](https://valgrind.org/docs/manual/cl-manual.html) is a profiling tool that is part of the Valgrind instrumentation framework. It is primarily used for profiling and analyzing the runtime behavior of programs, including information about function calls, cache usage, and more. This is a great option since it is easier to set up than some of the rest. It simulates cache unlike perf which we will talk about next, however, the results are still useful.

For example, you can use it like this to analyze an app using

`valgrind --tool=callgrind --cache-sim=yes --dump-instr=yes --branch-sim=yes ./a.out`

### 4. Linux Perf

Linux perf is a great tool for high-level profiling about cache accesses, instructions, etc. Unfortunately, it only works on Linux, which I don't use, but I will also leave a [link](https://perf.wiki.kernel.org/index.php/Main_Page) to it here.

### 5. Magic Trace

Magic is a profiling tool with a great visualization that was developed by Jane Street. I will leave a link to it [here](https://github.com/janestreet/magic-trace).


## Additional resources

Finally, I will link to some great talks and resources about performance to learn more (A lot of the ideas here are inspired by these talks):

- [Optimizing Software in C++](https://www.agner.org/optimize/optimizing_cpp.pdf)
- Effective Modern C++ by Scott Meyers
- [Carl Cook: When a Microsecond Is an Eternity](https://www.youtube.com/watch?v=NH1Tta7purM&ab_channel=CppCon)
- [Timur Doumler: Want fast C++? Know your hardware!](https://www.youtube.com/watch?v=BP6NxVxDQIs&ab_channel=CppCon)
- [The Most Important Optimizations to Apply in Your C++ Programs - Jan Bielak](https://www.youtube.com/watch?v=qCjEN5XRzHc&ab_channel=CppCon)
- [Chandler Carruth: Going Nowhere Faster](https://www.youtube.com/watch?v=2EWejmkKlxs&ab_channel=CppCon)
- [Chandler Carruth: Efficiency with Algorithms, Performance with Data Structure](https://www.youtube.com/watch?v=fHNmRkzxHWs&ab_channel=CppCon)

