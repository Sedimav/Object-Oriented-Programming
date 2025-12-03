cmake_minimum_required(VERSION 3.16)
project(lab06)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(lab6 main.cpp func.cpp)

add_executable(tests tests.cpp func.cpp)
target_link_libraries(tests ${GTEST_LIBRARIES} pthread)

enable_testing()
add_test(NAME AllTests COMMAND tests)