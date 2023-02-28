cmake_minimum_required(VERSION 3.10)
project(mini2048 CXX)

set(CMAKE_CXX_FLAGS "-O3 -mtune=native -march=native -Wall -pedantic")

add_executable(
    enumeration
    enumeration.cc
    board.cc
)

add_executable(
    retrograde
    retrograde.cc
    board.cc
)

add_executable(
    retrograde2
    retrograde2.cc
    board.cc
)

target_compile_features(enumeration PRIVATE cxx_std_17)
target_compile_features(retrograde PRIVATE cxx_std_17)
target_compile_features(retrograde2 PRIVATE cxx_std_17)