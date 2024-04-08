#  Class 12

In this week's notes we'll cover some small topics.

##  C++ Data Types

We have used several data types throughout the class without propoerly introducing them. Most of the primitve types such as boolean, void are trivial. There are two floating point types, `float` and `double` which follow the IEEE floating-point standard. `float` uses 4 bytes of memory and has 23 bits for mantissa and 8 for power, and 1 for sign. `double` has double the precision and 52 bits for mantissa, 1 for sign and 11 for power.

On the other hand, there are too many integer types:

####  `char`:
 `char` is the type used to represent characters. It is a one byte, 8 bit data type that is the ASCII of the characters. C string are just char arrays, and C++ strings are built on top of that.
chars can be easily converted to ints, and we can do arithmetic operations on them.

#### `int` 
`int` is the most commonly used type for integers. If used without modifiers it usually denotes a 4 byte, 32 bit integer, but the size can also depend on the platform.

###  `Modifiers` 
There are a few modifiers that can provide some extra information about the size and signedness of the type.

#### `signed` and `unsigned` 
signed and usigned will tell us whether the int is signed or not. If no midifiers are used, the int is assumed to be signed. The modifiers can also be used without the keyword int. for example `unsigned` just means `unsigned int`. `unsigned` values never flow. For example:

https://github.com/CIS-1901-S24/Notes/blob/5d923b5535b40171eb41607542e69ff9c4925c83/12/unsigned.cpp#L1-L35  

Notes that in this example, we have used `10u`

#### `short`, `long` and `long long`
These modifiers control the size of the int. Short will have at least 16 bits, long will have at least 32 bits and long long will have at least 64 bits. The exact size will depend on the platform.

Different modifiers can be combined together, for example, `unsigned long long int`. And similary, size modifiers can be used without the keyword `int`. So `usigned long x;` means `unsigned long int x;`. 

You can also construct literals for each type using the appropriate suffix. For example, `auto x = 10ul;` means the type of the literal (and the type deduced by auto) is `usigned long int`. `ll` means long long, `l` means long and `u` means `unsigned`. 

These literal suffixes are also avaialbe for floating point types. `10.f` means the literal is `float` while using no suffix will assuming any values with periods to be doubles. So `10.0` would assume a `double`.

### Fixed Width Integer Types

The modifiers we discussed are unforunately not standarized across platform. `long int` could mean different things if ran on different devices. There, I personally believe that using [Fixed Width Integer Types](https://en.cppreference.com/w/cpp/types/integer)  is best practice. These types are defined under the header `<cstdint>`

Fixed data types are generally in the format [u]intxx_t where u is optional for specifying singed vs unsigned and xx is the size of the integer. This types are macro definitions that expand to the correct defintion for that exact size based on your platform. For example, `uint32_t` will guarantee an unsigned int of 32 bits. I personally think it is always best practice to always use those so that you are always certain what the size of your type will be.

##  C++ Build Systems

In this part, I will briefly go over some of the build systems that are used for building C and C++ files. Hopefully this would be helpful for choosing a good  build system for your final project.

- Make
Make is the most common build system. Make usese Makefiles to describe how targets are built and their dependancies. Make also has some helpful features such as avoiding recompilation. When compiling, Make compares the timestamp of the source file and the last built target, if the source is not newer, it avoid recompiling. We have been using Make for most of the assignments in this call so far. In general, if you have trivial dependenceis and building stratigies and all your code is made of source files and headers which are all in the same folder with little or no external libraries. raw Makefiles are probably the best option.

- CMake
CMake is a more high level tool that is used to build systems and can automate generating makefiles. CMake is an industry standard tool and is very powerful specially when you are using dependencies in your application. Cmake is great for managing recursive builds, directory hierarchie, external libraries, version management, packaging and more. Whenever you have a large project, makefiles can become unmaintanable and CMake is the best option.  You can check out the documentation [here](https://cmake.org/documentation/).
- Bazel
Bazel was developed by Google in 2015. I am personal fan of Bazel since its syntax is much more intuitive. It is very to get started with. Bazel has good support for a few libraries, but can be very hard to work with on non-native libraries. It does however have good support for many of the most commonly used libraries such boost, nloham json library and more. I believe it is a good choice for large projects because most of the time, you would want to minimize the use of libraries and rewrite the code to optimize it for your specific use case.  you can read about it more [here](https://bazel.build/).
 
## Introduction to Networking

Since this class isn't focused on netwroking and there are other classes offered here that cover that in great detail, I will only focus on basics relevant to C++ and how they can be used to write some networking programs. If you want to learn more about networking, [Beej's guide](https://www.beej.us/guide/bgnet/html/) is an amazing resource.

In general, the networking stack consists of mutiple layers: Physical, Datalink, Network, Transport, Session, Presentation and Application. A good overview can be found here [On this page](https://docs.oracle.com/cd/E19504-01/802-5886/intro-78284/index.html). In addiiton multiple protocols are used to facilitate communication over these layers.Some of the most important are IP on the netwrok layer and TCP or UDP on the transport layer.

IP is the protocol responsible for indentfying and addressing devices on the network. Each device is given an IP address. On IPV4 the addresses were number up to (2^32) which is around 4 billion. Due to the explosion in use of the internet, this is not enough and IPV6, which uses   (2^128). IP addresses are generally represened with dot-decimal notation (each 4 bytes are repsented as a decimal value between 0 and 255 seperated by dots) for example:  (_172.16.254.1_) represents 2886794753.

TCP and UDP are two different protocols at the transport layer that deliver data from the lower layers and provide abstractions for applications and higher layers. **TCP** is a connection-based protocols that sends and receives a stream of bytes. It is reliable and ensure in order delivery. It requires setting up the connection and maintaining and sending metadata to ensure the reliablity of the messages. On the other hand, **UDP** is a lightweight communication protocol that delivers data in packets and does not guarantee in order delivery of every packets in order. TCP is often used for applications that require strong relability such as transactions while UDP is often used for applications that require performance but less reliablity such as streaming and multicasts. Since UDP doesn't send as much metadata and doesn't require setting up connections, it is much more perofrmant and scalable, but less reliable.

Establisihing TCP or UDP connections and sending and receiving messages through them is usally abstracted by the operating system through some interfaces. For this part, we will focus on the interface for unix-like operating systems and specifically Linux.

The most important abstraction that linux provides is the socket. A socket is identified by it is File Descriptor (fd) and can be thought of as file. You can read and write data into that file. 

For the next examples, we will focus on TCP, but keep in mind that UDP still has very good applications and is in some times the obvious choice to use.

1) For establishing the connection, there is usually a lot of boilerplate code you need to do for setting up the sockets and the connection. Generally, one side would create a server and listen for connections, and the other side will connect to that server. Here is a demo of the client and server side code for doing that.

Server Side
https://github.com/CIS-1901-S24/Notes/blob/5d923b5535b40171eb41607542e69ff9c4925c83/12/Networking/server.cpp#L13-L44  

Accepting connections
https://github.com/CIS-1901-S24/Notes/blob/5d923b5535b40171eb41607542e69ff9c4925c83/12/Networking/server.cpp#L51-L58  

client side
https://github.com/CIS-1901-S24/Notes/blob/5d923b5535b40171eb41607542e69ff9c4925c83/12/Networking/client.cpp#L17-L32


2) Once the conenction has been established, both sides get a file descriptor to the socket that will be used for the communication. You can read data from the socket or send data through the socket. 

reading
https://github.com/CIS-1901-S24/Notes/blob/5d923b5535b40171eb41607542e69ff9c4925c83/12/Networking/utils.cpp#L14-L15

Since TCP sends a receives data as a stream, you can read the stream chunk by chunk where you call read on the socket fd and pass in a buffer. The OS will read what's available in the socket and return to you how many bytes were read. If the return value is 0, that means the connection has been closed or and EOF was sent. If it is less than zero, this means there was some error. If there is not enough data, the call will block until there is enough data and return a positive value or zero.

writing
https://github.com/CIS-1901-S24/Notes/blob/5d923b5535b40171eb41607542e69ff9c4925c83/12/Networking/utils.cpp#L53-L64

Calls to write are not guaranteed to succeed. They return how many bytes they managed to send. Therefore, you must keep attempting to write the remaining bytes in your message until success in a lop.

3) Sockets are resources that need to be cleaned up. After the end of the session, you need to close the socket. In generally, if you working within a class, you should clean up all sockets on destruction.


With this in mind, I wrote down some boilerplate code and basic class defintions for a server and client for chatroom application. In class, we will walk through building more features for this application and hopefully this should provide some good practive on how to write some slightly longer progras than what we have been doing over class and be helpful for your final projects.