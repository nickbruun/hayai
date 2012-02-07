# hayai - the C++ benchmarking framework.

_hayai_ is a C++ framework for writing benchmarks for pieces of code along the lines of the normal approach for unit testing using frameworks such as [googletest](http://code.google.com/p/googletest/). For information on the origin of and how to use _hayai_, please read the [introductory blog post](http://nickbruun.dk/2012/02/07/easy-cpp-benchmarking) until I have the time to write a proper README.

## Building hayai

_hayai_ is fully header based, but to use the simple `main` function as described in the introducty blog post, the source code must be compiled to provide `libhayai_main`. Compiling the source code requires `CMake` to be present on the system:

    $ cd hayai
    $ cmake .
    $ make

This will also build the sample available in the `sample/` directory of the repository.
