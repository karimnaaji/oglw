cmake_minimum_required(VERSION 2.8)
project(OGLW)

option(OGLW_BUILD_SAMPLES "Build OGLW samples" ON)

# cmake output configuration
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# c++14 flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++1y")

# load OGLW library
add_subdirectory(${PROJECT_SOURCE_DIR}/oglw/)

# include OGLW headers
include_directories(${OGLW_INCLUDE_DIRS})

# utilities
include(cmake/samples.cmake)

# build samples
if(OGLW_BUILD_SAMPLES)
    file(GLOB SAMPLES ${PROJECT_SOURCE_DIR}/samples/*)
    foreach(_sample ${SAMPLES})
        if(IS_DIRECTORY ${_sample})
            message(STATUS "Adding sample " ${_sample})
            add_subdirectory(${_sample})
        endif()
    endforeach()
endif()
