# Array-Shared-Components

Holds shared code between the MPPT and Curve Tracer projects.
Instructions were written in particular for linux users in mind.

The repo structure is as follows:

- BUILD: An empty folder where output of your MBED builds be generated.
- TESTS: Testbenches for classes go into here.
- TEST_BUILD: An empty folder where output of your unit tests will be generated.
- src: A folder containing actual class source code.
- unbuilt: WIP classes for now.

---

## Requirements

- [GoogleTest](https://github.com/google/googletest) (Clone into `lib/`)
- GCC 6+
- [CMake 3.11+](https://anglehit.com/how-to-install-the-latest-version-of-cmake-via-command-line/)
  - 3.11+ allos us to use FetchContent and automates GTEST dependency fetching
- Python 2.7.x or 3.5+
- Pip 10.0+
- Gcovr 4.1+
- Mbed CLI 1.8.0+

---

## Writing Code and Contributing

If you are making modifications to existing code in this codebase (i.e. editing
the implementation of `src/Filter/MedianFilter.h`), make sure you run the steps in
**TESTING** prior to committing.

If you're writing new code for this codebase, the folder should be in the `src` folder
and in CamelCase, no spaces or dashes. (i.e. say a new class folder called dc-dc
converter should be called `src/DCDCConverter`.)

All you really need is the header and implementation file. Derived classes can
be in the same folder. See `src/Filter` as an example of this.

---

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

### Running Tests

To run the tests, navigate to the `/TEST_BUILD` folder.
Call the following command:

```bash
    cmake .. -DCMAKE_BUILD_TYPE=Debug -DCOVERAGE=True -G "Unix Makefiles"
```

You should see output like the following:

```bash
USER:~/.../TEST_BUILD$ cmake .. -DCMAKE_BUILD_TYPE=Debug -DCOVERAGE=True -G "Unix Makefiles"
-- The C compiler identification is GNU 7.5.0
-- The CXX compiler identification is GNU 7.5.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Found PythonInterp: /home/matthew/anaconda3/bin/python (found version "3.7.6")
-- Looking for pthread.h
-- Looking for pthread.h - found
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Failed
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

```bash
USER:~/.../TEST_BUILD$ make all
Scanning dependencies of target gtest
[  6%] Building CXX object _deps/googletest-build/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.o
[ 13%] Linking CXX static library ../../../lib/libgtestd.a
[ 13%] Built target gtest
Scanning dependencies of target gmock
[ 20%] Building CXX object _deps/googletest-build/googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.o
[ 26%] Linking CXX static library ../../../lib/libgmockd.a
[ 26%] Built target gmock
Scanning dependencies of target gmock_main
[ 33%] Building CXX object _deps/googletest-build/googlemock/CMakeFiles/gmock_main.dir/src/gmock_main.cc.o
[ 40%] Linking CXX static library ../../../lib/libgmock_maind.a
[ 40%] Built target gmock_main
Scanning dependencies of target gtest_main
[ 46%] Building CXX object _deps/googletest-build/googletest/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o
[ 53%] Linking CXX static library ../../../lib/libgtest_maind.a
[ 53%] Built target gtest_main
Scanning dependencies of target ArraySharedComponents_lib
[ 60%] Building CXX object src/CMakeFiles/ArraySharedComponents_lib.dir/Example/TestClass.cpp.o
[ 66%] Linking CXX static library libArraySharedComponents_lib.a
[ 66%] Built target ArraySharedComponents_lib
Scanning dependencies of target ArraySharedComponents_test
[ 73%] Building CXX object TESTS/CMakeFiles/ArraySharedComponents_test.dir/Example/test_TestClass.cpp.o
[ 80%] Building CXX object TESTS/CMakeFiles/ArraySharedComponents_test.dir/Filter/test_Filter.cpp.o
[ 86%] Building CXX object TESTS/CMakeFiles/ArraySharedComponents_test.dir/Filter/test_MedianFilter.cpp.o
[ 93%] Building CXX object TESTS/CMakeFiles/ArraySharedComponents_test.dir/main.cpp.o
[100%] Linking CXX executable ArraySharedComponents_test
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

---

## Building

Note that you need to set up the repo as an mbed project. Instructions on how to
install mbed-cli can be found
[here](https://os.mbed.com/docs/mbed-os/v6.3/build-tools/mbed-cli.html). You can
do so to navigating to the root of this repo and calling

`mbed new .`

So far, the only build target these components were designed for in mind has
been the STM32 L432KC Nucleos. However, these should be cross-compatible with
other devices compatible with the mbed library.

Since there is no main here, to see if everything compiles correctly for our
target, run the following:

`mbed compile -m NUCLEO_L432KC -t GCC_ARM -c --library`

At the end you should see a build success message with the target.
