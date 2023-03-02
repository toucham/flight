# I/O
Input/output is the process of copying data between main memory and external devices. There are several libaries that can assist with these operations. For example, in C++, high-level input/output operations can be done through either the standard I/O library: `printf()` and `scanf()`, or C++'s overloaded operations: `>>` and `<<`.

## UNIX I/O
Typically, all operating systems provide low-level I/O operations; however, the libary is different depending on the system. In the case for Linux system, the low-level I/O operations can be called using system-level Unix I/O functions provided by the kernel.

## Files
All I/O devices are treated as files, a sequence of <i>m</i> bytes. The I/O operations are performed by reading/writing on the files. There are also many types of files, the three that will be focused on are:

1. regular file
2. directory
3. socket

There are also <i> named pipes </i>, <i> symbolic links </i>, and <i>character </i> and <i>block divices</i> that we won't be exploring in deeper for this project.

## I/O Library
Although the kernel provides system-level I/O functions for doing I/O operations, they are not suitable for networking application. One reason why using Unix I/O functions can pose a problem for the application is the appearance of short count.
```cpp
#include <unistd.h>

/* returns: number of bytes read if OK, -1 on error */
ssize_t read(int fd, void *buf, size_t n);

/* returns: number of bytes write if OK, -1 on error */
ssize_t write(int fd, const void *buf, size_t n);
```
It is possible for `read()` function to return a `ssize_t` value; therefore, the function doesn't consider short count as an error and will operate normally. Therefore, it is necessary to use custom I/O functions like the standard library or robust input/output library inside the project: `rio` for socket programming (or networking application).

### Standard vs RIO
Then which library should we use for I/O in socket programming? The standard library or the robust I/O library? Although it is advisable to use the standard library for all I/O operations, in the case for socket programming, it can pose a problem. This is due to the fact that the standard I/O streams are full duplex, meaning that a program can perform input and output on the same stream. 

There are two restrictions that need to be considered while using standard library:
1. <b>An input function cannot follow an output function </b> without a call to `fflush`, `fseek`, `fsetpos`, or `rewind`.
2. <b>An output function cannot follow an input function </b> without a call to `fseek`, `fsetpos`, or `rewind`, unless the input function encounters an end-of-file.