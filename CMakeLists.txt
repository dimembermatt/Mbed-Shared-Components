cmake_minimum_required(VERSION 3.10)
project(ArraySharedComponents)

set(CMAKE_CXX_STANDARD 14)

# Folder includes for files in tests to find.
include_directories(Example)
include_directories(CANevice)
include_directories(Filter)
include_directories(Sensor)
include_directories(Serial)

# GTEST dependency
add_subdirectory(lib/googletest)
# We add this subdirectory to get a library called ArrayShareComponents_lib up.
add_subdirectory(Example)
# And then we build our gtests...
add_subdirectory(TESTS)

