cmake_minimum_required(VERSION 3.16)
project(lab1)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.0
)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(lab1_exe main.cpp turkish_number_converter.cpp)

add_executable(tests test_turkish_number_converter.cpp turkish_number_converter.cpp)
target_link_libraries(tests gtest gtest_main)

enable_testing()
add_test(NAME TurkishNumberTests COMMAND tests)