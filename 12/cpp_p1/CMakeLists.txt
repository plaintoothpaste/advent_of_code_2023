cmake_minimum_required(VERSION 3.22)
project(adventOfCode)
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed")
endif()
set(CMAKE_CXX_STANDARD 20)

set(resources
    input.txt
    example.txt
)
set(compile
    main.cpp
    file_parse.h
    enumerate.h
)

add_executable(aoc_2023 ${compile})

# Add all rescource files in a seperate folder next to the executable
foreach(resource IN LISTS resources)
    add_custom_command(
        TARGET aoc_2023
        POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/${resource}
                "$<TARGET_FILE_DIR:aoc_2023>/${resource}"
    )
endforeach()
message("Build complete")
# cmake -S . -B build; cmake --build build; clear; ./build/Debug/aoc_2023.exe
