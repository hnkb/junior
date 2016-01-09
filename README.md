# junior

An easy to use, yet powerful GUI library, designed especially for starting C/C++ programmers

*Do away with `<stdio.h>` and `std::cout`, teach young ones to create games and process images!*

**This library is under development and is not ready for use yet. Documentation will be prepared after**
**the API is fixed. For now, to get a general idea about this library, take a look at [`example`](https://github.com/labandierra/junior/blob/master/example) directory.**

The current implementation works on Windows 7 and up, uses Direct2D for high-performance graphics, supports
Unicode and markdown for text, and allows saving and loading different types of images. A recent version of
Microsoft Visual C++ with decent C++11 support (Visual C++ 2012 and newer) is required to compile junior.

### Design goals

- This is to help beginning programmers start programming in C/C++ with writing more exciting programs,
  like graphical games and image processing! The basic functions (like showing a window, drawing and
  printing text) shall work with the least amount of code and be understood by beginners with limited
  programming knowledge (take a look at
  [`example\simple\simple.cpp`](https://github.com/labandierra/junior/blob/master/example/simple/simple.cpp))
- Each line of code shall have an immediate utility for the programmer (so no `init()` or `uninit()`!).
  Do not involve programmer as much as possible, instead, use sensible default behaviour.
- Use standard C++ objects and conventions as much as possible. Favour modern C++. Never repeat things already
  in C++ and STD. The goal is to teach them C/C++, not this particular library.
- A function-only interface is also available for those who are yet to learn about objects, but it is
  not the main interface and is not supposed to be able to do everything.
- Although with an easy interface, this library shall enable programmers to create working real-world apps,
  for example 2D games with good performance
- Supporting widgets (controls) is not a priority for now. But it is definitely in the back-log.
- I'd like to make this library cross-platform, and it is designed in a way to make porting easier, but again, for
  now, development is focused on Windows.
