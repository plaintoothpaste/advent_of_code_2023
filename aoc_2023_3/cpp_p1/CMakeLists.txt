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

add_executable(aoc_2023_1
    main.cpp 
 "file_parse.h" "enumerate.h")

# Add all rescource files in a seperate folder next to the executable
foreach(resource IN LISTS resources)
    add_custom_command(
        TARGET aoc_2023_1 
        POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/${resource}
                "$<TARGET_FILE_DIR:aoc_2023_1>/${resource}"
    )
endforeach()
message("Build complete")