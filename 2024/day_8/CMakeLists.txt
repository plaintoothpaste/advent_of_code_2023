cmake_minimum_required(VERSION 3.22)
project(aoc8)
set(CMAKE_CXX_STANDARD 20)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed")
endif()

add_executable(aoc src/main.cpp)
target_include_directories(aoc PUBLIC src)

message("Build complete")
