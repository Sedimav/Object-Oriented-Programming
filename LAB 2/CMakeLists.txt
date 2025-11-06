cmake_minimum_required(VERSION 3.16)
project(lab2)
set(CMAKE_CXX_STANDARD 20)
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.0
)
set(BUILD_GMOCK OFF)
FetchContent_MakeAvailable(googletest)
add_executable(${CMAKE_PROJECT_NAME}_exe main.cpp twelve.cpp)
add_executable(tests test_twelve.cpp twelve.cpp)
target_link_libraries(tests gtest gtest_main)
enable_testing()
add_test(NAME TwelveTests COMMAND tests)
