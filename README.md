# Array-Shared-Components

Holds shared code between the MPPT and Curve Tracer projects.

## Writing Code and Contributing

If you are making modifications to existing code in this codebase (i.e. editing
the implementation of `filter/MedianFilter.h`), make sure you run the steps in
**TESTING** prior to committing.

If you're writing new code for this codebase, the folder should be in the root
and in CamelCase, no spaces or dashes. (i.e. say a new class called dc-dc
converter should be called `/DCDCConverter`.)

All you really need is the header and implementation file. Derived classes can
be in the same folder. See `/Filter` as an example of this.

## Testing

### Writing Tests

This testbench was setup thanks to this
[tutorial](https://raymii.org/s/tutorials/Cpp_project_setup_with_cmake_and_unit_tests.html).

After writing your potential driver, you need to write unit tests for this to
make sure it functions appropriately. Make a new folder in `/TESTS` with the
same naming convention as before (i.e. `/TESTS/DCDCConverter`), and then a file named `test_<CLASS>.cpp` for
every class you need to test.

We're using
[GoogleTest](https://github.com/google/googletest/blob/master/googletest/docs/primer.md)
to do unit testing; follow one of the other tests for syntax or take a decent
look at the primer link. Suggested parts in the primer are [Simple
Tests](https://github.com/google/googletest/blob/master/googletest/docs/primer.md#simple-tests)
and [Test Fixtures](https://github.com/google/googletest/blob/master/googletest/docs/primer.md#test-fixtures-using-the-same-data-configuration-for-multiple-tests-same-data-multiple-tests).

After writing your unit tests, to invoke these tests and see your output you
need to add some stuff to the CMakeLists.txt.

In the source code folder (i.e. `/DCDCConverter`), copy in a CMakeLists.txt that
looks like this:

```C
set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

set(SOURCES ${SOURCES})

add_library(${BINARY}_lib STATIC ${SOURCES})
```

Or you could simply copy it from another source folder like `/Filter`.

Then, add your reference to the root `/CMakeLists.txt`.

For example, if you've added the new class DCDCConverter,

```C
cmake_minimum_required(VERSION 3.10)
project(ArraySharedComponents)

set(CMAKE_CXX_STANDARD 14)

# Folder includes for files in tests to find.
include_directories(Example)
include_directories(CANDevice)
include_directories(DCDCConverter) # <- Add directory name here, in alphabetical order
include_directories(Filter)
include_directories(Sensor)
include_directories(Serial)

# GTEST dependency
add_subdirectory(lib/googletest)
# We add this subdirectory to get a library called ArrayShareComponents_lib up.
add_subdirectory(Example)
# And then we build our gtests...
add_subdirectory(TESTS)
```

### Running Tests

To run the tests, navigate to the `/TEST_BUILD` folder.
Call the following command:

```c
    cmake .. -DCMAKE_BUILD_TYPE=Debug -DCOVERAGE=True -G "Unix Makefiles"
```

You should see output like the following:

```c
-- The C compiler identification is GNU 7.5.0
-- The CXX compiler identification is GNU 7.5.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Found PythonInterp: /home/matthew/anaconda3/bin/python (found version "3.7.6")
-- Looking for pthread.h
-- Looking for pthread.h - found
-- Looking for pthread_create
-- Looking for pthread_create - not found
-- Looking for pthread_create in pthreads
-- Looking for pthread_create in pthreads - not found
-- Looking for pthread_create in pthread
-- Looking for pthread_create in pthread - found
-- Found Threads: TRUE
-- Configuring done
-- Generating done
CMake Warning:
  Manually-specified variables were not used by the project:

    COVERAGE


-- Build files have been written to: /home/matthew/Documents/Github/UTSVT/Array-Shared-Components/TEST_BUILD
```

Now, run `make all`. You should see something like the following:

```c
[ 13%] Built target gtest
[ 26%] Built target gmock
[ 40%] Built target gmock_main
[ 53%] Built target gtest_main
[ 66%] Built target ArraySharedComponents_lib
Scanning dependencies of target ArraySharedComponents_test
[ 73%] Building CXX object TESTS/CMakeFiles/ArraySharedComponents_test.dir/Filter/test_Filter.cpp.o
[ 80%] Building CXX object TESTS/CMakeFiles/ArraySharedComponents_test.dir/Filter/test_MedianFilter.cpp.o
[ 86%] Building CXX object TESTS/CMakeFiles/ArraySharedComponents_test.dir/main.cpp.o
[ 93%] Linking CXX executable ArraySharedComponents_test
[100%] Built target ArraySharedComponents_test
```

To see the output of your unit tests, run the compiled executable in
`TEST_BUILD/TESTS/`.

From the `TEST_BUILD` directory:

`./TESTS/ArraySharedComponents_test`

An example output looks like the following:

```c
[==========] Running 7 tests from 3 test suites.
[----------] Global test environment set-up.
[----------] 1 test from exampleTest
[ RUN      ] exampleTest.sayHello
[       OK ] exampleTest.sayHello (0 ms)
[----------] 1 test from exampleTest (0 ms total)

[----------] 3 tests from FilterTest
[ RUN      ] FilterTest.initialize
[       OK ] FilterTest.initialize (0 ms)
[ RUN      ] FilterTest.read
[       OK ] FilterTest.read (0 ms)
[ RUN      ] FilterTest.readSeries
[       OK ] FilterTest.readSeries (0 ms)
[----------] 3 tests from FilterTest (0 ms total)

[----------] 3 tests from MedianFilterTest
[ RUN      ] MedianFilterTest.initialize
[       OK ] MedianFilterTest.initialize (0 ms)
[ RUN      ] MedianFilterTest.read
[       OK ] MedianFilterTest.read (0 ms)
[ RUN      ] MedianFilterTest.readSeries
[       OK ] MedianFilterTest.readSeries (0 ms)
[----------] 3 tests from MedianFilterTest (0 ms total)

[----------] Global test environment tear-down
[==========] 7 tests from 3 test suites ran. (0 ms total)
[  PASSED  ] 7 tests.
```