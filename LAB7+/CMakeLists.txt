cmake_minimum_required(VERSION 3.10)
project(lab7)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(cppcoro REQUIRED)

add_executable(lab7
    main.cpp
    bull.cpp
    game.cpp
)

target_link_libraries(lab7 cppcoro::cppcoro)

if(UNIX)
    target_compile_options(lab7 PRIVATE -Wall -Wextra -pthread)
    target_link_libraries(lab7 pthread)
endif()

find_package(GTest QUIET)

if(GTest_FOUND)
    add_executable(tests
        tests.cpp
        bull.cpp
        game.cpp
    )
    
    target_link_libraries(tests GTest::gtest GTest::gtest_main cppcoro::cppcoro)
    
    if(UNIX)
        target_compile_options(tests PRIVATE -Wall -Wextra -pthread)
        target_link_libraries(tests pthread)
    endif()
    
    enable_testing()
    add_test(NAME Lab7Tests COMMAND tests)
endif()