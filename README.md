# bind-command
C++17 example of function type erasure with automated conversion to original types

## How to build

Use Clang 5.0+ or GCC 7.0+ in C++17 mode. Other compilers may fail.

- There is `charconv_hack.h` header which stubs [std::from_chars](http://en.cppreference.com/w/cpp/utility/from_chars) function which is not available in libc++ yet. If you have `<charconv>` header, you can remove or disable `charconv_hack.h`
