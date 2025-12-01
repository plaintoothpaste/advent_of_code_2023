cmake_minimum_required(VERSION 3.22)
project(aoc10)
set(CMAKE_CXX_STANDARD 20)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed")
endif()


include_directories("include")
set(MAIN_SRC
    "src/fileParse.cpp"
    "src/process.cpp"
)
add_executable(aoc "src/main.cpp" ${MAIN_SRC})
add_executable(aoc2 "src/main_2.cpp" ${MAIN_SRC})

# set_project_warnings(aoc) # extensive warnings

# set in conan
if (RUN_TESTS)
    message(STATUS "Building unit tests...")
    find_package(GTest REQUIRED) # just works cause of conan
    include(GoogleTest)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # recommended best practice
    
    set(TESTS_SRC
        "tests/stepTest.cpp"
    )
    add_executable(Tests tests/main.cpp ${TESTS_SRC} ${MAIN_SRC})
    target_link_libraries(Tests PRIVATE GTest::gtest)
    gtest_discover_tests(Tests)

endif ()

message("Build complete")
