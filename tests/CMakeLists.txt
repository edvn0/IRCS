cmake_minimum_required(VERSION 3.14)
project(IRCSTests)

set(CMAKE_CXX_STANDARD 20)

file(
	GLOB_RECURSE
	sources
    main.cpp
	ircs/**.cpp)

add_executable(IRCSTests ${sources})
target_link_libraries(IRCSTests GTest::gtest_main IRCS::IRCS)
target_include_directories(IRCSTests PRIVATE "${CMAKE_SOURCE_DIR}/include")
include(GoogleTest)
gtest_discover_tests(IRCSTests)