cmake_minimum_required(VERSION 3.14)
project(huffman)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "-Xlinker -v -v")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

include_directories(include)

add_executable(
  testentry
  test/test.cpp
)

add_library(huffman STATIC src/huffman.cpp src/symbol_tree.cpp include/huffman.hpp include/non_copyable.hpp)
target_compile_options(huffman PRIVATE -v)

target_link_libraries(testentry GTest::gtest_main huffman)

include(GoogleTest)
gtest_discover_tests(testentry)
