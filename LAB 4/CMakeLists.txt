cmake_minimum_required(VERSION 3.16)
project(lab4)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.0
    TLS_VERIFY false
)

set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_library(${CMAKE_PROJECT_NAME}_lib figures.c++)
add_executable(${CMAKE_PROJECT_NAME}_exe main.c++)
target_link_libraries(${CMAKE_PROJECT_NAME}_exe ${CMAKE_PROJECT_NAME}_lib)

add_executable(tests tests.c++)
target_link_libraries(tests ${CMAKE_PROJECT_NAME}_lib gtest gtest_main)

enable_testing()
add_test(NAME FigureTests COMMAND tests)
