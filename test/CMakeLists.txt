cmake_minimum_required(VERSION 3.14)
project(mini2048_test CXX)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

set(CMAKE_CXX_FLAGS "-g3 -mtune=native -march=native -Wall -pedantic")
add_executable(
    mini2048_test
    test.cc
    ../src/board.cc
)
target_compile_features(mini2048_test PRIVATE cxx_std_17)
target_link_libraries(
    mini2048_test
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(mini2048_test)