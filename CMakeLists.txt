cmake_minimum_required(VERSION 3.24)
project(IRCS CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        b796f7d44681514f58a683a3a71ff17c94edb0c1
)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE source_files src/ircs/ReferenceMutations.cpp src/**.cpp)
message(STATUS "${source_files}")

add_library(IRCS STATIC "${source_files}")
target_include_directories(IRCS PRIVATE "include")
add_library(IRCS::IRCS ALIAS IRCS)

add_subdirectory(tests)

