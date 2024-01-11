cmake_minimum_required(VERSION 3.20)

project(tohsaka)

set(CMAKE_BUILD_TYPE "DEBUG")

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SOURCES)
include_directories(${PROJECT_SOURCE_DIR}/include)
add_library(tohsaka ${SOURCES})
target_include_directories(tohsaka PUBLIC ${PROJECT_SOURCE_DIR}/include)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
aux_source_directory(tests TEST)
list(LENGTH TEST TEST_LENGTH)
math(EXPR TEST_LENGTH "${TEST_LENGTH}-1")

if(${TEST_LENGTH} GREATER 0)
    list(APPEND NUMBERS 0)

    foreach(NUM RANGE 1 ${TEST_LENGTH})
        list(APPEND NUMBERS ${NUM})
    endforeach()


    foreach(CASE NUM IN ZIP_LISTS TEST NUMBERS)
        string(REPLACE ".cc" "" test ${CASE})
        string(REPLACE "tests/" "" test ${test})
        add_executable(${test} ${CASE})
        target_link_directories(${test} PUBLIC include)
        target_link_libraries(${test} PUBLIC tohsaka pthread)
    endforeach()
endif()