cmake_minimum_required(VERSION 3.22)
project(linklistvsarray)
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed")
endif()
set(CMAKE_CXX_STANDARD 20)

add_executable(aoc
    main.cpp 
)
target_include_directories(aoc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../common/cpp)

message("Build complete")
