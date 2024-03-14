cmake_minimum_required(VERSION 3.22)
project(linklistvsarray)
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed")
endif()
set(CMAKE_CXX_STANDARD 20)

set(resources
    input.txt
    example.txt
)

add_executable(aoc
    main.cpp 
)
target_include_directories(aoc PUBLIC ../../../../common/cpp)

message("Build complete")
