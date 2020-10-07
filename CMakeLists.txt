cmake_minimum_required(VERSION 3.10)
project(ArraySharedComponents)

set(CMAKE_CXX_STANDARD 14)

# Fetch dependencies
include(FetchContent)
# MBED dependency
FetchContent_Declare(
    mbed
    GIT_REPOSITORY  https://github.com/ARMmbed/mbed-os
    GIT_TAG         mbed-os-6.3.0
)
# GTEST dependency
FetchContent_Declare(
    googletest
    GIT_REPOSITORY  https://github.com/google/googletest
    GIT_TAG         release-1.10.0
)
FetchContent_MakeAvailable(mbed googletest)


# We add this subdirectory to get our header references for our tests
include_directories(src)
add_subdirectory(src)
# And then we build our gtests...
add_subdirectory(TESTS)

